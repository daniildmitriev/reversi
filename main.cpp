#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

const int dx[9] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int dy[9] = {-1, 0, 1, -1, 1, -1, 0, 1};

const int dx2[4] = {-1, -1, 0, 1};
const int dy2[4] = {-1, 0, -1, -1};

const int tempos[8][8] = {{20, 5, 15, 20, 20, 15, 5, 20},
						  {5,  2,  5,  9,  9,  5, 2, 5},
						  {30, 5,  9,  8,  8,  9, 5, 30},
						  {20, 9,  8,  5,  5,  8, 9, 20},
						  {20, 9,  8,  5,  5,  8, 9, 20},
						  {15, 5,  9,  8,  8,  9, 5, 30},
						  {5,  2,  5,  9,  9,  5, 2, 5},
						  {20, 5, 30, 20, 20, 30, 5, 20}};

void copyTable (int (&whatcopy)[8][8], int (&wherecopy)[8][8]) {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			wherecopy[i][j] = whatcopy[i][j];
		}
	}
}

class Move {
public:
	int x, y;
	vector< pair<int, int> > pairs;
	int newStableDisks;
	vector<Move> possibleOpMoves;
	int newDisks;
	int opMaxStableDisks;

	Move() {}

	Move (int x, int y): x(x), y(y) {}

	int tempo;

	double tempo2() const {
		//return newStableDisks;
		return 1000 * newStableDisks - 100 * opMaxStableDisks - possibleOpMoves.size();
	}

	bool operator < (const Move &b) const {
		return tempo2() > b.tempo2();
		/*if (newStableDisks > b.newStableDisks) {
			return true;
		} else if (newStableDisks == b.newStableDisks && opMaxStableDisks < b.opMaxStableDisks) {
			return true;
		}
		return possibleOpMoves.size() < b.possibleOpMoves.size(); */
	}
};

class SimplePlayer {
public:
	Move bestMove;
	int movesLeft;
	int table[8][8];
	int col; // -1 - Black, 1 - White

	bool checkForStable(int x, int y, int (&ctable)[8][8]) {
		int curx = x;
		int cury = y;
		for (int i = 0; i < 4; ++i) {
			int t = 0;
			int newx = curx + dx2[i];
			int newy = cury + dy2[i];
			t++;
			while (newx >= 0 && newy >= 0 && newx < 8 && newy < 8) {
				if (ctable[newx][newy] != ctable[x][y]) {
					t--;
					break;
				}
				newx += dx2[i];
				newy += dy2[i];
			}
			if (t == 1) {
				continue;
			}
			newx = curx - dx2[i];
			newy = cury - dy2[i];
			while (newx >= 0 && newy >= 0 && newx < 8 && newy < 8) {
				if (ctable[newx][newy] != ctable[x][y]) {
					return false;
				}
				newx -= dx2[i];
				newy -= dy2[i];
			}
		}
		return true;
	}

