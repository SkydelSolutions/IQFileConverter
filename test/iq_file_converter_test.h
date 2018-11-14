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
