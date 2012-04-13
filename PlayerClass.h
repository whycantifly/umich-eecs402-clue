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
    bool aiFlag;                        //True = Ai; false = human player
    bool gameOverFlag;                  //True = hasn't lost the game
                                        //False = has lost the game
    bool movedThisTurnFlag;             //True = moved this turn; false = not
    ActionEnum lastAction;              //Last action taken by the player
    std::map<CardEnum, std::set<SuspectEnum> > hand;
                                        //Cards in the players hand and list
                                        //of suspects each card has been shown
                                        //to
    int dieRollThisTurn;                //Value of the die roll this turn
    int movesLeftThisTurn;              //Moves left to make this turn
    std::set<BoardLocationClass> locationsThisTurn;
                                        //List of locations player has visited
                                        //this turn
    std::pair<CardEnum, SuspectEnum> detectiveNotes[NUMBER_OF_CARDS];
                                        //Detective notes; keeps track of which
                                        //cards belong to which suspects
    DifficultyEnum aiDifficulty;        //Ai difficulty level
    bool correctAiSuggestionFlag;       //True = Ai has made a correct 
                                        //suggestion
                                        //False = Ai has not yet made a correct
                                        //suggestion
    SuggestionClass aiAccusation;       //The correct suggestion if the AI has
                                        //made a correct suggestion

  public:
    //Constructors

    //Constructor(); does not initialize any member variables.
    PlayerClass()
    {

    }
    //Constructor(bool, bool, BoardLocationClass, DifficultyEnum)
    PlayerClass(
        const bool aiValue,                         //True = Ai; False = human
        const bool gameHostValue,                   //True = game host; 
                                                    //False  = otherwise
        const BoardLocationClass &startingLocation, //Starting location
        const DifficultyEnum aiDiff = VERY_EASY     //Ai difficulty
        );

    //Function Prototypes
    
    // Prints out PlayerClass to transfer over network.
    string printPlayerToString();
    
    // Takes string from over network and unwraps it into playerClass.
    void stringToPlayer(string wrappedString);
    
    //Moves the player over one tile in the indicated direction.  Throws an 
    //exception if the move is an invalid move.
    void move(const QImage &currentBoard, const DirectionEnum &direction);

    //Adds the indicated card to the player's hand.
    void addCardToHand(
        CardEnum cardToAdd      //Card to add to the player's hand
        );

    //Sets the piece's starting location based on CHAR_STARTING_LOCATION.
    void setStartingLocation();

    void addToDetectiveNotes(CardEnum card, SuspectEnum suspect);
    
    //Gets the list of valid preroll moves for the Ai player.
    std::set<ActionEnum> getValidPrerollMoves();

    //Gets the cards in the players hand that match the indicated suggestion.
    map<CardEnum, set<SuspectEnum> > getSuggestionMatches(SuggestionClass
        &suggestion) const;

    //Gets a list of target doors ordered by distance from the current player
    //location to the door.
    std::multimap<int, BoardLocationClass> getTargetDoors() const;

    //Gets a list of the valid exit doors based on the player's location on the
    //indicted game board.
    std::set<BoardLocationClass> getValidExitDoors(const QImage &currentBoard)
        const;

    //Gets a list of valid move directions based on the player's location on the
    //indicated game board.
    std::set<DirectionEnum> getValidMoveDirections(const QImage &currentBoard)
        const;

    //Makes the decision of which decision to if the player is in a corner room.
    ActionEnum makeCornerRoomDecision() const;

    //Gets the Ai target door.
    BoardLocationClass getAiTargetDoor() const;

    //Gets the Ai exit door based on its location on the board and the target location.
    BoardLocationClass getAiExitDoor(QImage &currentBoard, BoardLocationClass
        &target) const;

    //Gets the next Ai move direction based on its location on the board and the target
    //location
    DirectionEnum getAiMove(QImage &currentBoard, BoardLocationClass &target)
        const;

    //Handles suggestions passed to the Ai.
    CardEnum handleSuggestionAi(SuggestionClass suggestion, SuspectEnum
        currentSuspect) const;

    //Handles all preroll decisions for the Ai.
    ActionEnum handlePrerollAi();

    //Handles all decisions after the roll for the Ai.
    ActionEnum handleAfterRollAi();

    //Makes suggestions for the Ai.
    SuggestionClass makeAiSuggestion() const;

    //Makes an accusation for the Ai.  Throws an exception if the Ai cannot make
    //an accusation this turn.
    void makeAiAccusation();

    //Sets the accusation for Ai players.
    void setAiAccusation(const SuggestionClass &suggestion);

    //Inline Functions

    //Sets the Ai difficulty.
    void setAiDifficulty(const DifficultyEnum newDifficulty)
    {
      aiDifficulty = newDifficulty;
    }

    //Adds the indicated BoardLocationClass object to the list of locations
    //the player has visited this turn.
    void addToLocationsThisTurn(BoardLocationClass location)
    {
      locationsThisTurn.insert(location);
    }

    //Gets the locations visited this turn.
    std::set<BoardLocationClass> getLocationsThisTurn() const
    {
      return locationsThisTurn;
    }

    //Resets the locations visited this turn by clearing out the current
    //locations listed and adding the player's current location.
    void resetLocationsThisTurn()
    {
      locationsThisTurn.clear();
      locationsThisTurn.insert(currentLocation);
    }

    //Sets the player's last action to the indicated action.
    void setLastAction(ActionEnum action)
    {
      lastAction = action;
    }

    //Gets the player's last action.
    ActionEnum getLastAction() const
    {
      return lastAction;
    }

    //Sets the moved this turn flag.
    void setMovedThisTurnFlag(bool value)
    {
      movedThisTurnFlag = value;
    }

    //Gets the moved this turn flag.
    bool getMovedThisTurnFlag() const
    {
      return movedThisTurnFlag;
    }
    
    //Sets the Ai flag.
    void setAiFlag(const bool aiValue)
    {
      aiFlag = aiValue;
    }

    //Gets the Ai flag.
    bool getAiFlag() const
    {
      return aiFlag;
    }

    //Sets the host flag.
    void setHostFlag(const bool value)
    {
      hostFlag = value;
    }

    //Gets the host flag.
    bool getHostFlag() const
    {
      return hostFlag;
    }

    //Sets the the game over flag.
    void setGameOverFlag(const bool value)
    {
      gameOverFlag = value;
    }

    //Gets the game over flag.
    bool getGameOverFlag() const
    {
      return gameOverFlag;
    }

    //Gets the current player location.
    BoardLocationClass getPlayerLocation() const
    {
      return currentLocation;
    }

    //Rolls the die and sets the die roll and moves left this turn to the die roll.
    void rollDie()
    {
      dieRollThisTurn = ((rand() % 6) + 1);
      movesLeftThisTurn = dieRollThisTurn;
    }

    //Sets the die roll to the indicated value.
    void setDieRoll(const int dieRoll)
    {
      dieRollThisTurn = dieRoll;
    }

    //Gets the die roll.
    int getDieRoll() const
    {
      return dieRollThisTurn;
    }

    //Sets the moves left.
    void setMovesLeft(const int movesLeft)
    {
      movesLeftThisTurn = movesLeft;
    }

    //Gest the moves left.
    int getMovesLeft() const
    {
      return movesLeftThisTurn;
    }

    //Decrements the moves left by 1.
    void decrementMovesLeft()
    {
      movesLeftThisTurn--;
    }

    //Sets currentLocation to newLocation.
    void setPlayerLocation(const BoardLocationClass newLocation)
    {
      currentLocation = newLocation;
    }

    //Returns the player's hand.
    std::map<CardEnum, std::set<SuspectEnum> > getHand() const
    {
      return hand;
    }

    //Adds the suspect indicated to the list of suspects shown for the 
    //indicated card.
    void addSuspectShowed(const CardEnum cardRevealed, const SuspectEnum 
        suspectShowed)
    {
      hand.find(cardRevealed)->second.insert(suspectShowed);
    }

    //Gets the detective notes for the indicated card.
    SuspectEnum getDetectiveNotes(const CardEnum card) const
    {
      return detectiveNotes[int(card)].second;
    }

    //Gets the Ai accusation.
    SuggestionClass getAiAccusation() const
    {
      return aiAccusation;
    }
};

#endif
