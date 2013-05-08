#include "HMap.h"
#include "defs.h"
#include "DebugConsole.h"
#include <fstream>
using namespace std;

HMap::HMap(char * filename, float scale) {
	//Input stream for reading in the bitmap
	ifstream fin;

	//Create header structures
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER info;

	//Initialize pointers, and create a place for pixel data to be stored
	BYTE * pixelData = NULL;
	_hdata = 0;

	// attempt to open the file using binary access
	fin.open(filename, ios::binary);

	if(fin.is_open()) {
		// read in the file info 
		fin.read((char *)(&header), sizeof(BITMAPFILEHEADER));
		fin.read((char *)(&info), sizeof(BITMAPINFOHEADER));

		// create an array that can take the pixel data
		pixelData = new BYTE[info.biSizeImage];

		//We want to store the actual heights
		_hdata = new float[info.biWidth * info.biHeight];

		// read the pixels
		fin.read((char *)(pixelData), info.biSizeImage);

		// close the file
		fin.close();

		// now, the hard part - we need to get the BGR data to RGBA
		BYTE * src(pixelData);
		float * dst(_hdata);

		//Pixel rows are padded to a full set of words
		int paddingSize = info.biWidth % 4;

		//Convert 24-bit rgb pixels to floating-point heights
		for(int index(0); index <  info.biWidth * info.biHeight; index++) {
			// Convert the average rgb value from 0-255 to an arbitrary floating-point scale
			*(dst) = scale * ((*(src + 2)) + (*(src + 1)) + (*(src + 0))) / 3.0f;

			//Skip padding bytes
			if((index) % info.biWidth == info.biWidth - 1) {
				src += paddingSize;
			}

			//get the next element in each array
			src += 3;
			dst++;
		}

		delete [] pixelData;
	} else {
		//File not found
		DC::get()->print("ERROR: Heightmap file %s not found\n", filename);
	}
}


HMap::~HMap(void) {
	if(_hdata) {
		delete [] _hdata;
	}
}


float HMap::getHeightAt(int i, int j) {
	return _hdata[i + j * _w];
}
