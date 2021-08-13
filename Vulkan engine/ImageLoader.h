#pragma once
#ifndef _IMAGE_LOADER_H_
#include <fstream>
#include <stdexcept>

class ImageLoader
{
public:
	static unsigned char* LoadTGA(std::string file, int &height,int &width, unsigned char& bpp);
	static unsigned char* LoadBMP(std::string file, int &height,int &width, unsigned char& bpp);
	private:
		struct TGAheader
		{
			unsigned char data1[12];
			unsigned short width;
			unsigned short height;
			unsigned char bpp;
			unsigned char data2;
		};
#pragma pack(push,1)
		struct BMPheader 
		{
			unsigned char data1[10];
			unsigned int offset;
			unsigned int DIBsize;
		};
#pragma pack(pop)
		struct BITMAPINFOHEADER
		{
			unsigned int width;
			unsigned int height;
			unsigned short colorPlanes;
			unsigned short bpp;
			unsigned int compression;
			unsigned char data[12];
			unsigned int palete;
			unsigned int importantColors;

		};

};

#endif // !_IMAGE_LOADER_H_

