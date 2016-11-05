#include"stdafx.h"
#include"Command.h"

char* Command::CreateNewLine(int len , char*buff){

	char *newLine = new char[len + 1];
	strcpy(newLine, buff);

	return newLine;

}