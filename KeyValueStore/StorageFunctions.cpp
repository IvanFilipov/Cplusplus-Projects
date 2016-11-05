#include"stdafx.h"
#include"StorageFunctions.h"
#include"HelpFunctions.h"
#include<direct.h>

bool Init(){
	

	//creating a folder for the store
	_mkdir("Store");

	for (int i = 0; i < nChains; i++){

		string path = "Store\\hash";
		path += std::to_string(i);
		path += ".bin";

		std::ifstream ifs(path, std::ios::binary | std::ios::_Nocreate);

		//if we don;t have the needed file we create one
		if (!ifs.is_open()){

			std::ofstream ofs(path, std::ios::binary);

			//if we don't succeed to create the new file  
			if (!ofs.is_open())
				return false;

			ofs.close();

		}
		else
			ifs.close();

	}

	return true;
}


string Store(const char* pBlock, int size,bool mode, std::ostream& Logger){


	//getting the key from the "NextName.txt" file
	string Key = giveName();

	//getting the hash of that key
	int hashPos = HashFunc(Key);

	Logger << "at :";

	//getting the current time
	time_t timer;
	time(&timer);

	//if available outputs only the current time 
	if (timer){

		string Curtime = ctime(&timer);

		for (int i = 10; i < 20; i++)
			Logger << Curtime[i];
	}

	Logger<< ":  STORING a block with key :" << Key
		<< " and size " << size << "(bytes)"
		<< " at hash " << hashPos
		<< " mode : " << mode
		<< "... ";


	string path = "Store\\hash";
	path += std::to_string(hashPos);

	string pathTemp = path;

	path += ".bin";
	pathTemp += ".tmp";


	//depending on the mode proceeding to
	//store the block
	if (mode){

		//we are in the safe mode
		//opening a new file in which we will 
		//write the new block and than copy 
		//all the old data
		std::ofstream TempFile(pathTemp, std::ios::binary);

		if (!TempFile.is_open()){

			Logger << "error / while creating the temp file !\n";
			Key = "FFFFFFFF";
			return Key;
		}

		//writing the new block
		TempFile.write((char*)&size, sizeof(int));
		TempFile.write(Key.c_str(), sizeof(char) * 8);
		TempFile.write(pBlock, sizeof(char)*size);

		if (!TempFile.good()){

			Logger << "error / while writing the new block !\n";
			Key = "FFFFFFFF";
			return Key;
		}



		std::ifstream OldFile(path, std::ios::binary);

		if (!OldFile.is_open()){

			Logger << "error / while opening the old file !\n";
			Key = "FFFFFFFF";
			return Key;
		}

		//copy all the data , if the old file is not empty
		OldFile.seekg(0, std::ios::end);
		if (OldFile.tellg()){

			OldFile.seekg(0, std::ios::beg);
			TempFile << OldFile.rdbuf();
		}

		if (!TempFile.good()){

			Logger << "error / while re-writing the old data !\n";
			Key = "FFFFFFFF";
			return Key;


		}

		OldFile.close();
		TempFile.close();

		remove(path.c_str());
		rename(pathTemp.c_str(), path.c_str());
	}
	else{

		//we are in the unsafe mode
		//we just append the new block in the and of 
		//the existing file of the key's hash
		std::ofstream File(path, std::ios::binary | std::ios::app);


		if (!File.is_open()){

			Logger << "error / while opening the file !\n";
			Key = "FFFFFFFF";
			return Key;

		}

		//writing the new block
		File.write((char*)&size, sizeof(int));
		File.write(Key.c_str(), sizeof(char) * 8);
		File.write(pBlock, sizeof(char)*size);

		if (!File.good()){

			Logger << "error / while writing the new block !\n";
			Key = "FFFFFFFF";
			return Key;
		}

		File.close();

	}

	Logger << "successful !\n";
	return Key;
}


