#ifndef _Diaballik_SingleMove_h_
#define _Diaballik_SingleMove_h_

#include <CtrlCore/CtrlCore.h>

#include "Position.h"

using namespace Upp;

class SingleMove : Moveable<SingleMove>
{
private:
	Position source;
	Position destination;
public:
	SingleMove();
	SingleMove(const SingleMove & singleMove);
	SingleMove(const Position & s, const Position & d);
	
	const Position & GetSource() const;
	const Position & GetDestination() const;
	
	void SetSource(const Position & position);
	void SetDestination(const Position & position);
	
	String ToString();
	
	void Serialize(Upp::Stream & stream);
};

#endif
