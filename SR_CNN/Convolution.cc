#include "Convolution.h"


template <unsigned int KSIDE, unsigned int KCOUNT, unsigned int ICOUNT, unsigned int ISIDE, unsigned int OSIDE>
Convolution <KSIDE, KCOUNT, ICOUNT, ISIDE, OSIDE >::Convolution()
{
	for (unsigned int k = 0; k < KCOUNT; ++k)
	{
		filters_[k].HeInitialize((ISIDE*ISIDE*ICOUNT) +(OSIDE*OSIDE*KCOUNT) );
	}
}

template <unsigned int KSIDE, unsigned int KCOUNT, unsigned int ICOUNT, unsigned int ISIDE, unsigned int OSIDE>
Convolution<KSIDE, KCOUNT, ICOUNT, ISIDE, OSIDE >::~Convolution()
{
}

template <unsigned int KSIDE, unsigned int KCOUNT, unsigned int ICOUNT, unsigned int ISIDE, unsigned int OSIDE>
Patch<OSIDE>* Convolution<KSIDE, KCOUNT, ICOUNT, ISIDE, OSIDE >::getOutput()
{
	return output_;

}

template <unsigned int KSIDE, unsigned int KCOUNT, unsigned int ICOUNT, unsigned int ISIDE, unsigned int OSIDE>
Patch<ISIDE>* Convolution<KSIDE, KCOUNT, ICOUNT, ISIDE, OSIDE >::getInputError()
{
	return inputError_;

}

template <unsigned int KSIDE, unsigned int KCOUNT, unsigned int ICOUNT, unsigned int ISIDE, unsigned int OSIDE>
Patch<KSIDE>* Convolution<KSIDE, KCOUNT, ICOUNT, ISIDE, OSIDE >::getFilters()
{
	return filters_;

}

template <unsigned int KSIDE, unsigned int KCOUNT, unsigned int ICOUNT, unsigned int ISIDE, unsigned int OSIDE>
double Convolution<KSIDE, KCOUNT, ICOUNT, ISIDE, OSIDE >::getFiltersMean()
{
	double L2 = 0;

	for (unsigned int ii = 0; ii < KCOUNT; ++ii)
		for (unsigned int yy = 0; yy < KSIDE; ++yy)
			for (unsigned int xx = 0; xx < KSIDE; ++xx)
			{
				const double val = filters_[ii].getImage()[yy][xx];
				L2 += val*val;
			}
	
	
	return L2/(KSIDE*KSIDE*KCOUNT);

}

template <unsigned int KSIDE, unsigned int KCOUNT, unsigned int ICOUNT, unsigned int ISIDE, unsigned int OSIDE>
void Convolution<KSIDE, KCOUNT, ICOUNT, ISIDE, OSIDE >::test(Patch<ISIDE> input[ICOUNT])
{
	input_ = input;

	int p = (KSIDE - 1) / 2;
	int maxISIDE = ISIDE;

	for (unsigned int k = 0; k < KCOUNT; ++k) // Kernels 
		for (unsigned int y = 0; y < OSIDE; ++y)
			for (unsigned int x = 0; x < OSIDE; ++x)
			{
				double value = 0.0;

				for (unsigned int i = 0; i < ICOUNT; ++i) // Inputs
				{
					

					for (int ky = -p; ky <= p; ++ky)
					{
						int ys = y + ky;
						if (ys < 0) ys = 0;
						else if (ys >= maxISIDE) ys = maxISIDE - 1;

						for (int kx = -p; kx <= p; ++kx)
						{
							int xs = x + kx;
							if (xs < 0) xs = 0;
							else if (xs >= maxISIDE) xs = maxISIDE - 1;

    							value += filters_[k].getImage()[ky + p][kx + p] * input[i].getImage()[ys][xs];
						}
					}
					
				}
				output_[k].getImage()[y][x] = value;
			}
}

