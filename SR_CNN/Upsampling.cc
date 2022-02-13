#include "Upsampling.h"

template <unsigned int COUNT, unsigned int ISIDE, unsigned int OSIDE>
Upsampling<COUNT, ISIDE, OSIDE>::Upsampling()
{
}

template <unsigned int COUNT, unsigned int ISIDE, unsigned int OSIDE>
Upsampling<COUNT, ISIDE, OSIDE>::~Upsampling()
{
}

template <unsigned int COUNT, unsigned int ISIDE, unsigned int OSIDE>
Patch<OSIDE>* Upsampling<COUNT, ISIDE, OSIDE>::getOutput()
{
	return output_;
}

template <unsigned int COUNT, unsigned int ISIDE, unsigned int OSIDE>
Patch<ISIDE>* Upsampling<COUNT, ISIDE, OSIDE>::getInputError()
{
	return inputError_;
}

template <unsigned int COUNT, unsigned int ISIDE, unsigned int OSIDE>
void Upsampling<COUNT, ISIDE, OSIDE>::train(Patch<OSIDE> errOutput[COUNT])
{
	Patch<ISIDE> errInput[COUNT];
	unsigned int yy, xx;
	double max = 0;
	for (unsigned int i = 0; i < COUNT; ++i)
		for (unsigned int y = 0; y < ISIDE; ++y)
			for (unsigned int x = 0; x < ISIDE; ++x)
			{
				yy = 2 * y; //OSIDE
				xx = 2 * x; //OSIDE
				max = errOutput[i].getImage()[yy][xx];
				if (max < errOutput[i].getImage()[yy][xx + 1]) max = errOutput[i].getImage()[yy][xx + 1];
				if (max < errOutput[i].getImage()[yy + 1][xx]) max = errOutput[i].getImage()[yy + 1][xx];
				if (max < errOutput[i].getImage()[yy + 1][xx + 1]) max = errOutput[i].getImage()[yy + 1][xx + 1];
				inputError_[i].getImage()[y][x] = max;
			}
}

template <unsigned int COUNT, unsigned int ISIDE, unsigned int OSIDE>
void Upsampling<COUNT, ISIDE, OSIDE>::test(Patch<ISIDE> imgs[COUNT])
{
	input_ = imgs;

	double value = 0.0;
	unsigned int yy = 0;
	unsigned int xx = 0;
	for (unsigned int i = 0; i < COUNT; ++i) 
		for (unsigned int y = 0; y < ISIDE; ++y)
			for (unsigned int x = 0; x < ISIDE; ++x)
			{
				yy = 2 * y;
				xx = 2 * x;
				value = imgs[i].getImage()[y][x];
				output_[i].getImage()[yy][xx] = value;
				output_[i].getImage()[yy][xx + 1] = value;
				output_[i].getImage()[yy + 1][xx] = value;
				output_[i].getImage()[yy + 1][xx + 1] = value;
			}
}