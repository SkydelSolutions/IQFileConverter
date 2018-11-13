#include "iq_file_converter.h"

#include <iostream>
#include <stdexcept>

#include "gnss_sdr_metadata_wrapper.h"

class IQFileConverter::Pimpl
{
public:
  GnssMetadata::Metadata inputMetadata;

  double inputSamplingRate;
  double inputCenterFrequency;

  QString inMetadataFilePath;
  QString outMetadataFilePath;
};

IQFileConverter::IQFileConverter()
    : m(new Pimpl())
{}

IQFileConverter::~IQFileConverter()
{}

void IQFileConverter::convertIQFile(const QString& inMetadataFilePath, const QString& outMetadataFilePath)
{
  m->inMetadataFilePath = inMetadataFilePath;
  m->outMetadataFilePath = outMetadataFilePath;

  // Load input metadata
  GnssMetadata::XmlProcessor xmlProcessor;
  if (!xmlProcessor.Load(inMetadataFilePath.toStdString().c_str(), false, m->inputMetadata))
  {
    std::string error = "File " + inMetadataFilePath.toStdString() + " was not found.";
    std::cout << error;
    throw new std::runtime_error(error);
  }

  if (!verifyGnssMetadata(m->inputMetadata))
  {
    throw new std::runtime_error("");
  }

  GnssSdrMetadataWrapper::convertIQData(m->inputMetadata, m->outMetadataFilePath);

  extractSignalProperties();
  writeOutputMetadata();
}

bool IQFileConverter::verifyGnssMetadata(const GnssMetadata::Metadata& metadata)
{
  if (metadata.Lanes().size() > 1)
  {
    QString error = "GNSS Metadata with more than 1 lane is not supported";
    std::cout << error.toStdString();
    return false;
  }

  GnssMetadata::Lane lane = metadata.Lanes().front();
  if (lane.Systems().size() > 1)
  {
    QString error = "GNSS Metadata with more than 1 system per lane is not supported";
    std::cout << error.toStdString();
    return false;
  }

  if (lane.Blocks().size() > 1)
  {
    QString error = "GNSS Metadata with more than 1 block per lane is not supported";
    std::cout << error.toStdString();
    return false;
  }

  GnssMetadata::Block block = lane.Blocks().front();
  if (block.Chunks().size() > 1)
  {
    QString error = "GNSS Metadata with more than 1 chunk per block is not supported";
    std::cout << error.toStdString();
    return false;
  }

  GnssMetadata::Chunk chunk = block.Chunks().front();
  if (chunk.Lumps().size() > 1)
  {
    QString error = "GNSS Metadata with more than 1 lump per chunk is not supported";
    std::cout << error.toStdString();
    return false;
  }

  GnssMetadata::Lump lump = chunk.Lumps().front();
  if (lump.Streams().size() > 1)
  {
    QString error = "GNSS Metadata with more than 1 stream per lump is not supported";
    std::cout << error.toStdString();
    return false;
  }

  GnssMetadata::IonStream stream = lump.Streams().front();
  if (stream.Bands().size() > 1)
  {
    QString error = "GNSS Metadata with more than 1 band per stream is not supported";
    std::cout << error.toStdString();
    return false;
  }

  if (metadata.Files().size() > 1)
  {
    QString error = "GNSS Metadata with more than 1 file is not supported";
    std::cout << error.toStdString();
    return false;
  }

  return true;
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
  chunk.Endian(GnssMetadata::Chunk::Little);
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
  QString outDataFilePath = m->outMetadataFilePath;
  outDataFilePath.replace(".xml", ".iq");
  file.Url(GnssMetadata::IonString(outDataFilePath.toStdString()));
  file.Lane(lane, true);

  //Assemble the Metadata object and write XML
  GnssMetadata::Metadata metadata;
  GnssMetadata::XmlProcessor xmlProcessor;
  metadata.Lanes().push_back(lane);
  metadata.Files().push_back(file);

  try
  {
    std::string metadataFilePath = m->outMetadataFilePath.toStdString();
    xmlProcessor.Save(metadataFilePath.c_str(), metadata);
  }
  catch(GnssMetadata::ApiException& e)
  {
    std::cout << "An error occurred while saving the xml file: " << e.what() << std::endl;
  }
}
