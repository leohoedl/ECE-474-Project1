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
#include <queue>
#include <iterator>
using namespace std;

bool Dispatch(Unit& u, ReservationStation RS[], int length);
bool checkALU(Unit u);
int checkRS(ReservationStation RS[], int length);

bool Issue();

int checkRAT(int index);

bool Broadcast(Unit& u, int result);
bool Execute(Unit& as, Unit& md);



void DisplayQueue();
void DisplayUnit(Unit);
void DisplayRAT_REG();
void DisplayRS();

int cycleCount = 1;
RAT rat[8];
queue<Inst> IQ;
int RF[8];
ReservationStation ASRS[3];
ReservationStation MDRS[2];

int main() {

	int maxInst;
	int input;
	int inputCount = 0;
	int maxCycles;

	Unit addsub;
	Unit muldiv;
	addsub.busy = false;
	muldiv.busy = false;

	ifstream inFile;
	string file;
	Inst in = { NULL,NULL,NULL,NULL };

	/*Initialize RAT*/
	for (int i = 0; i < 8; i++) {
		rat[i].reg = 0;
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
				in.opcode = input;
			}
			else if (n == 2) {
				in.dstReg = input;
			}
			else if (n == 3) {
				in.src1Reg = input;
			}
			else if (n == 4) {
				in.src2Reg = input;
				n = 0;
				cout << "I" << index << ": " << in.opcode << " " << in.dstReg << " " << in.src1Reg << " " << in.src2Reg << endl;
				index++;
				IQ.push(in);
			}
		}

		/*Register Input*/
		else {
			if (regIndex < 8) {
				RF[regIndex] = input;
				cout << "R" << regIndex << ": " << RF[regIndex] << endl;
				regIndex++;
			}
			inputCount++;
		}
	}

	for (cycleCount; cycleCount <= maxCycles; cycleCount++) {
		//Dispatch
		Dispatch(addsub, ASRS, size(ASRS));
		Dispatch(muldiv, MDRS, size(MDRS));
		//Issue
		Issue();
		//Broadcast
		Execute(addsub, muldiv);
		cout << "\nCC: " << cycleCount << endl;
		DisplayQueue();
		DisplayRAT_REG();
		DisplayRS();
		DisplayUnit(addsub);
		DisplayUnit(muldiv);
	}

	cin.ignore();

}

/*Display Functions*/
void DisplayQueue() {
	int n = 0;
	int op;
	string temp;
	queue<Inst> copy = IQ;
	cout << "\nInstruction Queue" << endl;
	while (!copy.empty()) {
		op = copy.front().opcode;
		if (op == 0)
			temp = "Add";
		if (op == 1)
			temp = "Sub";
		if (op == 2)
			temp = "Mul";
		if (op == 3)
			temp = "Div";
		cout << temp << ", " << "R" << copy.front().dstReg << ", " << "R" << copy.front().src1Reg << ", " << "R" << copy.front().src2Reg << endl;
		copy.pop();
		temp = "NULL";
	}
	cout << "End of Queue!\n";
	cout << "\n";
}

void DisplayUnit(Unit unit) {
	cout << "\nUnit: \n";
	int op = unit.ins.opcode;
	int countdown = unit.endCycle - cycleCount;
	string temp;
	if (op > -1 && countdown >= 0) {
		if (op == 0)
			temp = "Add";
		if (op == 1)
			temp = "Sub";
		if (op == 2)
			temp = "Mul";
		if (op == 3)
			temp = "Div";
		cout << temp << " " << "RS" << unit.RSsrc + 1 << ", Cycles left:  " << countdown << endl;
		cout << "\n";
	}
	if (op == -1)
		cout << "Unit is empty!\n";
}

void DisplayRAT_REG() {
	int count = 0;
	cout << "RegFile:      RAT: \n";
	while (count < 8) {
		if (rat[count].reg == 1) {
			cout << "R" << count << " " << RF[count] << "            " << "RS" << rat[count].RSTarget << endl;
		}
		if (rat[count].reg == 0) {
			cout << "R" << count << " " << RF[count] << "            " << "R" << count << endl;
		}
		count++;
	}
	cout << "\n";
}

