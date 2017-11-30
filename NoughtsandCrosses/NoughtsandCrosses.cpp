// NoughtsandCrosses.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Squares
{
public:

	bool Empty = true;
	bool IsX = false;

	char PrintSquare()
	{
		if (!Empty)
		{
			if (IsX) { return 'X'; }
			else { return 'O'; }
		}
		else { return '-'; }
	}
};

struct Moves
{
	int Index = 0;
	int Cost = 0;
};

void PrintBoard(Squares* Board);
void GetPlayerTurn(Squares BoardOUT[]);
int GetAITurn(Squares BoardOUT[], int Level);
bool IsGameWon(Squares* Board, int& WinStateOUT);
bool AskToPlayAgain();

int main()
{
	bool IsPlayersTurn = true;
	int WinState = 0; //1 = Draw, 2 = Player, 3 = AI
	bool WantsToPlayAgain = true;

	while (WantsToPlayAgain)
	{
		Squares Board[9];
		do
		{
			PrintBoard(Board);
			if (IsPlayersTurn)
			{
				GetPlayerTurn(Board);
				cout << endl;
				IsPlayersTurn = false;
			}
			else
			{
				cout << "My turn!\n\n";
				GetAITurn(Board, 0);
				IsPlayersTurn = true;
			}
		} while (!IsGameWon(Board, WinState));
		PrintBoard(Board);
		if (WinState == 1)
		{
			cout << "No more moves available, its a draw!\n";
		}
		if (WinState == 2)
		{
			cout << "Congratulations! You won!\n";
		}
		else { cout << "Better luck next time, yove been beaten!\n"; }
		WantsToPlayAgain = AskToPlayAgain();
	}

	return 0;
}

void PrintBoard(Squares* Board)
{
	cout << "|" << Board[0].PrintSquare();
	cout << "|" << Board[1].PrintSquare();
	cout << "|" << Board[2].PrintSquare() << "|\n";
	cout << "|" << Board[3].PrintSquare();
	cout << "|" << Board[4].PrintSquare();
	cout << "|" << Board[5].PrintSquare() << "|\n";
	cout << "|" << Board[6].PrintSquare();
	cout << "|" << Board[7].PrintSquare();
	cout << "|" << Board[8].PrintSquare() << "|\n\n";
}

void GetPlayerTurn(Squares BoardOUT[])
{
	//get turn
	bool TurnIsValid = false;
	do
	{
		cout << "Please choose a square to make your move in. (1-9)\n\n";
		string Choice = "";
		getline(cin, Choice);
		try //To catch invalid argument exception from stoi
		{
			unsigned int SquareChosen = 0;
			SquareChosen = stoi(Choice) - 1; //Will break here if exception thrown
			if (SquareChosen < 0 || SquareChosen > 9)
			{
				cout << "Your didn't enter a position on the board...\n";
			}
			else if (BoardOUT[SquareChosen].Empty)
			{
				BoardOUT[SquareChosen].Empty = false;
				BoardOUT[SquareChosen].IsX = true;
				TurnIsValid = true;
			}
			else { cout << "That space is already taken...\n"; }
		}
		catch (const invalid_argument& ia)
		{
			cout << "No valid input recognised.\n";
		}
	} while (!TurnIsValid);
}

