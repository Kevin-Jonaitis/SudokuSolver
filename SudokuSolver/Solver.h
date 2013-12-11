#include <vector>

using namespace std;


extern int **puzzle;
extern bool ***domain;
extern int N;
extern int p;
extern int q;
extern int SETTING;
extern int filledPositions;


void deleteObjects();
void initalizeDomain();

bool BT(vector<int> currentPosition);
void readInInput(string filename);

/* Format the output to print nicely to the screen*/
void printOutput();
void writeToFile(string filename);
bool generatePuzzle();
vector<int> createVectorOfValues();
void initalizeGenerator(string inputFile);
void printStatistics();
void runCalculations();
vector<int> runCalculation(int N, int M, int setting);