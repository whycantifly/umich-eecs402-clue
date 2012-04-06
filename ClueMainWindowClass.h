#ifndef _CLUEMAINWINDOWCLASS_H_
#define _CLUEMAINWINDOWCLASS_H_

#include <QtGui>
#include <map>
#include <set>
#include "ui_ClueMainWindowClass.h"
#include "constants.h"
#include "BoardLocationClass.h"
#include "DeckClass.h"
#include "ExceptionClass.h"
#include "PlayerClass.h"
#include "SuggestionClass.h"

class ClueMainWindowClass:public QWidget, private Ui::mainGameWindow
{
  Q_OBJECT
  private:
    QImage inProgressBoardImage;        //Board at current state of gameplay
    bool gameOver;                      //True = someone won;
                                        //False = no one won yet
    std::map<SuspectEnum, PlayerClass> gameParticipants;
                                        //Players in order of turn
    std::map<SuspectEnum, PlayerClass>::iterator currentPlayerIter;
                                        //Current player
    SuggestionClass caseFile;           //Case file of details of the crime
    SuspectEnum thisSuspect;            //Suspect that THIS player controls

  public:
    //Constructors

    //Constructor; shows the GUI and sets up game for play.
    ClueMainWindowClass();

    //Function Prototypes

    //Displays the cards in the player's hand
    void displayCardsInHand();

    //Sets up the board for play.
    void setupNewBoard();

    //Draws the character piece indicated to the tile indicated on
    //inProgressBoardImage.
    void drawPieceToBoard(
        SuspectEnum suspect,
        const BoardLocationClass &tile
        );

    void clearVisitedTiles();

    //Erases the piece on the tile indicated on inProgressBoardImage.
    void clearPiece(
        SuspectEnum suspect,
        const BoardLocationClass &tile
        );

    //Draws the movement from oldLocation to newLocation onto
    //inProgressBoardImage.
    void drawMove(
        SuspectEnum suspect,
        const BoardLocationClass &oldLocation,   //Original location of the piece
        const BoardLocationClass &newLocation    //New location of the piece
        );

    //Deals the cards to the players.
    void dealCards(DeckClass &cardDeck);

    //Sets the game up to play.
    void setupGame();

    //Displays the gameplay interface.
    void displayGameInterface();

    //Refreshes the display with available options
    void refreshDisplay();

    void updateDetectiveNotes(CardEnum card);

    //Starts the current player's turn.
    void startPlayerTurn();

    void continuePlayerTurn();

    void takeAiAction(
        const AiActionEnum action,
        SuggestionClass aiSuggestion = EMPTY_SUGGESTION,
        std::queue<DirectionEnum> aiMoves = EMPTY_MOVE_LIST,
        const int aiExitDoorNumber = 0
        );

    void updateRollInfoText();

    //Displays gameplay options from a corner room.
    void displayCornerRoomOptions();

    void displayRoomOptions();

    //Displays default gameplay options.
    void displayDefaultOptions();

    void disableAllControls();

    void enableAllControls();

    //Draws the pieces in the game on their starting tiles.
    void drawStartingPieces();

    //Displays an exception message box with an error message.
    void displayExceptionMessageBox(
        ExceptionClass exceptionText    //Exception
        ) const;

    RoomEnum getCurrentPlayerRoom();

    //Displays the accusation window and either eliminates the player from the
    //game or ends the game with the player as the winner.
    void makePlayerAccusation();

    //Displays the suggestion window and moves the appropriate piece to the
    //appropriate tile.
    void makePlayerSuggestion();

    void moveSuggestedSuspect(SuspectEnum suggestedSuspect);

    //Moves the current player out of the room through a door; throws an
    //exception if the door number selected is not a valid door number for the
    //room.
    void moveCurrentPlayerOutDoor(
        int doorNumber        //Door number of the door
        );

    void finishMove();

    //Moves the player one tile over; throws an exception if the move is
    //invalid.
    void moveCurrentPlayer(const DirectionEnum &direction);

    //Moves the player through the secret passage.
    void moveCurrentPlayerToSecretPassage();

    void displaySuggestionResults(CardEnum card);

    void handleSuggestion(
        const SuggestionClass &playerSuggestion
        );

    //Inline functions

    SuspectEnum getCurrentPlayerSuspect() const
    {
      return currentPlayerIter->first;
    }

    std::set<CardEnum> getThisPlayerHand() const
    {
      return gameParticipants.find(thisSuspect)->second.getHand();
    }

  public slots:

    //Starts the game.
    void startGame();

    //Submits the current move.
    void submitMove();

    //Resets the number of human players in the spinbox to 1.
    void resetHumanPlayersSpin()
    {
      humanPlayersSpin->setValue(humanPlayersSpin->minimum());
    }

    //Makes things visible/invisible or enabled/disabled depending on the
    //option buttons and check boxes selected during game setup for networked
    //play.
    void setNetworkOptVis();

    //Makes things visible/invisible or enabled/disabled depending on the
    //option buttons and check boxes selected during game setup for local
    //play.
    void setLocalOptVis();

    void updateDifficultyText(int sliderPosition);

    //Toggles the option to leave the room.
    void toggleLeaveRoomOpt()
    {
      leaveRoomOption->setChecked(true);
    }
};

#endif
