#include <iostream>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include "itkConnectedThresholdImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"
#include "itkCastImageFilter.h"
#include <itkLabelStatisticsImageFilter.h>
#include "itkNeighborhoodConnectedImageFilter.h"
#include <itkLabelStatisticsImageFilter.h>
#include "itkSliceBySliceImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include <itkConfidenceConnectedImageFilter.h>
#include "itkSmoothingRecursiveGaussianImageFilter.h"
#include "itkAdaptiveHistogramEqualizationImageFilter.h"

using OutputPixelType = unsigned char;
using InternalPixelType = float;
constexpr unsigned int Dimension = 3;
typedef itk::Image <InternalPixelType, Dimension> FloatImageType ;
typedef itk::Image <OutputPixelType, Dimension> OutImageType ;
typedef itk::ImageFileReader < FloatImageType > ReaderType ;
typedef itk::ImageFileWriter < FloatImageType > WriterType ;
typedef itk::ConnectedThresholdImageFilter <FloatImageType, FloatImageType> ConnectedFilter;
typedef itk::ConfidenceConnectedImageFilter<FloatImageType, FloatImageType> ConfidenceConnectedFilter;
typedef itk::SmoothingRecursiveGaussianImageFilter<FloatImageType, FloatImageType> GaussianFilterType;


FloatImageType::Pointer ConnectedThreshold(FloatImageType::Pointer input, FloatImageType::IndexType seed, float lower, float upper ){

  ConnectedFilter::Pointer connectedThreshold = ConnectedFilter::New();
  connectedThreshold-> SetInput (input);
  
  connectedThreshold->SetLower(lower);
  connectedThreshold->SetUpper(upper);
 
  connectedThreshold->SetReplaceValue(255);
  connectedThreshold->SetSeed(seed);
  connectedThreshold -> Update();
 
  return connectedThreshold->GetOutput();
}

FloatImageType::Pointer ConfidenceThreshold(FloatImageType::Pointer input, FloatImageType::IndexType seed){

  ConfidenceConnectedFilter::Pointer confidenceConnectedFilter = ConfidenceConnectedFilter::New();
  confidenceConnectedFilter->SetInput(input);
  confidenceConnectedFilter -> SetSeed(seed);
  confidenceConnectedFilter->SetReplaceValue(1);
  confidenceConnectedFilter->SetMultiplier(4);
  confidenceConnectedFilter->SetNumberOfIterations(5);
    
  confidenceConnectedFilter->Update();
  
  return confidenceConnectedFilter->GetOutput();
}

int main(int argc, char * argv[])
{
  
  if (argc < 4)
  {
    std::cout << "Usage: " << argv[0] << " inputImageFileName outputImageFileName1 outputImageFileName2" << std::endl;
    return -1;
  }
  
  FloatImageType::Pointer myImage = FloatImageType::New() ;
  ReaderType::Pointer myReader = ReaderType::New() ;
  GaussianFilterType::Pointer gaussianFilter = GaussianFilterType::New();
  
  WriterType::Pointer myWriter= WriterType::New() ;
  WriterType::Pointer myWriter2= WriterType::New() ;
  
  myReader->SetFileName (argv[1]) ;
  myWriter->SetFileName(argv[2]);
  myWriter2->SetFileName(argv[3]);
  myReader->Update() ;
  myImage = myReader->GetOutput() ;
 
 
  FloatImageType::RegionType region = myImage->GetLargestPossibleRegion();

  FloatImageType::SizeType size = region.GetSize();

  std::cout << size << std::endl;
  
  gaussianFilter->SetInput(myImage);
  gaussianFilter->SetSigma(1.0);  // Sigma value can be adjusted as needed
  gaussianFilter->Update();
    
    
  
  /*
  FloatImageType::IndexType seed;
   seed[0]= 127; //100
   seed[1]= 252; //231
   seed[2] = 20;*/
   
    /*FloatImageType::IndexType seed;
   seed[0]= 257; //100
   seed[1]= 278; //231
   seed[2] = 37;
   
   /*FloatImageType::IndexType seed1;
   seed1[0]= 416; //100
   seed1[1]= 252; //231
   seed1[2] = 20;
   
      std::vector<FloatImageType::IndexType> seeds;
   seeds.push_back(seed);
   seeds.push_back(seed1);
   seeds.push_back(seed2);
    */
   
   float lower = -1030;
   float upper = -648;
   FloatImageType::IndexType seed2;
   seed2[0]= 127;
   seed2[1]= 252; 
   seed2[2] = 20;
  
   FloatImageType::Pointer connected_output= ConnectedThreshold(gaussianFilter-> GetOutput(), seed2, lower, upper);
   FloatImageType::Pointer threshold_output = ConfidenceThreshold(gaussianFilter-> GetOutput(), seed2);
 
   myWriter->SetInput(connected_output);
   myWriter2->SetInput(threshold_output);

  try
  {
    myWriter->Update();
    myWriter2->Update();
  }
  catch (const itk::ExceptionObject & excep)
  {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
  }
  
 
  return 0 ;
}
