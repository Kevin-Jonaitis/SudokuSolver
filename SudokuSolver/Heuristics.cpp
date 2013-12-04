#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>     // std::back_inserter
#include <set>
#include "Heuristics.h"
#include <map>

using namespace std;
vector <int> allPossibleChoices;


void setValues() {
	for (int i = 1; i <= N; i++) {
		allPossibleChoices.push_back(i);
	}
}


vector<int> valuesInColumn(int x, int y){
	vector<int> column;
	for (int i = 0; i < N; i++){
		if (puzzle[i][y] != 0 && !(i == x)) {
					column.push_back(puzzle[i][y]);
		}
	}
	return column;
}

vector<int> valuesInRow(int x, int y) {
	vector<int> row;
	for (int i = 0; i < N; i++){
		if (puzzle[x][i] != 0 && !(i == y)) {
			row.push_back(puzzle[x][i]);
		}
	}
	return row;
}

vector<int> valuesInCell(int y, int x){
	vector<int> values;
	int box_Y;
	int box_X;
	box_Y = floor(y / p);
	box_X = floor(x / q);
	for (int i = box_Y * p; i < (box_Y + 1) * p; i++){
		for (int j = box_X * q; j < (box_X + 1) * q; j++){
			if (puzzle[i][j] != 0 && !(i == y && j == x)) {
						values.push_back(puzzle[i][j]);
			}
		}
	}
	return values;
}

vector<int> domainSetupPossibleValues(int x, int y) {
	if (puzzle[x][y] != 0) {
		vector <int> simple;
		simple.push_back(puzzle[x][y]);
		return simple;
	}
	vector<int> temp;
	vector<int> temp2;
	vector<int> result;
	vector<int> row = valuesInRow(x, y);
	vector<int> column = valuesInColumn(x, y);
	vector<int> cell = valuesInCell(x, y);
	

	//SLOOOOOOOOoooOoOoW
	sort(row.begin(), row.end());
	sort(column.begin(), column.end());
	sort(cell.begin(), cell.end());
	sort(allPossibleChoices.begin(), allPossibleChoices.end());

	set_union(row.begin(), row.end(), column.begin(), column.end(), back_inserter(temp));
	set_union(temp.begin(), temp.end(), cell.begin(), cell.end(), back_inserter(temp2));
	set_difference(allPossibleChoices.begin(), allPossibleChoices.end(), temp2.begin(), temp2.end(), back_inserter(result));


	return result;
}

void setStartingDomain() {
	setValues();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			vector<int> possibilities = domainSetupPossibleValues(i, j);
			for (auto k : possibilities) {
				domain[i][j][k] = true;
			}
		}
	}
}
vector<int> rasterScan(vector<int> position) {
	vector<int> next_position;
	//cout << "i is" << position.at(0);
	//cout << "j is" << position.at(1);
	int i = position.at(0);
	int j = position.at(1);
	while(i < N){
		while(j < N) { //plus one (although I removed it) so we don't start on the current value we assigned
			if (puzzle[i][j] == 0){
				next_position.push_back(i);
				next_position.push_back(j);
				return next_position;
			}
			j++;
		}
		j = 0;
		i++;
	}
	next_position.push_back(-1);
	next_position.push_back(-1);

	return next_position;
}

vector<int> possibleValues(vector<int> position){

	vector<int> result;
	switch (SETTING){
	case 1:
		for (int i = 1; i <= N; i++){
			result.push_back(i);
		}
		break;
	case 2:
	case 3:
	case 4:
		for (int i = 1; i <= N; i++){
			if (domain[position.at(0)][position.at(1)][i])
				result.push_back(i);
		}
		break;
	case 5:
		result = leastConstrainingValue(position);
		break;
	}
	return result;

}

/* Check if a value is allowed to be somewhere. If it is, assign it. */
bool isValidAssignment(vector<int> position, int value) {
	if (SETTING != 1 && SETTING != 0) //uses any form of FC
		return domain[position.at(0)][position.at(1)][value];
	else {
		vector<int> possibleAssignements = domainSetupPossibleValues(position.at(0), position.at(1));
		if (find(possibleAssignements.begin(), possibleAssignements.end(), value) != possibleAssignements.end()) {
			return true;
		}
		else {
			return false;
		}
	}
}

/**
	Adds a value to the eliminatedValues map, eliminating duplicate code in the process.
*/

