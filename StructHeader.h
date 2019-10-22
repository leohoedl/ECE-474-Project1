#pragma once
#define structures

struct RAT {
	bool reg; /*If rs_reg is equal to 1, then it is pointing to RS*/
	int RSTarget;
};

struct Inst {
	int opcode;
	int dstReg;
	int src1Reg;
	int src2Reg;
};

struct ReservationStation {
	int busy;
	int op;
	int dest;
	int V1;
	int V2;
	int T1;
	int T2;
	Inst ins;
};

struct Register {
	int regValue;
};



struct Unit {
	int endCycle;
	bool busy;
	Inst ins;
	int RSsrc;
};