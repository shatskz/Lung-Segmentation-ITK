#include <iostream>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include "itkImageRegionIterator.h"
#include <itkScalarImageKmeansImageFilter.h>
#include <itkArray.h>
#include "itkRelabelComponentImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include <random>

std::vector<float> kmeansPlusPlusInitialization(const std::vector<float>& data, float k)
{
    std::vector<float> initialMeans;
    std::default_random_engine generator;

    // randomly choose centroid
    std::uniform_int_distribution<size_t> distribution(0, data.size() - 1);
    initialMeans.push_back(data[distribution(generator)]);

    // Step 2: For each remaining centeroids, choose a data point with probability proportional to its squared distance from the nearest existing centroid
    for (unsigned int i = 1; i < k; ++i) {
        std::vector<double> distances(data.size(), std::numeric_limits<double>::max());

        for (const auto& mean : initialMeans) {
            for (size_t j = 0; j < data.size(); ++j) {
                double distance = static_cast<double>(mean) - static_cast<double>(data[j]);
                distance *= distance;
                distances[j] = std::min(distances[j], distance);
            }
        }

        std::discrete_distribution<size_t> probabilityDistribution(distances.begin(), distances.end());
        size_t selectedIndex = probabilityDistribution(generator);
        initialMeans.push_back(data[selectedIndex]);
    }

    return initialMeans;
}

int main(int argc, char* argv[]){
  // print out a usage message
  // argv[0]= name of executable itself
  // argv[1]= first arg which should be the input image file
  // argv[2]= second arg which should be output image file name
  // argv[3]= number of classes
  std::cout << "Usage: " << argv[0] << " " << "inputImageFileName outputImageFileName numClasses" << std::endl;

  //if there's no arg[1], quit
  if(argc < 4){
    std::cout << "Error: Too few  arguments given" << std::endl;
    return -1;
  }
    
   using PixelType = float;
  // typedef to create shorthand for template arguments
  typedef itk::Image<unsigned char, 3> ImageType;
  //typedef itk::Image<unsigned char, 3> ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::ImageFileWriter<ImageType> WriterType;
  typedef itk::ScalarImageKmeansImageFilter<ImageType> KMeansFilterType;
  
  // create the vars with ::Pointer smart pointers
  ImageType::Pointer myImage = ImageType::New();
  ReaderType::Pointer myReader = ReaderType::New(); 
  WriterType::Pointer myWriter = WriterType::New();
  KMeansFilterType::Pointer kmeansFilter = KMeansFilterType::New();

  //do something with the vars
  myReader->SetFileName(argv[1]);
  myReader->Update(); // Need to update things in ITK after setting them up
  myImage = myReader->GetOutput();
  float numClasses = std::stoi(argv[3]);
  std::vector<float> dataPoints;

    itk::ImageRegionIterator<ImageType> iterator(myImage, myImage->GetLargestPossibleRegion());

    while (!iterator.IsAtEnd())
    {
        dataPoints.push_back(iterator.Get());
        ++iterator;
    }

  kmeansFilter->SetInput(myImage);
  kmeansFilter->SetUseNonContiguousLabels(true);
 
  // std::vector<double> initialMeans = {0.0, 50.0, 150.0, 200.0, 250.0, 300.0};
  // for (int i = 0; i < initialMeans.size(); ++i) {
  //   kmeansFilter->AddClassWithInitialMean(initialMeans[i]);
  // }
  int n = 0;
  
 
  std::vector<float> initialMeans = kmeansPlusPlusInitialization(dataPoints, numClasses);
  for (unsigned k = 0; k < numClasses; ++k)
  {
    std::cout <<  " mean " << initialMeans[k] << std::endl;
    kmeansFilter->AddClassWithInitialMean(initialMeans[k]);
  }
  
  kmeansFilter -> Update();
  KMeansFilterType::ParametersType estimatedMeans = kmeansFilter->GetFinalMeans();

  const unsigned int numberOfClasses = estimatedMeans.Size();
  std::cout << numberOfClasses << std::endl;

  for (unsigned int i = 0; i < numberOfClasses; ++i)
  {
    std::cout << "cluster[" << i << "] ";
    std::cout << "    estimated mean : " << estimatedMeans[i] << std::endl;
  }
 
  typedef itk::RelabelComponentImageFilter <ImageType, ImageType> RelabelFilterType;

  auto relabeler = RelabelFilterType::New();

  relabeler->SetInput(kmeansFilter->GetOutput());

  typedef itk::RescaleIntensityImageFilter<ImageType, ImageType> RescaleFilterType;
  auto rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(relabeler->GetOutput());
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);

  
  //write result out to file
  //inculde
  //typedef
  //create variable
  //setup params
  
  myWriter->SetFileName(argv[2]);
  std::cout << "are you working" << std::endl;
  myWriter->SetInput(rescaleFilter-> GetOutput());
  
  //update
  myWriter->Update();
  
  return 0;
}
