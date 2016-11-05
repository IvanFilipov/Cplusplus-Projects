#include "stdafx.h"
#include "Formatter.h"

//default ctor
Formatter::Formatter(){
	usedTabs = false;
	usedLF = false;
}

//returns line's first significant symbol 
char Formatter::NextLineBeg(int i,TextManager& manager){

	if (i >= manager.getnLines()){
		return '\0';
	}

	char*line = manager.GetLine(i);

	if (!line){
		return '\0';
	}

	int curpos = 0;

	while (line[curpos] == '\n' || line[curpos] == '\r'
		|| line[curpos] == ' ' || line[curpos] == '\t'){

		curpos++;
	}

	return line[curpos];



}

//we need to know if there are any \t and \r in order to not changing the file format
//if we dont't find any \r we will use only \n when formating else we will use \r\n
//if we dont't find any \t we will use "   " when formating else we will use \t

void Formatter::CheckUsedSym(TextManager &manager){
	bool findTab = false;
	bool findCRFL = false;

	unsigned int nlines = manager.getnLines();

	for (unsigned int i = 0; i < nlines ; i++){

		

		char*line = manager.GetLine(i);

		if (line == NULL){
			continue;
		}
		//search first five elements at the beginning for \t 
		//and last five at the end for \r

		int search = 5;
		int curlen = strlen(line);
		
		if (curlen > search){
			while (search){
				if (line[search] == '\t'){
					findTab = true;
				}
				if (line[curlen - search] == '\r'){
					findCRFL = true;
				}
				search--;
			}
		}
		else{
			for (unsigned k = 0; k < curlen; k++){
				if (line[k] == '\t'){
					findTab = true;
				}
				if (line[k] == '\r'){
					findCRFL = true;
				}
			}
		}


		if (findTab&&findCRFL){
			usedTabs = true;
			usedLF = false;
			return;
		}
	}

	usedTabs = findTab;
	usedLF = !findCRFL;
}

//depends on the file's style adding newline symbol
void Formatter::AddNewLineSym(char *buff, int &bufpos){

	if (usedLF){
		buff[bufpos] = '\n';
		bufpos++;
	}
	else{
		buff[bufpos] = '\r';
		bufpos++;

		buff[bufpos] = '\n';
		bufpos++;
	}

}

//depends on file's style adding \t or four spaces  
void Formatter::AddindentationSym(char* buff, int& bufpos, int nBlocks){

	for (int i = 0; i < nBlocks; i++){
		if (usedTabs){
			buff[bufpos] = '\t';
			bufpos++;
		}
		else{
			for (int i = 0; i < 4; i++){
				buff[bufpos] = ' ';
				bufpos++;
			}
		}
	}

}

void Formatter::Format(TextManager& manager){

	std::cout << "formatting ...";

	CheckUsedSym(manager); 
	
	char buff[512];
	int curpos;
	int bufpos;
	int nBlocks =0; // current number of nested blocks
	int nOpenedBrks = 0;
	bool isSpecial; // 0 - if  the current character is not important for formating , 1 - else

	unsigned int nlines = manager.getnLines();


	for (unsigned int i = 0; i < nlines; i++){

		char*line = manager.GetLine(i);

		if (line == NULL){
			continue;
		}

		curpos = 0;
		bufpos = 0;
		memset(buff, 0, 512);

		//skipping the symbols we don not need 
		if (nBlocks){
			while (line[curpos ] == '\n' || line[curpos] == '\r'
				|| line[curpos ] == ' ' || line[curpos] == '\t'){

				curpos++;
			}
		}
		
		while (line[curpos] != '\0'){

			isSpecial = false;

			if (line[curpos] == '('){
				nOpenedBrks++;
				isSpecial = true;

				buff[bufpos] = line[curpos];
				bufpos++;
				curpos++;
			}


			if (line[curpos] == '{'){
				isSpecial = true;
			
				buff[bufpos] = line[curpos];
				bufpos++;
				
				
				      nBlocks++;

				while (line[curpos + 1] == '\n' || line[curpos + 1] == '\r'){
					curpos++;
				}

					AddNewLineSym(buff, bufpos);

					if (NextLineBeg(i + 1, manager) == '}')
						AddindentationSym(buff, bufpos, nBlocks - 1);
					else AddindentationSym(buff, bufpos, nBlocks);
				
				curpos++;
			}


			if (line[curpos] == '}'){
				isSpecial = true;

				if (bufpos){
				
					if (usedTabs){
				
						bufpos--;
					}
					else{

						for (int j = 4; j > 0; j--){
							buff[bufpos - j] = '\0';
							bufpos--;
						}
					}
				}
				

				buff[bufpos] = line[curpos];
				bufpos++;
				
				nBlocks--;
			
				while (line[curpos + 1] == '\n' || line[curpos + 1] == '\r'){
					curpos++;
				}
					AddNewLineSym(buff, bufpos);

					
					if (NextLineBeg(i + 1, manager) == '}')
						AddindentationSym(buff, bufpos, nBlocks - 1);
					else AddindentationSym(buff, bufpos, nBlocks);

				curpos++;
			}

			if (line[curpos] == ')'){

				nOpenedBrks--;

				isSpecial = true;
				buff[bufpos] = line[curpos];
				bufpos++;
				if (line[curpos + 1] != ';'&&(nOpenedBrks==0)){

					while (line[curpos + 1] == '\n' || line[curpos + 1] == '\r'){
						curpos++;
					}
						AddNewLineSym(buff, bufpos);

						if (NextLineBeg(i + 1, manager) == '}')
							AddindentationSym(buff, bufpos, nBlocks - 1);
						else AddindentationSym(buff, bufpos, nBlocks);
					
					
				}
				curpos++;
			}

			if (line[curpos] == '*'&&line[curpos + 1] == '/'){

				isSpecial = true;
				buff[bufpos] = line[curpos];
				bufpos++;
				curpos++;
				
				buff[bufpos] = line[curpos];
				bufpos++;
				curpos++;

				AddNewLineSym(buff, bufpos);
				AddindentationSym(buff, bufpos, nBlocks);
				
			}

			if (line[curpos] == '\r' || line[curpos] == '\n'){

				isSpecial = true;
				
				 

				while (line[curpos+1] == '\n' || line[curpos+1] == '\r'
					|| line[curpos+1] == ' ' || line[curpos+1] == '\t'){

					curpos++;
				}

				AddNewLineSym(buff, bufpos);
				AddindentationSym(buff, bufpos, nBlocks);
				curpos++;
			}

			if (line[curpos] == ';'){

				isSpecial = true;
				buff[bufpos] = line[curpos];
				bufpos++;
				

				while (line[curpos + 1] == '\n' || line[curpos + 1] == '\r'
					|| line[curpos + 1] == ' ' || line[curpos + 1] == '\t'){

					curpos++;
				}

					AddNewLineSym(buff, bufpos);

					if (NextLineBeg(i + 1, manager) == '}')
						AddindentationSym(buff, bufpos, nBlocks - 1);
					else AddindentationSym(buff, bufpos, nBlocks);


				curpos++;
			}

			if (!isSpecial){
				buff[bufpos] = line[curpos];
				curpos++;
				bufpos++;
				isSpecial = false;
			}

			
	}

		//if (changeMade){

			manager.SetLine(i, CreateNewLine(bufpos, buff));
		//}
	}

	std::cout << "done!\n";
}