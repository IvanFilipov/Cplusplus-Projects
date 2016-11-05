#include "stdafx.h"
#include "TaskCreator.h"
#include "Comments.h"
#include "NewlinesLF.h"
#include "NewlinesCRLF.h"
#include "indentationTab.h"
#include "indentationSpaces.h"
#include "Formatter.h"
#include "HTML.h"


Command* TaskCreator::Create(char*str){


	if (strcmp(str, "--comments") == 0){
		
		return new Comments;
		
	}

	if (strcmp(str, "--format") == 0){
		
		return new Formatter;
	}
	if (strcmp(str, "--newlines=LF") == 0){
		
		return  new NewlinesLF;
	}
	if (strcmp(str, "--newlines=CRLF") == 0){

		return  new NewlinesCRLF;
	}
	if (strcmp(str, "--indentation=tabs") == 0){
	
		return new indentationTab;
	}

	if (strcmp(str, "--indentation=spaces") == 0){
	
		return new indentationSpaces;
	}

	if (strcmp(str, "--html") == 0){
		
		return  new HTML;
	}

	std::cout << "error in " << str << ": ";
	throw std::exception("Invalid command!\n");

}
