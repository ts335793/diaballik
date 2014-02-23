#include "Position.h"
#include "Board.h"

Position::Position()
{
}

Position::Position(const int & x, const int & y) : x(x), y(y)
{
}

Position::Position(const Position & position) : x(position.x), y(position.y)
{
}

const Position & Position::operator=(const Position & position)
{
	x = position.x;
	y = position.y;
	return *this;
}

int Position::GetX() const
{
	return x;
}

int Position::GetY() const
{
	return y;
}

void Position::SetX(const int & x)
{
	this->x = x;
}

void Position::SetY(const int & y)
{
	this->y = y;
}

bool Position::IsInside()
{
	return 0 <= x && x < Board::WIDTH && 0 <= y && y < Board::HEIGHT;
}

String Position::ToString()
{
	String output;
	output << (char( 'a' + x)) << (char('0' + Board::HEIGHT - y));
	return output;
}

void Position::Serialize(Stream & stream)
{
	stream % x % y;
}

int Position::ToInt() const
{
	return y * 7 + x;
}