#pragma once

class InputReader
{
	private:
		char** commands;
		char ** paths;

		unsigned int nCommands;
		unsigned int npaths;

public:
	//big four
	InputReader();
	InputReader(const InputReader&);
	InputReader& operator=(const InputReader&);
	~InputReader();



	char ** getCommands()const;
	char ** getPaths()const;
	unsigned int getNumCommands()const;
	unsigned int getNumPaths()const;
	void Clear();
	void sortHTML();
	void Read(int,char**);
	bool isValid(char*);

};

