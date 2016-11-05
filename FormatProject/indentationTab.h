#pragma once

#include "Command.h"

class indentationTab :	public Command
{
public:
	virtual void Format(TextManager&);
	
};

