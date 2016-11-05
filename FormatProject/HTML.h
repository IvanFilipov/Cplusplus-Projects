#pragma once
#include "Command.h"
class HTML : public Command
{
public:
	virtual void Format(TextManager&);
	void AddNewLineTag(char*, int&);
	void AddDirectiveTag(char*, int&, char*, int);
	void AddSpaceTag(char*buff, int&bufpos);
	void AddCommentTag(char*,int&,char*,int);
	void AddNumberTag(char*, int&, char*, int);
	void AddWordTag(char*, int&, char*, int);

	bool isSpecialWord(char*,int);

	void ReadToNewline(char*, char*, int&, int&);
	void ReadToNonDigit(char*, char*, int&, int&);
	void ReadToNonAlpha(char*, char*, int&, int&);
};