bool Erase(const string& key, std::ostream& Logger){

	Logger << "at :";

	//getting the current time
	time_t timer;
	time(&timer);

	//if available outputs only the current time 
	if (timer){

		string Curtime = ctime(&timer);

		for (int i = 10; i < 20; i++)
			Logger << Curtime[i];
	}

	Logger<< ": ERASING the block with key :"
		  << key;



	if (!isValid(key)){

		Logger << " ...error / invalid key!\n";
		return false;
	}

	//getting the hash code of the key
	int hash = HashFunc(key);

	Logger << " at hash " << hash;

	string path = "Store\\hash";
	path += std::to_string(hash);
	string pathTemp = path;

	path += ".bin";
	pathTemp += ".tmp";

	//opening the file which corresponds to hash
	std::ifstream hashFile(path, std::ios::binary);

	if (!hashFile.is_open()){

		Logger << " ...error / can't open the file!\n";
		return false;
	}

	bool notFound = true;

	//holds a block size
	int bSize = 0;

	//holds a key
	char bKey[KSIZE + 1];

	//holds a block's data
	char *block = nullptr;

	hashFile.read((char*)&bSize, sizeof(int));

	//checking if there is a block with wanted key
	while (!hashFile.eof()){
         
		hashFile.read((char*)&bKey, sizeof(char)*KSIZE);
		bKey[KSIZE] = '\0';

		if (bKey == key){

			notFound = false;
			//returns to the beginning
			hashFile.seekg(0, std::ios::beg);
			break;

		}

		//jumping to the next block
		hashFile.seekg(bSize, std::ios::cur);

		hashFile.read((char*)&bSize, sizeof(int));
	}


	if (notFound){

		Logger << " ...error / not existing!\n";
		hashFile.close();

		return false;
	}

	//temporary file , which will contain the data without the wanted block
	std::ofstream newFile(pathTemp, std::ios::binary);

	//proceed to erase the block
	bSize = 0;
	hashFile.read((char*)&bSize, sizeof(int));

	while (!hashFile.eof()){

		hashFile.read((char*)&bKey, sizeof(char)*KSIZE);
		bKey[KSIZE] = '\0';

		//jumping over this block
		if (bKey == key){

			hashFile.seekg(bSize, std::ios::cur);

		}
		else{

			block = new char[bSize];

			hashFile.read(block, sizeof(char)*bSize);

			//writing to the temporary file
			newFile.write((char*)&bSize, sizeof(int));
			newFile.write(bKey, sizeof(char) * 8);
			newFile.write(block, sizeof(char)*bSize);

			delete[] block;
		}

		hashFile.read((char*)&bSize, sizeof(int));
	}

	hashFile.close();
	newFile.close();

	//deleting the old file
	remove(path.c_str());

	//changing the names of the two files
	rename(pathTemp.c_str(), path.c_str());


	Logger << " ...successful! \n";
	return true;

}

//if the operation succeeds , the block will be loaded in "pBlock" , and its size in "size"
//if the operation fails , pBlock = NULL , size = 0
bool Load(const string& key, char *& pBlock, int &size, std::ostream& Logger){

	Logger << "at :";

	//getting the current time
	time_t timer;
	time(&timer);

	//if available outputs only the current time 
	if (timer){

		string Curtime = ctime(&timer);

		for (int i = 10; i < 20; i++)
			Logger << Curtime[i];
	}


	Logger<< ":  LOADING a block with key : "
		<< key;


	//checking the key
	if (!isValid(key)){

		pBlock = nullptr;
		size = 0;

		Logger << " ... error / invalid key!\n";
		return false;

	}

	//get the key's hash
	int hash = HashFunc(key);

	Logger << " at hash " << hash;

	string path = "Store\\hash";
	path += std::to_string(hash);
	path += ".bin";


	//opening the file which corresponds to the hash
	std::ifstream hashFile(path, std::ios::binary | std::ios::_Nocreate);

	if (!hashFile.is_open()){

		pBlock = nullptr;
		size = 0;

		Logger << " ... error / not existing!\n";

		return false;

	}

	//holds a block size
	int bSize = 0;
	//holds a key
	char bKey[KSIZE + 1];

	hashFile.read((char*)&bSize, sizeof(int));

	//trying to find the block with wanted key
	while (!hashFile.eof()){

		hashFile.read((char*)&bKey, sizeof(char)*KSIZE);
		bKey[KSIZE] = '\0';

		//if we have found the wanted key
		//we proceed to load the block in the memory 
		if (bKey == key){

			pBlock = new (std::nothrow) char[bSize];

			if (pBlock == nullptr){

				size = 0;


				Logger << " ... error / can't load this block in dynamic memory!\n";

				hashFile.close();
				return false;

			}
			else{

				hashFile.read(pBlock, bSize*sizeof(char));
				size = bSize;


				Logger << " ... successful!"
					<< " Block loaded at Ox" << (void*)pBlock
					<< " with size " << size << "(bytes)!\n";


				hashFile.close();
				return true;
			}

		}

		//jumping to the next block
		hashFile.seekg(bSize, std::ios::cur);

		//reading the next block size
		hashFile.read((char*)&bSize, sizeof(int));
	}


	//not found ...
	pBlock = nullptr;
	size = 0;


	Logger << " ... error / not existing!\n";

	hashFile.close();

	return false;
}
