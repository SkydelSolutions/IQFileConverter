#ifndef GNSS_SDR_METADATA_WRAPPER_H
#define GNSS_SDR_METADATA_WRAPPER_H

#include <QString>

#include "GnssMetadata.h"

class GnssSdrMetadataWrapper
{
public:
    ~GnssSdrMetadataWrapper();

    static void convertIQData(GnssMetadata::Metadata& inputMetadata, QString outFilePath);
    static void writeBinarySamples(const QString& outFilePath, char* data, int samplesCount);

private:
    GnssSdrMetadataWrapper();
};

#endif // GNSS_SDR_METADATA_WRAPPER_H
