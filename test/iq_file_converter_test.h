#ifndef IQ_FILE_CONVERTER_TEST_H
#define IQ_FILE_CONVERTER_TEST_H

#include <QObject>

class IQFileConverterTest : public QObject
{
  Q_OBJECT

public:
    IQFileConverterTest();
    ~IQFileConverterTest();

private slots:
    void testIQFileConverter();

private:
    struct IQ {
      int16_t I;
      int16_t Q;
    };

    static const int IQ_DATA_SIZE = 25000;
    IQ m_iqData[IQ_DATA_SIZE];

    void generateIQData();
    void writeInputFile();
    void writeInputMetadataFile();
};

#endif // IQ_FILE_CONVERTER_TEST_H
