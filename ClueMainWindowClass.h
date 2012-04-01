#ifndef _CLUEMAINWINDOWCLASS_H_
#define _CLUEMAINWINDOWCLASS_H_

#include <QtGui>
#include <list>
#include <vector>
#include "ui_ClueMainWindowClass.h"
#include "constants.h"
#include "BoardLocationClass.h"
#include "CaseFileClass.h"
#include "DeckClass.h"
#include "ExceptionClass.h"
#include "PlayerClass.h"

class ClueMainWindowClass:public QWidget, private Ui::mainGameWindow
{
  Q_OBJECT
  private:
    QImage inProgressBoardImage;        //Board at current state of gameplay
    bool gameOver;                      //True = someone won;
                                        //False = no one won yet
    std::list<PlayerClass> gameParticipants;  //Players in order of turn
    std::list<PlayerClass>::iterator currentPlayerIter; //Current player
    DeckClass cardDeck;                 //Deck of cards
    CaseFileClass caseFile;             //Case file of details of the crime
    PlayerClass *thisPlayerPtr;         //Pointer to the PlayerClass object for
                                        //the human player at this computer

  public:
    //Constructors

    //Constructor; shows the GUI and sets up game for play.
    ClueMainWindowClass(QWidget *parent = 0);

    //Function Prototypes

    //Displays the cards in the player's hand
    void displayCardsInHand();

    //Sets up the board for play.
    void setupNewBoard();

    //Draws the character piece indicated to the tile indicated on
    //inProgressBoardImage.
    void drawPieceToBoard(
        const CardEnum &character,
        const BoardLocationClass &tile
        );

    //Erases the piece on the tile indicated on inProgressBoardImage.
    void eraseTileContents(
        const BoardLocationClass &tile
        );

    //Draws the movement from oldLocation to newLocation onto
    //inProgressBoardImage.
    void drawMove(
        const BoardLocationClass &oldLocation,   //Original location of the piece
        const BoardLocationClass &newLocation    //New location of the piece
        );

    //Selects cards from the deck to make the case file.
    void makeCaseFile();

    //Deals the cards to the players.
    void dealCards();

    //Sets the game up to play.
    void setupGame();

    //Displays the gameplay interface.
    void displayGameInterface();

    //Starts the current player's turn.
    void startPlayerTurn();

    //Gets a dice roll and displays it on the Gui.
    const int getAndDisplayDieRoll() const;

    //Displays gameplay options from a corner room.
    void displayCornerRoomOptions();

    //Displays default gameplay options.
    void displayDefaultOptions();

    //Draws the pieces in the game on their starting tiles.
    void drawStartingPieces();

    //Displays an exception message box with an error message.
    void displayExceptionMessageBox(
        ExceptionClass exceptionText    //Exception
        );

    CardEnum getCurrentPlayerRoom();

    //Displays the accusation window and either eliminates the player from the
    //game or ends the game with the player as the winner.
    void makePlayerAccusation();

    //Displays the suggestion window and moves the appropriate piece to the
    //appropriate tile.
    void makePlayerSuggestion();

    //Moves the current player out of the room through a door; throws an
    //exception if the door number selected is not a valid door number for the
    //room.
    void movePlayerOutDoor(
        int doorNumber        //Door number of the door
        );

    //Moves the player one tile over; throws an exception if the move is
    //invalid.
    void movePlayer(const DirectionEnum &direction);

    //Moves the player through the secret passage.
    void movePlayerToSecretPassage();

    //Throws an exception if the move to newLocation is invalid.
    void checkIfValidMove(BoardLocationClass &newLocation);

    //Gets the first empty designated piece storage area in the room and
    //returns the location.
    const BoardLocationClass getEmptyRoomTile(
        const BoardLocationClass &boardLocation
        );

    const BoardLocationClass getEmptyRoomTile(
        const CardEnum &room
        );

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

    //Toggles the option to leave the room.
    void toggleLeaveRoomOpt(int dummyVar)
    {
      leaveRoomOption->setChecked(true);
    }
};

#endif
