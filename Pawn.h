#ifndef _Diaballik_Pawn_h_
#define _Diaballik_Pawn_h_

#include <CtrlCore/CtrlCore.h>

#include "Position.h"
#include "PawnColor.h"

using namespace Upp;

class Pawn
{
private:
	Position position;
	PawnColor color;
	bool hasBall;
	
public:
	Pawn();
	Pawn(const Pawn & pawn);
	Pawn(const PawnColor & c, const Position & p);
	
	void SetColor(const PawnColor & color);
	void SetPosition(const Position & position);
	
	const PawnColor & GetColor() const;
	const Position & GetPosition() const;
	
	void Serialize(Stream & stream);
	
	bool HasBall() const;
	void GrabBall();
	void ThrowBall();
};

#endif
