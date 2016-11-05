#pragma once

#include"stdafx.h"


//initializing the base files 
//returns 1 if everything is alright
//        0 if there is a problem

bool Init();

//return the Key of the stored block
//if the operation fails than the function
//returns the "FFFFFFFF" key - which is a invalid key
//mode 1 safer , but slower - default one
//mode 0 less safe ,but faster
//outputs all results in the output stream, which by default is cout
string Store(const char*, int size,bool = 1, std::ostream& = std::cout);


//deletes a block from the store by a key
//returns 1 if the operation was successful
//returns 0 else ( not found or can't be deleted ) 
//outputs all results in the output stream, which by default is cout
bool Erase(const string& , std::ostream& = std::cout);


//reads a block form the store by a key
//returns 1 if the block were found
//returns 0 if fail to found or to load the wanted block
//outputs all results in the output stream, which by default is cout
bool Load(const string&, char*& , int& ,std::ostream& = std::cout);

//TODO
void Resize();




