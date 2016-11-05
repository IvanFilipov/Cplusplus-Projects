#pragma once

#include "Command.h"

class Formatter :public Command
{
	private:
		bool usedTabs;
		bool usedLF;
public:
	Formatter();
	char NextLineBeg(int i, TextManager& manager);
	virtual void Format(TextManager&);
	void CheckUsedSym(TextManager&);
	void AddNewLineSym(char *, int& );
	void AddindentationSym(char*, int&, int);
};

