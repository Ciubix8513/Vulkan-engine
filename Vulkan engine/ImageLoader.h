#pragma once
#ifndef _IMAGE_LOADER_H_
#include <fstream>
#include <stdexcept>

class ImageLoader
{
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

};

#endif // !_IMAGE_LOADER_H_

