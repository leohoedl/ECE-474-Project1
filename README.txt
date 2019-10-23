ECE-474 Project 1
Leo Hoedl & Matthew McLauhglin
October 20, 2019
----------------------------------------------------
Contents of "instr.txt"
3
20
0 2 4 6
2 4 3 5
1 5 2 4
3
4
6
21
3
4
9
0



Key for "instr.txt"
		        I	D	B	
I0: R2 = R4 + R6        1	2	4     result: R2 = 21 + 4 = 25
I1: R4 = R3 * R5	2	3	13    result: R4 = 6 * 3 = 18
I2: R5 = R2 * R4	3	14	16    result: R5 = 25 - 18 = 7    <-- Dependency on I0 & I1

Final Reg File Contents
R1: 3
R2: 25
R3: 6
R4: 18
R5: 7
R6: 4
R7: 9
R8: 0
----------------------------------------------------
Steps for setting up project:
1) Start Visual Studio 2019
2) Create a new empty C++ project
3) Right click on the project in the solution explorer and add files
4) Add "Main.cpp" & "StructHeader.h"
5) Right click on the project in the solution explorer and select "Open Folder in File Explorer"
6) Drag and drop "instr.txt" into the same directory as "Main.cpp"
7) Rebuild the project and run the simulation
----------------------------------------------------
