#include "ImageLoader.h"

unsigned char* ImageLoader::LoadTGA(std::string file, int& height, int& width,unsigned char& bpp,bool addAlpha )
{
	char footer[26];

	std::ifstream ifs(file, std::ifstream::binary);
	if(!ifs.is_open())
		throw std::runtime_error("Could not open file");
	ifs.seekg(-26,ifs.end  );
	ifs.read(footer, 26);
	ifs.close();
	TGAfooter Tfooter;
	memcpy(&Tfooter, footer, 26);

	std::FILE *f;
	//Open file
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
	unsigned int imageSize = width * height * (info.bpp/8);
	//Read useless data
	if (Tfooter.ExtOffset + Tfooter.DevOffset != 0)
	{
		unsigned char* UselessData = new unsigned char[Tfooter.ExtOffset + Tfooter.DevOffset - 18];
		fread(UselessData, 1, sizeof(UselessData), f);
	}

	unsigned char* TGAdata = new unsigned char[imageSize];
	//Read data
	auto count = fread(TGAdata, 1, imageSize, f);
	if( count != imageSize)
		throw std::runtime_error("Could not read file data");
	//Close file
	auto a = fclose(f);
 	if (a != 0)
		throw std::runtime_error("Could not close file");
	//tbh idk what this does lol
	if (!addAlpha|| (addAlpha && bpp  == 32) )
	{
		int index = 0;
		int k = (width * height * bpp / 4) - (width * bpp / 4);
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
				k += bpp / 4;
				index += bpp / 4;
			}
			k -= (width * (bpp / 4 * 2));
		}
		delete[] TGAdata;
		TGAdata = 0;
		return output;

	}
	else if(addAlpha&& bpp != 32)
	{
		int index = 0;
		int k = (width * height * bpp /8) - (width * bpp/8);
		unsigned char* output = new unsigned char[width * height * 4];

		for (size_t j = 0; j < height; j++)
		{
			for (size_t i = 0; i < width; i++)
			{
				output[index + 0] = TGAdata[k + 2];
				output[index + 1] = TGAdata[k + 1];
				output[index + 2] = TGAdata[k + 0];
				output[index + 3] = 1;
				k += 3;
				index +=  4;
			}
			k -= (width * (6));
		}
		delete[] TGAdata;
		TGAdata = 0;
		return output;
	}
	

}

unsigned char* ImageLoader::LoadBMP(std::string file, int& height, int& width, unsigned char& bpp, bool switchRB )
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

	unsigned int* palete = new unsigned int[header.palete];
	if (fread(&palete, 4, header.palete, f) != header.palete)
		throw std::runtime_error("Could not read palete");
	delete[] palete;

	height = header.height;
	width = header.width;
	bpp = header.bpp;
	unsigned int RowSize = ((header.bpp * header.width) / 32) * 4;
	unsigned int PixelArraySize = RowSize * header.height;
	if(RowSize != (width * (bpp /8)))
		throw std::runtime_error("Bit map padding is not supported");
	//unsigned char* output = new unsigned char[PixelArraySize];
	unsigned char* pixels = new unsigned char[header.imgSize];
	auto count = fread(pixels, 1, header.imgSize, f);
	if (count != header.imgSize)
		throw std::runtime_error("Could not read pixel array");
	fclose(f);
	if(switchRB)
	{
		unsigned char* output = new unsigned char[header.imgSize];
		for (size_t k = 0; k < header.imgSize-4;k+=4) 
		{


			output[k + 0] = pixels[k + 2];
			output[k + 1] = pixels[k + 1];
			output[k + 2] = pixels[k + 0];
			output[k + 3] = pixels[k + 3];

		}
		delete[] pixels;
		return output;
	}
	else	
	return pixels;
}
