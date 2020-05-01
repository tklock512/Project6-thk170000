/*
 * Usage of CDK Matrix
 *
 * File:   example1.cc
 * Author: Stephen Perkins
 * Email:  stephen.perkins@utdallas.edu
 *
 * Adding Bin reader and expanding CDK
 * Author: Terrence Klock
 * Email: thk170000@utdallas.edu
 * Date: 4/30/2020
 * 
 * For cs3377.501
 *
 * Description:
 * 
 * Added code to input a .bin file and output the information in the CDK matrix. Also adjusted MATRIX_WIDTH and BOX_WIDTH.
 */

#include <iostream>
#include <fstream>
#include "cdk.h"
#include "binReader.h"
#include <poll.h>
#include <stdint.h>
#include <sstream>
#include <string>

#define MATRIX_WIDTH 5
#define MATRIX_HEIGHT 3
#define BOX_WIDTH 25
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

//function for detecting a keyboard press
bool iskeypressed(unsigned timeout_ms = 0)
{ 
  struct pollfd pls[1];
  pls[0].fd = STDIN_FILENO;
  pls[0].events = POLLIN | POLLPRI;
  return poll(pls, 1, timeout_ms) > 0;
}

int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  const char 		*rowTitles[MATRIX_HEIGHT+1] = {"0", "a", "b", "c"};
  const char 		*columnTitles[MATRIX_WIDTH+1] = {"0", "a", "b", "c", "d", "e"};
  int		boxWidths[MATRIX_WIDTH+1] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[MATRIX_WIDTH+1] = {vMIXED, vMIXED, vMIXED, vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT,
			  MATRIX_NAME_STRING, (char **) columnTitles, (char **) rowTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }


  /* Fetch binary header */
  BinaryFileHeader *bfh = new BinaryFileHeader();

  BinaryFileRecord *rec1 = new BinaryFileRecord();

  ifstream inBin ("/scratch/perkins/cs3377.bin", ios::in | ios::binary);

  //if file was not opened, exit out
  if(!inBin)
    {
      cout << "Bin file could not be opened.\n";
      return 0;
    }

  inBin.read((char*) bfh, sizeof(BinaryFileHeader));

  //incorrect magic number check
  if(bfh->magicNumber != 4277009102)
    {
      cout << "Error: wrong magic number." << endl;
      inBin.close();
      return 0;
    }

  //change int to hex string for magic number
  stringstream magicstream;
  magicstream << "Magic: 0x"  << std::hex << bfh->magicNumber;
  string magic(magicstream.str());

  setCDKMatrixCell(myMatrix, 1, 1, magic.c_str()); 

  //change int to string for version number
  stringstream versionstream;
  versionstream << "Version: " << bfh->versionNumber;
  string version(versionstream.str());

  setCDKMatrixCell(myMatrix, 1, 2, version.c_str());

  //change int to string for number of records
  stringstream recordsstream;
  recordsstream << "NumRecords: " << bfh->numRecords;
  string records(recordsstream.str());

  setCDKMatrixCell(myMatrix, 1, 3, records.c_str());

  //loop to input strings for all rows
  
  int numRead = (bfh->numRecords < 4) ? bfh->numRecords : 4;

  for(int i = 0; i < numRead; i++)
    {
      inBin.read((char*)rec1, sizeof(BinaryFileRecord));

      setCDKMatrixCell(myMatrix, i+2, 2, rec1->stringBuffer);

      stringstream rec1stream;
      rec1stream << "strLength: " << (int)rec1->strLength;
      string rec1ln(rec1stream.str());

      setCDKMatrixCell(myMatrix, i+2,1, rec1ln.c_str());
    }
  
  inBin.close();

  /* Display the Matrix */

  drawCDKMatrix(myMatrix, true);    /* required  */

  //work until key is pressed
  while (!iskeypressed(500))
    {}
  // Cleanup screen
  endCDK();


}
