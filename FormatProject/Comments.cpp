#include "stdafx.h"
#include "Comments.h"


void Comments::Format(TextManager &manager){

	std::cout << "removing comments ...";

	char buff[256];
	int curpos ;
	int bufpos;
	bool hasComment;
	bool changeMade;
	//helps us to know if /**/ is still opened
	bool isOpenComment = false;

	unsigned int nlines = manager.getnLines();

	for (unsigned int i = 0; i < nlines; i++){
		
		char*line = manager.GetLine(i);

		if (line == NULL){
			continue;
		}

		 curpos = 0;
		 bufpos = 0;
		 hasComment = false;
		 changeMade = false;
		 memset(buff, 0, 256);

		while (line[curpos] != '\0'){

			if (isOpenComment){

				changeMade = true;
				while (line[curpos] != '\0'&&isOpenComment){
					if (line[curpos] == '*'){
						if (line[curpos + 1] == '/'){
							curpos++;
							isOpenComment = false;
						}
					}

					curpos++;
				}
				
			}

		

			if (line[curpos] == '/'){

				if (line[curpos + 1] == '/'){
					curpos = curpos + 2;
					hasComment = true;
				}
				else if (line[curpos + 1] == '*'){
					curpos = curpos + 2;
					isOpenComment = true;
				}

			}

			if (hasComment){
				changeMade = true;

				while (line[curpos] != '\0' && line[curpos] != '\n' && line[curpos] != '\r'){
					curpos++;
				}
				hasComment = false;
			}

			// else copy current character
		

			if (line[curpos] != '\0'&& !isOpenComment){
				buff[bufpos] = line[curpos];
				bufpos++;
				curpos++;
			}
		}

		if (changeMade){
			//we don't need this line , it is a comment
			if (isOpenComment){
				manager.SetLine(i, NULL);
				//continue to the next line
				continue;
			}

			manager.SetLine(i, CreateNewLine(bufpos, buff));
		}
	}
	
	std::cout << "done !\n";
}