template <unsigned int KSIDE, unsigned int KCOUNT, unsigned int ICOUNT, unsigned int ISIDE, unsigned int OSIDE>
void Convolution<KSIDE, KCOUNT, ICOUNT, ISIDE, OSIDE >::train(const double alpha, Patch<OSIDE> errOutput[KCOUNT])
{
	//1. ERROR INPUT
	
	for(unsigned int count=0; count < ICOUNT; ++count) //Inputs
		for (unsigned int y = 0; y < ISIDE; ++y)
			for (unsigned int x = 0; x < ISIDE; ++x)
				inputError_[count].getImage()[y][x] = 0;

	int p = (KSIDE - 1) / 2;
	int maxISIDE = ISIDE;


		for(unsigned int k = 0; k < KCOUNT;++k) // Kernels 
			for(unsigned int y = 0; y < OSIDE; ++y)
				for (unsigned int x = 0; x < OSIDE; ++x)
					for (unsigned int i = 0; i < ICOUNT; ++i) // Inputs
					{
						for (int ky = -p; ky <= p; ++ky) 
						{
							int ys = y + ky;
							if (ys < 0) ys = 0;
							else if (ys >= maxISIDE) ys = maxISIDE - 1;

							for (int kx = -p; kx <= p; ++kx) 
							{
								int xs = x + kx;
								if (xs < 0) xs = 0;
								else if (xs >= maxISIDE) xs = maxISIDE - 1;

								inputError_[i].getImage()[ys][xs] += filters_[k].getImage()[ky + p][kx + p] * errOutput[k].getImage()[ys][xs];		
							}
						}
						double val = inputError_[i].getImage()[y][x];
						if (abs(val) > 1.0)
							inputError_[i].getImage()[y][x] = (1.0 * val) / abs(val);

					}

	//2. UPDATE WEIGHTS

		for (unsigned int k = 0; k < KCOUNT; ++k) // Kernels 
		{
			for (unsigned int y = 0; y < OSIDE; ++y)
				for (unsigned int x = 0; x < OSIDE; ++x)
					for (unsigned int i = 0; i < ICOUNT; ++i) // Inputs
					{
						for (int ky = -p; ky <= p; ++ky)
						{
							int ys = y + ky;
							if (ys < 0) ys = 0;
							else if (ys >= maxISIDE) ys = maxISIDE - 1;

							for (int kx = -p; kx <= p; ++kx)
							{
								int xs = x + kx;
								if (xs < 0) xs = 0;
								else if (xs >= maxISIDE) xs = maxISIDE - 1;

								filters_[k].getImage()[ky + p][kx + p] += -alpha * errOutput[k].getImage()[ys][xs] * input_[i].getImage()[ys][xs];
							}
						}
					}
			 /*
			for (unsigned int yy = 0; yy < KSIDE; ++yy)
				for (unsigned int xx = 0; xx < KSIDE; ++xx)
				{
					const double val = filters_[k].getImage()[yy][xx];
					if (abs(val) > 4.0)
						filters_[k].getImage()[yy][xx] = (4.0 * val) / abs(val);
				}
				*/
		}
}

template <unsigned int KSIDE, unsigned int KCOUNT, unsigned int ICOUNT, unsigned int ISIDE, unsigned int OSIDE>
void Convolution<KSIDE, KCOUNT, ICOUNT, ISIDE, OSIDE >::save(FILE *file)
{
	if (file)
	{
		// Every filter
		for (unsigned int k = 0; k < KCOUNT; ++k)
		{
			// Save filter
			for (unsigned int s = 0; s < KSIDE; ++s)
				fwrite(filters_[k].getImage()[s], sizeof(double), KSIDE, file);
		}
	}
	else
	{
		printf("ERROR: cannot open file\n");
	}

}

template <unsigned int KSIDE, unsigned int KCOUNT, unsigned int ICOUNT, unsigned int ISIDE, unsigned int OSIDE>
void Convolution < KSIDE, KCOUNT, ICOUNT, ISIDE, OSIDE > ::load(FILE *file)
{
	if (file)
	{
		// Every filter
		for (unsigned int k = 0; k < KCOUNT; ++k)
		{
			// Read filter
			for (unsigned int s = 0; s < KSIDE; ++s)
				fread(filters_[k].getImage()[s], sizeof(double), KSIDE, file);
		}
	}
	else
	{
		printf("ERROR: cannot open file\n");
	}

}