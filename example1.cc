/*
 * Usage of CDK Matrix
 *
 * File:   example1.cc
 * Author: Stephen Perkins
 * Email:  stephen.perkins@utdallas.edu
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

  ifstream inBin ("/scratch/perkins/cs3377.bin", ios::in | ios::binary);
  
  if(!inBin)
    {
      return 0;
    }

  inBin.read((char*) bfh, sizeof(BinaryFileHeader));

  if(bfh->magicNumber != 4277009102)
    {
      cout << "Error: wrong magic number." << endl;
    }

  stringstream magicstream;
  magicstream << "Magic: 0x"  << std::hex << bfh->magicNumber;
  string magic(magicstream.str());

  setCDKMatrixCell(myMatrix, 1, 1, magic.c_str()); 

  stringstream versionstream;
  versionstream << "Version: " << bfh->versionNumber;
  string version(versionstream.str());

  setCDKMatrixCell(myMatrix, 1, 2, version.c_str());

  stringstream recordsstream;
  recordsstream << "NumRecords: " << bfh->numRecords;
  string records(recordsstream.str());

  setCDKMatrixCell(myMatrix, 1, 3, records.c_str());

  inBin.close();

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);

  

  drawCDKMatrix(myMatrix, true);    /* required  */


  while (!iskeypressed(500))
    {}
  // Cleanup screen
  endCDK();


}
