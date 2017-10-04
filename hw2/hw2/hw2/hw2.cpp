// hw2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<fstream>
#include<iostream>
#include<string>

using namespace std;

struct Square{
	char state;
	int value;
};

struct Evaluation{
	int row;
	int col;
	int score;
	string move;
	char state;
};

ofstream output("output.txt");

class Game{
	int width;
	Square **cell;
	int depth;
	Evaluation *eval;
	int remain;

public:
	Game(int width, Square **cell, int depth);
	void TempBoard(int d);
	int Score();
	int Minimax(int node, int d, bool isMax);
	int AlphaBeta();
};

Game::Game(int width, Square **cell, int depth){
	this->width = width;
	this->cell = cell;
	this->depth = depth;
}

int Game::Score(){
	int score_opponent = 0;
	int score_player = 0;
	for(int i = 0; i < width; ++i){
		for (int j = 0; j < width ; ++j){
			if(cell[i][j].state == opponent){
				score_opponent += cell[i][j].value;
			}
			else if(cell[i][j].state == play){
				score_player += cell[i][j].value;
			}
		}
	}
	score = score_player - score_opponent;
}

void Game::TempBoard(int d){

	remain = width * width;
	
	for(int i = 0; i < width; ++i){
		for (int j = 0; j < width ; ++j){
			if(cell[i][j].state == opponent){
				remain = remain - 1;
				
			}
			else if(cell[i][j].state == play){
				remain = remain -1;
				//score_player += cell[i][j].value;
			}
		}
	}
	//int score = score_player - score_opponent;
	if(d < depth){
		eval = new Evaluation[remain];
		int k = 0;
		for(int i = 0; i < width; ++i){
			for(int j = 0; j < width; ++j){
				if(cell[i][j].state == '.' && k < remain){
					eval[k].row = i;
					eval[k].col = j;
					eval[k].state = '.';
					++k;
				}
			}
		}
	}
}


int Game::Minimax(int node, int d, bool isMax){
	if(d == depth){
		return eval[node].score;
	}//(Stake()>= Raid())? Stake(): Raid()

	TempBoard(d);

	if(isMax){
		for(int i = 0; i < remain ; ++i){
			eval[i].state = opponent;
			int r = eval[i].row;
			int c = eval[i].col;
			cell[r][c].state = opponent;
			score[i] = Minimax(i, d+1, false);
			cell[r][c].state = '.';
		}
		for(int i = 1; i < remain; ++i){
			int j = 0;
			if(score[j] < score[i]){
				int temp;
				temp = score[j];
				score[j] = score[i];
				score[i] = temp;
			}
		}
		score = score[0];
	}
	else {
		for(int i = 0; i < remain ; ++i){
			eval[i].state = player;
			int r = eval[i].row;
			int c = eval[i].col;
			cell[r][c].state = player;
			Minimax(d+1, true);
		}
	}
	return 0;
}

int Game::AlphaBeta(){
}

int main()
{
	ifstream input("input.txt");
	int width;  // board width and height
	input >> width;
	string mode; // ¡°MINIMAX¡± or ¡°ALPHABETA¡± or ¡°COMPETITION¡±
	input >> mode;
	char play; // is either ¡°X¡± or ¡°O¡± and is the player which you will play on this turn
	input >> play;
	int depth; // the depth of your search
	input >> depth;
	Square **cell = new Square*[width];
	for(int i = 0; i < width; ++i){   // cell values
		cell[i] = new Square[width];
		for(int j = 0; j < width; ++j){
			input >> cell[i][j].value;
		}
	}
	for(int i = 0; i < width; ++i){ // board state
		for(int j = 0; j< width; ++j){
			input >> cell[i][j].state;
		}
	}
	char opponent;
	if(play == 'X')
		opponent == 'O';
	else opponent = 'X';
	Game g(width, cell, depth);

	/*int score;
	if (mode == "MINIMAX")
	score = g.Minimax( 0, true);
	else if (mode == "ALPHABETA")
	score = g.AlphaBeta();*/
	return 0;
}

