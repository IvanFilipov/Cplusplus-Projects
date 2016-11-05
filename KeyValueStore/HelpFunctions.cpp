#include"stdafx.h"
#include"HelpFunctions.h"


int HashFunc(const string& Key){

	unsigned long int result = 0;

	// first KSIZE prime numbers bigger than 9
	static const int primes[KSIZE] = { 11, 13, 17, 19, 23, 29, 31, 37 };

	for (int i = 0; i < KSIZE; i++)
		result += primes[KSIZE - i - 1] * (Key[i] - '\0');


	return result % nChains;
}


string giveName(){

	string name;
	string newName;
     
	//opening the file ,which contains the next name
	std::fstream NextName("Store\\NextName.txt", std::ios::_Nocreate);

	if (NextName.is_open()){

		//reading the current name
		NextName >> name;

		int nameCount = atoi(name.c_str());

		nameCount++;

		newName = std::to_string(nameCount);

		NextName.close();

	}
	else{

		//basic case , when we don't even have the "NextName" file
		name = "10000000";
        newName = "10000001";
	}


	//writing the next name
	std::ofstream nName("Store\\NextName.txt", std::ios::trunc);
	nName << newName;
	nName.close();

	return name;
}

bool isValid(const string& key){

	if (key.length() != KSIZE)
		return false;

	for (int i = 0; i < KSIZE; i++)
		if (!isdigit(key[i]))
			return false;

	int num = atoi(key.c_str());

	if (num > 99999999 || num < 10000000)
		return false;

	return true;
}
