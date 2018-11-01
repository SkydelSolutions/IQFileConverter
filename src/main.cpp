#include <QCoreApplication>

#include "gnss_sdr_pre_include.h"
#include "GnssMetadata.h"
#include "Converter.h"
#include "gnss_sdr_post_include.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}
