#include "MainWindow.h"
#include <iostream>
#include <algorithm>

#define IMAGEFILE <Diaballik/Fields.iml>
#include <Draw/iml_source.h>

void MainWindow::MainMenu(Bar & bar)
{
	bar.Add("File", THISBACK(FileMenu));
	bar.Add("New Game", THISBACK(NewGameMenu));
	bar.Add("Switch Mode", THISBACK(SwitchModeMenu));
}

void MainWindow::SwitchModeMenu(Bar & bar)
{
	bar.Add("Game mode", THISBACK1(SwitchMode, GameMode)).Help("Enter game mode.").Key(K_CTRL | K_G);
	bar.Add("Edit mode", THISBACK1(SwitchMode, EditMode)).Help("Edit game state.").Key(K_CTRL | K_E);
	bar.Add("History mode", THISBACK1(SwitchMode, HistoryMode)).Help("Browse history of current game.").Key(K_CTRL | K_H);
}

void MainWindow::NewGameMenu(Bar& bar)
{
	bar.Add("Human vs Human", THISBACK2(NewGame, true, true)).Help("Begin new game human vs human.").Key(K_CTRL | K_Z);
	bar.Add("Human vs AI", THISBACK2(NewGame, true, false)).Help("Begin new game human vs AI.").Key(K_CTRL | K_X);
	bar.Add("AI vs Human", THISBACK2(NewGame, false, true)).Help("Begin new game AI vs human.").Key(K_CTRL | K_C);
	bar.Add("AI vs AI", THISBACK2(NewGame, false, false)).Help("Begin new game AI vs AI.").Key(K_CTRL | K_V);
}

void MainWindow::FileMenu(Bar& bar)
{
	bar.Add("Open", THISBACK(Load)).Help("Open document from file").Help("Open document from file").Key(K_CTRL_O);
	bar.Add("Save", THISBACK(Save)).Help("Save document to file").Help("Save document to file").Key(K_CTRL_S);
}

void MainWindow::Load()
{
	Cout() << "loudf sie robi\n";
	if(!fs.ExecuteOpen())
		return;
	
	numberOfGame++; //!
	
	FileIn f(~fs);
	Serialize(f);
}

void MainWindow::Save()
{
	Cout() << "sejf sie robi\n";
	if(!fs.ExecuteSaveAs())
		return;
	
	FileOut f(~fs);
	Serialize(f);
}

void MainWindow::NotImplemented()
{
	GenerateMoves(WhitePawn);
}

MainWindow::MainWindow()
{
	CtrlLayout(*this, "Window title");
	boardArea.WhenClicked << THISBACK(WhenBoardClicked);
	
	getHint << THISBACK(WhenGetHint);
	applyMove << THISBACK(WhenApplyMove);
	undoChanges << THISBACK(WhenUndoChanges);
	
	previousState << THISBACK(PreviousState);
	nextState << THISBACK(NextState);
	continueFromHere << THISBACK(ContinueFromHere);
	goBackToTheFuture << THISBACK(GoBackToTheFuture);
	
	applyChanges << THISBACK(ApplyChanges);
	dropChanges << THISBACK(DropChanges);
	switchCurrentUser << THISBACK(WhenSwitchCurrentUser);
	
	getHint.Hide();
	applyMove.Hide();
	undoChanges.Hide();
	
	previousState.Hide();
	nextState.Hide();
	continueFromHere.Hide();
	goBackToTheFuture.Hide();
	
	applyChanges.Hide();
	dropChanges.Hide();
	switchCurrentUser.Hide();
	
	NewGame(true, false);
	countBallThrows = 0;
	countPawnPulls = 0;
	AddFrame(menu);
	AddFrame(status);
	menu.Set(THISBACK(MainMenu));
	
	numberOfGame = 0;
	
	RefreshBoard();
	
	menu.WhenHelp = status;
	fs.Type("save files", "*.save")
	  .DefaultExt("save");
}

