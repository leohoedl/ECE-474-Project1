#pragma once
#define structures

struct RAT {
	bool reg; 
	int regTarget;
};

struct ReservationStation {
	int busy;
	int op;
	int dest;
	int V1;
	int V2;
	int T1;
	int T2;
};

struct RegisterFile {
	int regValue;
};

struct InstQueue {
	int opcode;
	int dstReg;
	int src1Reg;
	int src2Reg;
};

struct Unit {
	int op;
	int regDst;
	int regS;
	int regT;
	int rsDst;
};