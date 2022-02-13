#ifndef __IMAGE_H__
#define __IMAGE_H__

//#include "Patch.h"

class Image
{
public:
	Image();
	// Creates a Image with the specified dimensions
	Image(const unsigned int w, const unsigned int h);
	//Reads a Image from a directory
	Image(const char* dir);

	~Image();

	void resize(const unsigned int w, const unsigned int h);

	// Getters
	double** getImage() const;
	unsigned int const getWidth() const;
	unsigned int const getHeight() const;
	unsigned int const getSize();

	// Sets a especified value on a position of the image
	void setValue(const double value, const unsigned int i , const unsigned int j);
	// Adds a value to a element
	void addValue(const double value, const unsigned int i, const unsigned int j);
	// Adds a value to all elements
	void addAllValue(const double value);
	void setAllValue(const double value);
	void mulAllValue(const double value);
	// Saves the difference between images 
	void diffImages(Image &img);
	void sumImages(Image &img);

	// Reads image .pgm from directory
	void readImagePGM(const char* dir);
	// Reads image from directory
	void readImage(const char* dir);
	// Exports image to disk as a .pgm file
	void exportImagePGM(const char* path);
	// Exports image to disk
	void exportImage(const char* path);


protected:
	// Allocates space for the image
	void initializeImage();
	// Ensures that a value doesnt get out of range
	double clip(const double v,const double min,const double max);

protected:

	double **image_;
	unsigned int width_;
	unsigned int heigth_;
	unsigned int size_;
};

#endif
