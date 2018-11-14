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

  try
  {
    IQFileConverter fileConverter;
    fileConverter.convertIQFile(argv[1], argv[2]);
  }
  catch (...)
  {
    return -1;
  }

  return 0;
}
