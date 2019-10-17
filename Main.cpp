/*
ECE 474 Project 1
Leo Hoedl, Matthew McLaughlin
*/

/*

────────────────████
───────────────█░░███
───────────────█░░████
────────────────███▒██─────████████
──────████████─────█▒█──████▒▒▒▒▒▒████
────███▒▒▒▒▒▒████████████░░████▒▒▒▒▒███
──██▒▒▒▒░▒▒████░░██░░░░██░░░░░█▒▒▒▒▒▒▒███
─██▒▒░░░░▒██░░░░░█▒░░░░░██▒░░░░░░░▒▒▒▒▒▒█
██▒░░░░░▒░░░░░░░░░▒░░░░░░░▒▒░░░░░░░▒▒▒▒▒██
█░░░░░░▒░░░██░░░░░░░░░░░░░██░░░░░░░░▒▒▒▒▒█
█░░░░░░░░█▒▒███░░░░░░░░░█▒▒███░░░░░░░▒▒▒▒█
█░░░░░░░████████░░░░░░░████████░░░░░░▒▒▒▒█
█░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒█
██░░░█░░░░░░░░░░░░░░░░░░░░░░░░░░░░░█░▒▒▒▒█
─█░░░░██░█░░░░░░░░░░░░░░░░░░░░░░░███▒▒▒▒▒█
─█▒▒░░░░█████░░░█░░░░██░░░██░░████░▒▒▒▒▒▒█
─██▒▒░░░░░█████████████████████░░░▒▒▒▒▒▒██
──██▒▒▒▒░░░░░██░░░███░░░██░░░█░░░▒▒▒▒▒▒██
───███▒▒▒░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒█████
─────███▒▒▒▒▒▒░░░░░░░░░░░░░▒▒▒▒▒▒████
────────██████████████████████████
Happy Halloween

*/

#include "StructHeader.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>
#include <stddef.h>
using namespace std;

