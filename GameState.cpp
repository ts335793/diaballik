#include "GameState.h"
#include <iostream>
#include <algorithm>

Vector<SingleMove> GameState::GenerateMoves(const Pawn & pawn)
{
	Vector<SingleMove> output;

	if(pawn.HasBall())
	{
		int DX[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
		int DY[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
		for(int it = 0; it < 8; it++)
		{
			for(int i = 1; ; i++)
			{
				Position position(pawn.GetPosition().GetX() + i * DX[it], pawn.GetPosition().GetY() + i * DY[it]);
				if(position.IsInside() && (board.IsEmpty(position) || board.Get(position).GetColor() == pawn.GetColor()))
				{
					if(!board.IsEmpty(position))
					{
						output.Add(SingleMove(pawn.GetPosition(), position));
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	else
	{
		int DX[] = { -1, 0, 0, 1 };
		int DY[] = { 0, 1, -1, 0 };
		for(int it = 0; it < 4; it++)
		{
			Position position(pawn.GetPosition().GetX() + DX[it], pawn.GetPosition().GetY() + DY[it]);
			if(position.IsInside() && board.IsEmpty(position))
			{
				SingleMove singleMove(pawn.GetPosition(), position);
				output.Add(singleMove);
			}
		}
	}

	return output;
}

void GameState::ApplyMove(Move move)
{
	Vector<SingleMove> singleMoves = move.ToSingleMovesVector();
	for(int i = 0; i < singleMoves.GetCount(); i++)
	{
		SingleMove singleMove = singleMoves[i];
		board.Switch(singleMove.GetSource(), singleMove.GetDestination());
	}
}

void GameState::UndoMove(Move move)
{
	Vector<SingleMove> singleMoves = move.ToSingleMovesVector();
	for(int i = singleMoves.GetCount() - 1; i >= 0; i--)
	{
		SingleMove singleMove = singleMoves[i];
		board.Switch(singleMove.GetSource(), singleMove.GetDestination());
	}
}

bool GameState::PawnsMadeLine(PawnColor pawnColor)
{
	// czy gracze są połączeni
	for(int i = 0; i < PAWNS_QUANTITY; i++)
	{
		Pawn * pawn;
		if(pawnColor == WhitePawn)
		{
			pawn = &whitePlayerPawns[i];
		}
		else
		{
			pawn = &blackPlayerPawns[i];
		}

		if(pawn->GetPosition().GetX() != Board::WIDTH - 1)
		{
			bool foundPawn = false;
			int DY[] = {-1, 0, 1};
			for(int j = 0; j < 3; j++)
			{
				Position position(pawn->GetPosition().GetX() + 1, pawn->GetPosition().GetY() + DY[j]);
				if(position.IsInside() && !board.IsEmpty(position) && board.Get(position).GetColor() == pawnColor)
				{
					foundPawn = true;
					break;
				}
			}
			if(!foundPawn)
			{
				return false;
			}
		}
	}

	int quantity = 0;
	if(pawnColor == WhitePawn)
	{
		for(int i = 0; i < PAWNS_QUANTITY; i++)
		{
			Position position(whitePlayerPawns[i].GetPosition().GetX(), whitePlayerPawns[i].GetPosition().GetY() - 1);
			if(position.IsInside() && !board.IsEmpty(position) && board.Get(position).GetColor() != pawnColor)
			{
				quantity++;
			}
		}
	}
	else
	{
		for(int i = 0; i < PAWNS_QUANTITY; i++)
		{
			Position position(blackPlayerPawns[i].GetPosition().GetX(), blackPlayerPawns[i].GetPosition().GetY() + 1);
			if(position.IsInside() && !board.IsEmpty(position) && board.Get(position).GetColor() != pawnColor)
			{
				quantity++;
			}
		}
	}

	return quantity >= 3;
}

bool GameState::PawnsReachedEnemiesLine(PawnColor pawnColor)
{
	if(pawnColor == WhitePawn)
	{
		return whitePlayerPawns[PAWNS_QUANTITY / 2].GetPosition().GetY() == 0;
	}
	else
	{
		return blackPlayerPawns[PAWNS_QUANTITY / 2].GetPosition().GetY() == Board::HEIGHT - 1;
	}
}

bool GameState::StateIsFinal()
{
	return PawnsReachedEnemiesLine(WhitePawn) || PawnsReachedEnemiesLine(BlackPawn) ||
		PawnsMadeLine(WhitePawn) || PawnsMadeLine(BlackPawn);
}

Move GameState::GenerateMove(PawnColor pawnColor)
{
	Tuple2<Move, int> pair = AlphaBeta(0, -INF, INF, pawnColor);
	return pair.a;
}

Vector< Tuple2<Move, long long int> > GameState::GenerateMoves(PawnColor pawnColor)
{
	Vector< Tuple2<Move, long long int> > result;
	Pawn * pawns;

	if(pawnColor == WhitePawn)
	{
		pawns = whitePlayerPawns;
	}
	else
	{
		pawns = blackPlayerPawns;
	}

	bool threwBall = false;
	for(int i = 0; i < PAWNS_QUANTITY; i++)
	{
		const Pawn & firstPawn = pawns[i];

		if(firstPawn.HasBall())
		{
			threwBall = true;
		}

		Vector<SingleMove> firstPawnMoves = GenerateMoves(firstPawn);
		for(int fit = 0; fit != firstPawnMoves.GetCount(); fit++)
		{
			ApplyMove(Move(firstPawnMoves[fit]));

			if(!PawnsMadeLine(pawnColor))
			{
				result.Add(MakeTuple(Move(firstPawnMoves[fit]), GetHash()));

				for(int j = i; j < PAWNS_QUANTITY; j++)
				{
					const Pawn & secondPawn = pawns[j];

					if(threwBall && secondPawn.HasBall())
					{
						continue;
					}

					if(secondPawn.HasBall())
					{
						threwBall = true;
					}

					Vector<SingleMove> secondPawnMoves = GenerateMoves(secondPawn);
					for(int sit = 0; sit != secondPawnMoves.GetCount(); sit++)
					{
						ApplyMove(Move(secondPawnMoves[sit]));

						if(!PawnsMadeLine(pawnColor))
						{
							result.Add(MakeTuple(Move(firstPawnMoves[fit], secondPawnMoves[sit]), GetHash()));

							for(int k = j; k < PAWNS_QUANTITY; k++)
							{
								const Pawn & thirdPawn = pawns[k];

								if(threwBall && thirdPawn.HasBall())
								{
									continue;
								}

								if(!threwBall && !thirdPawn.HasBall())
								{
									continue;
								}

								Vector<SingleMove> thirdPawnMoves = GenerateMoves(thirdPawn);
								for(int tit = 0; tit != thirdPawnMoves.GetCount(); tit++)
								{
									ApplyMove(Move(thirdPawnMoves[tit]));

									if(!PawnsMadeLine(pawnColor))
									{
										result.Add(MakeTuple(Move(firstPawnMoves[fit], secondPawnMoves[sit], thirdPawnMoves[tit]), GetHash()));
									}

									UndoMove(Move(thirdPawnMoves[tit]));
								}
							}
						}

						UndoMove(Move(secondPawnMoves[sit]));
					}

					if(secondPawn.HasBall())
					{
						threwBall = false;
					}
				}
			}

			UndoMove(Move(firstPawnMoves[fit]));
		}

		if(firstPawn.HasBall())
		{
			threwBall = false;
		}
	}

	return result;
}

int GameState::RateState(PawnColor pawnColor)
{
	int output = 0;
	if(pawnColor == WhitePawn)
	{
		for(int i = 0; i < PAWNS_QUANTITY; i++)
		{
			output += (Board::HEIGHT - 1) - whitePlayerPawns[i].GetPosition().GetY();
		}
		output += 10 * ((Board::HEIGHT - 1) - whitePlayerPawns[PAWNS_QUANTITY / 2].GetPosition().GetY());
	}
	else
	{
		for(int i = 0; i < PAWNS_QUANTITY; i++)
		{
			output += blackPlayerPawns[i].GetPosition().GetY();
		}
		output += 10 * blackPlayerPawns[PAWNS_QUANTITY / 2].GetPosition().GetY();
	}
	return output;
}

bool MoveHashPairComparator(const Tuple2<Move, long long int> & a, const Tuple2<Move, long long int> & b)
{
	return a.b < b.b;
}

bool MoveHashPairEqual(const Tuple2<Move, long long int> & a, const Tuple2<Move, long long int> & b)
{
	return a.b == b.b;
}

Tuple2<Move, int> GameState::AlphaBeta(const int & depth, int alpha, int beta, PawnColor pawnColor)
{
	PawnColor waitingPawnsColor;
	if(activePawnsColor == WhitePawn) waitingPawnsColor = BlackPawn;
	else waitingPawnsColor = WhitePawn;

	PawnColor oppositePawnsColor;
	if(pawnColor == WhitePawn) oppositePawnsColor = BlackPawn;
	else oppositePawnsColor = WhitePawn;

	if(StateIsFinal())
	{
		if(PawnsReachedEnemiesLine(activePawnsColor) || PawnsMadeLine(waitingPawnsColor))
		{
			return MakeTuple(Move(), 1000000);
		}
		else
		{
			return MakeTuple(Move(), -1000000);
		}
	}
	else if(depth == MAX_ALPHABETA_DEPTH)
	{
		return MakeTuple(Move(), 2*RateState(activePawnsColor) - RateState(waitingPawnsColor));
	}
	else
	{
		if(depth % 2 == 0)
		{
			Vector< Tuple2<Move, long long int> > moves = GenerateMoves(pawnColor);
			Move bestMove = moves[0].a;

			std::random_shuffle(moves.begin(), moves.end());

			for(int it = 0; it != moves.GetCount(); it++)
			{
				ApplyMove(moves[it].a);
				Tuple2<Move, int> pair = AlphaBeta(depth + 1, alpha, beta, oppositePawnsColor);
				UndoMove(moves[it].a);
				if(alpha < pair.b)
				{
					alpha = pair.b;
					bestMove = moves[it].a;
				}
				if(beta <= alpha)
				{
					break;
				}
			}

			return MakeTuple(bestMove, alpha);
		}
		else
		{
			Vector< Tuple2<Move, long long int> > moves = GenerateMoves(pawnColor);
			Move bestMove = moves[0].a;

			std::random_shuffle(moves.begin(), moves.end());

			for(int it = 0; it != moves.GetCount(); it++)
			{
				ApplyMove(moves[it].a);
				Tuple2<Move, int> pair = AlphaBeta(depth + 1, alpha, beta, oppositePawnsColor);
				UndoMove(moves[it].a);
				if(pair.b < beta)
				{
					beta = pair.b;
					bestMove = moves[it].a;
				}
				if(beta <= alpha)
				{
					break;
				}
			}

			return MakeTuple(bestMove, beta);
		}
	}
}

long long int GameState::GetHash() const
{
	int positionA = whitePlayerPawns[PAWNS_QUANTITY / 2].GetPosition().ToInt();
	int positionB = blackPlayerPawns[PAWNS_QUANTITY / 2].GetPosition().ToInt();
	return ((board.GetPartOfHash() | (((long long int) positionA) << 49)) | (((long long int) positionB) << 55)); // position max length equals 6 bits
}