void DisplayRS()
{
	cout << "Reservation Station: \n     Busy  OP  T1  T2  V1  V2\n";
	cout << "RS" << 1 << "| " << ASRS[0].busy << "    " << ASRS[0].op << "    " << ASRS[0].T1 << "    "
		<< ASRS[0].T2 << "    " << ASRS[0].V1 << "    " << ASRS[0].V2 << endl;
	cout << "RS" << 2 << "| " << ASRS[1].busy << "    " << ASRS[1].op << "    " << ASRS[1].T1 << "    "
		<< ASRS[1].T2 << "    " << ASRS[1].V1 << "    " << ASRS[1].V2 << endl;
	cout << "RS" << 3 << "| " << ASRS[2].busy << "    " << ASRS[2].op << "    " << ASRS[2].T1 << "    "
		<< ASRS[2].T2 << "    " << ASRS[2].V1 << "    " << ASRS[2].V2 << endl;
	cout << "RS" << 4 << "| " << MDRS[0].busy << "    " << MDRS[0].op << "    " << MDRS[0].T1 << "    "
		<< MDRS[0].T2 << "    " << MDRS[0].V1 << "    " << MDRS[0].V2 << endl;
	cout << "RS" << 5 << "| " << MDRS[1].busy << "    " << MDRS[1].op << "    " << MDRS[1].T1 << "    "
		<< MDRS[1].T2 << "    " << MDRS[1].V1 << "    " << MDRS[1].V2 << endl;

}
//Execution Functions
bool Dispatch(Unit& u, ReservationStation RS[], int length) {
	if (!checkALU(u)) {
		int index = checkRS(RS, length);
		if (index != -1) {
			u.ins = RS[index].ins;
			if (u.ins.opcode == 2)
				u.endCycle = cycleCount + 10;
			if (u.ins.opcode == 3)
				u.endCycle = cycleCount + 40;
			if (u.ins.opcode < 2) {
				u.endCycle = cycleCount + 2;
				u.RSsrc = index;
			}
			else
				u.RSsrc = index + 3;
			u.busy = true;
			return true;
		}
	}
	return false;
}
bool checkALU(Unit u) {
	return u.busy;
}
int checkRS(ReservationStation RS[], int length) {
	for (int i = 0; i < length; i++) {
		if (RS[i].busy && RS[i].T1 == NULL && RS[i].T2 == NULL)
			return i;
	}
	return -1;
}
bool Issue() {
	if (!IQ.empty()) {
		Inst inst = IQ.front();
		if (inst.opcode < 2) {
			for (int i = 0; i < size(ASRS); i++) {
				if (!ASRS[i].busy) {
					ASRS[i].busy = true;
					ASRS[i].dest = inst.dstReg;
					ASRS[i].ins = inst;
					ASRS[i].op = inst.opcode;
					if (checkRAT(inst.src1Reg) == -1)
						ASRS[i].V1 = RF[inst.src1Reg];
					else {
						ASRS[i].T1 = checkRAT(inst.src1Reg);
					}
					if (checkRAT(inst.src2Reg) == -1)
						ASRS[i].V2 = RF[inst.src2Reg];
					else {
						ASRS[i].T2 = checkRAT(inst.src2Reg);
					}
					rat[inst.dstReg].reg = true;
					rat[inst.dstReg].RSTarget = i + 1;
					IQ.pop();
					break;
				}
			}
		}
		if (inst.opcode > 1) {
			for (int i = 0; i < size(MDRS); i++) {
				if (!MDRS[i].busy) {
					MDRS[i].busy = true;
					MDRS[i].dest = inst.dstReg;
					MDRS[i].ins = inst;
					MDRS[i].op = inst.opcode;
					if (checkRAT(inst.src1Reg) == -1)
						MDRS[i].V1 = RF[inst.src1Reg];
					else {
						MDRS[i].T1 = checkRAT(inst.src1Reg);
					}
					if (checkRAT(inst.src2Reg) == -1)
						MDRS[i].V2 = RF[inst.src2Reg];
					else {
						MDRS[i].T2 = checkRAT(inst.src2Reg);
					}
					rat[inst.dstReg].reg = true;
					rat[inst.dstReg].RSTarget = i + 4;
					IQ.pop();
					break;
				}
			}
		}

		return true;
	}
	return false;
}
//Returns -1 if RAT points to reg
int checkRAT(int index) {
	if (rat[index].reg)
		return rat[index].RSTarget;
	return -1;
}
bool Broadcast(Unit& u, int result) {
	if (u.RSsrc < 3) {
		ASRS[u.RSsrc].busy = false;
		ASRS[u.RSsrc].dest = 0;
		ASRS[u.RSsrc].T1 = 0;
		ASRS[u.RSsrc].T2 = 0;
		ASRS[u.RSsrc].V1 = 0;
		ASRS[u.RSsrc].V2 = 0;
		ASRS[u.RSsrc].op = 0;

	}
	else {
		MDRS[u.RSsrc - 3].busy = false;
		MDRS[u.RSsrc - 3].T1 = 0;
		MDRS[u.RSsrc - 3].T2 = 0;
		MDRS[u.RSsrc - 3].V1 = 0;
		MDRS[u.RSsrc - 3].V2 = 0;
		MDRS[u.RSsrc - 3].dest = 0;
		MDRS[u.RSsrc - 3].op = 0;
	}
	int ind = checkRAT(u.ins.dstReg);
	for (int i = 0; i < 3; i++) {
		if (ASRS[i].T1 == ind && ASRS[i].busy) {
			ASRS[i].V1 = result;
			ASRS[i].T1 = 0;
		}
		if (ASRS[i].T2 == ind && ASRS[i].busy) {
			ASRS[i].V2 = result;
			ASRS[i].T2 = 0;
		}
	}
	for (int i = 0; i < 2; i++) {
		if (MDRS[i].T1 == ind && MDRS[i].busy) {
			MDRS[i].V1 = result;
			MDRS[i].T1 = 0;
		}
		if (MDRS[i].T2 == ind && MDRS[i].busy) {
			MDRS[i].V2 = result;
			MDRS[i].T2 = 0;
		}
	}
	if (rat[u.ins.dstReg].RSTarget == ind)
		rat[u.ins.dstReg].reg = false;
	RF[u.ins.dstReg] = result;
	u.busy = false;
	return true;
}
bool Execute(Unit& as, Unit& md) {
	int res;
	if (md.endCycle == cycleCount) {
		if (md.ins.opcode == 2) {
			res = RF[md.ins.src1Reg] * RF[md.ins.src2Reg];
		}
		if (md.ins.opcode == 3) {
			res = RF[md.ins.src1Reg] / RF[md.ins.src2Reg];
		}
		Broadcast(md, res);
		return true;
	}
	else if (as.endCycle == cycleCount) {
		if (as.ins.opcode == 0) {
			res = RF[as.ins.src1Reg] + RF[as.ins.src2Reg];
		}
		if (as.ins.opcode == 1) {
			res = RF[as.ins.src1Reg] - RF[as.ins.src2Reg];
		}
		Broadcast(as, res);
		return true;
	}
	return false;
}
