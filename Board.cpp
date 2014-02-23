#include "Board.h"
#include "Pawn.h"
#include "Position.h"

Board::Board()
{
	for(int i = 0; i < WIDTH; i++)
	{
		for(int j = 0; j < HEIGHT; j++)
		{
			matrix[i][j] = 0;
			isHilighted[i][j] = false;
		}
	}
	partOfHash = 0;
}

Board::Board(const Board & board)
{
	for(int i = 0; i < WIDTH; i++)
	{
		for(int j = 0; j < HEIGHT; j++)
		{
			matrix[i][j] = board.matrix[i][j];
			isHilighted[i][j] = board.isHilighted[i][j];
		}
	}
}

const Board & Board::operator=(const Board & board)
{
	for(int i = 0; i < WIDTH; i++)
	{
		for(int j = 0; j < HEIGHT; j++)
		{
			matrix[i][j] = board.matrix[i][j];
			isHilighted[i][j] = board.isHilighted[i][j];
		}
	}
	
	return *this;
}

Pawn & Board::Get(const Position & position) const
{
	return *matrix[position.GetX()][position.GetY()];
}

void Board::Set(Pawn & pawn)
{
	matrix[pawn.GetPosition().GetX()][pawn.GetPosition().GetY()] = &pawn;
	partOfHash |= (1ll << pawn.GetPosition().ToInt());
}

bool Board::IsEmpty(const Position & position) const
{
	return matrix[position.GetX()][position.GetY()] == 0;
}

void Board::Highlight(Position position)
{
	isHilighted[position.GetX()][position.GetY()] = true;
}

void Board::TurnOffHighlights()
{
	for(int x = 0; x < WIDTH; x++)
	{
		for(int y = 0; y < HEIGHT; y++)
		{
			isHilighted[x][y] = false;
		}
	}
}

bool Board::IsHighlighted(const Position & position)
{
	return isHilighted[position.GetX()][position.GetY()];
}

void Board::Switch(const Position & source, const Position & destination)
{
	if(!IsEmpty(source))
	{
		Get(source).SetPosition(destination);
	}
	if(!IsEmpty(destination))
	{
		Get(destination).SetPosition(source);
	}
	
	Pawn * pawn = matrix[source.GetX()][source.GetY()];
	matrix[source.GetX()][source.GetY()] = matrix[destination.GetX()][destination.GetY()];
	matrix[destination.GetX()][destination.GetY()] = pawn;
	
	if(IsEmpty(source) || IsEmpty(destination))
	{
		partOfHash ^= (1ll << source.ToInt()); // switch
		partOfHash ^= (1ll << destination.ToInt()); // switch
	}
}

long long int Board::GetPartOfHash() const
{
	return partOfHash;
}