ReservationStation addInstToRS(InstQueue, RAT[], RegisterFile[]);
int execute(Unit, int, int);
Unit RStoUnit(ReservationStation, int);
RAT updateRAT(ReservationStation, int);
void DisplayQueue(InstQueue[], int);
void DisplayUnit(Unit, int, int, int);
void DisplayRAT_REG(RAT[], RegisterFile[]);
void DisplayRS(ReservationStation[], int);

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
	int execUnitAdd = 10;
	int execUnitMul = -1;
	int AddSubCC = 2;
	int MulCC = 10;
	int DivCC = 40;
	int captureAns = NULL;
	int captureMul = NULL;
	int captureAdd = NULL;
	int execAns = NULL;
	int unitAddSubRSTarget = 0;
	int unitMulDivRSTarget = 0;
	int captureAddSubRSTarget = 0;
	int captureMulDivRSTarget = 0;
	int captureAddSubRegTarget = 0;
	int captureMulDivRegTarget = 0;


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
		RS[i] = { 0, NULL, NULL, NULL, NULL, NULL, NULL };
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

	/*Displays status of the structures*/
	while (cycleCount <= maxCycles) {

		/*********************************************************************************************/
		/*Update RS and tag RAT*/
		if ((headQueue <= tailQueue) && cycleCount > 0) {
			if (instQ[headQueue].opcode == 0 || instQ[headQueue].opcode == 1) {
				for (int i = 0; i < 9; i++) {
					if (RS[addSubRSTail].busy == 0) {
						RS[addSubRSTail] = addInstToRS(instQ[headQueue], rat, regFile);
						rat[RS[addSubRSTail].dest] = updateRAT(RS[addSubRSTail], addSubRSTail);
						AddUnit[addSubRSTail] = cycleCount + 1;
						addSubRSTail++;
						break;
					}
					else {
						if (addSubRSTail < 3) {
							addSubRSTail++;
						}
						else {
							addSubRSTail = 0;
						}
					}
				}
			}
			else if (instQ[headQueue].opcode == 2 || instQ[headQueue].opcode == 3) {
				for (int i = 0; i < 8; i++) {
					if (RS[multDivRSTail].busy == 0) {
						RS[multDivRSTail] = addInstToRS(instQ[headQueue], rat, regFile);
						rat[RS[multDivRSTail].dest] = updateRAT(RS[multDivRSTail], multDivRSTail);
						MulUnit[multDivRSTail] = cycleCount + 1;
						multDivRSTail++;
						break;
					}
					else {
						if (multDivRSTail < 5) {
							multDivRSHead++;
						}
						else {
							multDivRSTail = 3;
						}
					}
				}
			}
		}
		/***************************************************************************************************/
		/*Update Instruction Queue*/
		if (instQ[headQueue].opcode >= 0 && instQ[headQueue].opcode <= 3) {
			for (int i = 0; i < 10; i++) {
				if (instQ[i + 1].opcode < 0 && instQ[i + 1].opcode > 3) {
					instQ[i] = { NULL, NULL, NULL, NULL };
				}
				else {
					instQ[i] = instQ[i + 1];
				}
			}
			if (tailQueue >= -1) {
				tailQueue--;
			}
		}
		/***************************************************************************************************/
		/*Dispatch stage*/
		for (int i = 0; i < 3; i++) {
			if (muldiv.op == -1 && MulUnit[multDivRSHead] == cycleCount) {
				if (RS[multDivRSHead].T1 == NULL && RS[multDivRSHead].busy == 1) {
					if (RS[multDivRSHead].T2 == NULL) {
						muldiv = RStoUnit(RS[multDivRSHead], multDivRSHead);
						unitMulDivRSTarget = multDivRSHead;
						i = 2;
						if (multDivRSHead < 5)
							multDivRSHead++;
						else
							multDivRSHead = 3;
						if (muldiv.op = 2) {
							execUnitMul = cycleCount + 10;
							if (execUnitAdd == execUnitMul)
								execUnitAdd++;
						}
						else {
							execUnitMul = cycleCount + 40;
							if (execUnitAdd == execUnitMul)
								execUnitAdd++;
						}
						if (MulUnit[multDivRSHead] == cycleCount)
							MulUnit[multDivRSHead] = cycleCount + 1;
					}
					else {
						if (multDivRSHead < 5)
							multDivRSHead++;
						else
							multDivRSHead = 3;
					}
				}
				else {
					if (MulUnit[multDivRSHead] == cycleCount)
						MulUnit[multDivRSHead] = cycleCount + 1;
					if (multDivRSHead < 5)
						multDivRSHead++;
					else
						multDivRSHead = 3;
				}
			}
			else if (MulUnit[multDivRSHead] == cycleCount) {
				MulUnit[multDivRSHead] = cycleCount + 1;
				if (multDivRSHead < 5)
					multDivRSHead++;
				else
					multDivRSHead = 3;
			}
			else {
				if (multDivRSHead < 5)
					multDivRSHead++;
				else
					multDivRSHead = 3;
			}
		}

		for (int i = 0; i < 4; i++) {
			if (addsub.op == -1 && AddUnit[addSubRSHead] == cycleCount) {
				if (RS[addSubRSHead].busy == 1 && RS[addSubRSHead].T1 == NULL) {
					if (RS[addSubRSHead].T2 == NULL) {
						addsub = RStoUnit(RS[addSubRSHead], addSubRSHead);
						unitAddSubRSTarget = addSubRSHead;
						i = 3;
						if (addSubRSHead < 3)
							addSubRSHead++;
						else
							addSubRSHead = 0;
						if (AddUnit[addSubRSHead] == cycleCount)
							AddUnit[addSubRSHead] = cycleCount + 1;
						execUnitAdd = cycleCount + 2;
					}
					else {
						if (addSubRSHead < 3)
							addSubRSHead++;
						else
							addSubRSHead = 0;
					}
				}
				else {
					if (AddUnit[addSubRSHead] == cycleCount)
						AddUnit[addSubRSHead] = cycleCount + 1;
					if (addSubRSHead < 3)
						addSubRSHead++;
					else
						addSubRSHead = 0;
				}
			}
			else if (AddUnit[addSubRSHead] == cycleCount) {
				AddUnit[addSubRSHead] = cycleCount + 1;
				if (addSubRSHead < 3)
					addSubRSHead++;
				else
					addSubRSHead = 0;
			}
			else {
				if (addSubRSHead < 3)
					addSubRSHead++;
				else
					addSubRSHead = 0;
			}
		}

		execAns = execute(addsub, cycleCount, execUnitAdd);
		if (execAns != NULL) {
			captureAdd = cycleCount;
			captureAns = execAns;
			captureAddSubRSTarget = addsub.rsDst;
			captureAddSubRegTarget = addsub.regDst;
		}
		int execAnsmul = execute(muldiv, cycleCount, execUnitMul);
		if (execAnsmul != NULL) {
			captureMul = cycleCount;
			if (captureMul == captureAdd)
				captureAdd++;
			captureAns = execAnsmul;
			captureMulDivRSTarget = muldiv.rsDst;
			captureMulDivRegTarget = muldiv.regDst;
		}
		/***************************************************************************************/
		/*Broadcast*/
		int captureIndex;
		if (captureAdd == cycleCount && captureAns != NULL) {
			rat[captureAddSubRegTarget].reg = 0;
			rat[captureAddSubRegTarget].regTarget = captureAddSubRegTarget;
			regFile[captureAddSubRegTarget].regValue = captureAns;
			captureIndex = 0;
			for (int i = 0; i < 11; i++) {
				if (RS[captureIndex].busy == 1) {
					if (RS[captureIndex].T1 == captureAddSubRSTarget) {
						RS[captureIndex].V1 = captureAns;
						RS[captureIndex].T1 = NULL;
					}
					else if (RS[captureIndex].T2 == captureAddSubRSTarget) {
						RS[captureIndex].V2 = captureAns;
						RS[captureIndex].T2 = NULL;
					}
					else {
						if (captureIndex < 5)
							captureIndex++;
						else
							captureIndex = 0;
					}
				}
				else {
					if (captureIndex < 5)
						captureIndex++;
					else
						captureIndex = 0;
				}
			}
			RS[captureAddSubRSTarget] = { 0,NULL,NULL,NULL,NULL,NULL,NULL };
			addsub = { -1,0,0,0,0 };
		}
		else if (captureMul == cycleCount && captureAns != NULL) {
			rat[captureMulDivRegTarget].reg = 0;
			rat[captureMulDivRegTarget].regTarget = captureMulDivRegTarget;
			regFile[captureMulDivRegTarget].regValue = captureAns;
			captureIndex = 0;
			for (int i = 0; i < 11; i++) {
				if (RS[captureIndex].busy == 1) {
					if (RS[captureIndex].T1 == captureMulDivRSTarget) {
						RS[captureIndex].V1 = captureAns;
						RS[captureIndex].T1 = NULL;
					}
					else if (RS[captureIndex].T2 == captureMulDivRSTarget) {
						RS[captureIndex].V2 = captureAns;
						RS[captureIndex].T2 = NULL;
					}
					else {
						if (captureIndex < 5)
							captureIndex++;
						else
							captureIndex = 0;
					}
				}
				else {
					if (captureIndex < 5)
						captureIndex++;
					else
						captureIndex = 0;
				}

			}
			RS[captureMulDivRSTarget] = { 0,NULL,NULL,NULL,NULL,NULL,NULL };
			muldiv = { -1,0,0,0,0 };
		}

		cout << "\nCC: " << cycleCount << endl;
		DisplayQueue(instQ, tailQueue + 1);
		DisplayRAT_REG(rat, regFile);
		DisplayRS(RS, 5);
		DisplayUnit(addsub, cycleCount, execUnitAdd, unitAddSubRSTarget);
		DisplayUnit(muldiv, cycleCount, execUnitMul, unitMulDivRSTarget);
		cycleCount++;
	}
	/**********************************************************************************************************/

	cin.ignore();

}