Vector<SingleMove> MainWindow::GenerateMoves(const Pawn & pawn)
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

void MainWindow::NewGame(bool whitePlayerIsHuman, bool blackPlayerIsHuman)
{
	Cout() << "Nowa gra\n";
	numberOfGame++; // !
	
	UndoChanges(); // !
	
	this->isWhitePlayerHuman = whitePlayerIsHuman;
	this->isBlackPlayerHuman = blackPlayerIsHuman;
	
	board = Board();
	
	for(int i = 0; i < PAWNS_QUANTITY; i++)
	{
		whitePlayerPawns[i] = Pawn(WhitePawn, Position(i, Board::HEIGHT - 1));
		board.Set(whitePlayerPawns[i]);
	}
	
	for(int i = 0; i < PAWNS_QUANTITY; i++)
	{
		blackPlayerPawns[i] = Pawn(BlackPawn, Position(i, 0));
		board.Set(blackPlayerPawns[i]);
	}
	
	whitePlayerPawns[PAWNS_QUANTITY / 2].GrabBall();
	blackPlayerPawns[PAWNS_QUANTITY / 2].GrabBall();
	
	activePawnsColor = WhitePawn;
	
	
	
	SwitchMode(GameMode);
}

void MainWindow::Serialize(Stream & stream)
{
	Cout() << "Serializacja\n";
	
	UndoChanges(); // !
	
	for(int i = 0; i < 7; i++)
	{
		stream % whitePlayerPawns[i];
	}
	
	for(int i = 0; i < 7; i++)
	{
		stream % blackPlayerPawns[i];
	}
	
	int tmp = (int) activePawnsColor;
	stream % tmp % isWhitePlayerHuman % isBlackPlayerHuman % turnsHistory;
	activePawnsColor = (PawnColor) tmp;
	
	board = Board();
	
	for(int i = 0; i < 7; i++)
	{
		board.Set(whitePlayerPawns[i]);
		board.Set(blackPlayerPawns[i]);
	}
	
	SwitchMode(GameMode);
}

void MainWindow::RefreshBoard()
{
	Size size = boardArea.GetSize();
	//std::cout << "Rozmiar: " << size.cx << " " << size.cy << std::endl;
	int fieldWidth = size.cx / 7;
	int fieldHeight = size.cy / 7;
	int ellipseSize = fieldWidth / 2;
	int ballSize = ellipseSize - 10;
	
	DrawingDraw boardDraw(size);
	
	for(int y = 0; y < 7; y++)
	{
		for(int x = 0; x < 7; x++)
		{
			DrawingDraw fieldDraw(Size(fieldWidth, fieldHeight));
			
			if((x + y) % 2)
			{
				if(board.IsHighlighted(Position(x, y)))
				{
					fieldDraw.DrawImage(Size(fieldWidth, fieldHeight), IMAGECLASS::FieldAHilighted());
				}
				else
				{
					fieldDraw.DrawImage(Size(fieldWidth, fieldHeight), IMAGECLASS::FieldA());
				}
			}
			else
			{
				if(board.IsHighlighted(Position(x, y)))
				{
					fieldDraw.DrawImage(Size(fieldWidth, fieldHeight), IMAGECLASS::FieldBHilighted());
				}
				else
				{
					fieldDraw.DrawImage(Size(fieldWidth, fieldHeight), IMAGECLASS::FieldB());
				}
			}
			
			if(!board.IsEmpty(Position(x, y)))
			{
				if(board.Get(Position(x, y)).GetColor() == WhitePawn)
				{
					if(board.Get(Position(x, y)).HasBall())
					{
						fieldDraw.DrawEllipse(fieldWidth / 2 - ellipseSize / 2, fieldHeight / 2 - ellipseSize / 2, ellipseSize, ellipseSize, Color(255, 255, 192));
						fieldDraw.DrawEllipse(fieldWidth / 2 - ballSize / 2, fieldHeight / 2 - ballSize / 2, ballSize, ballSize, Black());
					}
					else
					{
						fieldDraw.DrawEllipse(fieldWidth / 2 - ellipseSize / 2, fieldHeight / 2 - ellipseSize / 2, ellipseSize, ellipseSize, Color(255, 255, 192));
					}
				}
				else
				{
					if(board.Get(Position(x, y)).HasBall())
					{
						fieldDraw.DrawEllipse(fieldWidth / 2 - ellipseSize / 2, fieldHeight / 2 - ellipseSize / 2, ellipseSize, ellipseSize, Color(255,   0,   0));
						fieldDraw.DrawEllipse(fieldWidth / 2 - ballSize / 2, fieldHeight / 2 - ballSize / 2, ballSize, ballSize, Black());
					}
					else
					{
						fieldDraw.DrawEllipse(fieldWidth / 2 - ellipseSize / 2, fieldHeight / 2 - ellipseSize / 2, ellipseSize, ellipseSize, Color(255,   0,   0));
					}
				}
			}
			boardDraw.DrawDrawing(x * fieldWidth, y * fieldHeight, fieldWidth, fieldHeight, fieldDraw.GetResult());
		}
	}
	
	boardArea = boardDraw.GetResult();
}

