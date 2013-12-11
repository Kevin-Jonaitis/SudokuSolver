We assume taht Monster soduku puzzles will start at 1, not 0, and therefore have offset reading in and outputing the data by 1.

To run the solver, input command line arguments like so:
SudokuSolver.exe S [inputFileName] [HeuristicLevel]

Ex: 
SodokuSolver.exe S hard.txt 5

Where inputFileName is the name of the file you want to program to solve
and HeuristicLevel is an number 1 - 5 of the Huertistic level you want to run. THe levels are detailed below:

Heuristic Levels:
1: Backtracking only (BT)
2: Backtracking plus Forward checking (BT + FC)
3: BT + FC + minimum remaining values (MRV)
4: BT + FC + MRV + Degree Heuristic (DH)
5: BT + FC + MRV + DH + Least Constraining Value (LCV)

To run the generator, input command line arguments like so:
SudokuSolver.exe G [inputFileName] [outputFileName]

Ex:
SudokuSolver.exe G input.txt output.txt

Where inputFileName is the name of the input file to read in from and outputFileName is the name of the output file.