/***************************************************************************************************************/
/*Methods for simulation*/
ReservationStation addInstToRS(InstQueue in, RAT file[], RegisterFile RF[]) {
	ReservationStation RS;
	RS.busy = 1;
	RS.op = in.opcode;
	RS.dest = in.dstReg;

	if (file[in.src1Reg].reg == 1) /*RAT pointing to RS*/
	{
		RS.T1 = file[in.src1Reg].regTarget;
		RS.V1 = NULL;
	}
	if (file[in.src2Reg].reg == 1) /*RAT pointing to RS*/
	{
		RS.T2 = file[in.src2Reg].regTarget;
		RS.V2 = NULL;
	}
	if (file[in.src1Reg].reg == 0)
	{
		RS.T1 = NULL;
		RS.V1 = RF[file[in.src1Reg].regTarget].regValue;
	}
	if (file[in.src2Reg].reg == 0)
	{
		RS.T2 = NULL;
		RS.V2 = RF[file[in.src2Reg].regTarget].regValue;
	}
	return RS;
}

RAT updateRAT(ReservationStation in, int RSin) {
	RAT newRat;
	newRat.regTarget = RSin;
	newRat.reg = 1;
	return newRat;
}

Unit RStoUnit(ReservationStation RS, int in) {
	Unit unit;
	unit.op = RS.op;
	unit.regDst = RS.dest;
	unit.regS = RS.V1;
	unit.regT = RS.V2;
	unit.rsDst = in;
	return unit;
}

