#include "ImageLoader.h"

unsigned char* ImageLoader::LoadTGA(std::string file, int& height, int& width,unsigned char& bpp )
{
	//open file
	std::FILE *f;
	//Check if it's open
	if (fopen_s(&f,file.c_str(),"rb")!= 0)
		throw std::runtime_error("Could not open file");
	TGAheader info;
	//Read file info
	if (fread(&info, sizeof(TGAheader), 1, f) != 1)
		throw std::runtime_error("Could not read file information");
	height = info.height;
	width = info.width;
	bpp = info.bpp;
	if (info.bpp < 24)
		throw std::runtime_error("Pixel depth less than 24 is not supported");	
	unsigned int imageSize = width * height * info.bpp;
	unsigned char* TGAdata = new unsigned char[imageSize];
	//Read data
	if(fread(TGAdata,1,imageSize,f) != imageSize)
		throw std::runtime_error("Could not read file data");
	//Close file
	if (fclose(f) != 0)
		throw std::runtime_error("Could not close file");
	//tbh idk what this does lol
	int index = 0;	
	int k = (width * height * bpp/4) - (width * bpp/4);
	unsigned char* output = new unsigned char[imageSize];

	for (size_t j = 0; j < height; j++)
	{
		for (size_t i = 0; i < width; i++)
		{
			output[index + 0] = TGAdata[k + 2];
			output[index + 1] = TGAdata[k + 1];
			output[index + 2] = TGAdata[k + 0];
			if (bpp == 32)
				output[index + 3] = TGAdata[k + 3];
			k += bpp/4;
			index += bpp/4;
		}
		k -= (width * (bpp/4 * 2));
	}
	delete [] TGAdata;
	TGAdata = 0;

	return output;
}

unsigned char* ImageLoader::LoadBMP(std::string file, int& height, int& width, unsigned char& bpp)
{
	//open file
	std::FILE* f;
	//Check if it's open
	if (fopen_s(&f, file.c_str(), "rb") != 0)
		throw std::runtime_error("Could not open file");
	BMPheader info;	
	//Read file info
	if (fread(&info, sizeof(BMPheader), 1, f) != 1)
		throw std::runtime_error("Could not read file information");
	if(info.DIBsize != 40)
		throw std::runtime_error("Bit map header is not suported");
	//Read bit map header
	BITMAPINFOHEADER header;
	if (fread(&header, sizeof(BITMAPINFOHEADER), 1, f) != 1)
		throw std::runtime_error("Could not read file information");
	if(header.compression != 0)
		throw std::runtime_error("Bit map compresion is not suported");
	if (info.offset !=( header.palete * 4) + 54)
		throw std::runtime_error("Found unknown data");
	height = header.height;
	width = header.width;
	bpp = header.bpp;
	unsigned int RowSize = ((header.bpp * header.width) / 32) * 4;
	unsigned int PixelArraySize = RowSize * header.height;
	if(RowSize != width)
		throw std::runtime_error("Bit map padding is not supported");
	unsigned char* output = new unsigned char[PixelArraySize];
	if (fread(&output, 1, PixelArraySize, f) != 1)
		throw std::runtime_error("Could not read pixel array");
	return output;
}
