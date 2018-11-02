#include "gnss_sdr_metadata_wrapper.h"

#include <iostream>

#include "gnss_sdr_pre_include.h"
#include "Converter.h"
#include "gnss_sdr_post_include.h"

GnssSdrMetadataWrapper::GnssSdrMetadataWrapper()
{}

GnssSdrMetadataWrapper::~GnssSdrMetadataWrapper()
{}

void GnssSdrMetadataWrapper::convertIQData(GnssMetadata::Metadata& inputMetadata, const QString& outFilePath)
{
  // Read samples and convert them into a new IQ file
  SampleFrontEnd sampleReader;
  if (!sampleReader.Open<int16_t>(inputMetadata))
  {
    std::cout << "IQ binary file was not found." << std::endl;
  }
  sampleReader.Convert();

  SampleFileSink<int16_t> sampleWriter(outFilePath.toStdString());
  std::map<std::string, std::pair<const SampleSource*, const SampleStreamInfo*>> sources = sampleReader.GetSources();

  for (auto it = sources.begin(); it != sources.end(); ++it)
  {
    std::string sourceName = it->first;
    const SampleSource* source = sampleReader.GetSource(sourceName);

    const int16_t* buffer;
    int samplesCount = source->GetSamples(buffer);

    // Write converted IQ binary data in a new file
    for (int i = 0; i < samplesCount; i += 2)
    {
      sampleWriter.AddSample(buffer[i], buffer[i + 1]);
    }
  }

  sampleWriter.Flush();
  sampleWriter.Close();
  sampleReader.Clear();
  sampleReader.Close();
}

void GnssSdrMetadataWrapper::writeBinarySamples(const QString& outFilePath, char* data, int bytesCount)
{
  BinaryFileSink file(outFilePath.toStdString());
  file.Put(data, bytesCount);

  file.Flush();
  file.Close();
}
