#include <iostream>

#include "iq_file_converter.h"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Wrong number of arguments: Expected 2 and received " << (argc - 1) << std::endl;
    std::cout << "  - Input metadata file path" << std::endl;
    std::cout << "  - Output metadata file path" << std::endl;
    return -1;
  }

  IQFileConverter fileConverter;
  fileConverter.convertIQFile(argv[1], argv[2]);

  return 0;
}
