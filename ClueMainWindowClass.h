//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    ClueMainWindowClass class definition

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

// Network stuff
#include "ServerSocket.h"
#include "PackageClass.h"
#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>

//For ClueMainWindowClass, all references to THIS player refer to the player
//controlling this particular instance of the game.

class ClueMainWindowClass:public QWidget, private Ui::mainGameWindow
{
  Q_OBJECT
  private:
    QImage inProgressBoardImage;  //Board image at the current state of gameplay
    std::map<SuspectEnum, PlayerClass> gameParticipants;
                                  //List of game participants: first = suspect,
                                  //second = player
    std::map<SuspectEnum, PlayerClass>::iterator currentPlayerIter;
                                  //Iterator pointing to the current player
    SuggestionClass caseFile;     //Case file include details of the crime
    SuspectEnum thisSuspect;      //Suspect controlled by THIS player
    int aiMoveDelay;              //Delay of Ai moves in milliseconds
    
    //ServerSocket server;
    //ServerSocket serverSock;
    //ClientSocket cliSock();
    
    //ServerSocket server(int);
    //ServerSocket serverSock;
    //ClientSocket cliSock(string, int);

  public:
    //CONSTRUCTORS

    //Constructor; shows the GUI and sets up game for play.
    ClueMainWindowClass();

    //GUI DISPLAY FUNCTIONS

    //Sets up the board for play.
    void setupNewBoard();

    //Displays the cards in THIS player's hand on the game interface.
    void displayCardsInHand();

    //Draws the movement of the suspect from the old location to the new
    //location.
    void drawMove(
        SuspectEnum suspect,                    //Suspect to move
        const BoardLocationClass &oldLocation,  //Original location of the piece
        const BoardLocationClass &newLocation   //New location of the piece
        );

    //Draws the pieces in the game on their starting tiles.
    void drawStartingPieces();

    //Draws the character piece indicated to the tile indicated on
    //inProgressBoardImage.
    void drawPieceToBoard(
        SuspectEnum suspect,            //Suspect piece to draw
        const BoardLocationClass &tile  //Location to draw the piece to
        );

    //Erases the piece belonging to the suspect from the tile indicated.
    void clearPiece(
        SuspectEnum suspect,            //Suspect to be cleared
        const BoardLocationClass &tile  //Tile to clear the suspect from
        );

    //Clears the travel trail of the current player.
    void clearVisitedTiles();

    //Displays the gameplay interface.
    void displayGameInterface();

    //Refreshes the display controls for THIS player.
    void refreshDisplay();

    //Updates the roll information text on the display.
    void updateRollInfoText();

    //Updates the detective notes shown on the display for the specified card.
    void updateDetectiveNotes(
        CardEnum card                   //Card to update the display for
        );

    //Makes the default gameplay options visible.
    void displayDefaultOptions();

    //Makes the room gameplay options visible.
    void displayRoomOptions();

    //Makes the corner room gameplay options visible.
    void displayCornerRoomOptions();

    //Disables all controls.
    void disableAllControls();

    //Enables all controls
    void enableAllControls();

    //Disables all movement controls.
    void disableMovementControls();

    //Enables all movement controls.
    void enableMovementControls();

    //Displays an exception message box with an error message.
    void displayExceptionMessageBox(
        ExceptionClass exceptionText    //Exception text
        ) const;

    //Displays the accusation window when THIS player makes an accusation.
    void makePlayerAccusation();

    //Displays the suggestion window when THIS player makes a suggestion.
    void makePlayerSuggestion();

    //GAME MECHANIC FUNCTIONS

    //Deals the cards to the players.
    void dealCards(
        DeckClass &cardDeck             //Card deck to deal the cards from
        );

    //Sets the game up for the host.
    void setupGame();

    //Starts the current player's turn up before the current player rolls.
    void startPlayerTurn();

    //Continues the current player's turn after the roll.
    void continuePlayerTurn();