//Will search for terminal game state, will output value between 1-3 if game is over (1 = draw, 2 = PlayerWin, 3 = AIWin)
bool IsGameWon(Squares* Board, int& WinState)
{
	//search vertically
	for (int i = 0; i < 3; i++)
	{
		if (!Board[i].Empty)
		{
			if (
				Board[i].IsX == Board[i + 3].IsX
				&& !Board[i + 3].Empty
				&& Board[i].IsX == Board[i + 6].IsX
				&& !Board[i + 6].Empty)
			{
				if (Board[i].IsX)
				{
					WinState = 2;
				}
				else { WinState = 3; }
				return true;
			}
		}
	}

	//search horizontally
	for (int i = 0; i < 7; i = i + 3)
	{
		if (!Board[i].Empty)
		{
			if (
				Board[i].IsX == Board[i + 1].IsX
				&& !Board[i + 1].Empty
				&& Board[i].IsX == Board[i + 2].IsX
				&& !Board[i + 2].Empty)
			{
				if (Board[i].IsX)
				{
					WinState = 2;
				}
				else { WinState = 3; }
				return true;
			}
		}
	}

	//search diagonally
	if (!Board[0].Empty)
	{
		if (
			Board[0].IsX == Board[4].IsX
			&& !Board[4].Empty
			&& Board[0].IsX == Board[8].IsX
			&& !Board[8].Empty)
		{
			if (Board[0].IsX)
			{
				WinState = 2;
			}
			else { WinState = 3; }
			return true;
		}
	}
	if (!Board[2].Empty)
	{
		if (
			Board[2].IsX == Board[4].IsX
			&& !Board[4].Empty
			&& Board[2].IsX == Board[6].IsX
			&& !Board[6].Empty)
		{
			if (Board[2].IsX)
			{
				WinState = 2;
			}
			else { WinState = 3; }
			return true;
		}
	}

	//Check for draw
	for (int i = 0; i < 9; i++)
	{
		if (Board[i].Empty)
		{
			return false;
		}
	}
	WinState = 1;
	return true;
}

int GetAITurn(Squares BoardOUT[], int Level)
{
	//Check for winning state (for later iterations)
	int WinState = 0;
	if (IsGameWon(BoardOUT, WinState))
	{
		if (WinState == 1) 
		{
			return 0;
		}
		else if (WinState == 2)
		{
			return -10;
		}
		else { return 10; }
	}

	//Create vector of possible moves (hold index# of empty spaces)      
	vector<Moves> PossMoves;
	for (int i = 0; i < 9; i++)
	{
		if (BoardOUT[i].Empty)
		{
			Moves Possibility;
			Possibility.Index = i;
			PossMoves.push_back(Possibility);
		}
	}

	//Get cost value for each possible move
	for (unsigned int i = 0; i < PossMoves.size(); i++)
	{
		Squares NewBoard[9];
		for (int i = 0; i < 9; i++)
		{
			NewBoard[i] = BoardOUT[i];
		} // Populate new board with game
		if (Level % 2 == 0) //if ai turn, play O else play X
		{
			NewBoard[PossMoves[i].Index].Empty = false;
		}
		else
		{
			NewBoard[PossMoves[i].Index].Empty = false;
			NewBoard[PossMoves[i].Index].IsX = true;
		}

		PossMoves[i].Cost = GetAITurn(NewBoard, Level + 1);
	}

	//If initial call of function, play the best move.
	if (Level == 0)
	{
		Moves BestMove{0,-50};
		for (unsigned int i = 0; i < PossMoves.size(); i++)
		{
			if (PossMoves[i].Cost > BestMove.Cost)
			{
				BestMove = PossMoves[i];
			}
		}
		//Plays the best move to original board passed by ref
		BoardOUT[BestMove.Index].Empty = false; 
		return 10;
	}

	//If it's 'AI's turn' in recursive iterations, return max cost.
	else if (Level % 2 == 0)
	{
		Moves BestMove{ 0,-50 };
		for (unsigned int i = 0; i < PossMoves.size(); i++)
		{
			if (PossMoves[i].Cost > BestMove.Cost)
			{
				BestMove = PossMoves[i];
			}
		}
		return BestMove.Cost;
	}

	//If 'players turn', return min cost.
	else
	{
		Moves WorstMove{ 0,50 };
		for (unsigned int i = 0; i < PossMoves.size(); i++)
		{
			if (PossMoves[i].Cost < WorstMove.Cost)
			{
				WorstMove = PossMoves[i];
			}
		}
		return WorstMove.Cost;
	}
}

bool AskToPlayAgain()
{
	bool IsResponseValid = false;
	while (!IsResponseValid)
	{
		cout << "Would you like to try beat me again?\n";
		cout << "(Type Y id youd like to be beaten again, N if not.)\n";
		string Response;
		getline(cin, Response);
		if (Response[0] == 'Y' || Response[0] == 'y')
		{ 
			cout << "I like your positivity!\n";
			return true; 
		}
		else if (Response[0] == 'N' || Response[0] == 'n')
		{ return false;	}
		else
		{
			cout << "I didnt get that, use your words..\n";
				IsResponseValid = false;
		}
	}
}
