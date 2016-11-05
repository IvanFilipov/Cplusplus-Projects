#include "stdafx.h"
#include "TextManager.h"


TextManager::TextManager() :lines(NULL) , nlines(0) , HTML(false){

	/*...*/
}


TextManager& TextManager::operator=(const TextManager &other){

	if (this == &other){

		return *this;
	}

	Clear();

	nlines = other.nlines;

	HTML = other.HTML;

	lines = new char*[nlines];

	for (unsigned int i = 0; i < nlines; i++){

		lines[i] = new char[strlen(other.lines[i]) + 1];
		strcpy(lines[i], other.lines[i]);

	}

	return *this;
}

TextManager::TextManager(const TextManager& other){

	nlines = other.nlines;

	HTML = other.HTML;

	lines = new char*[nlines];

	for (unsigned int i = 0; i < nlines; i++){

		lines[i] = new char[strlen(other.lines[i]) + 1];
		strcpy(lines[i], other.lines[i]);

	}

}


void TextManager::Clear()
{

	if (lines == NULL){
		return;
	}

	for (unsigned int i = 0; i < nlines; i++){

		delete[] lines[i];
	}

	nlines = 0; 
	HTML = false;

	delete[] lines;
	lines = NULL;
}

TextManager::~TextManager(){

	Clear();

}

void TextManager::setHTML(bool html){

	HTML = html;
}

bool TextManager::hasHTML()const{

	return HTML;
}

void TextManager::setNumLines(const char*str){

	

	int  i = 0;
	while (str[i]!='\0'){

		if (str[i] == '\n' || str[i] == '\r'){

			nlines++;

			while ((str[i] == '\n' || str[i] == '\r')) i++;
		}
		else {
			i++;
		}

	}

	//add one more line , because the last line in the source file we may not have newline symbol
	if (str[i - 1] != '\n'&&str[i - 1] != '\n'){
		nlines++;
	}
	
}


void TextManager::Read(const char* path){



	std::ifstream ifs(path, std::ios::_Nocreate | std::ios::binary);


	if (!ifs.is_open()){

		std::cout << "error while reading file " << path << " : ";
		
		throw std::exception("there is not such file !\n");
	}
	
	//get length of ifs
	ifs.seekg(0, ifs.end);
	unsigned int len = ifs.tellg();
	ifs.seekg(0, ifs.beg);


	char*buff = new (std::nothrow)char[len+1];

	if (buff == NULL){
		std::cout << "error while reading file " << path << " : ";
		throw std::exception("the file is too big to be read ! \n");
	}

	ifs.read(buff, len); // reading all info at once

	//checking the file
	if (!ifs){
		std::cout << "the file" << path << " ";
		delete[]buff;
		throw std::exception("hasn't been successfully read\n!");
	}
	//insurance  
	buff[len] = '\0';
	

	ifs.close();


		// counting how many lines do we need
	setNumLines(buff); 

	//throws std::bad_alloc if memory can't be allocatedele
	try {
		lines = new  char*[nlines];
	}
	catch (std::bad_alloc&e){
		delete[] buff;
		throw e;
	}

	//buffer in which we are reading chars
	char curline[256];

	//how many lines have already been ridden
	int rode = 0;

	//how many chars do we have for the current line
	int charsRode;

	bool endReached;

	unsigned i = 0;

	while (buff[i]!='\0'){

		memset(curline, 0, 256);
		charsRode = 0;
		endReached = false;

		 while (!endReached){

			 if (buff[i] != '\n' && buff[i] != '\r'&&buff[i] != '\0'){
				 curline[charsRode] = buff[i];
				 charsRode++;
				 i++;

			 }
			 else endReached = true;
		 }

		while ((buff[i] == '\n' || buff[i] == '\r')){

			curline[charsRode] = buff[i];
			charsRode++;
			i++;
		}

		try {
			lines[rode] = new char[strlen(curline) + 1];
		}
		catch (std::bad_alloc & e){
			delete[] buff;
			throw e;
		}
		

		strcpy(lines[rode], curline);

		rode++;

	}

	delete []buff;
	std::cout << "the file " << path << " has been successfully read\n";

	
}


/// write on the disk 
void TextManager::Write(const char* path){

	const char *usePath;

	if (!hasHTML()){

		//renaming the file by adding .old to the name of file
		char *newPath = new char[strlen(path) + 5];

		strcpy(newPath, path);

		strcat(newPath, ".old");

		rename(path, newPath);

		delete[] newPath;

		//we will use the same path to create the new file
		usePath = path;
	}
	else{
		int pointPos = 0;
		char * temp = new char[strlen(path) + 4];

		strcpy(temp, path);

		for (int i = strlen(temp); i >= 0; --i){
			if (temp[i] == '.'){
				pointPos = i;
				break;
			}
		}

		temp[pointPos] = '\0';

		strcat(temp, ".html");
		

		usePath = temp;
	}

	std::ofstream ofs(usePath,std::ios::binary);

	if (!ofs.is_open()){
		throw std::exception("file can't be written on disk");
	}

	for (unsigned int i = 0; i < nlines; i++){

		if (lines[i] != NULL){
			ofs.write(lines[i], strlen(lines[i]));
		}
	}

	if (ofs){
		
		ofs.close();
		std::cout << "file " << usePath << " has been successfully written\n\n";
    	//return true;
	}
	else {		
		ofs.close();
		std::cout << "file " << usePath << " has NOT been successfully written\n";
		//return false;
	}
}


char* TextManager::GetLine(unsigned int pos){

	return pos < nlines ? lines[pos] : NULL;


}

void TextManager::SetLine(unsigned int pos, char * NewLine){

	if (pos >=  nlines){
		return;
	}

	delete[] lines[pos];

	lines[pos] = NewLine;


}

unsigned int TextManager::getnLines()const{

	return nlines;

}
