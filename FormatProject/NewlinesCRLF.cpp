#include "stdafx.h"
#include "NewlinesCRLF.h"


void NewlinesCRLF::Format(TextManager& manager){

	std::cout << "making all newline symbols \\r\\n ...";

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

			if (line[curpos] == '\n'){

				changeMade = true;
				buff[bufpos] = '\r';
				bufpos++;
				buff[bufpos] = '\n';
				bufpos++;

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
	std::cout << "done! \n";
}
