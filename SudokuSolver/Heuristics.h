/* The possible values the x, y coordinate can take on*/
#include <vector>
#include <unordered_map>
#include "Solver.h"
#include "Position.h"
using namespace std;

vector<int> domainSetupPossibleValues(int x, int y);
vector<int> valuesInCell(int x, int y);
vector<int> valuesInRow(int x, int y);
vector<int> valuesInColumn(int x, int y);

/* Returns the next best value of the puzzle. Returns -1,-1 if there are no more possible values*/
vector<int> getNextPosition(vector<int> position);
void setValues();

/*Sets the starting domain by specifying what each value can initally be*/
void setStartingDomain(); //sets the starting domain

/* Return the next possible position scanning left to right, up to down. If nothing is available, simply return -1,-1 */
vector<int> rasterScan(vector<int> position);

/* Returns a list of possible values, based on if we are using FC or not*/
vector<int> possibleValues(vector<int> position);

/* Check if the assignment is valid*/
bool isValidAssignment(vector<int>, int value);

void update(unordered_map<vector<int>, int, PositionHash, PositionEqual> &setValues, unordered_map<vector<int>, vector<int>, PositionHash, PositionEqual> &eliminatedValues, vector<int> position, int value);

/* For each item in set values, remove that set from the puzzle*/
/* For each item in the domain, make "true" that item in the domain */
void undoUpdate(unordered_map<vector<int>, int, PositionHash, PositionEqual> &setValues, unordered_map<vector<int>, vector<int>, PositionHash, PositionEqual> &eliminatedValues, vector<int> position);

void removeDomainValuesInRow(unordered_map<vector<int>, vector<int>, PositionHash, PositionEqual> &eliminatedValues, vector<int> position, int value);
void removeDomainValuesInColumn(unordered_map<vector<int>, vector<int>, PositionHash, PositionEqual> &eliminatedValues, vector<int> position, int value);
void removeDomainValuesInCell(unordered_map<vector<int>, vector<int>, PositionHash, PositionEqual> &eliminatedValues, vector<int> position, int value);
void addValueToEliminatedValues(unordered_map<vector<int>, vector<int>, PositionHash, PositionEqual> &eliminatedValues, vector<int> position, int value);
vector<int> minimumRemainingValues();
bool degreeHeuristic(int x, int y, vector<int> minPosition);
int numberOfDegrees(int x, int y);
vector<int> leastConstrainingValue(vector<int> minimumRemainingValues);