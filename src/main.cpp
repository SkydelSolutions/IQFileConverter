#include "iq_file_converter.h"

int main(int /* argc */, char* argv[])
{
  IQFileConverter fileConverter;
  fileConverter.convertIQFile(argv[1], argv[2]);

  return 0;
}
