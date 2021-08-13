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
	int k = (width * height * bpp) - (width * bpp);
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
			k += bpp;
			index += bpp;
		}
		k -= (width * (bpp * 2));
	}
	delete [] TGAdata;
	TGAdata = 0;

	return output;
}
