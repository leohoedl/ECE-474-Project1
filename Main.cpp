/*
ECE 474 Project 1
Leo Hoedl, Matthew McLaughlin
*/

#include "StructHeader.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>
#include <stddef.h>
using namespace std;

int main() {

	int headQueue = 0;
	int tailQueue = 0;

	int addSubRSHead = 0;
	int addSubRSTail = 0;
	int multDivRSHead = 3;
	int multDivRSTail = 3;

	int maxInst;
	int input;
	int inputCount = 0;
	int maxCycles;
	int cycleCount = 0;
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
		instQ[i] = { NULL, NULL, NULL, NULL };
	}

	/*Initialize RS*/
	for (int i = 0;i < 5;i++) {
		RS[i] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	}

	/*Initialize RAT*/
	for (int i = 0;i < 8;i++) {
		rat[i].reg = 0;
		rat[i].regTarget = i;
	}

	/*Responsible for opening file with ifstream*/
	while (1) {
		cout << "Enter file name (Make sure file is stored where Main.cpp is located)" << endl;
		cin >> file;
		cout << "Opening File" << endl;

		inFile.open(file);
		if (!inFile) {
			cout << "Failed to open file. Please Try Again" << endl;
		}
		else
			break;
	}

	/*Read instr.txt and assign appropriate values*/
	int n = 0;
	int index = 0;
	int regIndex = 0;

	while (inFile >> input) {
		/*Number of instructions input*/
		if (inputCount == 0) {
			maxInst = input;
			cout << "Number of Instructions: " << maxInst << endl;
			inputCount++;
		}
		/*Cycle input*/
		else if (inputCount == 1) {
			maxCycles = input;
			cout << "Number of Cycles: " << maxCycles << endl;
			inputCount++;
		}
		/*Instruction input*/
		else if (inputCount > 1 && inputCount <= ((maxInst * 4) + 1)) {
			n++;
			inputCount++;
			if (n == 1) {
				instQ[index].opcode = input;
			}
			else if (n == 2) {
				instQ[index].dstReg = input;
			}
			else if (n == 3) {
				instQ[index].src1Reg = input;
			}
			else if (n == 4) {
				instQ[index].src2Reg = input;
				n = 0;
				cout << "I" << index << ": " << instQ[index].opcode << " " << instQ[index].dstReg << " " << instQ[index].src1Reg << " " << instQ[index].src2Reg << endl;
				index++;
				tailQueue++;
			}
		}
		/*Register Input*/
		else {
			if (regIndex < 8) {
				regFile[regIndex].regValue = input;
				cout << "R" << regIndex << ": " << regFile[regIndex].regValue << endl;
				regIndex++;
			}
			inputCount++;
		}
	}
}
