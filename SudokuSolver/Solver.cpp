#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>    // std::random_shuffle
#include <cstdlib>      // std::rand, std::srand
#include <random>
#include <ctime>
#include <chrono>       // std::chrono::system_clock


#include "Utilities.h"
#include "Settings.h"
#include "Heuristics.h"
#include <unordered_map>
#include "Solver.h"
#include <math.h>

#include <sys\timeb.h> 

using namespace std;





int **puzzle = 0;
bool ***domain = 0; //the possible domain for the top-level puzzle
int N;
int p; // rows (in a cell)
int q; //columns (in a cell)
int SETTING = 0; //Setting level; Can be 1 - 5
int filledPositions = 0;
int M;
int timeout = 60000;


//Statistics
bool solutionFound;
struct timeb TOTAL_START;
struct timeb SEARCH_START;
struct timeb END;
struct timeb TOTAL_TIME;
struct timeb SEARCH_TIME;

int initalFilledPositions;



void printStatistics() {
	cout << "Time: " << (float) (1000.0 * (END.time - TOTAL_START.time) + (END.millitm - TOTAL_START.millitm)) << "\n";
	cout << "Assignments: " << filledPositions - initalFilledPositions << "\n";
	if (solutionFound)
		cout << "Solution: Yes" << "\n";
	else
		cout << "Solution: No" << "\n";
	if ((1000.0 * (END.time - TOTAL_START.time) + (END.millitm - TOTAL_START.millitm) > 60000))
		cout << "Timeout: Yes" << "\n";
	else
		cout << "Timeout: No" << "\n";

}

/*Reads in the input, checks to make sure the input is correct, and initilizes any settings */
void readInInput(string filename) {

	ifstream file(filename);
	file >> N;
	file >> p;
	file >> q;

	if (N != (p * q)) {
		cout << "There is an error in the input parameters.";
		return;
	}

	//allocate the 2D array
	puzzle = new int *[N];
	for (int i = 0; i < N; i++){
		puzzle[i] = new int[N];
	}


	//initalize the array
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++){
			string number;
			int number_int;
			file >> number;
			number_int = odometer_number_to_int(number);
			number_int = number_int + 1; //OFFSET BY 1, PROBLEM WITH MONSTER SUDOKU
			if (number_int != 0) {
				filledPositions++;
				initalFilledPositions++; //Only place this is used.
			}
			puzzle[i][j] = number_int;
		}
	}

	initalizeDomain();

}
void initalizeGenerator(string inputfile) {

	srand(time(0)); //set the seed to the current time

	ifstream file(inputfile);
	file >> N;
	file >> p;
	file >> q;
	file >> M;

	//allocate the 2D array
	puzzle = new int *[N];
	for (int i = 0; i < N; i++){
		puzzle[i] = new int[N];
	}

	setValues(); //Sets the global list of all possibles values for a spot in the Heursitics class


}


bool generatePuzzle() {
	int filledSpace = 0;

	
	//Initialize the puzzle to all 0's
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			puzzle[i][j] = 0;
		}
	}

	vector<vector<int>> positions;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			vector<int> position;
			position.push_back(i);
			position.push_back(j);
			//vector<int> position= { i, j };
			positions.push_back(position);
		}
	}

	while (filledSpace != M) {
		int get;
		rand();
		rand();
		get = rand() % (positions.size()); //0 through N * N - 1
		vector<int> position = positions.at(get);
		positions.erase(positions.begin() + get);

		vector<int> allValues = createVectorOfValues(); // Shuffled in method
		bool valueSet = false;

		while (!valueSet) {
			if (allValues.size() == 0) {
				return false;
			}
			get = rand() % allValues.size(); //1 through N
			int value = allValues.at(get);
			allValues.erase(allValues.begin() + get);

			if (isValidAssignment(position, value)) {
				puzzle[position.at(0)][position.at(1)] = value;
				filledSpace++;
				valueSet = true;
			}
		}
	}
}

vector<int> createVectorOfValues() {
	vector<int> result;
	for (int i = 1; i <= N; i++)
		result.push_back(i);
	return result;
}

bool BT(vector<int> currentPosition) {

	//Timing logic
	struct timeb CURRENT_TIME;
	ftime(&CURRENT_TIME);
	if ((1000.0 * (CURRENT_TIME.time - TOTAL_START.time) + (CURRENT_TIME.millitm - TOTAL_START.millitm) > timeout)) //60000 = 1 minute
		return false;

	unordered_map<vector<int>, vector<int>, PositionHash, PositionEqual> eliminatedValues;//hash map, implemented in 2011 standard, MIGHT NOT BE SUPPOPORTED ON THE ICS LAB COMPILER RIGGERMAJIGGER
	unordered_map<vector<int>, int, PositionHash, PositionEqual> setValues; //hash map, implemented in 2011 standard, MIGHT NOT BE SUPPOPORTED ON THE ICS LAB COMPILER RIGGERMAJIGGER

	vector<int> values = possibleValues(currentPosition);

	for (auto i : values) {
			if (isValidAssignment(currentPosition, i)){

			update(setValues,eliminatedValues, currentPosition, i); //combine with set values. as you add each set value, update the puzzle

			vector<int> nextPosition = getNextPosition(currentPosition);
			if (!(nextPosition.at(0) == -1 && nextPosition.at(1) == -1)) { //if it has next
				if (BT(nextPosition))
					return true;
				
			}
			else if (filledPositions = N * N){
				return true;
			}

			undoUpdate(setValues,eliminatedValues,currentPosition); //For each item in the domain, make "true" that item in the domain

		}
	}

	return false;
}

