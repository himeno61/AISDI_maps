#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

#include "TreeMap.h"
#include "HashMap.h"
#include <chrono>

namespace
{
template <typename K, typename V>
using Hash = aisdi::HashMap<K, V>;

template <typename K, typename V>
using Tree = aisdi::TreeMap<K, V>;

using timeValue = std::chrono::time_point<std::chrono::system_clock>;
using durationValue = std::chrono::duration<double>;

void int_Test()
{
	unsigned int NUM_ELEMENTS = 1000;
	std::ofstream outputFile;
	outputFile.open("test_times_append.txt");
	outputFile.setf(std::ios::scientific);
	outputFile << "Numer\tHashMap\t\tTreeMap" <<std::endl;
    for (size_t j=0;j <5;j++)
    {
		//Hash<int,std::string>* hash = new Hash<int,std::string>;
		//Tree<int,std::string>* tree = new Tree<int,std::string>;
		Hash<int,std::string> hash;
		Tree<int,std::string> tree;
		outputFile<<NUM_ELEMENTS<<"\t";
        timeValue start, stop;
        start = std::chrono::system_clock::now();
		for (size_t k=0; k<NUM_ELEMENTS; k++)
		{
			//int temp = rand()%NUM_ELEMENTS;
			//hash->add_element(temp,"test");
			hash[rand()%NUM_ELEMENTS]="test";
		}
        stop = std::chrono::system_clock::now();
        durationValue differ = stop - start;
        outputFile<<differ.count();
        
		outputFile<<"\t\t";
        start = std::chrono::system_clock::now();
        for (size_t k=0; k<NUM_ELEMENTS; ++k)
		{
			//int temp = rand()%NUM_ELEMENTS;
			//tree->add_element(temp,"test");
			hash[rand()%NUM_ELEMENTS]="test";
		}
        stop = std::chrono::system_clock::now();
		differ = stop - start;
        outputFile<<differ.count()<<std::endl;
        NUM_ELEMENTS = NUM_ELEMENTS*2;
        //delete hash;
        //delete tree;
        std::cout<<"kupa";
    }
  
	outputFile.close();
}

} 

int main()
{
  int_Test();
  return 0;
}
