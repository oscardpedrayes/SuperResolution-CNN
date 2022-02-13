#ifndef __UPSAMPLING_H__
#define __UPSAMPLING_H__

#include "Image.h"

template <unsigned int COUNT, unsigned int ISIDE, unsigned int OSIDE>
class Upsampling
{
public:
	Upsampling();
	~Upsampling();

	Patch<OSIDE>* getOutput();
	Patch<ISIDE>* getInputError();

	void train(Patch<OSIDE> errOutput[COUNT]);
	void test(Patch<ISIDE> imgs[COUNT]);

protected:
	Patch<OSIDE> output_[COUNT];
	Patch<ISIDE>* input_;
	Patch<ISIDE> inputError_[COUNT];


};
#include "Upsampling.cc"
#endif