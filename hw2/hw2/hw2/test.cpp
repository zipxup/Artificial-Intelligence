#include "stdafx.h"
#include<fstream>
#include<iostream>
#include<string>

using namespace std;
#define INFINITY 1000

struct Square{
	char state;
	int value;
};

struct Eval{
	int row;
	int col;
	int value;
	int score;
	string move;
};

Eval *eval;
int remain;
int count = 0;
class Game{
	int width;
	int depth;
	char player;
	char opponent;
	Square **tempBoard; //when there is a change in the board, change tempBoard rather than board;

	void RemainCell(Square **board);
	int Score(Square **board);
	void Raid(int node, bool isMax);

public:
	Game(int width, int depth, char player, char opponent);
	int Minimax(Square **board, int d, bool isMax);
};

Game::Game(int width, int depth, char player, char opponent){
	this->width = width;
	this->depth = depth;
	this->player = player;
	this->opponent = opponent;
}


int Game::Score(Square **board){
	/*for(int i = 0; i < width; ++i){ // board state
			for(int j = 0; j< width; ++j){
				cout << board[i][j].state;
			}
			cout << endl;
		}*/
	int score_player = 0;
	int score_opponent = 0;
	int score = 0;
	for(int i = 0; i < width; ++i){
		for(int j = 0; j < width; ++j){
			if(board[i][j].state == player){
				score_player += board[i][j].value;
			}
			else if (board[i][j].state == opponent){
				score_opponent += board[i][j].value;
			}
		}
	}
	score = score_player - score_opponent;
	return score ;
}

void Game::Raid(int node, bool isMax){
	int r = eval[node].row;
	int c = eval[node].col;
	int left = r -1;
	int right = r + 1;
	int up = c - 1;
	int down = c + 1;
	if(isMax){
		if(left >= 0 && tempBoard[left][c].state == player){
			if( right < width && tempBoard[right][c].state == opponent){
				tempBoard[right][c].state = player;
			}
			else if( up >= 0 && tempBoard[r][up].state == opponent){				
				tempBoard[r][up].state = player;
			}
			else if( down < width && tempBoard[r][down].state == opponent){
				tempBoard[r][down].state = player;
			}
		}
		else if(right < width && tempBoard[right][c].state == player){
			if( left >= 0 && tempBoard[left][c].state == opponent){
				tempBoard[left][c].state = player;
			}
			else if( up >= 0 && tempBoard[r][up].state == opponent){
				tempBoard[r][up].state = player;
			}
			else if( down < width && tempBoard[r][down].state == opponent){
				tempBoard[r][down].state = player;
			}
		}
		else if(up >= 0 && tempBoard[r][up].state == player){
			if( right < width && tempBoard[right][c].state == opponent){
				tempBoard[right][c].state = player;
			}
			else if( left >= 0 && tempBoard[left][c].state == opponent){
				tempBoard[left][c].state = player;
			}
			else if( down < width && tempBoard[r][down].state == opponent){
				tempBoard[r][down].state = player;
			}
		}
		else if(down < width && tempBoard[r][down].state == player){
			if( right < width && tempBoard[right][c].state == opponent){
				tempBoard[right][c].state = player;
			}
			else if( left >= 0 && tempBoard[left][c].state == opponent){
				tempBoard[left][c].state = player;
			}
			else if( up >= 0 && tempBoard[r][up].state == opponent){
				tempBoard[r][up].state = player;
			}
		}
	}
	else{
		//tempBoard[r][c].state = opponent;
		if(left >= 0 && tempBoard[left][c].state == opponent){
			if( right < width && tempBoard[right][c].state == player){
				tempBoard[right][c].state = opponent;
			}
			else if( up >= 0 && tempBoard[r][up].state == player){
				tempBoard[r][up].state = opponent;
			}
			else if( down < width && tempBoard[r][down].state == player){
				tempBoard[r][down].state = opponent;
			}
		}
		else if(right < width && tempBoard[right][c].state == opponent){
			if( left >= 0 && tempBoard[left][c].state == player){
				tempBoard[left][c].state = opponent;
			}
			else if( up >= 0 && tempBoard[r][up].state == player){
				tempBoard[r][up].state = opponent;
			}
			else if( down < width && tempBoard[r][down].state == player){
				tempBoard[r][down].state = opponent;
			}
		}
		else if(up >= 0 && tempBoard[r][up].state == opponent){
			if( right < width && tempBoard[right][c].state == player){
				tempBoard[right][c].state = opponent;
			}
			else if( left >= 0 && tempBoard[left][c].state == player){
				tempBoard[left][c].state = opponent;
			}
			else if( down < width && tempBoard[r][down].state == player){
				tempBoard[r][down].state = opponent;
			}
		}
		else if(down < width && tempBoard[r][down].state == opponent){
			if( right < width && tempBoard[right][c].state == player){
				tempBoard[right][c].state = opponent;
			}
			else if( left >= 0 && tempBoard[left][c].state == player){
				tempBoard[left][c].state = opponent;
			}
			else if( up >= 0 && tempBoard[r][up].state == player){
				tempBoard[r][up].state = opponent;
			}
		}
	}

}


