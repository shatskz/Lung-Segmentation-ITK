#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkOtsuMultipleThresholdsImageFilter.h"
#include "itkAdaptiveHistogramEqualizationImageFilter.h"
#include "itkSmoothingRecursiveGaussianImageFilter.h"
#include "itkHuangThresholdImageFilter.h"
#include "itkYenThresholdImageFilter.h"
#include "itkLiThresholdImageFilter.h"
#include "itkMaximumEntropyThresholdImageFilter.h"
#include "itkTriangleThresholdImageFilter.h"


constexpr unsigned int Dimension = 3;
using PixelType = signed short;
using ImageType = itk::Image<PixelType, Dimension>;
using ReaderType = itk::ImageFileReader<ImageType>;
using WriterType = itk::ImageFileWriter<ImageType>;
using BinaryThresholdFilterType = itk::BinaryThresholdImageFilter<ImageType, ImageType>;
using OtsuThresholdFilterType = itk::OtsuMultipleThresholdsImageFilter<ImageType, ImageType>;
using AdaptiveEqualizationFilterType = itk::AdaptiveHistogramEqualizationImageFilter<ImageType>;

void CustomWriteImage(const ImageType::Pointer &image, const char *fileName)
{
    auto writer = WriterType::New();
    writer->SetFileName(fileName);
    writer->SetInput(image);

    try
    {
        writer->Update();
    }
    catch (const itk::ExceptionObject &error)
    {
        std::cerr << "Error: " << error << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <InputFileName>" << std::endl;
        return EXIT_FAILURE;
    }

    const char *inputFileName = argv[1];

    auto reader = ReaderType::New();
    reader->SetFileName(inputFileName);
    reader->Update();

    // Preprocessing with Gaussian Smoothing
    using GaussianFilterType = itk::SmoothingRecursiveGaussianImageFilter<ImageType, ImageType>;
    auto gaussianFilter = GaussianFilterType::New();
    gaussianFilter->SetInput(reader->GetOutput());
    gaussianFilter->SetSigma(1.0);  // Sigma value can be adjusted as needed
    gaussianFilter->Update();
    //CustomWriteImage(gaussianFilter->GetOutput(), "gaussianResult.nii");

    // Otsu Multiple Thresholds Thresholding
    //auto otsuThresholdFilter = OtsuThresholdFilterType::New();
    //otsuThresholdFilter->SetInput(gaussianFilter->GetOutput());
    //otsuThresholdFilter->SetNumberOfHistogramBins(256);
    //otsuThresholdFilter->SetNumberOfThresholds(3); // Example value, can be adjusted
    //otsuThresholdFilter->SetLabelOffset(1); // Example value, can be adjusted
    //otsuThresholdFilter->Update();
    //CustomWriteImage(otsuThresholdFilter->GetOutput(), "otsuMultipleThresholdsResult.nii");

    // Binary Thresholding
    auto binaryThresholdFilter = BinaryThresholdFilterType::New();
    binaryThresholdFilter->SetInput(reader->GetOutput());
    binaryThresholdFilter->SetLowerThreshold(-1030 );
    binaryThresholdFilter->SetUpperThreshold(-648 );
    binaryThresholdFilter->SetInsideValue(300);
    binaryThresholdFilter->SetOutsideValue(10);
    binaryThresholdFilter->Update();
    CustomWriteImage(binaryThresholdFilter->GetOutput(), "binaryThresholdResult.nii");

    // Huang Thresholding
    //using HuangThresholdFilterType = itk::HuangThresholdImageFilter<ImageType, ImageType>;
    //auto huangThresholdFilter = HuangThresholdFilterType::New();
    //huangThresholdFilter->SetInput(gaussianFilter->GetOutput());
    //huangThresholdFilter->Update();
    //CustomWriteImage(huangThresholdFilter->GetOutput(), "huangThresholdResult.nii");
    
    // Yen Thresholding
    //using YenThresholdFilterType = itk::YenThresholdImageFilter<ImageType, ImageType>;
    //auto yenThresholdFilter = YenThresholdFilterType::New();
    //yenThresholdFilter->SetInput(gaussianFilter->GetOutput());
    //yenThresholdFilter->Update();
    //CustomWriteImage(yenThresholdFilter->GetOutput(), "yenThresholdResult.nii");
    
    //Li Thresholding
    //using LiThresholdFilterType = itk::LiThresholdImageFilter<ImageType, ImageType>;
    //auto liThresholdFilter = LiThresholdFilterType::New();
    //liThresholdFilter->SetInput(gaussianFilter->GetOutput());
    //liThresholdFilter->Update();
    //CustomWriteImage(liThresholdFilter->GetOutput(), "liThresholdResult.nii");
    
    // MaximumEntropy Thresholding
    //using MaximumEntropyThresholdFilterType = itk::MaximumEntropyThresholdImageFilter<ImageType, ImageType>;
    //auto maxEntropyThresholdFilter = MaximumEntropyThresholdFilterType::New();
    //maxEntropyThresholdFilter->SetInput(gaussianFilter->GetOutput());
    //maxEntropyThresholdFilter->Update();
    //CustomWriteImage(maxEntropyThresholdFilter->GetOutput(), "maxEntropyThresholdResult.nii");
    
    // Triangle Thresholding
    //using TriangleThresholdFilterType = itk::TriangleThresholdImageFilter<ImageType, ImageType>;
    //auto triangleThresholdFilter = TriangleThresholdFilterType::New();
    //triangleThresholdFilter->SetInput(gaussianFilter->GetOutput());
    //triangleThresholdFilter->Update();
    //CustomWriteImage(triangleThresholdFilter->GetOutput(), "triangleThresholdResult.nii");

    
    return EXIT_SUCCESS;
}
