#pragma once

#include "TextManager.h"

class Command{

public:
	virtual void Format(TextManager &) = 0;
	char *CreateNewLine(int , char*);
	virtual ~Command() {};

};
