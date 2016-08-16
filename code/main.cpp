#include <iostream>
#include "DNest4/code/DNest4.h"
#include "Data.h"
#include "MyModel.h"

int main(int argc, char** argv)
{
    HotHand::Data::instance.load("data.txt");
	DNest4::start<HotHand::MyModel>(argc, argv);
	return 0;
}

