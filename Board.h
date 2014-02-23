#ifndef _Diaballik_Board_h_
#define _Diaballik_Board_h_

class Pawn;
class Position;

class Board
{
private:
	Pawn * matrix[7][7];
	bool isHilighted[7][7];
	long long int partOfHash;
public:
	static const int WIDTH  = 7;
	static const int HEIGHT = 7;
	
	Board();
	Board(const Board & board);
	
	const Board & operator=(const Board & board);
	
	Pawn & Get(const Position & position) const;
	void Set(Pawn & pawn);
	bool IsEmpty(const Position & position) const;
	void Highlight(Position position);
	void TurnOffHighlights();
	bool IsHighlighted(const Position & position);
	void Switch(const Position & source, const Position & destination);
	long long int GetPartOfHash() const;
};

#endif