void removeDomainValuesInRow(unordered_map<vector<int>, vector<int>, PositionHash, PositionEqual> &eliminatedValues, vector<int> position, int value) {
	int x = position.at(0);
	int y = position.at(1);
	for (int i = 0; i < N; i++){
		vector<int> newPostion;
		newPostion.push_back(x);
		newPostion.push_back(i);
		if (!(i == y) && domain[x][i][value]) {
			domain[x][i][value] = false;
			addValueToEliminatedValues(eliminatedValues, newPostion, value);
		}
	}
}

void removeDomainValuesInColumn(unordered_map<vector<int>, vector<int>, PositionHash, PositionEqual> &eliminatedValues, vector<int> position, int value){
	int x = position.at(0);
	int y = position.at(1);
	for (int i = 0; i < N; i++){
		vector<int> newPostion;
		newPostion.push_back(i);
		newPostion.push_back(y);
		if (domain[i][y][value] && !(i == x)) {
			domain[i][y][value] = false;
			addValueToEliminatedValues(eliminatedValues, newPostion, value);
		}
	}
}

/* THIS COULD BE FLIPPED. WAS ORIGINALLY Y AND X*/
void removeDomainValuesInCell(unordered_map<vector<int>, vector<int>, PositionHash, PositionEqual> &eliminatedValues, vector<int> position, int value){
	int box_Y;
	int box_X;
	//WE FLIPPED THESE VALUES. BE CAREFUL
	int x = position.at(1);
	int y = position.at(0);
	box_Y = floor(y / p);
	box_X = floor(x / q);
	//cout << "Original x and y " << position.at(0) << " " << position.at(1);
	//cout << "\n";
	for (int i = box_Y * p; i < (box_Y + 1) * p; i++){
		for (int j = box_X * q; j < (box_X + 1) * q; j++){
		//	cout << i << " " << j << " " << puzzle[i][j] << "\n";
			vector<int> newPostion;
			newPostion.push_back(i);
			newPostion.push_back(j);
			if (domain[i][j][value] && !(i == y && j == x)) {
				domain[i][j][value] = false;
				addValueToEliminatedValues(eliminatedValues, newPostion, value);
			}
		}
	}
}

void addValueToEliminatedValues(unordered_map<vector<int>, vector<int>, PositionHash, PositionEqual> &eliminatedValues,vector<int> position, int value) {
	if (eliminatedValues.count(position)) {
		eliminatedValues.at(position).push_back(value);
	}
	else {
		vector<int> newEliminated;
		newEliminated.push_back(value);
		eliminatedValues.emplace(position, newEliminated);
	}
}

/*
Should be consistant across all hueristics
*/
void undoUpdate(unordered_map<vector<int>, int, PositionHash, PositionEqual> &setValues, unordered_map<vector<int>, vector<int>, PositionHash, PositionEqual> &eliminatedValues, vector<int> position) {
	//undo values that were set
	for (auto i : setValues) {
		vector<int> position = i.first;
		puzzle[position.at(0)][position.at(1)] = 0;
		filledPositions--;
	}

	for (auto i : eliminatedValues) {
		vector<int> position = i.first;
		vector<int> values = i.second;
		for (auto v : values){
			domain[position.at(0)][position.at(1)][v] = true;
		}
	}
}

vector<int> getNextPosition(vector<int> current_position){
	switch (SETTING) {
	case 1: //raster approach
	case 2:
		return rasterScan(current_position);
		break;
	case 3:
	case 4:
	case 5:
		return minimumRemainingValues();
		break;
	}
}

void update(unordered_map<vector<int>, int, PositionHash, PositionEqual> &setValues, unordered_map<vector<int>, vector<int>, PositionHash, PositionEqual> &eliminatedValues, vector<int> position, int value) {

	switch (SETTING){
	case 1: //BACKTRACKING ONLY
		setValues.emplace(position, value);
		puzzle[position.at(0)][position.at(1)] = value;
		filledPositions++;
		break;
	case 2: // BT + FC
	case 3: //min remaining
	case 4: // degree hueristics
	case 5: // Least Constraining Value
		//update the values
		setValues.emplace(position, value);//case 1
		puzzle[position.at(0)][position.at(1)] = value;
		filledPositions++;

		// Set all other domain variables to false, except the winner which is now true
		for (int i = 0; i <= N; i++) {
			//update the values which have been eliminated
			if (domain[position.at(0)][position.at(1)][i] && !(i == value)) {
				domain[position.at(0)][position.at(1)][i] = false;
				addValueToEliminatedValues(eliminatedValues, position, i);
			}
		}
		//FC Juciness
		removeDomainValuesInRow(eliminatedValues, position, value);
		removeDomainValuesInColumn(eliminatedValues, position, value);
		removeDomainValuesInCell(eliminatedValues, position, value);


	}
}

