#pragma once

#include "Command.h"

class Comments : public Command
{
public:
	virtual void Format(TextManager &);
};

