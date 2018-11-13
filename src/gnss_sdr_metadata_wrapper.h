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
