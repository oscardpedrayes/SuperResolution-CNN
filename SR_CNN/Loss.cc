#include "Loss.h"

template <unsigned int COUNT, unsigned int SIDE>
Loss<COUNT,SIDE>::Loss()
{
}

template <unsigned int COUNT, unsigned int SIDE>
Loss<COUNT,SIDE>::~Loss()
{
}

template <unsigned int COUNT, unsigned int SIDE>
Patch<SIDE>* Loss<COUNT, SIDE>::getError()
{
	return error_;
}



template <unsigned int COUNT, unsigned int SIDE>
void Loss<COUNT,SIDE>::evalError(const Patch<SIDE> target[COUNT], const Patch<SIDE> output[COUNT], const double L2)
{
	double diff = 0.0;
	double loss = 0.0;
	const double SIDE2 = SIDE * SIDE;
	for (unsigned int i = 0; i < COUNT; ++i)
		for (unsigned int y = 0; y < SIDE; ++y)
			for (unsigned int x = 0; x < SIDE; ++x)
			{
				double val = output[i].getImage()[y][x];
	
					
				diff = val - target[i].getImage()[y][x];

			
				if (abs(diff) > 1.0)
					diff = (1.0 * diff) / abs(diff);
					

		
				error_[i].getImage()[y][x] =  diff;//+ 0.0005*L2;

				loss += diff*diff;		
			}

	
	//printf("%f loss", loss/SIDE2*COUNT);
}
