#ifndef __LAYER_H__
#define __LAYER_H__


class Layer 
{
public:

	virtual void save(FILE *file) {};
	virtual void load(FILE *file) {};

};


#endif