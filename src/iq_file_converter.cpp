#include "iq_file_converter.h"

#include <iostream>

#include "gnss_sdr_metadata_wrapper.h"

class IQFileConverter::Pimpl
{
public:
  GnssMetadata::Metadata inputMetadata;

  double inputSamplingRate;
  double inputCenterFrequency;

  QString inFilePath;
  QString outFilePath;
};

IQFileConverter::IQFileConverter()
    : m(new Pimpl())
{}

IQFileConverter::~IQFileConverter()
{}

void IQFileConverter::convertIQFile(const QString& inFilePath, const QString& outFilePath)
{
  m->inFilePath = inFilePath;
  m->outFilePath = outFilePath;

  // Load input metadata
  GnssMetadata::XmlProcessor xmlProcessor;
  if (!xmlProcessor.Load(inFilePath.toStdString().c_str(), false, m->inputMetadata))
  {
    std::cout << "File " << inFilePath.toStdString() << " was not found.";
  }

  GnssSdrMetadataWrapper::convertIQData(m->inputMetadata, m->outFilePath);

  extractSignalProperties();
  writeOutputMetadata();
}

void IQFileConverter::extractSignalProperties()
{
  GnssMetadata::Lane lane = m->inputMetadata.Lanes().front();
  GnssMetadata::System system = lane.Systems().front();
  GnssMetadata::Frequency baseSampleRate = system.BaseFrequency();

  GnssMetadata::IonStream stream = lane.Blocks().front().Chunks().front().Lumps().front().Streams().front();
  m->inputSamplingRate = static_cast<double>(stream.RateFactor()) * baseSampleRate.toHertz();

  GnssMetadata::Band band = stream.Bands().front();
  m->inputCenterFrequency = band.CenterFrequency().toHertz();
}

void IQFileConverter::writeOutputMetadata()
{
  GnssMetadata::Band channel("Channel");
  channel.CenterFrequency(GnssMetadata::Frequency(m->inputCenterFrequency, GnssMetadata::Frequency::Hz));
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
  system.BaseFrequency(GnssMetadata::Frequency(m->inputSamplingRate, GnssMetadata::Frequency::Hz));

  GnssMetadata::Lane lane("RF-1");
  lane.Blocks().push_back(block);
  lane.Systems().push_back(system);

  // Define the file
  GnssMetadata::File file;
  file.Url(GnssMetadata::IonString(m->outFilePath.toStdString()));
  file.Lane(lane, true);

  //Assemble the Metadata object and write XML
  GnssMetadata::Metadata metadata;
  GnssMetadata::XmlProcessor xmlProcessor;
  metadata.Lanes().push_back(lane);
  metadata.Files().push_back(file);

  try
  {
    std::string metadataFilePath = m->outFilePath.toStdString() + ".xml";
    xmlProcessor.Save(metadataFilePath.c_str(), metadata);
  }
  catch(GnssMetadata::ApiException& e)
  {
    std::cout << "An error occurred while saving the xml file: " << e.what() << std::endl;
  }
}