    //Handles any suggestion made.
    void handleSuggestion(
        const SuggestionClass &playerSuggestion   //Suggestion posed
        );

    //Handles any accusation made.
    void handleAccusation(
        const SuggestionClass playerAccusation   //Accusation posed
        );

    //Moves the current player one tile over; throws an exception if the move is
    //invalid.
    void moveCurrentPlayer(
        const DirectionEnum &direction  //Direction to move in
        );

    //Moves the player through the secret passage.
    void moveCurrentPlayerToSecretPassage();

    //Moves the suspect included in a suggestion after the suggestion is made.
    void moveSuggestedSuspect(
        SuspectEnum suggestedSuspect    //Suggested suspect (to be moved)
        );

    //Moves the current player out of the room through a door; throws an
    //exception if the door number selected is not a valid door number for the
    //room.  Overloaded to accept either the door location or the door number.
    void moveCurrentPlayerOutDoor(BoardLocationClass doorLoc);

    void moveCurrentPlayerOutDoor(
        int doorNumber        //Door number of the door
        );

    //Finishes the move.
    void finishMove();

    //Ends the turn and changes the current player to the next player.
    void endTurn();

    //AI GAMEPLAY FUNCTIONS

    //Takes the specified Ai action.
    void takeAiAction(
        const ActionEnum action         //Action to take
        );

    //NETWORK FUNCTIONS

    //Sends turn information to remote players.
    void sendRemoteMoveInfo(
        ActionEnum playerAction,                //Remote player action
        int dieRoll = DEFAULT_DIE_ROLL,         //Remote player die roll
        BoardLocationClass locationAfterMove = EMPTY_LOCATION,
                                                //Location after this move
        SuggestionClass suggestionMade = EMPTY_SUGGESTION
                                                //Suggestion made by the
                                                //player
        );
        
    void sendInfoFromHost();
    
    void receiveInfoFromHost();

    void sendInfoFromClient();
    
    void receiveInfoFromClient();

    //Receives turn information from any remote players.
    void receiveRemoteTurnInfo();

    //Sends card information for the card revealed.
    void sendRevealedCard(CardEnum revealedCard);

    //Receives card information for the card revealed to the player when a
    //suggestion is made.
    CardEnum receiveRevealedCard();

    //Checks to make sure all players have acknowledged the suggestion or
    //response to the suggestion before continuing.
    void checkSuggestionAcknowledged();

    //GUI SUPPORT FUNCTIONS

    //Gets the suspect assigned to the current player.
    SuspectEnum getCurrentPlayerSuspect() const
    {
      return currentPlayerIter->first;
    }

    //Gets the current player's room.
    RoomEnum getCurrentPlayerRoom() const
    {
      return currentPlayerIter->second.getPlayerLocation().getRoom();
    }

    //Gets THIS player's hand.
    std::map<CardEnum, std::set<SuspectEnum> > getThisPlayerHand() const
    {
      return gameParticipants.find(thisSuspect)->second.getHand();
    }

  public slots:

    //Starts the game.
    void startGame();

    //Submits the current move.
    void submitMove();

    //Makes things visible/invisible or enabled/disabled depending on the
    //option buttons and check boxes selected during game setup for networked
    //play.
    void setNetworkOptVis();

    //Makes things visible/invisible or enabled/disabled depending on the
    //option buttons and check boxes selected during game setup for local
    //play.
    void setLocalOptVis();

    //Toggles the option to leave the room.
    void toggleLeaveRoomOpt()
    {
      leaveRoomOption->setChecked(true);
    }

    //Resets the number of human players in the spinbox to 1.
    void resetHumanPlayersSpin()
    {
      humanPlayersSpin->setValue(humanPlayersSpin->minimum());
    }

    //Opens the Clue rules page
    void openClueRules()
    {
      QDesktopServices::openUrl(QUrl("files/clueins.pdf"));
    }
    
    void updateNumberOfAis(int numberOfAis);

    void setClientSocket(ClientSocket &cliSock, int ipAddress, int portNumber);
};

#endif
