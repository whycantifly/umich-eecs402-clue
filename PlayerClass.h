#ifndef _PLAYERCLASS_H_
#define _PLAYERCLASS_H_

#include <QString>
#include <QImage>
#include <list>
#include <vector>
#include <stdlib.h>

#include "constants.h"
#include "BoardLocationClass.h"

class PlayerClass
{
  private:
    QString playerName;                 //Player name
    CardEnum character;                 //Assigned character piece
    BoardLocationClass currentLocation; //Current location on the board
    bool gameHost;                      //True = host (or local play);
                                        //False = not host
    bool ai;                            //True = AI; false = human player
    bool activePlayer;                  //True = has not lost yet;
                                        //False = has lost (cannot move or win)
    std::list<CardEnum> hand;           //Cards in the players hand; ordered in
                                        //order of CardEnum
    int dieRollThisTurn;                //Value of the die roll this turn
    int movesLeftThisTurn;              //Moves left to make this turn
    std::vector<BoardLocationClass> movesThisTurn;  //List of moves player has
                                                    //made this turn

  public:
    //Constructors

    //Constructor (QString, CardEnum, bool, bool)
    PlayerClass(
        QString name,           //Player name
        CardEnum suspect,       //Character piece
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

    //Moves the AI.
    void moveAi();

    //Inline Functions

    //Gets the current player location.
    const BoardLocationClass getPlayerLocation() const
    {
      return currentLocation;
    }

    //Gets the player's character piece.
    const CardEnum getCharacter() const
    {
      return character;
    }

    //Gets whether the player is active or not.
    const bool getActivePlayer() const
    {
      return activePlayer;
    }

    //Returns a random int between 1 and 6.
    int rollDie() const
    {
      return ((rand() % 6) + 1);
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
};

#endif