void MainWindow::WhenBoardClicked(Point p, dword keyflags)
{
	Position position(p.x / (boardArea.GetSize().cx / 7), p.y / (boardArea.GetSize().cy / 7));
	if(mode == GameMode && ((activePawnsColor == WhitePawn && isWhitePlayerHuman) || (activePawnsColor == BlackPawn && isBlackPlayerHuman)))
	{
		if(currentMove.ToSingleMovesVector().GetCount() < 3)
		{
			if(pawnWasMarked)
			{
				if(board.IsHighlighted(position))
				{
					if(board.Get(markedPawnPosition).HasBall() == true)
					{
						countBallThrows ++;
					}
					else
					{
						countPawnPulls ++;
					}
					std::cout << countBallThrows << " " << countPawnPulls << std::endl;
					
					SingleMove singleMove(markedPawnPosition, position);
					currentMove.Add(singleMove);
					ApplyMove(Move(singleMove));
				}
				pawnWasMarked = false;
				board.TurnOffHighlights();
				RefreshBoard();
			}
			else
			{
				if(!board.IsEmpty(position) && board.Get(position).GetColor() == activePawnsColor)
				{
					if((board.Get(position).HasBall() == true && countBallThrows == 0) ||
						(board.Get(position).HasBall() == false && countPawnPulls < 2))
					{
						pawnWasMarked = true;
						markedPawnPosition = position;
						Vector<SingleMove> singleMoves = GenerateMoves(board.Get(position));
						for(int i = 0; i < singleMoves.GetCount(); i++)
						{
							SingleMove singleMove = singleMoves[i];
							board.Highlight(singleMove.GetDestination());
						}
						RefreshBoard();
					}
				}
			}
		}
	}
	else if(mode == EditMode)
	{
		if(pawnWasMarked)
		{
			SingleMove singleMove(markedPawnPosition, position);
			ApplyMove(Move(singleMove));
			changes.Add(singleMove);
			pawnWasMarked = false;
			RefreshBoard();
		}
		else
		{
			if(!board.IsEmpty(position))
			{
				pawnWasMarked = true;
				markedPawnPosition = position;
			}
		}
	}
}

