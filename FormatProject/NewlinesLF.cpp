#include "stdafx.h"
#include "NewlinesLF.h"


void NewlinesLF::Format(TextManager& manager){

	std::cout << "making all newline symbols \\n ...";

	char buff[256];
	int curpos;
	int bufpos;
	bool changeMade;

	unsigned int nlines = manager.getnLines();


	for (unsigned int i = 0; i < nlines; i++){

		char*line = manager.GetLine(i);

		if (line == NULL){
			continue;
		}

		curpos = 0;
		bufpos = 0;
		memset(buff, 0, 256);
		changeMade = false;

		while (line[curpos] != '\0'){

			if (line[curpos] == '\r'){

				changeMade = true;
				buff[bufpos] = '\n';
				bufpos++;
				curpos++;

				//skipping the next newline char
				if (line[curpos] == '\n')
					curpos++;
			}
			else {
				//just copy the info
				buff[bufpos] = line[curpos];
				bufpos++;
				curpos++;
			}

		}

		if (changeMade){

			manager.SetLine(i, CreateNewLine(bufpos, buff));
		}

	}

	std::cout << "done ! \n";
}