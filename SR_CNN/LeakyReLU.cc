#include "LeakyReLU.h"


template <unsigned int COUNT, unsigned int SIDE>
LeakyReLU<COUNT, SIDE>::LeakyReLU()
{
}

template <unsigned int COUNT, unsigned int SIDE>
LeakyReLU<COUNT, SIDE>::~LeakyReLU()
{
}

template <unsigned int COUNT, unsigned int SIDE >
Patch<SIDE>* LeakyReLU<COUNT, SIDE>::getOutput()
{
	return output_;
}

template <unsigned int COUNT, unsigned int SIDE>
Patch<SIDE>* LeakyReLU<COUNT, SIDE>::getInputError()
{
	return inputError_;
}


template <unsigned int COUNT, unsigned int SIDE>
void LeakyReLU<COUNT, SIDE>::train(Patch<SIDE> errOutput[COUNT])
{
	for (unsigned int i = 0; i < COUNT; ++i) 
		for (unsigned int y = 0; y < SIDE; ++y)
			for (unsigned int x = 0; x <SIDE; ++x)
			{
				double value = errOutput[i].getImage()[y][x];

				if (input_[i].getImage()[y][x] < 0.0)
					value *= factor_;

				inputError_[i].getImage()[y][x] = value;
			}		
}

template <unsigned int COUNT, unsigned int SIDE>
void LeakyReLU<COUNT, SIDE>::test(const double factor, Patch<SIDE> imgs[COUNT])
{
	input_ = imgs;
	factor_ = factor;
	for (unsigned int i = 0; i < COUNT; ++i)
		for (unsigned int y = 0; y < SIDE; ++y)
			for (unsigned int x = 0; x < SIDE; ++x)
			{
				double value = imgs[i].getImage()[y][x];

				if (imgs[i].getImage()[y][x] < 0.0)
					value *= factor;
			
				output_[i].getImage()[y][x] = value;
			}
}

