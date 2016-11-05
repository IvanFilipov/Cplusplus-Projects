#include "stdafx.h"
#include "HTML.h"


//defines often used keywords in c / c++

const char * keywords[] = { "int", "double", "float", "bool", "unsigned", "short",
"if", "else", "false", "friend", "sizeof", "const", "using", "namespace",
"long", "char", "void", "new", "delete", "true",
"class", "struct", "virtual", "while",
"for", "default", "do", "inline", "static",
"return", "break", "enum",
"try", "catch", "case", "switch",
"typedef", "union", "template" };

const int allwords = 37;





void HTML::ReadToNewline(char *dest, char * source, int& destpos, int &spos){

	while (source[spos] != '\r'&&source[spos] != '\n'&&source[spos] != '\0'){
		dest[destpos] = source[spos];
		destpos++;
		spos++;
	}



}

void HTML::ReadToNonDigit(char *dest, char * source, int& destpos, int &spos){

	while (isdigit(source[spos]) || (source[spos] <= 'f'&&source[spos] >= 'a')){
		dest[destpos] = source[spos];
		destpos++;
		spos++;
	}



}

void HTML::ReadToNonAlpha(char *dest, char * source, int& destpos, int &spos){

	while (isalpha(source[spos])){
		dest[destpos] = source[spos];
		destpos++;
		spos++;
	}

//	dest[destpos] = '\0';
//dest[destpos]++;

}

bool HTML::isSpecialWord(char* word, int pos){

	word[pos] = '\0';

	for (int i = 0; i < allwords; i++){
		if (strcmp(word, keywords[i]) == 0){
			return true;
		}	
	}

	return false;
}

void HTML::AddNewLineTag(char *buff, int& bufpos){

	buff[bufpos] = '<';
	bufpos++;
	buff[bufpos] = 'b';
	bufpos++;
	buff[bufpos] = 'r';
	bufpos++;
	buff[bufpos] = '>';
	bufpos++;
}

void HTML::AddSpaceTag(char*buff, int&bufpos){

	buff[bufpos] = '&';
	bufpos++;
	buff[bufpos] = 'n';
	bufpos++;
	buff[bufpos] = 'b';
	bufpos++;
	buff[bufpos] = 's';
	bufpos++;
	buff[bufpos] = 'p';
	bufpos++;
	buff[bufpos] = ';';
	bufpos++;
	
}

void HTML::AddDirectiveTag(char*buff, int&bufpos, char*directive, int symreaded){

	char* tag = "<font color=purple>";
	char*endTag = "</font>";

	strcat(buff, tag);
	bufpos += strlen(tag);

	for (int i = 0; i < symreaded; i++){


		if (directive[i] == '<'){

			buff[bufpos] = '&';
			bufpos++;
			buff[bufpos] = 'l';
			bufpos++;
			buff[bufpos] = 't';
			bufpos++;
			buff[bufpos] = ';';
			bufpos++;

			continue;
		}

		if (directive[i] == '>'){
			buff[bufpos] = '&';
			bufpos++;
			buff[bufpos] = 'g';
			bufpos++;
			buff[bufpos] = 't';
			bufpos++;
			buff[bufpos] = ';';
			bufpos++;



			continue;
		}

		buff[bufpos] = directive[i];
		bufpos++;


	}

	strcat(buff, endTag);
	bufpos += strlen(endTag);

}

void HTML::AddCommentTag(char*buff, int&bufpos, char*comment, int comlen){

	char* tag = "<font color=green>";
	char*endTag = "</font>";

	strcat(buff, tag);
	bufpos += strlen(tag);

	for (int i = 0; i < comlen; i++){

		buff[bufpos] = comment[i];
		bufpos++;


	}

	strcat(buff, endTag);
	bufpos += strlen(endTag);

}

void HTML::AddNumberTag(char*buff, int&bufpos, char*number, int ndigits){

	char* tag = "<font color=red>";
	char*endTag = "</font>";

	strcat(buff, tag);
	bufpos += strlen(tag);

	for (int i = 0; i < ndigits; i++){

		buff[bufpos] = number[i];
		bufpos++;


	}

	strcat(buff, endTag);
	bufpos += strlen(endTag);

}

void HTML::AddWordTag(char*buff, int&bufpos, char*word, int nalphas){

	char* tag = "<font color=blue>";
	char*endTag = "</font>";

	strcat(buff, tag);
	bufpos += strlen(tag);

	for (int i = 0; i < nalphas; i++){

		buff[bufpos] = word[i];
		bufpos++;


	}

	strcat(buff, endTag);
	bufpos += strlen(endTag);

}