vector<int> minimumRemainingValues() {
	vector<int> minPosition;
	//Default to no position
	minPosition.push_back(-1);
	minPosition.push_back(-1); 
	int minCount = N * N + 100;
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
			if (puzzle[i][j] != 0) //skip over values that are already assigned
				continue;
			int testCount= 0; //there will never be a number this big; biggest it can be is N;
			for (int k = 1; k <= N; k++){
				if (domain[i][j][k]){
					testCount++;
				}
			}
			if (testCount ==  minCount && SETTING > 3) {
				//cout << "We have a tieeee";
				bool newPosWon = degreeHeuristic(i,j, minPosition);
				if (newPosWon) {
					minCount = testCount;
					minPosition.clear();
					minPosition.push_back(i);
					minPosition.push_back(j);
				}
			}else if (testCount <= minCount){
				minCount = testCount;
				minPosition.clear();
				minPosition.push_back(i);
				minPosition.push_back(j);
			}

		}
	}

	return minPosition;
}


int numberOfDegrees(int x, int y) {
	//column
	int totalNumber = 0;
	for (int i = 0; i < N; i++){
		if (puzzle[i][y] == 0 && !(i == x)) {
			totalNumber++;
		}
	}

	//row
	for (int i = 0; i < N; i++){
		if (puzzle[x][i] == 0 && !(i == y)) {
			totalNumber++;
		}
	}

	//cell
	int box_Y;
	int box_X;
	box_Y = floor(y / p);
	box_X = floor(x / q);
	for (int i = box_Y * p; i < (box_Y + 1) * p; i++){
		for (int j = box_X * q; j < (box_X + 1) * q; j++){
			if (j == x || i == y) //we already counted these
				continue;
			if (puzzle[i][j] == 0 && !(i == y && j == x)) {
				totalNumber++;
			}
		}
	}

	return totalNumber;

}
bool degreeHeuristic(int x, int y, vector<int> minPosition) {
	if (numberOfDegrees(x, y) > numberOfDegrees(minPosition.at(0), minPosition.at(1)))
		return true;
	else
		return false;

}

vector<int> leastConstrainingValue(vector<int> minimumRemainingValue){

	//unordered_map<vector<int>, vector<int>, PositionHash, PositionEqual> eliminatedValues;//hash map, implemented in 2011 standard, MIGHT NOT BE SUPPOPORTED ON THE ICS LAB COMPILER RIGGERMAJIGGER
	//unordered_map<vector<int>, int, PositionHash, PositionEqual> setValues; //hash map, implemented in 2011 standard, MIGHT NOT BE SUPPOPORTED ON THE ICS LAB COMPILER RIGGERMAJIGGER
	map<int, vector<int>> orderedValues;
	
	int x = minimumRemainingValue.at(0);
	int y = minimumRemainingValue.at(1);

	//cout << "x: " << x << " y: " << y << "\n";
	vector<int> possibleValues;
	for (int i = 1; i <= N; i++){
		if (domain[x][y][i])
			possibleValues.push_back(i);
	}

	for (auto p : possibleValues) {
		int countOnBoard = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++){
				if (puzzle[i][j] == p)
					countOnBoard++;
			}
		}

		if (orderedValues.count(countOnBoard)) {
			orderedValues.at(countOnBoard).push_back(p);
		}
		else {
			vector<int> newCount;
			newCount.push_back(p);
			orderedValues.emplace(countOnBoard, newCount);
		}
	}
	//for (auto i : possibleValues){
	//	
	//	update(setValues, eliminatedValues, minimumRemainingValue, i); //combine with set values. as you add each set value, update the puzzle
	//	int count = 0;
	//	for (unordered_map<vector<int>, vector<int>, PositionHash, PositionEqual>::iterator it = eliminatedValues.begin(); it != eliminatedValues.end(); ++it) {
	//		count = count + it->second.size();
	//	}
	//	undoUpdate(setValues, eliminatedValues, minimumRemainingValue); //combine with set values. as you add each set value, update the puzzle
	//	orderedValues.emplace(count, i);
	//}

	vector<int> ordered_results;
	for (map<int, vector<int>>::iterator it = orderedValues.begin(); it != orderedValues.end(); ++it){
		for (auto value : it->second){
			ordered_results.push_back(value);
		}
		//cout << "The count " << it->first << " the value " << it->second << "\n";
	}
	return ordered_results;

}