#pragma once
class TextManager
{
	char **lines;

	unsigned int nlines;

	bool HTML;

public:
	//the big four
	TextManager();
	~TextManager();
	TextManager& operator=(const TextManager&);
	TextManager(const TextManager&);

	void setHTML(bool);
	bool hasHTML()const;
	void Clear();
	void setNumLines(const char*);
	void Read(const char*);
	void Write(const char*);
	char * GetLine(unsigned int);
	void SetLine(unsigned int, char*);
	unsigned int getnLines()const;
};
