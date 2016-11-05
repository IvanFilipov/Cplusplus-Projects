#pragma once

#include"stdafx.h"

//checks if a key is valid for the store
//valid key are strings , containing only digits with length KSIZE
//from 10000000 to 99999999 
bool isValid(const string&);


//takes a valid key and returns an integer
int HashFunc(const string&);

//returns a string ,which is a valid key
//and save the next one in a file called "NextName.txt"
string giveName();

