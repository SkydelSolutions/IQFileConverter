////////////////////////////////////////////////////////////////////////////////
// IQFileConverter
// Copyright (c) Skydel Solutions Inc, All rights reserved.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library. See lgpl-3.0.txt
////////////////////////////////////////////////////////////////////////////////

#include "iq_file_converter_test.h"

#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "gnss_sdr_metadata_wrapper.h"
#include "iq_file_converter.h"

IQFileConverterTest::IQFileConverterTest()
{
}

IQFileConverterTest::~IQFileConverterTest()
{
}

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
  const std::string outputDataPath {"output_unit_test.iq"};
  std::ifstream outputDataFile(outputDataPath);
  if (!outputDataFile.is_open())
  {
    std::cerr << "Could not open \"output_unit_test.iq\" file." << std::endl;
    std::abort();
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

    if (I != m_iqData[i].I)
    {
      std::cerr << "I != m_iqData[" << i << "].I: " << I << " != " << m_iqData[i].I << std::endl;
      std::abort();
    }
    if (Q != m_iqData[i].Q)
    {
      std::cerr << "Q != m_iqData[" << i << "].Q: " << Q << " != " << m_iqData[i].Q << std::endl;
      std::abort();
    }
  }

  // Close and delete unit test files
  outputDataFile.close();
  std::filesystem::remove(outputDataPath);

  std::filesystem::remove("output_unit_test.xml");

  std::filesystem::remove("input_unit_test.xml");

  std::filesystem::remove("input_unit_test.gnss");
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

  // Assemble the Metadata object and write XML
  GnssMetadata::Metadata metadata;
  GnssMetadata::XmlProcessor xmlProcessor;
  metadata.Lanes().push_back(lane);
  metadata.Files().push_back(file);

  try
  {
    xmlProcessor.Save("input_unit_test.xml", metadata);
  }
  catch (GnssMetadata::ApiException& e)
  {
    std::cerr << "An error occurred while saving the input_unit_test.xml file: " << e.what() << std::endl;
    std::abort();
  }
}

int main()
{
  IQFileConverterTest test;
  test.testIQFileConverter();
  return EXIT_SUCCESS;
}
