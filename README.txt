ECE-474 Project 1
Leo Hoedl & Matthew McLauhglin
October 20, 2019
----------------------------------------------------
Contents of "instr.txt"
7
68
1 1 2 3
3 3 5 4
2 6 1 2
0 5 3 6
1 7 5 4
2 1 7 5
0 8 7 1
0
16  
5  
7
21
10
7
9

Key for "instr.txt"
		            I	D	B	
I0: R1 = R2 - R3    1	2	4     
I1: R3 = R5 / R4	2	3	43
I2: R6 = R1 * R2	3	44	54  <-- Dependency on I0
I3: R5 = R3 + R5	4	44	46  <-- Dependency on I1
I4: R7 = R5 - R4	5	47	49  <-- Dependency on I3
I5: R1 = R4 * R5	44	55	65  <-- Dependency on I3
I6: R8 = R7 + R1	45	66	68  <-- Dependency on I4

Final Reg File Contents
R1: 11 
R2: 16
R3: 3
R4: 7
R5: 24
R6: 176
R7: 17
R8: 9
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
