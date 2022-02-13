#ifndef __LEAKYRELU_H__
#define __LEAKYRELU_H__

#include "Image.h"

template <unsigned int COUNT, unsigned int SIDE>
class LeakyReLU
{
public:
	LeakyReLU();
	~LeakyReLU();

	Patch<SIDE>* getOutput();
	Patch<SIDE>* getInputError();

	void train(Patch<SIDE> errOutput[COUNT]);
	void test(const double factor, Patch<SIDE> imgs[COUNT]);


protected:

	Patch<SIDE> output_[COUNT];
	Patch<SIDE> *input_;
	Patch<SIDE> inputError_[COUNT];
	double factor_;


};
#include "LeakyReLU.cc"
#endif