void MainWindow::EndTurn()
{
	turnsHistory.Add(currentMove);
	
	if(activePawnsColor == WhitePawn)
	{
		activePawnsColor = BlackPawn;
	}
	else
	{
		activePawnsColor = WhitePawn;
	}
	ResetMoveInput();
	
	RefreshBoard();
	
	if(activePawnsColor == WhitePawn)
	{
		status.Temporary("White player turn.");
	}
	else
	{
		status.Temporary("Black player turn.");
	}
	
	int oldNumberOfGame = this->numberOfGame;
	//Cout() << "Gra: " << oldNumberOfGame << " " << this->numberOfGame << "\n";
	
	ProcessEvents(); // !
	
	Cout() << "Gra: " << this->numberOfGame << " " << oldNumberOfGame << "\n";
	
	if(this->numberOfGame != oldNumberOfGame || mode != GameMode)
	{
		//Cout() << "Zakonczona gra nr. " << oldNumberOfGame;
		return;
	}
	
	if(StateIsFinal())
	{
		if(PawnsReachedEnemiesLine(WhitePawn) || PawnsMadeLine(BlackPawn))
		{
			PromptOK("White player won!");
		}
		else
		{
			PromptOK("Black player won!");
		}
		SwitchMode(IdleMode);
	}
	else if((activePawnsColor == WhitePawn && isWhitePlayerHuman == false) || (activePawnsColor == BlackPawn && isBlackPlayerHuman == false))
	{
		GenerateAndApplyMove(activePawnsColor);
	}
}

void MainWindow::ApplyMove(Move move)
{
	Vector<SingleMove> singleMoves = move.ToSingleMovesVector();
	for(int i = 0; i < singleMoves.GetCount(); i++)
	{
		SingleMove singleMove = singleMoves[i];
		board.Switch(singleMove.GetSource(), singleMove.GetDestination());
	}
}

void MainWindow::UndoMove(Move move)
{
	Vector<SingleMove> singleMoves = move.ToSingleMovesVector();
	for(int i = singleMoves.GetCount() - 1; i >= 0; i--)
	{
		SingleMove singleMove = singleMoves[i];
		board.Switch(singleMove.GetSource(), singleMove.GetDestination());
	}
}

void MainWindow::UndoChanges()
{
	Vector<SingleMove> singleMoves = currentMove.ToSingleMovesVector();
	for(int i = singleMoves.GetCount() - 1; i >= 0; i--)
	{
		SingleMove singleMove = singleMoves[i];
		board.Switch(singleMove.GetDestination(), singleMove.GetSource());
	}
	ResetMoveInput();
	RefreshBoard();
}

void MainWindow::EnterGameMode()
{
	mode = GameMode;
	ResetMoveInput();
	applyMove.Show();
	undoChanges.Show();
	getHint.Show();
	RefreshBoard();
	
	RefreshBoard();
	
	if(activePawnsColor == WhitePawn)
	{
		status.Temporary("White player turn.");
	}
	else
	{
		status.Temporary("Black player turn.");
	}
	
	/*int oldNumberOfGame = numberOfGame;
	
	ProcessEvents(); // !
	
	if(numberOfGame != oldNumberOfGame)
	{
		return;
	}*/
	
	if(StateIsFinal())
	{
		if(PawnsReachedEnemiesLine(WhitePawn) || PawnsMadeLine(BlackPawn))
		{
			PromptOK("White player won!");
		}
		else
		{
			PromptOK("Black player won!");
		}
		SwitchMode(IdleMode);
	}
	else if((activePawnsColor == WhitePawn && isWhitePlayerHuman == false) || (activePawnsColor == BlackPawn && isBlackPlayerHuman == false))
	{
		GenerateAndApplyMove(activePawnsColor);
	}
}

void MainWindow::ResetMoveInput()
{
	pawnWasMarked = false;
	countBallThrows = 0;
	countPawnPulls = 0;
	markedPawnPosition;
	currentMove = Move();
}

void MainWindow::SwitchMode(Mode m)
{
	if(mode == GameMode) LeaveGameMode();
	else if(mode == EditMode) LeaveEditMode();
	else if(mode == HistoryMode) LeaveHistoryMode();
	
	mode = m;
	
	if(mode == GameMode) EnterGameMode();
	else if(mode == EditMode) EnterEditMode();
	else if(mode == HistoryMode) EnterHistoryMode();
}

