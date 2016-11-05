#include "stdafx.h"
#include "indentationTab.h"

void indentationTab::Format(TextManager& manager){

	std::cout << "transforming all indentation symbols to \\t ...";

	char buff[256];
	int curpos;
	int bufpos;
	bool changeMade;
	int nSpaces;

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
		nSpaces = 0;

//reading spaces in the beginning of the line
		while (line[curpos]== ' '){

				nSpaces++;
				curpos++;
		}

		//copy spaces if they are less than four else write \t
		while (nSpaces){

			if (nSpaces >= 4){
				buff[bufpos] = '\t';
				bufpos++;
				nSpaces -= 4;
				changeMade = true;
			}
			else {
				buff[bufpos] = ' ';
				bufpos++;
				nSpaces--;
			}
		}



	while (line[curpos] != '\0'){

		//checking after each newline symbol
			if (line[curpos] == '\n' ||line[curpos]=='\r'){

				buff[bufpos] = line[curpos];
			    bufpos++;
				curpos++;

				while (line[curpos] == ' '){

						nSpaces ++;
						curpos++;
				}

				while (nSpaces){

					if (nSpaces >= 4){
						buff[bufpos] = '\t';
						bufpos++;
						nSpaces -= 4;
						changeMade = true;
					}
					else {
						buff[bufpos] = ' ';
						bufpos++;
						nSpaces--;
					}
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