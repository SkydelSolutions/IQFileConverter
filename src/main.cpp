#include <iostream>

#include "gnss_sdr_pre_include.h"
#include "GnssMetadata.h"
#include "Converter.h"
#include "gnss_sdr_post_include.h"

void convertIQData(std::string outFilePath, GnssMetadata::Metadata& metadata)
{
  // Read samples and convert them into a new IQ file
  SampleFrontEnd sampleReader;
  if (!sampleReader.Open<int16_t>(metadata))
  {
    std::cout << "IQ binary file was not found." << std::endl;
  }
  sampleReader.Convert();

  SampleFileSink<int16_t> sampleWriter(outFilePath);
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

void loadSignalValues(GnssMetadata::Metadata& metadata, double& samplingRate, double& centerFrequency)
{
  GnssMetadata::Lane lane = metadata.Lanes().front();
  GnssMetadata::System system = lane.Systems().front();
  GnssMetadata::Frequency baseSampleRate = system.BaseFrequency();

  GnssMetadata::IonStream stream = lane.Blocks().front().Chunks().front().Lumps().front().Streams().front();
  samplingRate = static_cast<double>(stream.RateFactor()) * baseSampleRate.toHertz();

  GnssMetadata::Band band = stream.Bands().front();
  centerFrequency = band.CenterFrequency().toHertz();
}

void writeSdxMetadata(std::string outFilePath, double samplingRate, double centerFrequency)
{
  GnssMetadata::Band channel("Channel");
  channel.CenterFrequency(GnssMetadata::Frequency(centerFrequency, GnssMetadata::Frequency::Hz));
  channel.TranslatedFrequency(GnssMetadata::Frequency(0, GnssMetadata::Frequency::Hz));

  GnssMetadata::IonStream stream("Stream");
  stream.RateFactor(1);
  stream.Quantization(16);
  stream.Packedbits(32);
  stream.Encoding("INT16");
  stream.Format(GnssMetadata::IonStream::IQ);
  stream.Bands().push_back(channel);

  // Define the lane
  GnssMetadata::Lump lump;
  lump.Streams().push_back(stream);

  GnssMetadata::Chunk chunk;
  chunk.SizeWord(2);
  chunk.CountWords(2);
  chunk.Endian(GnssMetadata::Chunk::Big);
  chunk.Lumps().push_back(lump);

  GnssMetadata::Block block(256);
  block.Chunks().push_back(chunk);

  // Define the system
  GnssMetadata::System system("System");
  system.BaseFrequency(GnssMetadata::Frequency(samplingRate, GnssMetadata::Frequency::Hz));

  GnssMetadata::Lane lane("RF-1");
  lane.Blocks().push_back(block);
  lane.Systems().push_back(system);

  // Define the file
  GnssMetadata::File file;
  file.Url(GnssMetadata::IonString(outFilePath));
  file.Lane(lane, true);

  //Assemble the Metadata object and write XML
  GnssMetadata::Metadata metadata;
  GnssMetadata::XmlProcessor xmlProcessor;
  metadata.Lanes().push_back(lane);
  metadata.Files().push_back(file);

  try
  {
    std::string metadataFilePath = outFilePath + ".xml";
    xmlProcessor.Save(metadataFilePath.c_str(), metadata);
  }
  catch(GnssMetadata::ApiException& e)
  {
    std::cout << "An error occurred while saving the xml file: " << e.what() << std::endl;
  }
}

int main(int /* argc */, char* argv[])
{
  // Load metadata
  GnssMetadata::Metadata metadata;
  GnssMetadata::XmlProcessor xmlProcessor;

  if (!xmlProcessor.Load(argv[1], false, metadata))
  {
    std::cout << "File " << argv[1] << " was not found.";
    return -1;
  }

  // Convert IQ data to new file
  convertIQData(argv[2], metadata);

  // Query values from user's metadata file
  double samplingRateHz;
  double centerFrequencyHz;
  loadSignalValues(metadata, samplingRateHz, centerFrequencyHz);

  // Write new metadata file
  writeSdxMetadata(argv[2], samplingRateHz, centerFrequencyHz);

  return 0;
}
