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

#ifndef IQ_FILE_CONVERTER_H
#define IQ_FILE_CONVERTER_H

#include <memory>
#include <string>

#include "GnssMetadata.h"

class IQFileConverter
{
public:
  IQFileConverter();
  ~IQFileConverter();

  void convertIQFile(const std::string& inMetadataFilePath, const std::string& outMetadataFilePath);

private:
  class Pimpl;
  std::unique_ptr<Pimpl> m;

  bool verifyGnssMetadata(const GnssMetadata::Metadata& metadata);

  void extractSignalProperties();
  void writeOutputMetadata();
};

#endif // IQ_FILE_CONVERTER_H
