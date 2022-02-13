// main.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Image.h"
#include "MyCNN.h"
#include "Patch.h"

#define STEP 1
#define NUM_CHANNELS 1
#define LEARNING_RATE 0.001
#define LR_SIDE 3
#define HR_SIDE 6

#pragma warning (disable : 4996)
#define CLK_TO_SEC(_clk_)  (((double) (_clk_)) / ((double) CLOCKS_PER_SEC))


void testImage(MyCNN<LR_SIDE, HR_SIDE, NUM_CHANNELS> &myCNN, Image &lrImg, const char* pathSave, const char* pathExp)
{
	// LR-image
	const unsigned int heightLR = lrImg.getHeight();
	const unsigned int widthLR = lrImg.getWidth();

	// HR-image
	const unsigned int heightHR = heightLR * 2;
	const unsigned int widthHR = widthLR * 2;

	// Max dimensions to step
	const unsigned int maxHeightLR = ((lrImg.getHeight() - LR_SIDE) / LR_SIDE) * LR_SIDE;
	const unsigned int maxWidthLR = ((lrImg.getWidth() - LR_SIDE) / LR_SIDE) * LR_SIDE;

	Patch<LR_SIDE>* lrPatch = new Patch<LR_SIDE>();
	
	Patch<HR_SIDE> hrPatch = Patch<HR_SIDE>();
	const Patch<HR_SIDE> *gauss = hrPatch.getGauss();

	Image outputImage(widthHR, heightHR);
	Image scores(widthHR, heightHR);

	for (unsigned int y = 0; y < maxHeightLR; y += STEP)
	{
		for (unsigned int x = 0; x < maxWidthLR; x += STEP)
		{
			// Load current LR-patch
			lrPatch->readPatch(lrImg, y, x);

			// Processing
			myCNN.test(lrPatch);
			Patch<HR_SIDE> *outputPatch = myCNN.getOutput();
			outputPatch->mulPatch(*gauss);

			// Add patch to image
			outputPatch->addPatch(outputImage, 2 * y, 2 * x);

			// Add gaussian factor to scores
			gauss->addPatch(scores, 2 * y, 2 * x);
		}
	}
	for (unsigned int y = 0; y < heightHR; ++y)
		for (unsigned int x = 0; x < widthHR; ++x) {	
			outputImage.setValue( (255.0 *(outputImage.getImage()[y][x]) / scores.getImage()[y][x]) , y, x);

		}

	// Save image
	outputImage.exportImagePGM(pathExp);
}

void trainImage(MyCNN<LR_SIDE, HR_SIDE, NUM_CHANNELS> &myCNN, Image &lrImg, Image &hrImg, const char* path)
{
	// LR-image
	const unsigned int heightLR = lrImg.getHeight();
	const unsigned int widthLR = lrImg.getWidth();

	// Full destination HR-image
	const unsigned int heightHR = hrImg.getHeight();
	const unsigned int widthHR = hrImg.getHeight();

	const unsigned int maxHeightLR = ((lrImg.getHeight() - LR_SIDE) / LR_SIDE) * LR_SIDE;
	const unsigned int maxWidthLR = ((lrImg.getWidth() - LR_SIDE) / LR_SIDE) * LR_SIDE;

	Patch<LR_SIDE> lrPatch = Patch<LR_SIDE>();
	Patch<HR_SIDE> hrPatch = Patch<HR_SIDE>();

	// Checks if the high-resoluton image is 2 times the low-resolution image
	if ((heightHR == heightLR*2) && (widthHR == widthLR*2))
	{
		printf("ERROR: Low resolution image dimensions doesnt match high resolution image");
		return;
	}
	printf("LR-width:%u LR-height:%u\n",  widthLR, heightLR);



	for (unsigned int y = 0; y < maxHeightLR; y += STEP)
	{
		for (unsigned int x = 0; x < maxWidthLR; x += STEP)
		{
			
			// Load current LR-patch
			lrPatch.readPatch(lrImg, y, x);
			// Load current HR-patch
			hrPatch.readPatch(hrImg, 2 * y, 2 * x);

			// Processing		
			myCNN.test(&lrPatch);

			// Train current patch
			myCNN.train(LEARNING_RATE, &hrPatch);

		}
	}

	//Save convolution layers
	
	FILE *file = NULL;
	char tmpPath[2048];
	sprintf(tmpPath, "%s.tmp", path);
	fopen_s(&file, tmpPath, "wb");
	if (file)
	{
		myCNN.save(file);
		fclose(file);
	}
	else
	{
		printf("ERROR: cannot open file\n");
	}
	remove(path);
	rename(tmpPath, path);
	
}