void MainWindow::LeaveGameMode()
{
	UndoChanges();
	board.TurnOffHighlights();
	applyMove.Hide();
	undoChanges.Hide();
	getHint.Hide();
	RefreshBoard();
}

void MainWindow::EnterHistoryMode()
{
	mode = HistoryMode;
	continueFromHere.Show();
	nextState.Show();
	previousState.Show();
	goBackToTheFuture.Show();
	positionInHistory = turnsHistory.GetCount() - 1;
	historyModeActivePawnsColor = activePawnsColor;
	
	status.Temporary(String().Cat() << "State " << (positionInHistory + 2) << " out of " << (turnsHistory.GetCount() + 1) << ".");
}

void MainWindow::SwitchHistoryModeActivePawnsColor()
{
	if(historyModeActivePawnsColor == WhitePawn) historyModeActivePawnsColor = BlackPawn;
	else historyModeActivePawnsColor = WhitePawn;
}

void MainWindow::NextState()
{
	if(positionInHistory != turnsHistory.GetCount() - 1)
	{
		positionInHistory++;
		SwitchHistoryModeActivePawnsColor();
		ApplyMove(turnsHistory[positionInHistory]);
		RefreshBoard();
		status.Temporary(String().Cat() << "State " << (positionInHistory + 2) << " out of " << (turnsHistory.GetCount() + 1) << ".");
	}
	else
	{
		PromptOK("Last state reached - cannot go any further.");
	}
}

void MainWindow::PreviousState()
{
	if(positionInHistory != -1)
	{
		UndoMove(turnsHistory[positionInHistory]);
		positionInHistory--;
		SwitchHistoryModeActivePawnsColor();
		RefreshBoard();
		status.Temporary(String().Cat() << "State " << (positionInHistory + 2) << " out of " << (turnsHistory.GetCount() + 1) << ".");
	}
	else
	{
		PromptOK("First state reached - cannot go any further.");
	}
}

void MainWindow::ContinueFromHere()
{
	if(PromptOKCancel("Moves in turns history will be dropped. Do you want to proceed?"))
	{
		while(positionInHistory != turnsHistory.GetCount() - 1)
			turnsHistory.Pop();
		activePawnsColor = historyModeActivePawnsColor;
		SwitchMode(GameMode);
	}
}

void MainWindow::GoBackToTheFuture()
{
	SwitchMode(GameMode);
}

void MainWindow::LeaveHistoryMode()
{
	while(positionInHistory != turnsHistory.GetCount() - 1)
		NextState();
	continueFromHere.Hide();
	nextState.Hide();
	previousState.Hide();
	goBackToTheFuture.Hide();
}

void MainWindow::EnterEditMode()
{
	mode = EditMode;
	changes = Move();
	applyChanges.Show();
	dropChanges.Show();
	switchCurrentUser.Show();
	pawnWasMarked = false;
}

void MainWindow::ApplyChanges()
{
	if(PromptOKCancel("Turns history will be dropped. Are you sure you want to proceed?"))
	{
		turnsHistory = Vector<Move>();
		changes = Move();
		SwitchMode(GameMode);
	}
}

void MainWindow::DropChanges()
{
	UndoMove(changes);
	changes = Move();
	RefreshBoard();
}

void MainWindow::LeaveEditMode()
{
	DropChanges();
	applyChanges.Hide();
	dropChanges.Hide();
	switchCurrentUser.Hide();
}

void MainWindow::EnterIdleMode()
{
	mode = IdleMode;
	status.Temporary("End of game.");
}

void MainWindow::LeaveIdleMode()
{
}

bool MainWindow::PawnsMadeLine(PawnColor pawnColor)
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

bool MainWindow::PawnsReachedEnemiesLine(PawnColor pawnColor)
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

