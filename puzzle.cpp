#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_set>
#include <utility>

using namespace std;

int num = 0;

class State {

public:

	//This field holds the values of 3X3 matrix
	vector<vector<int>> tiles;

	//value of h refers to number of wrong tiles when compared present state with goal
	int h;

	// value of g refers to number of nodes traversed from start state to the current state
	int g;

	State* parent;
	State(vector<vector<int>> tiles);

	State* generateNextStates(int x, int y, int pos);
	bool compareEquals(State& rhs);

	void setParent(State *parent)
	{
		(*this).parent = parent;
	};

	vector<vector<int>> getTiles()
	{
		return (*this).tiles;
	}

	int getG()
	{
		return (*this).g;
	}

	int getH()
	{
		return (*this).h;
	};

	void setG(State *parent)
	{
		if (parent != nullptr)
			(*this).g = (*parent).g + 1;
	};

	void setH(int h)
	{
		(*this).h = h;
	};

	vector<int> getMovableTileLocation();
	void calculateH(State *goal);
	void printState();
	void printTraversal();
};


class Comparator {
public:
	bool operator()(State* s1, State* s2) {
		return ( s1->getG() + s1->getH()) > ( s2->getG() + s2->getH());
	}
};

State::State(vector<vector<int>> tiles)
{
	(*this).tiles = tiles;
	(*this).parent = nullptr;
	(*this).g = 0;
	(*this).h = 0;
}


bool State:: compareEquals(State& rhs)
{
	for (int i = 0; i < (*this).tiles.size(); i++) {
		if ((*this).tiles[i] != rhs.tiles[i]) {
			return false;
		}
	}
	return true;
}

State* State::generateNextStates(int x, int y, int pos)
{
	State* newState = new State((*this).tiles);

	if (pos == 0 && (y - 1) >= 0 ) {
		swap((*newState).tiles[x][y], (*newState).tiles[x][y - 1]);
	}
	else if (pos == 1 && (x - 1) >= 0 ) {
		swap((*newState).tiles[x][y], (*newState).tiles[x - 1][y]);
	}
	else if (pos == 2 && (y + 1) <= 2 ) {
		swap((*newState).tiles[x][y], (*newState).tiles[x][y + 1]);
	}
	else if (pos == 3 && (x + 1) <= 2 ) {
		swap((*newState).tiles[x][y], (*newState).tiles[x + 1][y]);
	}
	else {
		return nullptr;
	}

	return newState;
}


void State::printState()
{	num += 1;
	int array_len = (*this).tiles.size();
	for (int i = 0; i < array_len; i++) {
		for (int j = 0; j < (*this).tiles[i].size(); j++) {
			if ((*this).tiles[i][j]  == 0)
				cout << '_' << ' ';
			else
				cout << (*this).tiles[i][j] << ' ';
		}
		cout << endl;
	}
}

void State::printTraversal()
{
	if (! ((*this).parent) ) {
		(*this).printState();
		cout << endl;
	}
	else {
		(* ((*this).parent) ).printTraversal();
		(*this).printState();
		cout << endl;
	}

	return;
}

// This function returns the position of movable tile _
vector<int> State::getMovableTileLocation()
{
	vector<int> v;
	vector<vector<int>> tiles = (*this).getTiles();
	int array_len = tiles.size();
	for (int i = 0; i < array_len; i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			if ((*this).tiles[i][j] == 0) {
				v.push_back(i);
				v.push_back(j);
				return v;
			}
		}
	}
	return v;
}

void State::calculateH(State *goal)
{
	int h = 0;
	vector<vector<int>> tiles = (*this).getTiles();
	int array_len = tiles.size();

	for (int i = 0; i < array_len; i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			if (tiles[i][j] != (*goal).tiles[i][j]) {
				h = h + 1 ;
			}
		}
	}
	(*this).setH(h);
}


string toStringFormat(vector<vector<int>> tiles) {

	int len = tiles.size();
	string result = "";
	for (int i = 0; i < len ; i++) {
		for (int  j = 0; j < tiles[i].size(); j++) {
			result += to_string(tiles[i][j]) ;
		}
	}
	return result;
}

void sequenceOfStates( State *start , State *goal) {

	num = 0;
	priority_queue<State*, vector<State*>, Comparator> q;
	unordered_set<string> visited_states;

	start->calculateH(goal);
	q.push(start);

	while (!q.empty()) {

		State* top_ele = q.top();
		q.pop();

		visited_states.insert( toStringFormat( (*top_ele).getTiles() ) );

		if ((*top_ele).compareEquals(*goal)) {
			cout << "The sequence is:" << endl;
			(*top_ele).printTraversal();
			cout << "The number of states in the sequence is: ";
			cout << (*top_ele).getG() + 1  << endl;
			cout << endl;
			return;
		}

		for (int i = 0; i < 4; i++) {

			vector<int> coord = top_ele->getMovableTileLocation();
			State *nxt = top_ele->generateNextStates(coord[0], coord[1], i);

			if (nxt != nullptr) {
				nxt->setParent(top_ele);
				nxt->calculateH(goal);
				nxt->setG(top_ele);

				string key_ = toStringFormat(nxt->getTiles());
				if (visited_states.find(key_) == visited_states.end()) {
					visited_states.insert(toStringFormat((*nxt).getTiles()));
					q.push(nxt);
				}

			}
		}
	}


}


//	main function
int main()
{
	
		cout << "This is an implementation of the 8-puzzle problem with the \"Tiles out of place\" heuristics." << endl ;
		cout << "Please enter an initial state: " ;

		string input;
		getline(cin, input);


		vector<vector<int>> input_state(3, vector<int>(3, 0));

		// reading the initial state into a 3X3 array
		int tmp = 0;
		int j = 0;
		for (int i = 0; i < input.length(); i++) {
			if (input[i] > 48  && input[i] < 58) {
				input_state[j][tmp] = input[i] - 48;
				tmp++;
			}
			if (input[i] == '_') tmp++ ;
			if (tmp == 3) {tmp = 0; j++;}
		}

		State *start = new State(input_state);

		vector<vector<int>> final_state = { {1, 2, 3}, {4, 5, 6}, {7, 8, 0} };
		State* goal = new State(final_state);

		sequenceOfStates(start, goal) ;

		// vector<vector<int>>  ex1;
		// State *ee;

		// ex1 = { {1,2,3}, {4,5,6}, {7,0,8} };
		// ex1 = { {2,0,1}, {6,3,7}, {4,5,8} };
		// ex1 = { {4, 5, 7}, {8, 1, 2}, {3, 6, 0} };
		// ex1 = { {1,2,3},{4,0,6},{7,5,8} };
		// ex1 = { {2,8,6},{7,5,3},{0,4,1} };
		// ex1 = { {1,2,3},{5,4,8},{7,6,0} };
		// ee = new State(ex1);
		// sequenceOfStates(ee, goal);
		// delete ee;


		delete start;
		delete goal;

	
	return 0;
}