void test(MyCNN <LR_SIDE, HR_SIDE, NUM_CHANNELS> &myCNN, const char* pathLR, const char* outputPath, const char* pathSave)
{
	char tmpLR[2048];
	char tmpHR[2048];
	sprintf(tmpLR, "%s/*.pgm", pathLR);

	struct _finddata_t pgm_file;
	intptr_t hFile;

	// Find first .pgm in the path
	unsigned int count = 1;
	if ((hFile = _findfirst(tmpLR, &pgm_file)) == -1L)
		printf("No *.pgm files in current directory!\n");
	else
	{
		do
		{
			sprintf(tmpLR, "%s/%s", pathLR, pgm_file.name);
			Image lrImg(tmpLR);
			sprintf(tmpHR, "%s/%s", outputPath, pgm_file.name);
			printf("\nTesting image %d:\n  * %s\n  *output:  %s\n", count, tmpLR, tmpHR);

			// Tests full image
			const clock_t  clkIni = clock();
			testImage(myCNN, lrImg, pathSave, tmpHR);
			printf("  * Tested image %s in %f secs.", pgm_file.name, CLK_TO_SEC(clock() - clkIni));

			++count;
		} while (_findnext(hFile, &pgm_file) == 0);
		_findclose(hFile);
	}
}

void train(MyCNN<LR_SIDE, HR_SIDE, NUM_CHANNELS> &myCNN, const char* pathLR, const char* pathHR, const char* pathSave)
{
	char tmpLR[2048];
	char tmpHR[2048];
	sprintf(tmpLR, "%s/*.pgm", pathLR);
	printf("Searching .pgm in %s\n", pathLR);
	struct _finddata_t pgm_file;
	intptr_t hFile;

	// Find first .pgm in the path
	unsigned int count = 1;
	if ((hFile = _findfirst(tmpLR, &pgm_file)) == -1L)
		printf("No *.pgm files in current directory!\n");
	else
	{
		do
		{
			sprintf(tmpLR, "%s/%s", pathLR, pgm_file.name);
			sprintf(tmpHR, "%s/%s", pathHR, pgm_file.name);
			printf("\nTraining image %d:\n  * %s\n  * %s\n", count, tmpLR, tmpHR);

			// Loads both low-resolution and high-resolution images
			Image lrImg(tmpLR);
			Image hrImg(tmpHR);

			// Train images
			const clock_t  clkIni = clock();
			trainImage(myCNN, lrImg, hrImg, pathSave);

			printf("  * Trained image %s in %f secs.", pgm_file.name, CLK_TO_SEC(clock() - clkIni));
			++count;
		} while (_findnext(hFile, &pgm_file) == 0);
		_findclose(hFile);
	}
}


int main(int argc, char*argv[])
{
	const char *pathSave = NULL;
	const char *pathLR = NULL;
	const char *pathHR = NULL;
	bool bTrain = false;
	bool bLoad = false;
	for (int i = 1; i < argc; ++i)
	{
		if (strstr(argv[i], "-sv:") == argv[i])
		{
			pathSave = argv[i] + 4;
		}
		else if (strcmp(argv[i], "-ld") == 0)
		{
			bLoad = true;
		}
		else if (strstr(argv[i], "-lr:") == argv[i])
		{
			pathLR = argv[i] + 4;
		}
		else if (strstr(argv[i], "-hr:") == argv[i])
		{
			pathHR = argv[i] + 4;
		}
		else if (strcmp(argv[i], "-test") == 0)
		{
			bTrain = false;
			bLoad = true;
		}
		else if (strcmp(argv[i], "-learn") == 0)
		{
			bTrain = true;
		}
		else
		{
			printf("Unknown argument: %s\n", argv[i]);
			return -1;
		}
	}

	//Initialize Layers
	MyCNN<LR_SIDE,HR_SIDE, NUM_CHANNELS> myCNN= MyCNN<LR_SIDE,HR_SIDE, NUM_CHANNELS>();

	//LOAD
	if (bLoad) 
	{
		FILE *file = nullptr;
		fopen_s(&file, pathSave, "rb");

		if(file)
		{
			// Loads every layer
			myCNN.load(file);
			fclose(file);

			printf("Layers loaded successfully.\n");
		}
		else
		{
			printf("ERROR: cannot open file\n");
		}
	}
	

	// TRAIN
	if (bTrain) 
	{
		train(myCNN, pathLR, pathHR, pathSave);
	}
	// TEST
	else 
	{
		test(myCNN, pathLR, pathHR, pathSave);
	}

	printf("End.\n");
}