bool MainWindow::StateIsFinal()
{
	return PawnsReachedEnemiesLine(WhitePawn) || PawnsReachedEnemiesLine(BlackPawn) ||
		PawnsMadeLine(WhitePawn) || PawnsMadeLine(BlackPawn); 
}

void MainWindow::GenerateAndApplyMove(PawnColor pawnColor)
{
	currentMove = GenerateMove(pawnColor);
	ApplyMove(currentMove);
	EndTurn();
	RefreshBoard();
}

Move MainWindow::GenerateMove(PawnColor pawnColor)
{
	Tuple2<Move, int> pair = AlphaBeta(0, -INF, INF, pawnColor);
	return pair.a;
}

Vector< Tuple2<Move, long long int> > MainWindow::GenerateMoves(PawnColor pawnColor)
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

int MainWindow::RateState(PawnColor pawnColor)
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

Tuple2<Move, int> MainWindow::AlphaBeta(const int & depth, int alpha, int beta, PawnColor pawnColor)
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
			
			//std::cout << "Przed " << moves.size() << " ";
			//std::sort(moves.begin(), moves.end(), MoveHashPairComparator);
			//Vector< Tuple2<Move, long long int> >::Iterator it = std::unique(moves.begin(), moves.end(), MoveHashPairEqual);
			//while(moves.End() != it)
			//	moves.Pop();
			std::random_shuffle(moves.begin(), moves.end());
			
			//std::cout << "Po " << moves.size() << std::endl;
			//Cout() << moves.GetCount() << '\n';
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

			//std::cout << "Przed " << moves.size() << " ";
			//std::sort(moves.begin(), moves.end(), MoveHashPairComparator);
			//Vector< Tuple2<Move, long long int> >::Iterator it = std::unique(moves.begin(), moves.end(), MoveHashPairEqual);
			//while(moves.End() != it)
			//	moves.Pop();
			std::random_shuffle(moves.begin(), moves.end());
			
			//std::cout << "Po " << moves.size() << std::endl;
			//Cout() << moves.GetCount() << '\n';
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

long long int MainWindow::GetHash() const
{
	int positionA = whitePlayerPawns[PAWNS_QUANTITY / 2].GetPosition().ToInt();
	int positionB = blackPlayerPawns[PAWNS_QUANTITY / 2].GetPosition().ToInt();
	return ((board.GetPartOfHash() | (((long long int) positionA) << 49)) | (((long long int) positionB) << 55)); // position max length equals 6 bits
}

void MainWindow::WhenApplyMove()
{
	if(mode == GameMode && ((activePawnsColor == WhitePawn && isWhitePlayerHuman) || (activePawnsColor == BlackPawn && isBlackPlayerHuman)))
	{
		if(currentMove.ToSingleMovesVector().GetCount() == 0)
		{
			PromptOK("Empty move is not allowed!");
		}
		else
		{
			EndTurn();
		}
	}
}

void MainWindow::WhenUndoChanges()
{
	if((activePawnsColor == WhitePawn && isWhitePlayerHuman) || (activePawnsColor == BlackPawn && isBlackPlayerHuman))
		UndoChanges();
}

void MainWindow::WhenGetHint()
{
	if((activePawnsColor == WhitePawn && isWhitePlayerHuman) || (activePawnsColor == BlackPawn && isBlackPlayerHuman))
	{
		UndoChanges();
		PromptOK("Computer told that move: " + GenerateMove(activePawnsColor).ToString() + " is OK.");
	}
}

void MainWindow::WhenSwitchCurrentUser()
{
	if(PromptOKCancel("Moves in turns history will be dropped. Do you want to proceed?"))
	{
		if(activePawnsColor == WhitePawn)
			activePawnsColor = BlackPawn;
		else
			activePawnsColor = WhitePawn;
		SwitchMode(GameMode);
	}
}

GUI_APP_MAIN
{
	MainWindow().Run();
}