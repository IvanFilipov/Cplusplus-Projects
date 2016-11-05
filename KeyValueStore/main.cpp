// KeyValue_Store.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include"StorageFunctions.h"
#include<random>

//doing some test over here , outputting all results in Logger
void Test(std::ostream& Logger = std::cout){


	/*make your tests here 
	
	for example :

	storing a small block , using safe mode

	char *block = "....." ;
	int size = 11;

	string key = Store(block ,size ,1 , Logger);

	Erasing a block by key

	Erase(key,Logger);
	
	Loading a block in buffer ,with Nsize by key

	Load(key,buffer,Nsize,Logger);

	*/

	srand(time(NULL));

	int size = 0;
	char * pBlock;

	for (int i = 0; i < 200; i++){

		size = rand() % 1000;

		pBlock = new char[size];

		Store(pBlock, size, 0, Logger);

	}



}

#include<vector>

int main(){

	//opening the output file
	std::ofstream Logger("Logger.txt", std::ios::app);

	//getting the current time
	time_t timer;
	time(&timer);
     
	Logger << "\nProgram started at : "
		 << ctime(&timer)<< "\n\n";



	if (!Init()){
		Logger << "\nProgram ended with code -1\n\n";
		return -1;
	}
	

	//running some tests , outputting the results in "Logger"
	Test(Logger);



	time(&timer);
	Logger << "\nProgram ended at : "
		<< ctime(&timer) << "with code 0\n\n";

	Logger.close();

	return 0;

}
