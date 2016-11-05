#include "stdafx.h"
#include "TaskCleaner.h"


void TaskCleaner::Clear(int num, Command **coms){

	for (int i = 0; i < num; i++){
		delete coms[i];
	}

	delete coms;

}