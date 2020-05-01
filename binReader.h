/*
 * Filename: binReader.cc
 * Date: 4/30/2020
 * Author: Terrence Klock
 * Email: thk170000@utdallas.edu
 * Version 1.0
 * Copyright 2020, All Rights Reserved
 *
 * Description:
 *
 * Reads through a .bin file to give information to CDK.
 */

#include <iostream>
#include <stdint.h>

using namespace std;

class BinaryFileHeader
{
public:
  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;
};

  const int maxRecordStringLength = 25;

class BinaryFileRecord
{
public:
  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];
};


