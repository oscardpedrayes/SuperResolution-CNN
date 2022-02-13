#ifndef __LOSS_H__
#define __LOSS_H__

#include "Image.h"

template <unsigned int COUNT, unsigned int SIDE>
class Loss
{
public:
	Loss();
	~Loss();

	Patch<SIDE>* getError();
	void evalError(const Patch<SIDE> target[COUNT], const Patch<SIDE> output[COUNT], const double L1);


protected:
	Patch<SIDE> error_[COUNT];

};
#include "Loss.cc"
#endif