#pragma once
#include <iostream>
#include <fstream>

using namespace std;

struct Header
{
	char idLength;
	char colorMapType;
	char dataTypeCode;
	short colorMapOrigin;
	short colorMapLength;
	char colorMapDepth;
	short xOrigin;
	short yOrigin;
	short width;
	short height;
	char bitsPerPixel;
	char imageDescriptor;
	void Load(ifstream &file);
};

void Header::Load(ifstream &file)
{
	file.read(&idLength, sizeof(idLength));
	file.read(&colorMapType, sizeof(colorMapType));
	file.read(&dataTypeCode, sizeof(dataTypeCode));
	file.read((char*)&colorMapOrigin, sizeof(colorMapOrigin));
	file.read((char*)&colorMapLength, sizeof(colorMapLength));
	file.read(&colorMapDepth, sizeof(colorMapDepth));
	file.read((char*)&xOrigin, sizeof(xOrigin));
	file.read((char*)&yOrigin, sizeof(yOrigin));
	file.read((char*)&width, sizeof(width));
	file.read((char*)&height, sizeof(height));
	file.read(&bitsPerPixel, sizeof(bitsPerPixel));
	file.read(&imageDescriptor, sizeof(imageDescriptor));
}