#ifndef _PLAYERCLASS_H_
#define _PLAYERCLASS_H_

#include <QString>
#include <QImage>
#include <set>
#include <stdlib.h>

#include "constants.h"
#include "BoardLocationClass.h"
#include "SuggestionClass.h"

//Added for networking
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class PlayerClass
{
  private:
    BoardLocationClass currentLocation; //Current location on the board
    bool hostFlag;                      //True = host (or local play);
                                        //False = not host
    bool aiFlag;                        //True = AI; false = human player
    bool gameOverFlag;                  //True = hasn't lost the game
                                        //False = has lost the game
    bool movedThisTurnFlag;             //True = moved this turn; false = not
    ActionEnum lastAction;              //Last action taken by the player
    std::map<CardEnum, std::set<SuspectEnum> > hand;
                                        //Cards in the players hand; ordered in
                                        //order of CardEnum
    int dieRollThisTurn;                //Value of the die roll this turn
    int movesLeftThisTurn;              //Moves left to make this turn
    std::set<BoardLocationClass> locationsThisTurn;
                                        //List of locations player has visited
                                        //this turn
    std::pair<CardEnum, SuspectEnum> detectiveNotes[NUMBER_OF_CARDS];
    DifficultyEnum aiDifficulty;
    bool correctAiSuggestionFlag;
    SuggestionClass aiAccusation;

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
    
    // Prints out PlayerClass to transfer over network
    string printPlayerToString();
    
    // Takes string from over network and unwraps it into playerClass
    void stringToPlayer(string wrappedString);
    
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

    void setLastAction(ActionEnum action)
    {
      lastAction = action;
    }

    ActionEnum getLastAction() const
    {
      return lastAction;
    }

    void setMovedThisTurnFlag(bool value)
    {
      movedThisTurnFlag = value;
    }

    bool getMovedThisTurnFlag() const
    {
      return movedThisTurnFlag;
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

    //Sets the host flag.
    void setHostFlag(bool value)
    {
      hostFlag = value;
    }

    //Gets the host flag.
    bool getHostFlag() const
    {
      return hostFlag;
    }

    //Sets the the game over flag.
    void setGameOverFlag(bool value)
    {
      gameOverFlag = value;
    }

    //Gets the game over flag.
    bool getGameOverFlag()
    {
      return gameOverFlag;
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

    int getMovesLeft() const
    {
      return movesLeftThisTurn;
    }

    void decrementMovesLeft()
    {
      movesLeftThisTurn--;
    }

    void setCorrectSuggestion(const SuggestionClass &suggestion)
    {
      correctAiSuggestionFlag = true;
      aiAccusation = suggestion;
    }

    //Sets currentLocation to newLocation.
    void setPlayerLocation(BoardLocationClass newLocation)
    {
      currentLocation = newLocation;
    }

    std::map<CardEnum, std::set<SuspectEnum> > getHand() const
    {
      return hand;
    }

    void addSuspectShowed(CardEnum cardRevealed, SuspectEnum suspectShowed)
    {
      hand.find(cardRevealed)->second.insert(suspectShowed);
    }

    SuspectEnum getDetectiveNotes(CardEnum card)
    {
      return detectiveNotes[int(card)].second;
    }

    std::set<ActionEnum> getValidPrerollMoves();

    map<CardEnum, set<SuspectEnum> > getSuggestionMatches(SuggestionClass
        &suggestion);

    std::multimap<int, BoardLocationClass> getTargetDoors()const;

    std::set<BoardLocationClass> getValidExitDoors(const QImage &currentBoard);

    std::set<DirectionEnum> getValidMoveDirections(const QImage &currentBoard);

    ActionEnum makeCornerRoomDecision();

    BoardLocationClass getAiTargetDoor();

    BoardLocationClass getAiExitDoor(QImage &currentBoard, BoardLocationClass
        &target);

    DirectionEnum getAiMove(QImage &currentBoard, BoardLocationClass &target);

    //Handles suggestions passed to the AI.
    CardEnum handleSuggestionAi(SuggestionClass suggestion);

    ActionEnum handlePrerollAi(const QImage &currentBoard);

    ActionEnum handleAfterRollAi();

    SuggestionClass makeAiSuggestion() const;

    SuggestionClass makeAiAccusation() const;

};

#endif
