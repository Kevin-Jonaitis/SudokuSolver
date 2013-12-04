#include <vector>

using namespace std;

struct PositionHash {
	size_t operator()(const vector<int>& p) const {
		return p.at(0) + p.at(1) * N;
	}
};

struct PositionEqual {
	bool operator()(const vector<int>& c1, const vector<int>& c2) const {
		return (c1.at(0) == c2.at(0) && c1.at(1) == c2.at(1));
	}
};


//class Position {
//public:
//	int x;
//	int y;
//	Position(int x, int y);
//	size_t Position::HashValue() const; 
//	bool Position::Equals(const Position& other) const;
//};

