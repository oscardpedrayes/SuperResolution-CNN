#include <stdio.h>
#include <errno.h>

#include "Image.h"

Image::Image() 
	:image_(nullptr)
	, width_(0)
	, heigth_(0)
	, size_(0)
{
}


Image::Image(const unsigned int w, const unsigned int h)
	: image_(nullptr)
	, width_(w)
	, heigth_(h)
	, size_(w*h)
{
	initializeImage();
}

Image::Image(const char* dir)
	: image_(nullptr)
	, width_(0)
	, heigth_(0)
	, size_(0)
{
	readImagePGM(dir);
	//mulAllValue(1.0 / 255.0);

}


Image::~Image()
{
	if (image_ != nullptr)
	{
		for (unsigned int i = 0; i < heigth_; ++i)
			delete[] image_[i];
		delete[] image_;
	}
}

void Image::resize(const unsigned int w, const unsigned int h)
{
	if ((width_ != w) || (heigth_ != h))
	{
		if (image_ != nullptr)
		{
			for (unsigned int i = 0; i < heigth_; ++i)
				delete[] image_[i];
			delete[] image_;
			image_ = nullptr;
		}
		width_ = w;
		heigth_ = h;
		size_ = w * h;
		initializeImage();
	}
}

//Reserva espacio para la imagen
void Image::initializeImage()
{
	if((width_ > 0) && (heigth_ > 0))
	{
		image_ = new double*[heigth_];
		for (unsigned int y = 0; y < heigth_; ++y)
		{
			image_[y] = new double[width_];
			for (unsigned int x = 0; x < width_; ++x)
			{
				image_[y][x] = 0;
			}
		}
	}
}

double** Image::getImage() const
{
	return image_;
}

unsigned int const Image::getWidth() const
{
	return width_;
}

unsigned int const Image::getHeight() const
{
	return heigth_;
}

unsigned int const Image::getSize()
{
	return size_;
}

void Image::setValue(const double value,const unsigned int i, const unsigned int j)
{
	if(i < heigth_ && j < width_ )
		image_[i][j]= value;
}

void Image::addValue(const double value,const unsigned int i,const unsigned int j)
{
	if (i < heigth_ && j < width_)
		image_[i][j] += value;
}

void Image::addAllValue(const double value)
{
	for (unsigned int y = 0; y < heigth_; ++y)	
		for (unsigned int x = 0; x < width_; ++x)
		
			image_[y][x] += value;		
}
void Image::mulAllValue(const double value)
{
	for (unsigned int y = 0; y < heigth_; ++y)
		for (unsigned int x = 0; x < width_; ++x)

			image_[y][x] *= value;
}

void Image::setAllValue(const double value)
{
	for (unsigned int y = 0; y < heigth_; ++y)
		for (unsigned int x = 0; x < width_; ++x)
			image_[y][x] = value;
}

void Image::diffImages(Image &img2)
{
	for (unsigned int y = 0; y < heigth_; ++y)
		for (unsigned int x = 0; x < width_; ++x)
			image_[y][x] -= img2.image_[y][x];
}

void Image::sumImages(Image &img2)
{
	for (unsigned int y = 0; y < heigth_; ++y)
		for (unsigned int x = 0; x < width_; ++x)
			image_[y][x] += img2.image_[y][x];
}

void Image::readImagePGM(const char* dir)
{
	unsigned char *imgBuffer=nullptr;
	FILE *fich;
	errno_t err;

	//Se abre el fichero de la imagen
	if ((err = fopen_s(&fich, dir, "rb")) != 0)
	{
		printf("ERROR: cannot open file '%s'\n", dir);
	}
	else
	{
		//Se obtiene ancho y alto
		if (fscanf_s(fich, "P5\n%u %u\n255\n", &width_, &heigth_) == 2)
		{
			size_ = width_ * heigth_;
			initializeImage();
			//Se reserva espacio para la imagen
			imgBuffer = new unsigned char[width_*heigth_];
			//TODO: VALIDAR ancho y alto con el que espera la red

			//Se lee la imagen
			fread(imgBuffer, size_, 1, fich);

			//Se convierte a double
			for (unsigned int i = 0; i < heigth_; i++)
			{
				for (unsigned int j = 0; j < width_; j++)
				{
					const double value = imgBuffer[i*width_ + j];
					image_[i][j] = value/255.0;//(value - 127.5) / 255.0;
				}
			}	
		}
		else
		{
			printf("ERROR:Unknown image format\n");
		}
		fclose(fich);
	}
	if(imgBuffer!=nullptr)
		delete[] imgBuffer;
}

void Image::readImage(const char* dir)
{
	FILE *fich;
	errno_t err;

	//Se abre el fichero de la imagen
	if ((err = fopen_s(&fich, dir, "rb")) != 0)
	{
		printf("ERROR: cannot open file '%s'\n", dir);

	}
	else
	{
		//Se obtiene ancho y alto
		if (fscanf_s(fich, "%u %u\n", &width_, &heigth_) == 2)
		{
			size_ = width_ * heigth_;
			initializeImage();
		
			//Se lee la imagen
			for (unsigned int i = 0; i < heigth_; i++)
			{
				fread(image_[i], sizeof(double), width_, fich);
			}
		}
		else
		{
			printf("ERROR:Unknown image format\n");
		}
		fclose(fich);
	}
}

void Image::exportImagePGM(const char* path)
{
	FILE *file = NULL;
	fopen_s(&file, path, "wb");
	if (file)
	{
		unsigned char byte;
		fprintf(file, "P5\n%u %u\n255\n", width_, heigth_);
		for (unsigned int y = 0; y < heigth_; ++y)
		{
			for (unsigned int x = 0; x < width_; ++x)
			{
				byte = (unsigned char)(clip(image_[y][x], 0, 255));
				fwrite(&byte, 1, 1, file);
			}

		}
		fclose(file);
	}
}

void Image::exportImage(const char* path)
{
	FILE *file = NULL;
	fopen_s(&file, path, "wb");
	if (file)
	{
		fprintf(file, "%u %u\n", width_, heigth_);
		for (unsigned int y = 0; y < heigth_; ++y)
		{
			fwrite(image_[y], 1, width_, file);
		}
		fclose(file);
	}
}

double Image::clip(const double v,const double min,const double max)
{
	if (v >= max)
		return max;
	if (v <= min)
		return min;
	return v;
}

