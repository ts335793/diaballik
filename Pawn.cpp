#include "Pawn.h"

Pawn::Pawn()
{
}

Pawn::Pawn(const Pawn & pawn) : color(pawn.color), position(pawn.position), hasBall(pawn.hasBall)
{
}

Pawn::Pawn(const PawnColor & c, const Position & p) : color(c), position(p), hasBall(false)
{
}

void Pawn::SetColor(const PawnColor & color)
{
	this->color = color;
}

void Pawn::SetPosition(const Position & position)
{
	this->position = position;
}

const PawnColor & Pawn::GetColor() const
{
	return color;
}

const Position & Pawn::GetPosition() const
{
	return position;
}

void Pawn::Serialize(Stream & stream)
{
	int tmp = (int)color;
	stream % position % tmp % hasBall;
	color = (PawnColor)tmp;
}

bool Pawn::HasBall() const
{
	return hasBall;
}

void Pawn::GrabBall()
{
	hasBall = true;
}

void Pawn::ThrowBall()
{
	hasBall = false;
}