void Game::RemainCell(Square **board){
	
	remain = width * width;
	for(int i = 0; i < width; ++i){
		for(int j = 0; j < width; ++j){
			if(board[i][j].state == player){
				remain = remain - 1;
			}
			else if (board[i][j].state == opponent){
				remain = remain - 1;
			}
		}
	}
	eval = new Eval[2*remain];
	int k = 0;
	for(int i = 0; i < width; ++i){
		for(int j = 0; j < width; ++j){
			if(board[i][j].state == '.'&& k < 2*remain){
				eval[k].row = i;
				eval[k].col = j;
				eval[k].move = "Stake";
				++k;
				eval[k].row = i;
				eval[k].col = j;
				eval[k].move = "Raid";
				++k;
			}
		}
	}
}
int Game::Minimax(Square **board, int d, bool isMax){		
	if(d == depth){
		for(int i = 0; i < width; ++i){ // board state
			for(int j = 0; j< width; ++j){
				cout << board[i][j].state;
			}
			cout <<  "  ";
		}
		int s = Score(board);
		cout << count++ << " " << s << endl;
		return s;
	}
	RemainCell(board);
	
	if(isMax){
		int s = - INFINITY;
		tempBoard = new Square*[width];
		for(int i = 0; i < 2 * remain; ++i){
			for(int j = 0; j < width; ++j){
				tempBoard[j] = new Square[width];
				for(int k = 0; k < width; ++k){
					tempBoard[j][k].state = board[j][k].state;
					tempBoard[j][k].value = board[j][k].value;
				}
			}
			int r = eval[i].row;
			int c = eval[i].col;
			tempBoard[r][c].state = player;
			if(eval[i].move == "Raid"){
				Raid(i,isMax);
			}
			eval[i].score = Minimax(tempBoard, d+1, false);
			
			if( s < eval[i].score ){
				s = eval[i].score;
			}
		return s;
	}
	else{
		int s = INFINITY;
		tempBoard = new Square*[width];
		for(int i = 0; i < 2 * remain; ++i){
			for(int j = 0; j < width; ++j){
				tempBoard[j] = new Square[width];
				for(int k = 0; k < width; ++k){
					tempBoard[j][k].state = board[j][k].state;
					tempBoard[j][k].value = board[j][k].value;
				}
			}
			int r = eval[i].row;
			int c = eval[i].col;
			tempBoard[r][c].state = opponent;
			if(eval[i].move == "Raid"){
				Raid(i,isMax);
			}
			eval[i].score = Minimax(tempBoard, d+1, true);
			if(s > eval[i].score){
				s = eval[i].score;
			}
		return s;
	}
}

int main()
{
	ifstream input("input.txt");
	int width;  // board width and height
	input >> width;
	string mode; // ¡°MINIMAX¡± or ¡°ALPHABETA¡± or ¡°COMPETITION¡±
	input >> mode;
	char player; // is either ¡°X¡± or ¡°O¡± and is the player which you will play on this turn
	input >> player;
	int depth; // the depth of your search
	input >> depth;
	Square **board = new Square*[width];
	for(int i = 0; i < width; ++i){   // cell values
		board[i] = new Square[width];
		for(int j = 0; j < width; ++j){
			input >> board[i][j].value;
			
		}
	}
	for(int i = 0; i < width; ++i){ // board state
		for(int j = 0; j< width; ++j){
			input >> board[i][j].state;
		}
	}
	char opponent;
	if(player == 'X')
		opponent = 'O';
	else opponent = 'X';
	Game g(width, depth, player, opponent);

	int gameScore;
	if (mode == "MINIMAX")
		gameScore = g.Minimax( board, 0, true);

	ofstream output("output.txt");
	int row = 0;
	char col = 0;
	string move;
	for(int i = 0; i < 2* remain; ++i){
		if(eval[i].score == gameScore){
			int r = eval[i].row;
			int c = eval[i].col;
			board[r][c].state = player;
			cout << gameScore;
			row = eval[i].row + 1;
			col = char(eval[i].col + 65);
			move = eval[i].move;
			break;
		}
	}

	output << col << row << " " << move << endl;
	for(int i = 0; i < width; ++i){ // board state
		for(int j = 0; j< width; ++j){
			output << board[i][j].state;
		}
		output << endl;
	}
	return 0;
}