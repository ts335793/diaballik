#include "Move.h"

Move::Move()
{
}

Move::Move(SingleMove a)
{
	singleMoves.Add(a);
}

Move::Move(SingleMove a, SingleMove b)
{
	singleMoves.Add(a);
	singleMoves.Add(b);
}

Move::Move(SingleMove a, SingleMove b, SingleMove c)
{
	singleMoves.Add(a);
	singleMoves.Add(b);
	singleMoves.Add(c);
}

Move::Move(Vector<SingleMove> singleMoves)
{
	singleMoves <<= singleMoves;
}

Move::Move(const Move & move)
{
	singleMoves <<= move.singleMoves;
}

void Move::Add(const SingleMove & singleMove)
{
	singleMoves.Add(singleMove);
}

Vector<SingleMove> Move::ToSingleMovesVector()
{
	Vector<SingleMove> copy;
	copy <<= singleMoves;
	return copy;
}

String Move::ToString()
{
	String output = "";
	for(int i = 0; i < singleMoves.GetCount(); i++)
	{
		output += singleMoves[i].ToString();
	}
	return output;
}

void Move::Serialize(Stream & stream)
{
	stream % singleMoves;
}
