#include "SingleMove.h"

SingleMove::SingleMove()
{
}

SingleMove::SingleMove(const SingleMove & singleMove) : source(singleMove.source), destination(singleMove.destination)
{
}

SingleMove::SingleMove(const Position & s, const Position & d) : source(s), destination(d)
{
}

const Position & SingleMove::GetSource() const
{
	return source;
}

const Position & SingleMove::GetDestination() const
{
	return destination;
}

void SingleMove::SetSource(const Position & position)
{
	source = position;
}

void SingleMove::SetDestination(const Position & position)
{
	destination = position;
}

String SingleMove::ToString()
{
	return String().Cat() << source.ToString() << destination.ToString();
}

void SingleMove::Serialize(Stream & stream)
{
	stream % source % destination;
}