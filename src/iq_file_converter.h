#ifndef IQ_FILE_CONVERTER_H
#define IQ_FILE_CONVERTER_H

#include <QString>
#include <memory>

#include "GnssMetadata.h"

class IQFileConverter
{
public:
    IQFileConverter();
    ~IQFileConverter();

    void convertIQFile(const QString& inMetadataFilePath, const QString& outMetadataFilePath);

private:
    class Pimpl;
    std::unique_ptr<Pimpl> m;

    bool verifyGnssMetadata(const GnssMetadata::Metadata& metadata);

    void extractSignalProperties();
    void writeOutputMetadata();
};

#endif // IQ_FILE_CONVERTER_H
