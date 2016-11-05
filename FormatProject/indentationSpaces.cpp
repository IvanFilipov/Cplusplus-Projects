#include "stdafx.h"
#include "indentationSpaces.h"

void indentationSpaces::Format(TextManager& manager){

	std::cout << "transforming all indentation symbols to four spaces ..";


	char buff[256];
	int curpos;
	int bufpos;
	bool changeMade;
	int nTabs;

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
		nTabs = 0;

		//reading spaces in the beginning of the line
		while (line[curpos] == '\t'){

			nTabs++;
			curpos++;
		}

		//copy spaces if they are less than four else write \t
		while (nTabs){

			changeMade = true;

			int times = 4;
			while (times){
				buff[bufpos] = ' ';
				bufpos++;
				times--;
			}

			nTabs--;
		}



		while (line[curpos] != '\0'){

			if (line[curpos] == '\n' || line[curpos] == '\r'){

				buff[bufpos] = line[curpos];
				bufpos++;
				curpos++;

				while (line[curpos] == '\t'){

					nTabs++;
					curpos++;
				}

				//for each tab write four space symbols
				while (nTabs){

					changeMade = true;

					int times = 4;

					while (times){
						buff[bufpos] = ' ';
						bufpos++;
						times--;
					}

					nTabs--;
				}

			}
			else{

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