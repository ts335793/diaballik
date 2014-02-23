#ifndef _Diaballik_GameState_h_
#define _Diaballik_GameState_h_

#include <CtrlLib/CtrlLib.h>
#include "Board.h"
#include "Pawn.h"
#include "Mode.h"
#include "SingleMove.h"
#include "Move.h"
#include "PawnColor.h"

class GameState
{
public:
	static const int PAWNS_QUANTITY = 7;
	static const int MAX_ALPHABETA_DEPTH = 2;
	static const int INF = 1000000;
	
	Board board;
	Pawn whitePlayerPawns[7];
	Pawn blackPlayerPawns[7];
	bool isWhitePlayerHuman;
	bool isBlackPlayerHuman;
	PawnColor activePawnsColor;
	Vector<Move> turnsHistory;
	
	long long int GetHash() const;
	Tuple2<Move, int> AlphaBeta(const int & depth, int alpha, int beta, PawnColor pawnColor);
	int RateState(PawnColor pawnColor);
	bool PawnsMadeLine(PawnColor pawnColor);
	bool PawnsReachedEnemiesLine(PawnColor pawnColor);
	bool StateIsFinal();
	Vector<SingleMove> GenerateMoves(const Pawn & pawn);
	Vector< Tuple2<Move, long long int> > GenerateMoves(PawnColor pawnColor);
	Move GenerateMove(PawnColor pawnColor);
	void ApplyMove(Move move);
	void UndoMove(Move move);
};

#endif
