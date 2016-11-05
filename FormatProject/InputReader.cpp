#include "stdafx.h"
#include "InputReader.h"




///default ctor

InputReader::InputReader() :nCommands(0), npaths(0), commands(NULL), paths(NULL){
	/* ... */
}

///release used memory
void InputReader::Clear(){

	if (npaths&&paths){

		for (unsigned int i = 0; i < npaths; i++){

			delete[] paths[i];
		}

		delete paths;
	}

	npaths = 0;
	paths = NULL;


	if (nCommands&&commands){

		for (unsigned int i = 0; i < nCommands; i++){

			delete[] commands[i];
		}

		delete commands;
	}

	nCommands = 0;
	commands = NULL;



}

InputReader::~InputReader()
{
	Clear();
}


///if there is a HTML command put her in the end of commands array
void InputReader::sortHTML(){

	if (nCommands == 1){
		return;
	}

	for (int i = 0; i < nCommands; i++){

		if (strcmp(commands[i], "--html") == 0){

			char*temp = commands[nCommands - 1];
			commands[nCommands - 1] = commands[i];

			while (i < nCommands - 2){

				commands[i] = commands[i + 1];
				i++;

			}

			commands[i] = temp;
			i++;

		}
	}

}


///returns true if str is a path to cpp or c file
/// false - else
bool InputReader::isValid(char* str){

	int i = strlen(str) -1;



	while (str[i] != '.'&&i >= 0){

		if (str[i] != 'p' &&str[i] != 'c'){
			//the file format cannot be neither c++ nor c  
			return false;
		}
		i--;
	}

	//the file has not type ?!
	if (i == 0){
		return false;
	}
	if (str[i + 1] == 'c'){
		
		if (str[i + 2] == '\0'){
			//c type
			return true;
		}
		else if (str[i + 2] == 'p'&&str[i + 3] == 'p'&&str[i + 4] == '\0'){
			//c++ type
			return true;
		}
	}

   //not one of ours
	return false;

}


char ** InputReader::getCommands()const{
	return commands;
}

char ** InputReader::getPaths()const{

	return paths;
}

unsigned int InputReader::getNumCommands()const{

	return nCommands;
}

unsigned int InputReader::getNumPaths()const{

	return npaths;
}

///creating two arrays using arguments passed to the main function 
///one containing commands , and one with paths to files

///throws std::bad_alloc if cannot allocate memory
///throws exception if there is a path to a file ,which is not .c or .cpp
void InputReader::Read(int nargs,char**arg){

	Clear();

	for (int i = 1; i < nargs; i++){

		if (arg[i][0] == '-'&&arg[i][1] == '-'){
			nCommands++;
		}
		else{
			npaths++;
		}

	}


	commands = new char*[nCommands];

	paths = new char*[npaths];

	int curCom = 0;
	int curPath = 0;
	for (int i = 1; i < nargs; i++){

		if (arg[i][0] == '-'&&arg[i][1] == '-'){

			commands[curCom] = new char[strlen(arg[i]) + 1];
			strcpy(commands[curCom], arg[i]);

			//std::cout << "com : \n" << commands[curCom] <<'\n';
			curCom++;

		}
		else{
			if (isValid(arg[i])){

				paths[curPath] = new char[strlen(arg[i]) + 1];
				strcpy(paths[curPath], arg[i]);

				//std::cout << "path : \n" << paths[curPath]<<'\n';
				curPath++;
			}
			else{

				//we need to now how many string we have to clear
				npaths = curPath;
				nCommands = curCom;
				//cleaning the current object
				Clear();

				std::cout << "error in file " << arg[i] << ":  ";
				throw std::exception("not supported file type\n");
			}
		}

	}

}