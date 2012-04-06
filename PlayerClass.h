#ifndef _PLAYERCLASS_H_
#define _PLAYERCLASS_H_

#include <QString>
#include <QImage>
#include <set>
#include <stdlib.h>

#include "constants.h"
#include "BoardLocationClass.h"
#include "SuggestionClass.h"


class PlayerClass
{
  private:
    BoardLocationClass currentLocation; //Current location on the board
    bool hostFlag;                      //True = host (or local play);
                                        //False = not host
    bool aiFlag;                        //True = AI; false = human player
    bool movedSinceLastTurnFlag;        //True = moved since last turn;
                                        //False = not moved since last turn
    bool enteredRoomThisMoveFlag;       //True = just entered a room;
                                        //False = did not just enter a room
    std::set<CardEnum> hand;            //Cards in the players hand; ordered in
                                        //order of CardEnum
    int dieRollThisTurn;                //Value of the die roll this turn
    int movesLeftThisTurn;              //Moves left to make this turn
    std::set<BoardLocationClass> locationsThisTurn;
                                        //List of locations player has visited
                                        //this turn
    std::pair<CardEnum, SuspectEnum> detectiveNotes[NUMBER_OF_CARDS];
    DifficultyEnum aiDifficulty;
    bool endTurnFlag;

  public:
    //Constructors


    PlayerClass()
    {

    }
    //Constructor (bool, bool)
    PlayerClass(
        const bool aiValue,                         //AI or human
        const bool gameHostValue,                   //Game host or not
        const BoardLocationClass &startingLocation, //Starting location
        const DifficultyEnum aiDiff = EASY          //AI difficulty
        );

    //Function Prototypes

    //Moves the player over one tile; throws an exception if the move is
    //outside the bounds of the board.
    void move(const QImage &currentBoard, const DirectionEnum &direction);

    //Adds cardToAdd to the hand.
    void addCardToHand(
        CardEnum cardToAdd      //Card to add to the player's hand
        );

    //Sets the piece's starting location based on CHAR_STARTING_LOCATION.
    void setStartingLocation();

    void addToDetectiveNotes(CardEnum card, SuspectEnum suspect);

    //Inline Functions

    void setEndTurnFlag(bool value)
    {
      endTurnFlag = value;
    }

    bool getEndTurnFlag() const
    {
      return endTurnFlag;
    }

    void addToLocationsThisTurn(BoardLocationClass location)
    {
      locationsThisTurn.insert(location);
    }

    std::set<BoardLocationClass> getLocationsThisTurn() const
    {
      return locationsThisTurn;
    }

    void resetLocationsThisTurn()
    {
      locationsThisTurn.clear();
      locationsThisTurn.insert(currentLocation);
    }

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

    //Gets the current player location.
    BoardLocationClass getPlayerLocation() const
    {
      return currentLocation;
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

    //Sets currentLocation to newLocation.
    void setPlayerLocation(BoardLocationClass newLocation)
    {
      currentLocation = newLocation;
    }

    std::set<CardEnum> getHand() const
    {
      return hand;
    }

    SuspectEnum getDetectiveNotes(CardEnum card)
    {
      return detectiveNotes[int(card)].second;
    }

    std::set<BoardLocationClass> getValidExitDoors(const QImage &currentBoard);

    std::set<DirectionEnum> getValidMoveDirections(const QImage &currentBoard);

//DUMMY AI CODE/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    AiActionEnum makeCornerRoomDecision();

    BoardLocationClass getAiTargetDoor();

    BoardLocationClass getAiExitDoor(QImage &currentBoard);

    DirectionEnum getAiMove(QImage &currentBoard, BoardLocationClass &target);

    //Handles suggestions passed to the AI.
    CardEnum handleSuggestionAi(SuggestionClass suggestion);

    AiActionEnum handlePrerollAi(const QImage &currentBoard);

    AiActionEnum handleAfterRollAi();

    SuggestionClass makeAiSuggestion() const;

    SuggestionClass makeAiAccusation() const;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

};

#endif
