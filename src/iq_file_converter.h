#ifndef IQ_FILE_CONVERTER_H
#define IQ_FILE_CONVERTER_H

#include <QString>
#include <memory>

class IQFileConverter
{
public:
    IQFileConverter();
    ~IQFileConverter();

    void convertIQFile(const QString& inFilePath, const QString& outFilePath);

private:
    class Pimpl;
    std::unique_ptr<Pimpl> m;

    void extractSignalProperties();
    void writeOutputMetadata();
};

#endif // IQ_FILE_CONVERTER_H
