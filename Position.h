#ifndef _Diaballik_Position_h_
#define _Diaballik_Position_h_

#include <CtrlCore/CtrlCore.h>

using namespace Upp;

struct Position
{
private:
	int x;
	int y;
public:
	Position();
	Position(const int & x, const int & y);
	Position(const Position & position);
	
	const Position & operator=(const Position & position);
	
	int GetX() const;
	int GetY() const;
	
	void SetX(const int & x);
	void SetY(const int & y);
	
	bool IsInside();
	
	int ToInt() const;
	
	String ToString();
	
	void Serialize(Stream & stream);
};

#endif
