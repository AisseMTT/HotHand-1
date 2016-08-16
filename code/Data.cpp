#include "Data.h"
#include <fstream>
#include <iostream>

namespace HotHand
{

// The static instance
Data Data::instance;

Data::Data()
{

}

void Data::load(const char* filename)
{
    std::fstream fin(filename, std::ios::in);
    if(!fin)
        return;

    shots.clear();

    bool temp;
    while(fin>>temp)
        shots.push_back(temp);

    fin.close();

    std::cout<<"# Loaded "<<shots.size()<<" shots from file ";
    std::cout<<filename<<'.'<<std::endl;
}

} // namespace HotHand

