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

#ifndef GNSS_SDR_METADATA_WRAPPER_H
#define GNSS_SDR_METADATA_WRAPPER_H

#include <QString>

#include "GnssMetadata.h"

namespace GnssSdrMetadataWrapper
{

void convertIQData(GnssMetadata::Metadata& inputMetadata, QString outFilePath);
void writeBinarySamples(const QString& outFilePath, char* data, uint32_t samplesCount);

}

#endif // GNSS_SDR_METADATA_WRAPPER_H