int main(int argc, char* argv[]) {
	ftime(&TOTAL_START);

	string mode = argv[1];
	if (mode == "S"){
		SETTING = std::stoi(argv[3]);
		readInInput(argv[2]);
		vector<int> start;
		start.push_back(0);
		start.push_back(-1);
		ftime(&SEARCH_START);
		solutionFound = BT(getNextPosition(start));
		ftime(&END);
		if (argc == 5)
			writeToFile(argv[4]);
		printOutput();
		printStatistics();
		deleteObjects();
	}
	else if (mode == "G") {
		initalizeGenerator(argv[2]);
		while(!generatePuzzle()); //do while a puzzle hasn't been fully generated
		writeToFile(argv[3]);
	}
	else if (mode == "C"){
		runCalculations();
	}
	else {
		cout << "Incorrect Input. Try again.";
	}
	cout << "Press enter to exit the program...";
	getchar();
}

void printOutput() {

	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
			if (puzzle[i][j] == 0)
				cout << puzzle[i][j] << " ";
			else
				cout << int_to_odometer_number(puzzle[i][j] - 1) << " ";//offset by 1, becuase we start the counting at 1 in the puzzles; 0 is a reserved number
		}
		cout << "\n";
	}
}

void writeToFile(string filename) {
	ofstream file(filename);
	file << N << " " << p << " " << q << "\n";
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
			if (puzzle[i][j] == 0)
				file << puzzle[i][j] << " ";
			else
				file << int_to_odometer_number(puzzle[i][j] - 1) << " ";//offset by 1, becuase we start the counting at 1 in the puzzles; 0 is a reserved number
		}
		file << "\n";
	}
}

void deleteObjects() {

	//deletes the 2D array which is the puzzle
	for (int i = 0; i < N; i++){
		delete[] puzzle[i];
	}
	delete[] puzzle;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++){
			delete[] domain[i][j]; //Why does this break things
		}
		delete[] domain[i];
	}
	delete[] domain;
}

void initalizeDomain() {

	//allocate 3D array of bool
	domain = new bool **[N];
	for (int i = 0; i < N; i++) {
		domain[i] = new bool *[N];
		for (int j = 0; j < N; j++){
			domain[i][j] = new bool[N+1];
		}
	}

	//initalize all values to false
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++){
			for (int k = 0; k <= N; k++) {
				domain[i][j][k] = false;
			}
		}
	}


	setStartingDomain();
}

void runCalcuationsTwo(ofstream& file, int i) {

	for (int j = 0; j <= i*i / 2; j++) { //only do up to the middle of M
		double failures = 0;
		double totals = 0;
		for (int k = 3; k <= 5; k++){
			for (int m = 0; m < 10; m++) {
				vector<int> results = runCalculation(i, j, k);
				if (results.at(4) == 0) //check for solvability
					failures++;
				totals++;
				double percent = failures / totals;
				if (percent > 0.50 && totals > 5) {
					file << std::flush;
					return;
				}

				for (auto res : results) {
					file << res << ",";
				}
				file << "\n";
			}
			file << std::flush;

		}
	}

}
void runCalculations(){

	srand(time(0)); //set the seed to the current time


	//vector<int> results = runCalculation(1,0,3);
	//ofstream fileName;
	ofstream file("50percentFailureStats.txt");
	for (int i = 11; i <= 40; i++){ // N
		runCalcuationsTwo(file, i);
	}

	file.close();
}

vector<int> runCalculation(int N_pass, int M_pass, int setting) {

	for (int i = sqrt(N_pass); i > 0; i--){
		if (N_pass % i == 0) {
			p = i;
			q = N_pass / p;
			break;
		}
	}
	N = N_pass;
	M = M_pass;
	SETTING = 0; //must be 0 for generator to run correctly
	setValues(); //DO NOT DELETE, OR FEEL THE WRATH OF A DEEPLY-SET PROGRAMMING BUG.

	//allocate the 2D array
	puzzle = new int *[N];
	for (int i = 0; i < N; i++){
		puzzle[i] = new int[N];
	}

	while(!generatePuzzle());
	//printOutput();

	ftime(&TOTAL_START);
	initalizeDomain();
	SETTING = setting;
	filledPositions = M;

	vector<int> start;
	start.push_back(0);
	start.push_back(-1);
	ftime(&SEARCH_START);
	solutionFound = BT(getNextPosition(start));
	//printOutput();
	deleteObjects();
	ftime(&END);

	int time;
	int solution;
	int timeout;
	int searchTime;

	time = (int)(1000.0 * (END.time - TOTAL_START.time) + (END.millitm - TOTAL_START.millitm));
	searchTime = (int)(1000.0 * (END.time - SEARCH_START.time) + (END.millitm - SEARCH_START.millitm));

	if (solutionFound)
		solution = 1;
	else
		solution = 0;
	if ((1000.0 * (END.time - TOTAL_START.time) + (END.millitm - TOTAL_START.millitm) > 60000))
		timeout = 1;
	else
		timeout = 0;


	
	vector<int> result;
	result.push_back(N);
	result.push_back(M);
	result.push_back(time);
	result.push_back(timeout);
	result.push_back(solution);
	result.push_back(SETTING);
	result.push_back(searchTime);

	return result;
}