#ifndef __CONVOLUTION_H__
#define __CONVOLUTION_H__

#include "Image.h"
#include "Patch.h"

template <unsigned int KSIDE, unsigned int KCOUNT, unsigned int ICOUNT, unsigned int ISIDE, unsigned int OSIDE>
class Convolution
{
public:
	Convolution();
	~Convolution();

	Patch<OSIDE>* getOutput();
	Patch<ISIDE>* getInputError();
	Patch<KSIDE>* getFilters();


	void test(Patch<ISIDE> input[ICOUNT]);
	void train(const double alpha, Patch<OSIDE> errOutput[KCOUNT]);
	double getFiltersMean();

	void save(FILE *file);
	void load(FILE *file);


protected:

	Patch<KSIDE> filters_[KCOUNT];
	Patch<OSIDE> output_[KCOUNT];
	Patch<ISIDE> *input_;
	Patch<ISIDE> inputError_[ICOUNT];

};
#include "Convolution.cc"
#endif