#include "iq_file_converter_test.h"

#include <QFile>
#include <QTest>

#include "iq_file_converter.h"
#include "gnss_sdr_metadata_wrapper.h"

IQFileConverterTest::IQFileConverterTest()
{}

IQFileConverterTest::~IQFileConverterTest()
{}

void IQFileConverterTest::testIQFileConverter()
{
  // Prepare files for the unit test
  generateIQData();
  writeInputFile();
  writeInputMetadataFile();

  // Convert IQ data to ouput .iq file
  IQFileConverter fileConverter;
  fileConverter.convertIQFile("input_unit_test.xml", "output_unit_test.xml");

  // Verify that the IQ data has been writen correctly
  QFile outputDataFile("output_unit_test.iq");
  if (!outputDataFile.open(QIODevice::ReadOnly))
  {
    QFAIL("Could not open \"output_unit_test.iq\" file.");
  }

  char data[sizeof(IQ)];
  for (int i = 0; i < IQ_DATA_SIZE; i++)
  {
    outputDataFile.read(data, sizeof(IQ));

    int16_t I;
    *(reinterpret_cast<char*>(&I)) = data[0];
    *(reinterpret_cast<char*>(&I) + 1) = data[1];

    int16_t Q;
    *(reinterpret_cast<char*>(&Q)) = data[2];
    *(reinterpret_cast<char*>(&Q) + 1) = data[3];

    QCOMPARE(I, m_iqData[i].I);
    QCOMPARE(Q, m_iqData[i].Q);
  }

  // Close and delete unit test files
  outputDataFile.close();
  outputDataFile.remove();

  QFile outputMetadataFile("output_unit_test.xml");
  outputMetadataFile.remove();

  QFile inputMetadataFile("input_unit_test.xml");
  inputMetadataFile.remove();

  QFile inputDataFile("input_unit_test.gnss");
  inputDataFile.remove();
}

void IQFileConverterTest::generateIQData()
{
  double t = 0.0;

  for (int i = 0; i < IQ_DATA_SIZE; ++i)
  {
    double angle = 2.0 * M_PI * t;
    m_iqData[i].I = static_cast<int16_t>(std::sin(angle) * std::pow(2.0, 14.0));
    m_iqData[i].Q = static_cast<int16_t>(std::cos(angle) * std::pow(2.0, 14.0));

    t += M_PI / 2.0 / static_cast<double>(IQ_DATA_SIZE);
  }
}

void IQFileConverterTest::writeInputFile()
{
  const int bytesCount = sizeof(IQ) * IQ_DATA_SIZE;
  char data[bytesCount];
  const int sampleSize = sizeof(IQ);

  for (int i = 0; i < IQ_DATA_SIZE; ++i)
  {
    // Simulate big endian architecture
    data[i * sampleSize] = *(reinterpret_cast<char*>(&m_iqData[i].I) + 1);
    data[i * sampleSize + 1] = *(reinterpret_cast<char*>(&m_iqData[i].I));
    data[i * sampleSize + 2] = *(reinterpret_cast<char*>(&m_iqData[i].Q) + 1);
    data[i * sampleSize + 3] = *(reinterpret_cast<char*>(&m_iqData[i].Q));
  }

  GnssSdrMetadataWrapper::writeBinarySamples("input_unit_test.gnss", data, bytesCount);
}

void IQFileConverterTest::writeInputMetadataFile()
{
  // Define Band 1 and L1 C/A Stream.
  GnssMetadata::Band channel("Channel");
  channel.CenterFrequency(GnssMetadata::Frequency(1575.42, GnssMetadata::Frequency::MHz));
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
  system.BaseFrequency(GnssMetadata::Frequency(5, GnssMetadata::Frequency::MHz));

  GnssMetadata::Lane lane("RF-1");
  lane.Blocks().push_back(block);
  lane.Systems().push_back(system);

  // Define the file
  GnssMetadata::File file;
  file.Url(GnssMetadata::IonString("input_unit_test.gnss"));
  file.Lane(lane, true);

  //Assemble the Metadata object and write XML
  GnssMetadata::Metadata metadata;
  GnssMetadata::XmlProcessor xmlProcessor;
  metadata.Lanes().push_back(lane);
  metadata.Files().push_back(file);

  try
  {
    xmlProcessor.Save("input_unit_test.xml", metadata);
  }
  catch(GnssMetadata::ApiException& e)
  {
    std::string error = std::string("An error occurred while saving the input_unit_test.xml file: ") + e.what();
    QFAIL(error.c_str());
  }
}

QTEST_APPLESS_MAIN(IQFileConverterTest)