int execute(Unit in, int cc, int targetcc) {
	int ans = 0;
	if (cc == targetcc) {
		if (in.op == 0) {
			ans = in.regS + in.regT;
		}
		if (in.op == 1) {
			ans = in.regS - in.regT;
		}
		if (in.op == 2) {
			ans = in.regS * in.regT;
		}
		if (in.op == 3) {
			ans = in.regS / in.regT;
		}
		return ans;
	}
	else
		return ans = NULL;
}

void DisplayQueue(InstQueue in[], int max) {
	int n = 0;
	int op;
	string temp;
	cout << "\nInstruction Queue" << endl;
	if (max > 0) {
		while (n < max) {
			op = in[n].opcode;
			if (op == 0) {
				temp = "Add";
			}
			if (op == 1) {
				temp = "Sub";
			}
			if (op == 2) {
				temp = "Mul";
			}
			if (op == 3) {
				temp = "Div";
			}
			cout << temp << ", " << "R" << in[n].dstReg << ", " << "R" << in[n].src1Reg << ", " << "R" << in[n].src2Reg << endl;
			n++;
			temp = "NULL";
		}
	}
	else
		cout << "Queue is Empty!\n";

	cout << "\n";
}

void DisplayUnit(Unit unit, int cc, int targetcc, int rsIndex) {
	cout << "\nUnit: \n";
	int op = unit.op;
	int countdown = targetcc - cc;
	string temp;
	if (op > -1 && countdown >= 0) {
		if (op == 0) {
			temp = "Add";
		}
		if (op == 1) {
			temp = "Sub";
		}
		if (op == 2) {
			temp = "Mul";
		}
		if (op == 3) {
			temp = "Div";
		}
		cout << temp << " " << "RS" << rsIndex << ", CC:  " << countdown << endl;
		cout << "\n";
	}
	else if (op == -1)
		cout << "Unit is empty!\n";
}

void DisplayRAT_REG(RAT rat[], RegisterFile regFile[]) {
	int count = 0;
	cout << "RegFile:      RAT: \n";
	while (count < 8) {
		if (rat[count].reg == 1) {
			cout << "R" << count << " " << regFile[count].regValue << "            " << "RS" << rat[count].regTarget << endl;
		}
		if (rat[count].reg == 0) {
			cout << "R" << count << " " << regFile[count].regValue << "            " << "R" << rat[count].regTarget << endl;
		}
		count++;
	}
	cout << "\n";
}

void DisplayRS(ReservationStation RS[], int max)
{
	int n = 0;
	int temp1;
	int temp2;
	int temp3;
	int temp4;
	int temp5;

	cout << "Reservation Station: \n     Busy  OP  T1  T2  V1  V2\n";
	while (n < max) {
		if (RS[n].op == NULL)
			temp1 = 0;
		else
			temp1 = RS[n].op;
		if (RS[n].T1 == NULL)
			temp2 = 0;
		else
			temp2 = RS[n].T1;
		if (RS[n].T2 == NULL)
			temp3 = 0;
		else
			temp3 = RS[n].T2;
		if (RS[n].V1 == NULL)
			temp4 = 0;
		else
			temp4 = RS[n].V1;
		if (RS[n].V2 == NULL)
			temp5 = 0;
		else
			temp5 = RS[n].V2;
		cout << "RS" << n << "| " << RS[n].busy << "    " << temp1 << "    " << temp2 << "    "
			<< temp3 << "    " << temp4 << "    " << temp5 << endl;
		n++;
	}
}
/************************************************************************************************************************************/