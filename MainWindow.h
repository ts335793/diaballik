#ifndef _Diaballik_Diaballik_h
#define _Diaballik_Diaballik_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include "Board.h"
#include "Pawn.h"
#include "Mode.h"
#include "SingleMove.h"
#include "BoardArea.h"
#include "Move.h"
#include "PawnColor.h"

#include "GameState.h"

#define IMAGEFILE <Diaballik/Fields.iml>
#include <Draw/iml_header.h>

#define LAYOUTFILE <Diaballik/MainWindow.lay>
#include <CtrlCore/lay.h>

class MainWindow : public WithMainWindowLayout<TopWindow>, public GameState {
private:
	int numberOfGame;
	Mode mode;
	
	// modes
	void SwitchMode(Mode m);
	
	// game mode - click
	bool pawnWasMarked;
	int countBallThrows;
	int countPawnPulls;
	Position markedPawnPosition;
	Move currentMove;
	void ResetMoveInput();
	void EnterGameMode();
	void EndTurn();
	void UndoChanges();
	void LeaveGameMode();
	void WhenApplyMove();
	void WhenUndoChanges();
	void WhenGetHint();
	
	// history mode
	int positionInHistory;
	PawnColor historyModeActivePawnsColor;
	void EnterHistoryMode();
	void SwitchHistoryModeActivePawnsColor();
	void NextState();
	void PreviousState();
	void ContinueFromHere();
	void GoBackToTheFuture();
	void LeaveHistoryMode();
	
	// edit mode
	Move changes;
	void EnterEditMode();
	void ApplyChanges();
	void DropChanges();
	void LeaveEditMode();
	void WhenSwitchCurrentUser();
	void SetAboutGameText();
	void SwitchPlayerType(PawnColor color);
	void UpdateWhoIsCurrentPlayer();
	
	// menu bar
	MenuBar menu;
	FileSel	fs;
	void MainMenu(Bar & bar);
	void SwitchModeMenu(Bar & bar);
	void NewGameMenu(Bar& bar);
	void FileMenu(Bar& bar);
	void Load();
	void Save();
	
	// status bar
	StatusBar status;
	
	// idle mode
	void EnterIdleMode();
	void LeaveIdleMode();
	
	// AI
	void GenerateAndApplyMove(PawnColor pawnColor);
	
	void NotImplemented();
	
public:
	typedef MainWindow CLASSNAME;
	
	MainWindow();
	
	void NewGame(bool whitePlayerIsHuman, bool blackPlayerIsHuman);
	void RefreshBoard();
	void Serialize(Stream & stream);
	void WhenBoardClicked(Point p, dword keyflags);
};

#endif
