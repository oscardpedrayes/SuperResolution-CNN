#ifndef __PATCH_H__
#define __PATCH_H__

#include "Image.h"

template <unsigned int PSIDE>
class Patch
{
public:
	// Constructors
	Patch();
	~Patch();

	// Initializers
	void randomInitialize(double sigma);
	void HeInitialize(double inputSize);
	void zeroInitialize();

	// Getters
	double** getImage() const;
	unsigned int const getSide() const;
	unsigned int const getSize() const;
	static const Patch<PSIDE>* getGauss();


	// Mixes with the specified Patch by a factor
	void setPatch(const Patch<PSIDE> &p, const double factor);


	// Sums Patch values to the local one
	void sumPatch(const Patch<PSIDE> &p);
	// Multiply Patch values to the local one
	void mulPatch(const Patch<PSIDE> &p);
	// Substracts Patch values to the local one
	void diffPatch(const Patch<PSIDE> &p);

	// Adds a value to a position of the image
	void addValue(const double val, const unsigned int i, const unsigned int j);
	// Adds a value to every position of the image
	void addAllValue(const double val);	
	// Set Value to all positions
	void setAllValue(const double value);
	// Multiply a value to every position of the image
	void mulAllBy(const double val);

	// Reads Patch from a Image
	void readPatch(const Image &img, const unsigned int i, const unsigned int j);
	// Sets Patch into a Image
	void writePatch(Image &image, const unsigned int i, const unsigned int j) const;
	// Adds Patch into a Image
	void addPatch(Image &image, const unsigned int i, const unsigned int j) const;



protected:
	static void initGaussian();

protected:

	double **imagePatch_;
	unsigned int sizePatch_;
	double avg_;
	double dev_;

	static Patch<PSIDE> *__gauss;
};
#include "Patch.cc"
#endif
