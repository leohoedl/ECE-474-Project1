/*
ECE 474 Project 1
Leo Hoedl, Matthew McLaughlin
*/

#include "StructHeader.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>
using namespace std;

int main() {

	int head_queue = 0;
	int tail_queue = 0;

	int addSubRSHead = 0;
	int addSubRSTail = 0;
	int multDivRSHead = 3;
	int multDivRSTail = 3;

	int maxInst;
	int input;
	int inputCount = 0;
	int maxCycles;
	int cycleCount;
	int AddUnit[5];
	int MulUnit[5];
	int AddSubCC = 2;
	int MulCC = 10;
	int DivCC = 40;

	RegisterFile regFile[8];
	ReservationStation RS[5];
	InstQueue instQ[10];
	RAT rat[8];
	Unit addsub = { -1,0,0,0,0 };
	Unit muldiv = { -1,0,0,0,0 };

	ifstream inFile;
	string file;

	/*Initialize instQueue*/
	for (int i = 0;i < 10;i++) {
		instQ[i] = { null, null, null, null };
	}

	/*Initialize RS*/
	for (int i = 0;i < 5;i++) {
		RS[i] = { 0, null, null, null, null, null, null };
	}

	/*Initialize RAT*/
	for (int i = 0;i < 8;i++) {
		rat[i].reg = 0;
		rat[i].regTarget = i;
	}


}
