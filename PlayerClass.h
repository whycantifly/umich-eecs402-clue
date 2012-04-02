#ifndef _PLAYERCLASS_H_
#define _PLAYERCLASS_H_

#include <QString>
#include <QImage>
#include <list>
#include <vector>
#include <stdlib.h>

#include "constants.h"
#include "BoardLocationClass.h"
#include "SuggestionClass.h"

class PlayerClass
{
  private:
    QString playerName;                 //Player name
    SuspectEnum character;              //Assigned character piece
    BoardLocationClass currentLocation; //Current location on the board
    bool hostFlag;                      //True = host (or local play);
                                        //False = not host
    bool aiFlag;                        //True = AI; false = human player
    bool movedSinceLastTurnFlag;        //True = moved since last turn;
                                        //False = not moved since last turn
    bool enteredRoomThisMoveFlag;       //True = just entered a room;
                                        //False = did not just enter a room
    std::list<CardEnum> hand;           //Cards in the players hand; ordered in
                                        //order of CardEnum
    int dieRollThisTurn;                //Value of the die roll this turn
    int movesLeftThisTurn;              //Moves left to make this turn
    std::vector<BoardLocationClass> movesThisTurn;  //List of moves player has
                                                    //made this turn
    std::pair<CardEnum, SuspectEnum> detectiveNotes[NUMBER_OF_CARDS];

  public:
    //Constructors


    PlayerClass()
    {

    }
    //Constructor (QString, CardEnum, bool, bool)
    PlayerClass(
        QString name,           //Player name
        SuspectEnum suspect,       //Character piece
        bool aiValue,           //AI or human
        bool gameHostValue      //Game host or not
        );

    //Function Prototypes

    //Adds cardToAdd to the hand.
    void addCardToHand(
        CardEnum cardToAdd      //Card to add to the player's hand
        );

    //Sets the piece's starting location based on CHAR_STARTING_LOCATION.
    void setStartingLocation();

    //Inline Functions

    bool getMovedSinceLastTurnFlag() const
    {
      return movedSinceLastTurnFlag;
    }

    void setMovedSinceLastTurnFlag(bool newValue)
    {
      movedSinceLastTurnFlag = newValue;
    }



    bool getEnteredRoomThisMoveFlag() const
    {
      return enteredRoomThisMoveFlag;
    }

    void setEnteredRoomThisMoveFlag(bool newValue)
    {
      enteredRoomThisMoveFlag = newValue;
    }
    //Sets the AI flag.
    void setAiFlag(bool aiValue)
    {
      aiFlag = aiValue;
    }

    //Gets the AI flag.
    bool getAiFlag() const
    {
      return aiFlag;
    }

    //Gets the host flag.
    bool getHostFlag() const
    {
      return hostFlag;
    }

    QString getName() const
    {
      return playerName;
    }

    //Gets the current player location.
    BoardLocationClass getPlayerLocation() const
    {
      return currentLocation;
    }

    //Gets the player's character piece.
    const SuspectEnum getCharacter() const
    {
      return character;
    }

    //Sets dieRollThisTurn and movesLeftThisTurn to a die roll.
    void rollDie()
    {
      dieRollThisTurn = ((rand() % 6) + 1);
      movesLeftThisTurn = dieRollThisTurn;
    }

    void setDieRoll(int dieRoll)
    {
      dieRollThisTurn = dieRoll;
    }

    int getDieRoll()
    {
      return dieRollThisTurn;
    }

    void setMovesLeft(int movesLeft)
    {
      movesLeftThisTurn = movesLeft;
    }

    int getMovesLeft()
    {
      return movesLeftThisTurn;
    }

    void decrementMovesLeft()
    {
      movesLeftThisTurn--;
    }


    //Moves the player over one tile; throws an exception if the move is
    //outside the bounds of the board.
    void move(const DirectionEnum &direction)
    {
      currentLocation.move(direction);
    }

    //Sets currentLocation to newLocation.
    void setPlayerLocation(BoardLocationClass newLocation)
    {
      currentLocation = newLocation;
    }

    std::list<CardEnum> getHand()
    {
      return hand;
    }

//DUMMY AI CODE/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    //Moves the AI.
    void moveAi();

    //Handles suggestions passed to the AI.
    CardEnum handleSuggestionAi(SuggestionClass suggestion);

    AiActionEnum handlePrerollAi(SuggestionClass &aiSuggestion,
        DirectionEnum &direction, bool &secretPassageFlag);

    SuggestionClass makeSuggestionAi();
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

};

#endif