void HTML::Format(TextManager& manager){

	std::cout << "transforming the code to HTML ...";

	manager.setHTML(true);

	char buff[4096];
	int curpos;
	int bufpos;
	bool changeMade;
	bool TagNeeded;

	bool isComOpened = false;
	bool isStringOpened = false;

	

	unsigned int nlines = manager.getnLines();


	for (unsigned int i = 0; i < nlines; i++){

		char*line = manager.GetLine(i);

		if (line == NULL){
			continue;
		}

		curpos = 0;
		bufpos = 0;
		TagNeeded = false;
		memset(buff, 0, 4096);
		changeMade = false;

		

		//skipping the symbols we don't need
		while (line[curpos] == '\n' || line[curpos] == '\r'){
			curpos++;
		}
		//dealing with spaces and tabs at line beginning
		while (line[curpos] == '\t'){
			for (int j = 0; j < 4; j++){
				AddSpaceTag(buff,bufpos);
			}
			curpos++;
		}

		while (line[curpos] == ' '){
				AddSpaceTag(buff, bufpos);
		
			curpos++;
		}

		if (line[curpos] == '#'&&!isComOpened){

			char directive[64];
			//memset(directive, 0, 64);
			int dirlen = 0;

            ReadToNewline(directive, line, dirlen, curpos);

			AddDirectiveTag(buff, bufpos, directive, dirlen);
		}

		
		while (line[curpos] != '\0'){


			if (line[curpos] == '\n' || line[curpos] =='\r'){
				changeMade = true;
				TagNeeded = true;
				AddNewLineTag(buff, bufpos);
				curpos++;

				//handle with \r\n
				if (line[curpos] == '\n'){
					curpos++;
				}

				while (line[curpos] == '\t'){
					for (int j = 0; j < 4; j++){
						AddSpaceTag(buff, bufpos);
					}
					curpos++;
				}

				while (line[curpos] == ' '){
					AddSpaceTag(buff, bufpos);

					curpos++;
				}

			}


			if (line[curpos] == '<'&&!isComOpened&&!isStringOpened){
				buff[bufpos] = '&';
				bufpos++;
				buff[bufpos] = 'l';
				bufpos++;
				buff[bufpos] = 't';
				bufpos++;
				buff[bufpos] = ';';
				bufpos++;
				TagNeeded = true;
				curpos++;
			}


			if (line[curpos] == '>'&&!isComOpened&&!isStringOpened){
				buff[bufpos] = '&';
				bufpos++;
				buff[bufpos] = 'g';
				bufpos++;
				buff[bufpos] = 't';
				bufpos++;
				buff[bufpos] = ';';
				bufpos++;
				TagNeeded = true;
				curpos++;
			}


			if (line[curpos] == '/' && line[curpos + 1] == '/'&&!isComOpened){


				char comment[128];

				int comlen = 0;
				
				ReadToNewline(comment, line, comlen, curpos);

				AddCommentTag(buff, bufpos, comment, comlen);

				TagNeeded = true;

			}


			if (line[curpos] == '/'&&line[curpos+1] == '*'){

				char* tag = "<font color=green>";
				strcat(buff, tag);
				bufpos += strlen(tag);
				isComOpened = true;

				buff[bufpos] = line[curpos];
				bufpos++;
				curpos++;

				buff[bufpos] = line[curpos];
				bufpos++;
				curpos++;


			}
			
			if (line[curpos] == '*'&&line[curpos + 1] == '/'&&isComOpened){

				char* tag = "</font>";
				strcat(buff, tag);
				bufpos += strlen(tag);
				isComOpened = false;

				buff[bufpos] = line[curpos];
				bufpos++;
				curpos++;

				buff[bufpos] = line[curpos];
				bufpos++;
				curpos++;
			}
			
			if (line[curpos] == '"'&&!isComOpened&&!isStringOpened){

				TagNeeded = true;
				char* tag = "<font color=orange>";
				strcat(buff, tag);
				bufpos += strlen(tag);
				isStringOpened = true;

				buff[bufpos] = line[curpos];
				bufpos++;
				curpos++;
			}

			if (line[curpos] == '"'&&!isComOpened&&isStringOpened){

				buff[bufpos] = line[curpos];
				bufpos++;
				curpos++;

				char* tag = "</font>";
				strcat(buff, tag);
				bufpos += strlen(tag);
				isStringOpened = false;

			}

			if (line[curpos] == '\''&&!isComOpened&&!isStringOpened){

				TagNeeded = true;
				char* tag = "<font color=brown>";
				strcat(buff, tag);
				bufpos += strlen(tag);

				
				buff[bufpos] = line[curpos];
				bufpos++;
				curpos++;
			    
				buff[bufpos] = line[curpos];
				bufpos++;
				curpos++;

				if (line[curpos] != '\''){
					buff[bufpos] = line[curpos];
					bufpos++;
					curpos++;
				}

				buff[bufpos] = line[curpos];
				bufpos++;
				curpos++;

				char* endtag = "</font>";
				strcat(buff, endtag);
				bufpos += strlen(endtag);
			}


			if (isdigit(line[curpos]) && !isComOpened && !isStringOpened)
			{

				TagNeeded = true;
				char number[32];
				int ndig = 0;

				ReadToNonDigit(number, line, ndig, curpos);

				AddNumberTag(buff, bufpos, number, ndig);



			}

			if (isalpha(line[curpos]) && !isComOpened && !isStringOpened)
			{

				TagNeeded = true;
				char word[32];
				memset(word, 0, 32);
				int nalpha = 0;

				ReadToNonAlpha(word, line, nalpha, curpos);


				if (isSpecialWord(word,nalpha)){
					AddWordTag(buff, bufpos, word, nalpha);
				}
				else{

					strcat(buff, word);
					bufpos += strlen(word);
				}

			}



			//just copy
			if (!TagNeeded){
				buff[bufpos] = line[curpos];
				bufpos++;
				curpos++;
			}
			
			TagNeeded = false;

		}

		if (changeMade){
			manager.SetLine(i, CreateNewLine(bufpos, buff));
		}
	}

	std::cout << "done! \n";
}

