#ifndef _Diaballik_Move_h_
#define _Diaballik_Move_h_

#include <CtrlCore/CtrlCore.h>

#include "SingleMove.h"

using namespace Upp;

class Move : Moveable<Move>
{
private:
	Vector<SingleMove> singleMoves;
public:
	Move();
	Move(SingleMove a);
	Move(SingleMove a, SingleMove b);
	Move(SingleMove a, SingleMove b, SingleMove c);
	Move(Vector<SingleMove> singleMoves);
	Move(const Move & move);
	
	void Add(const SingleMove & singleMove);
	
	Vector<SingleMove> ToSingleMovesVector();
	String ToString();
	
	void Serialize(Stream & stream);
};

#endif
