#include "MyCNN.h"

template<unsigned int ISIDE, unsigned int OSIDE, unsigned int COUNT>
MyCNN<ISIDE, OSIDE, COUNT>::MyCNN()
{
	
	layer1_ = new Convolution<1, 16, 1, ISIDE, ISIDE>();
	lrelu1_ = new LeakyReLU<16, ISIDE>();

	upsampling_ = new Upsampling<16, ISIDE, OSIDE>();

	layer2_ = new Convolution<3, 4, 16, OSIDE, OSIDE>();
	lrelu2_ = new LeakyReLU<4, OSIDE>();

	layer3_ = new Convolution<3, 2, 4, OSIDE, OSIDE>();
	lrelu3_ = new LeakyReLU<2, OSIDE>();
	
	layer4_ = new Convolution<3, 1, 2, OSIDE, OSIDE>();

	loss_ = new Loss<1, OSIDE>();
	


	
}

template<unsigned int ISIDE, unsigned int OSIDE, unsigned int COUNT>
MyCNN<ISIDE, OSIDE, COUNT>::~MyCNN()
{
	delete layer1_;
	delete lrelu1_;
	delete upsampling_;
	delete layer2_;
	delete lrelu2_;
	delete layer3_;
	delete layer4_;
	delete loss_;
	


}

template<unsigned int ISIDE, unsigned int OSIDE, unsigned int COUNT>
void MyCNN<ISIDE, OSIDE, COUNT>::test(Patch<ISIDE> input[COUNT])
{
	double reluFactor = 0.01;

	layer1_->test(input);
	lrelu1_->test(reluFactor, layer1_->getOutput());

	upsampling_->test(lrelu1_->getOutput());
	
	layer2_->test(upsampling_->getOutput());
	lrelu2_->test(reluFactor, layer2_->getOutput());

	layer3_->test(lrelu2_->getOutput());
	lrelu3_->test(reluFactor, layer3_->getOutput());

	layer4_->test(lrelu3_->getOutput());

	//lrelu4_->test(reluFactor, layer4_->getOutput());
	
}

template<unsigned int ISIDE, unsigned int OSIDE, unsigned int COUNT>
void MyCNN<ISIDE, OSIDE, COUNT>::train(const double alpha, const Patch<OSIDE> reference[COUNT])
{
	const double L2 = (layer1_->getFiltersMean() + layer2_->getFiltersMean() + layer3_->getFiltersMean() + layer4_->getFiltersMean())/4;
	//printf("\n%f W\t", L2);


	loss_->evalError( reference, getOutput(), L2);
	
	layer4_->train(alpha, loss_->getError());

	lrelu3_->train(layer4_->getInputError());
	layer3_->train(alpha, lrelu3_->getInputError());
	//layer3_->train(alpha, loss_->getError());

	lrelu2_->train(layer3_->getInputError());
	layer2_->train(alpha, lrelu2_->getInputError());
	//layer2_->train(alpha, loss_->getError());

	upsampling_->train(layer2_->getInputError());

	lrelu1_->train(upsampling_->getInputError());
	layer1_->train(alpha, lrelu1_->getInputError());


}

template<unsigned int ISIDE, unsigned int OSIDE, unsigned int COUNT>
Patch<OSIDE> *MyCNN<ISIDE, OSIDE, COUNT>::getOutput()
{
	return layer4_->getOutput();
}

template<unsigned int ISIDE, unsigned int OSIDE, unsigned int COUNT>
void MyCNN<ISIDE, OSIDE, COUNT>::save(FILE *file)
{
	
	layer1_->save(file);
	layer2_->save(file);
	layer3_->save(file);
	layer4_->save(file);
	

}

template<unsigned int ISIDE, unsigned int OSIDE, unsigned int COUNT>
void MyCNN<ISIDE, OSIDE, COUNT>::load(FILE *file)
{
	
	layer1_->load(file);
	layer2_->load(file);
	layer3_->load(file);
	layer4_->load(file);
	
}