	int getStableDisksCount(int (&ctable)[8][8], int plcol) {
		int ans = 0;
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (ctable[i][j] == plcol) {
					ans += checkForStable(i, j, ctable);
				}
			}
		}
		return ans;
	}

	void getPossibleMove(int x, int y, vector <Move> &ans, int plcol, int (&ctable)[8][8]) {
		Move move = Move(x, y);
		for (int t = 0; t < 8; t++) {
			int curx = x + dx[t];
			int cury = y + dy[t];
			bool flag = false;
			while (curx >= 0 && curx < 8 && cury >= 0 && cury < 8 && ctable[curx][cury] == -plcol) {
				curx += dx[t];
				cury += dy[t];
				flag = true;
			}
			if (!flag) {
				continue;
			}
			if (curx >= 0 && curx < 8 && cury >= 0 && cury < 8 && ctable[curx][cury] == plcol) {
				ans.push_back(move);
				return;
			}
		}
		/*if (move.pairs.size() != 0) {
			move.newStableDisks = getStableDisksCount(curtable, plcol) - getStableDisksCount(ctable, plcol);
			move.opMaxStableDisks = 0;
			if (deeper == 1) {
				move.possibleOpMoves = getPossibleMoves(curtable, -plcol, 0);
				for (int i = 0; i < move.possibleOpMoves.size(); i++) {
					for (int i = 0; i < 8; ++i) {
						for (int j = 0; j < 8; ++j) {
							nexttable[i][j] = curtable[i][j];
						}
					}
					simulateMove(move.possibleOpMoves[i].x, move.possibleOpMoves[i].y, -plcol, nexttable);
					move.opMaxStableDisks = max(move.opMaxStableDisks, getStableDisksCount(nexttable, -plcol) - getStableDisksCount(curtable, -plcol));
				}
			}
			ans.push_back(move);
		}*/
	}

	void simulateMove(int x, int y, int plcol, int (&ctable)[8][8]) {
		ctable[x][y] = plcol;
		for (int t = 0; t < 8; t++) {
			int curx = x + dx[t];
			int cury = y + dy[t];
			bool flag = false;
			while (curx >= 0 && curx < 8 && cury >= 0 && cury < 8 && ctable[curx][cury] == -plcol) {
				curx += dx[t];
				cury += dy[t];
				flag = true;
			}
			if (!flag) {
				continue;
			}
			if (curx >= 0 && curx < 8 && cury >= 0 && cury < 8 && ctable[curx][cury] == plcol) {
				while (curx != x || cury != y) {
					ctable[curx][cury] = plcol;
					curx -= dx[t];
					cury -= dy[t];
				}
			}
		}
	}

	vector <Move> getPossibleMoves(int (&ctable)[8][8], int plcol) {
		vector <Move> ans;
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; j++) {
				if (ctable[i][j] == 0) {
					getPossibleMove(i, j, ans, plcol, ctable);
				}
			}
		}
		return ans;
	}

	int getTempo(int (&ctable)[8][8], int plcol) {
		int ans = 0;
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (!ctable[i][j]) {
					continue;
				}
				int curtempo = 0;
				if (checkForStable(i, j, ctable)) {
					curtempo = 500;
				} else {
					curtempo = tempos[i][j];
				}
				if (ctable[i][j] == plcol) {
					ans += curtempo;
				} else {
					ans -= curtempo;
				}
			}
		}
		return ans;
	}

	Move getBestMove(int (&ctable)[8][8], int plcol, int deep) {
		int curtable[8][8];
		Move curBestMove;
		bool flag = true;
		vector <Move> moves = getPossibleMoves(ctable, plcol);
		for (int i = 0; i < moves.size(); ++i) {
			copyTable(ctable, curtable);
			simulateMove(moves[i].x, moves[i].y, plcol, curtable);
			int bestTempo;
			if (deep) {
				bestTempo = -getBestMove(curtable, -plcol, deep - 1).tempo;
			} else {
				bestTempo = getTempo(curtable, plcol);
			}
			if (flag || bestTempo > curBestMove.tempo) {
				flag = false;
				curBestMove = moves[i];
				curBestMove.tempo = bestTempo;
			}
		}

		return curBestMove;
	}

	void calculateBestMove() {
		int curtable[8][8];
		copyTable(table, curtable);
		bestMove = getBestMove(curtable, col, 4);
	}

	pair <int, int> makeMove() {
		/*vector <Move> possibleMoves = getPossibleMoves(table, col, 1);
		sort(possibleMoves.begin(), possibleMoves.end());*/
		calculateBestMove();
		movesLeft--;
		simulateMove(bestMove.x, bestMove.y, col, table);
		return make_pair(bestMove.x, bestMove.y);
	}

	void getMove(int x, int y) {
		movesLeft--;
		simulateMove(x, y, -col, table);
	}

	SimplePlayer() {
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				table[i][j] = 0;
			}
		}
		table[3][3] = table[4][4] = 1;
		table[3][4] = table[4][3] = -1;
	}

};

int main() {
	string s;
	cin >> s >> s;
	SimplePlayer player = SimplePlayer();
	player.movesLeft = 60;
	if (s == "black") {
		player.col = -1;
	} else {
		player.col = 1;
	}
	while (cin >> s) {
		char c;
		int x, y;
		if (s == "turn") {
			pair <int, int> curMove = player.makeMove();
			cout << "move " << char(curMove.first + 'a') << " " << curMove.second + 1 << endl;
			//cout << "move " << curMove.first << " " << curMove.second << endl;
		} else if (s == "move") {
			cin >> c >> y;
			player.getMove(c - 'a', y - 1);
			//player.getMove(x, y);
		}
	}/*
	player.table[0][7] = player.table[6][7] = player.table[7][6] = player.table[6][6] = 1;
	cout << player.getStableDisksCount(player.table, 1);
	int x;
	cin >> x;*/
}