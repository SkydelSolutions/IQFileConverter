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

#include "gnss_sdr_metadata_wrapper.h"

#include <QFile>
#include <iostream>
#include <stdexcept>

#include "gnss_sdr_pre_include.h"
#include "Converter.h"
#include "gnss_sdr_post_include.h"

void GnssSdrMetadataWrapper::convertIQData(GnssMetadata::Metadata& inputMetadata, QString outMetadataFilePath)
{
  // Read samples and convert them into a new IQ file
  SampleFrontEnd sampleReader;
  if (!sampleReader.Open<int16_t>(inputMetadata))
  {
    std::cout << "IQ binary file was not found." << std::endl;
  }
  sampleReader.Convert();

  // Convert IQ data
  QString outDataFilePath = outMetadataFilePath.replace(".xml", ".iq");
  if (QFile::exists(outDataFilePath))
  {
    std::string error = "File " + outDataFilePath.toStdString() + " already exists";
    std::cout << error << std::endl;
    throw std::runtime_error(error);
  }

  SampleFileSink<int16_t> sampleWriter(outDataFilePath.toStdString());
  std::map<std::string, std::pair<const SampleSource*, const SampleStreamInfo*>> sources = sampleReader.GetSources();

  for (auto it = sources.begin(); it != sources.end(); ++it)
  {
    std::string sourceName = it->first;
    const SampleSource* source = sampleReader.GetSource(sourceName);

    const int16_t* buffer;
    uint32_t samplesCount = source->GetSamples(buffer);

    // Write converted IQ binary data in a new file
    for (uint32_t i = 0; i < samplesCount; i += 2)
    {
      sampleWriter.AddSample(buffer[i], buffer[i + 1]);
    }
  }

  sampleWriter.Flush();
  sampleWriter.Close();
  sampleReader.Clear();
  sampleReader.Close();
}

void GnssSdrMetadataWrapper::writeBinarySamples(const QString& outDataFilePath, char* data, uint32_t bytesCount)
{
  BinaryFileSink file(outDataFilePath.toStdString());
  file.Put(data, bytesCount);

  file.Flush();
  file.Close();
}
