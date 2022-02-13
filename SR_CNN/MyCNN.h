#ifndef __MYCNN_H__
#define __MYCNN_H__

#include "Patch.h"
#include "LeakyReLU.h"
#include "Loss.h"
#include "Upsampling.h"
#include "Convolution.h"

template<unsigned int ISIDE, unsigned int OSIDE, unsigned int COUNT>
class MyCNN
{
public:
	MyCNN();
	~MyCNN();

	void test(Patch<ISIDE> input[COUNT]);
	void train(const double alpha, const Patch<OSIDE> reference[COUNT]);
	Patch<OSIDE> *getOutput();

	void save(FILE *file);
	void load(FILE *file);

protected:
	
	Convolution<1, 16, 1, ISIDE, ISIDE> *layer1_;

	LeakyReLU<16, ISIDE> *lrelu1_;
	Upsampling<16, ISIDE, OSIDE> *upsampling_;

	Convolution<3, 4, 16, OSIDE, OSIDE> *layer2_;
	LeakyReLU<4, OSIDE> *lrelu2_;

	Convolution<3, 2, 4, OSIDE, OSIDE> *layer3_;
	LeakyReLU<2, OSIDE> *lrelu3_;

	Convolution<3, 1, 2, OSIDE, OSIDE> *layer4_;
	LeakyReLU<1, OSIDE> *lrelu4_;

	Loss<1, OSIDE> *loss_;
	

};
#include "MyCNN.cc"
#endif