//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    ClueMainWindowClass member function definitions

#include <QtGui>
#include <QTest>
#include <map>
#include <set>
#include <stdlib.h>

#include "ClueMainWindowClass.h"
#include "constants.h"
#include "AccusationDialogClass.h"
#include "BoardLocationClass.h"
#include "HandleSuggestionDialogClass.h"
#include "ExceptionClass.h"
#include "SuggestionDialogClass.h"
#include "SuggestionClass.h"
#include "getCard.h"
#include "getCardType.h"
#include "getStartingDoorIndex.h"

// These are used for building two binaries to keep sockets in scope
// If successful, will figure out a way to hardwire into main.cpp or something
int socketPort = 30000;
//ServerSocket server(socketPort);
//ServerSocket serverSock;
//ClientSocket cliSock("localhost", socketPort);

//Function used by all human players
ClueMainWindowClass::ClueMainWindowClass() : QWidget()
{
  //Set up the GUI
  setupUi(this);

  //Signals and slots
  connect(networkedPlayOption, SIGNAL(clicked()), this,
      SLOT(setNetworkOptVis()));
  connect(localPlayOption, SIGNAL(clicked()), this, SLOT(setLocalOptVis()));
  connect(gameHostCheck, SIGNAL(clicked()), this, SLOT(setNetworkOptVis()));
  connect(doorNumberSpin, SIGNAL(valueChanged(int)), this,
      SLOT(toggleLeaveRoomOpt()));
  connect(okayButton, SIGNAL(clicked()), this, SLOT(submitMove()));
  connect(startGameButton, SIGNAL(clicked()), this, SLOT(startGame()));
  connect(helpButton, SIGNAL(clicked()), this, SLOT(openClueRules()));
  connect(computerPlayersSpin, SIGNAL(valueChanged(int)), this, SLOT(
      updateNumberOfAis(int)));

  //Setup the board
  setupNewBoard();
}

//Function used by all human players
void ClueMainWindowClass::setupNewBoard()
{
  //Variable Declarations
  QString aiNumber;
  QString cardNumber;

  //Reset and show game board picture
  inProgressBoardImage = CLUE_BOARD_IMAGE;
  gameBoard->setPixmap(QPixmap::fromImage(inProgressBoardImage));

  //Clear out any personal notes
  personalNotes->clear();

  //Make gameplay interface invisible
  currentTurnText->setVisible(false);
  rollInfoText->setVisible(false);
  selectActionLabel->setVisible(false);
  actionOptionsFrame->setVisible(false);
  okayButton->setVisible(false);
  detectiveNotesLabel->setVisible(false);
  detectiveNotesFrame->setVisible(false);
  personalNotesLabel->setVisible(false);
  personalNotes->setVisible(false);
  youAreText->setVisible(false);

  //Make game setup interface visible
  gameWelcomeLabel->setVisible(true);
  gameSetupFrame->setVisible(true);
  startGameButton->setVisible(true);

  networkedPlayOption->toggle();
  networkedPlayOption->setFocus();

  gameHostCheck->setChecked(true);
  hostIPEdit->setVisible(false);
  hostIPLabel->setVisible(false);

  for(int i = 0; i < NUMBER_OF_SUSPECTS; i++)
  {
    findChildren<QLabel*>(CARD_VALUES[i].split(" ").last().toLower() +
        "NameLabel").first()->setVisible(false);
  }

  for(int i = 0; i < MAX_CARDS_IN_HAND; i++)
  {
    cardNumber.setNum(i + 1);
    findChildren<QLabel*>("cardInHand" + cardNumber).first()->setVisible(false);
  }

  for(int i = 0; i < MAX_AIS; i++)
  {
    aiNumber.setNum(i + 1);
    findChildren<QComboBox*>("ai" + aiNumber + "Difficulty").first()->
        setCurrentIndex(0);
  }
  computerPlayersSpin->setValue(0);
  updateNumberOfAis(0);
}

//Function used by all human players; only applies to member variables for THIS
//player
void ClueMainWindowClass::displayCardsInHand()
{
  //Variable Declarations
  map<CardEnum, set<SuspectEnum> > hand = gameParticipants.find(thisSuspect)->
      second.getHand();     //THIS player's hand
  map<CardEnum, set<SuspectEnum> >::iterator currentCardIter = hand.begin();
  QString cardNumber;

  for(int i = 0; i < hand.size(); i++, currentCardIter++)
  {
    cardNumber.setNum(i + 1);
    findChildren<QLabel*>("cardInHand" + cardNumber).first()->setPixmap
        (QPixmap::fromImage(CARD_IMAGES[currentCardIter->first]));
    updateDetectiveNotes(currentCardIter->first);
  }
}

//Function used for all players
void ClueMainWindowClass::drawMove(SuspectEnum suspect, const BoardLocationClass
    &oldLocation, const BoardLocationClass &newLocation)
{
  //Clear the piece from its old location
  clearPiece(suspect, oldLocation);
  //Draw the piece to the new location
  drawPieceToBoard(suspect, newLocation);
  //Add the new location to the player's list of visited locations
  gameParticipants.find(suspect)->second.addToLocationsThisTurn(newLocation);
  gameParticipants.find(suspect)->second.setLastAction(MOVE);
  gameParticipants.find(suspect)->second.setMovedThisTurnFlag(true);
  //Update the board image on the gui
  gameBoard->setPixmap(QPixmap::fromImage(inProgressBoardImage));
}

//Function used for all players
void ClueMainWindowClass::drawStartingPieces()
{
  //Variable Declarations
  map<SuspectEnum, PlayerClass>::iterator playerIterator;
                        //Iterator to go through gameParticipants

  for(playerIterator = gameParticipants.begin(); playerIterator !=
      gameParticipants.end(); playerIterator++)
  {
    drawPieceToBoard(playerIterator->first,
        STARTING_LOCATIONS[playerIterator->first]);
    gameBoard->setPixmap(QPixmap::fromImage(inProgressBoardImage));
    findChildren<QLabel*>(CARD_VALUES[getCard(playerIterator->first)].split(" ")
        .last().toLower() + "NameLabel").first()->setVisible(true);
  }
}

//Function used for all players
void ClueMainWindowClass::drawPieceToBoard(SuspectEnum suspect,
    const BoardLocationClass &tile)
{
  //Variable Declarations
  QPoint boardLocTopLeft = tile.getTopLeftPixel();  //Top left pixel of the tile
                                                    //to draw the piece on

  for(int i = 0; i < TILE_WIDTH; i++)
  {
    for(int j = 0; j < TILE_HEIGHT; j++)
    {
      if(CHAR_PIECE_IMAGES[suspect].pixel(i, j) !=
          TRANSPARENT_RGB)
      {
        inProgressBoardImage.setPixel(boardLocTopLeft + QPoint(i, j),
            CHAR_PIECE_IMAGES[suspect].pixel(i, j));
      }
    }
  }
}

//Function used for all players
void ClueMainWindowClass::clearPiece(SuspectEnum suspect,
    const BoardLocationClass &tile)
{
  //Variable Declarations
  QPoint boardLocTopLeft = tile.getTopLeftPixel();    //Top left pixel of the 
                                                      //tile the piece is on
  int drawOffset[NUMBER_OF_DIRECTIONS] = {1, 1, 1, 1};
                 //Pixel offset; 0 clears entire tile (including borders)

  //If the tile to be cleared is a room tile and the tile in a direction is
  //unoccupied, clear the tile including that border; otherwise, do not clear 
  //the border
  if(tile.getTileType(CLUE_BOARD_IMAGE) == ROOM_TILE)
  {
    for(DirectionEnum direction = UP; direction <= RIGHT; direction =
        DirectionEnum(int(direction) + 1))
    {
      if(tile.getTileInDir(direction).getTileType(inProgressBoardImage) != 
          OCCUPIED_TILE)
      {
        drawOffset[direction] = 0;
      }
    }
  }
  else
  {
    for(DirectionEnum direction = UP; direction <= RIGHT; direction =
        DirectionEnum(int(direction) + 1))
    {
      if(tile.getTileInDir(direction).getTileType(
          inProgressBoardImage) == OUT_OF_BOUNDS_TILE)
      {
        drawOffset[direction] = 0;
      }
    }

    try
    {
      drawOffset[DOOR_DIRECTIONS[tile.getDoorIndex()]] = 0;
    }
    catch(ExceptionClass notADoor)
    {
      //Tile is not a door
    }
  }

  //Clear the tile
  for(int i = drawOffset[LEFT]; i < TILE_WIDTH - drawOffset[RIGHT]; i++)
  {
    for(int j = drawOffset[UP]; j < TILE_HEIGHT - drawOffset[DOWN]; j++)
    {
      if(tile.getTileType(CLUE_BOARD_IMAGE) != ROOM_TILE &&
          suspect == currentPlayerIter->first)
      {
        inProgressBoardImage.setPixel(boardLocTopLeft + QPoint(i, j),
            VISITED_TILE_RGB[suspect]);
      }
      else
      {
        inProgressBoardImage.setPixel(boardLocTopLeft + QPoint(i, j),
            CLUE_BOARD_IMAGE.pixel(boardLocTopLeft + QPoint(i, j)));
      }
    }
  }
}

//Function used for all players
void ClueMainWindowClass::clearVisitedTiles()
{
  //Variable Declarations
  set<BoardLocationClass> visitedLocations =                //Locations visited
      currentPlayerIter->second.getLocationsThisTurn();     //this turn
  set<BoardLocationClass>::iterator visitedLocationIter =   //Iterator to the
      visitedLocations.begin();                             //locations visited
  QPoint tileTopLeftPixel;  //Location of top left pixel of *visitedLocationIter

  //Reset the tiles in the list of locations visited to the original color,
  //with the exception of the player's current location
  while(visitedLocationIter != visitedLocations.end())
  {
    if(visitedLocationIter->getTileType(inProgressBoardImage) != OCCUPIED_TILE)
    {
      tileTopLeftPixel = visitedLocationIter->getTopLeftPixel();

      for(int i = 0; i < TILE_WIDTH; i++)
      {
        for(int j = 0; j < TILE_HEIGHT; j++)
        {
          inProgressBoardImage.setPixel(tileTopLeftPixel + QPoint(i, j),
              CLUE_BOARD_IMAGE.pixel(tileTopLeftPixel + QPoint(i, j)));
        }
      }
    }
    visitedLocationIter++;
  }
  //Update the board shown on the gui
  gameBoard->setPixmap(QPixmap::fromImage(inProgressBoardImage));
}

//Function used for all human players
void ClueMainWindowClass::displayGameInterface()
{
  //Variable Declarations
  QString cardNumber;

  youAreText->setText("You are " + CARD_VALUES[thisSuspect] + ".");

  //Make gameplay interface visible
  currentTurnText->setVisible(true);
  rollInfoText->setVisible(true);
  selectActionLabel->setVisible(true);
  actionOptionsFrame->setVisible(true);
  okayButton->setVisible(true);
  detectiveNotesLabel->setVisible(true);
  detectiveNotesFrame->setVisible(true);
  personalNotesLabel->setVisible(true);
  personalNotes->setVisible(true);
  for(int i = 0; i < MAX_CARDS_IN_HAND; i++)
  {
    cardNumber.setNum(i + 1);
    findChildren<QLabel*>("cardInHand" + cardNumber).first()->setVisible(true);
  }

  youAreText->setVisible(true);

  //Make game setup interface invisible
  gameWelcomeLabel->setVisible(false);
  gameSetupFrame->setVisible(false);
  startGameButton->setVisible(false);

  //Clear out detective notes
  for(int i = 0; i < NUMBER_OF_CARDS; i++)
  {
    updateDetectiveNotes(CardEnum(i));
  }
}

//Function used for all human players
void ClueMainWindowClass::refreshDisplay()
{
  //Variable Declarations
  map<SuspectEnum, PlayerClass>::const_iterator nextPlayerIter;
                                    //Iterator pointing at the next player

  //Enable or disable controls depending on player status
  if(currentPlayerIter->first == thisSuspect ||
      gameParticipants.find(thisSuspect)->second.getLastAction() == MOVE)
  {
    if(currentPlayerIter->second.getPlayerLocation().getTileType(
        CLUE_BOARD_IMAGE) == UNOCCUPIED_TILE)
    {
      displayDefaultOptions();
      //Can move
      if(currentPlayerIter->second.getMovesLeft() > 0)
      {
        enableMovementControls();
        moveUpOption->toggle();
        moveUpOption->setFocus();
      }
      else
      {
        disableMovementControls();
        endTurnOption->toggle();
        endTurnOption->setFocus();
      }
    }
    //In a room
    else
    {
      if(currentPlayerIter->second.getPlayerLocation().checkCornerRoom() ==
          true)
      {
        displayCornerRoomOptions();
        if(currentPlayerIter->second.getLastAction() == ROLL)
        {
          useSecretPassageOption->setEnabled(false);
        }
      }
      else
      {
        displayRoomOptions();
      }
     //Can roll
     if((currentPlayerIter->second.getLastAction() == MOVE && currentPlayerIter
         ->second.getMovedThisTurnFlag() == false) || currentPlayerIter->second.
         getLastAction() == END_TURN)
     {
       enableMovementControls();
       rollDieOption->toggle();
       rollDieOption->setFocus();
       if(currentPlayerIter->second.getLastAction() == MOVE)
       {
         makeSuggestionOption->setEnabled(true);
       }
       else
       {
         makeSuggestionOption->setEnabled(false);
       }
     }
     //Rolled
     else if(currentPlayerIter->second.getLastAction() == ROLL)
     {
       enableMovementControls();
       makeSuggestionOption->setEnabled(false);
       leaveRoomOption->toggle();
       leaveRoomOption->setFocus();
     }
     else if(currentPlayerIter->second.getLastAction() != SUGGEST)
     {
       disableMovementControls();
       makeSuggestionOption->setEnabled(true);
       makeSuggestionOption->toggle();
       makeSuggestionOption->setFocus();
     }
     else
     {
       makeSuggestionOption->setEnabled(false);
       endTurnOption->toggle();
       endTurnOption->setFocus();
     }
    }
  }
  
  if((currentPlayerIter->second.getMovesLeft() <= 0 && currentPlayerIter->
      second.getMovedThisTurnFlag() == true) || (currentPlayerIter->
      second.getPlayerLocation().getTileType(CLUE_BOARD_IMAGE) ==
      UNOCCUPIED_TILE && currentPlayerIter->second.getValidMoveDirections(
      inProgressBoardImage).empty()) || (currentPlayerIter->second.
      getPlayerLocation().getTileType(CLUE_BOARD_IMAGE) == ROOM_TILE &&
      ((currentPlayerIter->second.getValidExitDoors(inProgressBoardImage).
      empty() == true && currentPlayerIter->second.getPlayerLocation().
      checkCornerRoom() == false) || currentPlayerIter->second.
      getMovedThisTurnFlag() == true)))
  {
    endTurnOption->setEnabled(true);
    if(makeSuggestionOption->isChecked() == false)
    {
      endTurnOption->toggle();
      endTurnOption->setFocus();
    }
  }
  else
  {
    endTurnOption->setEnabled(false);
  }


  nextPlayerIter = currentPlayerIter;
  do
  {
    nextPlayerIter++;
    if(nextPlayerIter == gameParticipants.end())
    {
      nextPlayerIter = gameParticipants.begin();
    }
  }
  
  //Update the current player is text
  while(nextPlayerIter->second.getGameOverFlag() == true);

  //Current player is this player.
  if(currentPlayerIter->first == thisSuspect)
  {
    enableAllControls();
    currentTurnText->setText("It is your turn.  " +
        CARD_VALUES[getCard(nextPlayerIter->first)] +
        "'s turn is next.");
  }
  else
  {
    if(nextPlayerIter->first == thisSuspect)
    {
      currentTurnText->setText("It is " +
          CARD_VALUES[getCard(currentPlayerIter->first)]
          + "'s turn.  Your turn is next.");
    }
    else
    {
      currentTurnText->setText("It is " +
          CARD_VALUES[getCard(currentPlayerIter->first)]
          + "'s turn.  " + CARD_VALUES[getCard(nextPlayerIter->first)]
          + "'s turn is next.");
    }
    disableAllControls();
  }
  updateRollInfoText();
}

//Function used for all human players
void ClueMainWindowClass::updateRollInfoText()
{
  //Variable Declarations
  QString textManip;                //String for text manipulation
  QString suspectName = "You";      //Suspect's name
  QString pluralString = "";        //Plural suffix
  QString haveTense = "have";       //Tense of have
  QString isTense = "are";          //Tense of is

  //If the current player is someone other than THIS player, change the 
  //tense
  if(currentPlayerIter->first != thisSuspect)
  {
    suspectName = CARD_VALUES[getCard(currentPlayerIter->first)];
    haveTense = "has";
    isTense = "is";
  }

  //If there are 0 or more than 1 moves left, change the plural string
  if(currentPlayerIter->second.getMovesLeft() != 1)
  {
    pluralString = "s";
  }

  //Display roll info text
  if(currentPlayerIter->second.getPlayerLocation().getTileType(CLUE_BOARD_IMAGE)
      == ROOM_TILE && currentPlayerIter->second.getLastAction() == MOVE)
  {
    rollInfoText->setText(suspectName + " just entered the " +
        CARD_VALUES[getCard(currentPlayerIter->second.
        getPlayerLocation().getRoom())] + " and " + haveTense +
        " no more moves left.");
  }
  else if(currentPlayerIter->second.getPlayerLocation().getTileType(
      CLUE_BOARD_IMAGE) == ROOM_TILE && currentPlayerIter->second.
      getMovedThisTurnFlag() == false && currentPlayerIter->second.
      getLastAction() != ROLL)
  {
    rollInfoText->setText(suspectName + " " + isTense + " in the " +
        CARD_VALUES[getCard(currentPlayerIter->second.getPlayerLocation().
            getRoom())] + ".");
  }
  else
  {
    textManip.setNum(currentPlayerIter->second.getDieRoll());
    rollInfoText->setText(suspectName + " rolled a " + textManip + ".  " +
        suspectName + " " + haveTense + " ");
    textManip.setNum(currentPlayerIter->second.getMovesLeft());
    rollInfoText->setText(rollInfoText->text() + textManip + " move" +
        pluralString + " left.");
  }
}

//Function for all human players; only applies to member variables for THIS
//player
void ClueMainWindowClass::updateDetectiveNotes(CardEnum updatedCard)
{
  //Variable Declarations
  QStringList cardWords = CARD_VALUES[updatedCard].split(" ");
  QString labelName;

  QLabel *textToUpdatePtr;        //Pointer to the label on the gui to update

  if(getCardType(updatedCard) == SUSPECT_CARD)
  {
    labelName = cardWords.last().toLower() + "Text";
  }
  else
  {
    labelName = cardWords.first().toLower();
    cardWords.pop_front();
    while(cardWords.size() > 0)
    {
      labelName.append(cardWords.last());
      cardWords.pop_front();
    }
    labelName.append("Text");
  }

  if(gameParticipants.find(thisSuspect)->second.getDetectiveNotes(updatedCard)
      == UNKNOWN_SUSPECT)
  {
    findChildren<QLabel*>(labelName).first()->setText("Unknown");
  }
  else
  {
    findChildren<QLabel*>(labelName).first()->
        setText("<span style='color:#ff0000;'>" + CARD_VALUES[getCard(
        gameParticipants.find(thisSuspect)->second.getDetectiveNotes(
        updatedCard))] + "</span>");
  }
}

//Function used for all human players; triggered only upon THIS player's turn
void ClueMainWindowClass::displayDefaultOptions()
{
  rollDieOption->setVisible(false);
  useSecretPassageOption->setVisible(false);
  leaveRoomOption->setVisible(false);
  doorNumberSpin->setVisible(false);
  moveUpOption->setVisible(true);
  moveDownOption->setVisible(true);
  moveLeftOption->setVisible(true);
  moveRightOption->setVisible(true);
  makeSuggestionOption->setEnabled(false);
}

//Function used for all human players; triggered only upon THIS player's turn or
//when THIS player is moved via suggestion
void ClueMainWindowClass::displayRoomOptions()
{
  useSecretPassageOption->setVisible(false);
  makeSuggestionOption->setEnabled(true);

  if(currentPlayerIter->second.getLastAction() != ROLL || (currentPlayerIter->
      second.getLastAction() == MOVE && currentPlayerIter->second.
      getMovedThisTurnFlag() == false))
  {
    rollDieOption->setVisible(true);
    leaveRoomOption->setVisible(false);
    doorNumberSpin->setVisible(false);
    moveDownOption->setVisible(false);
    moveUpOption->setVisible(false);
    moveLeftOption->setVisible(false);
    moveRightOption->setVisible(false);
  }
  else
  {
    rollDieOption->setVisible(false);
    leaveRoomOption->setVisible(true);
    doorNumberSpin->setVisible(true);
    moveDownOption->setVisible(false);
    moveUpOption->setVisible(false);
    moveLeftOption->setVisible(false);
    moveRightOption->setVisible(false);
  }
}

//Function used for all human players; triggered only upon THIS player's turn or
//when THIS player is moved via suggestion
void ClueMainWindowClass::displayCornerRoomOptions()
{
  displayRoomOptions();

  useSecretPassageOption->setVisible(true);
}

//Function used for all human players; triggered upon end of THIS player's turn
void ClueMainWindowClass::disableAllControls()
{
  actionOptionsFrame->setEnabled(false);
  okayButton->setEnabled(false);
}

//Function used for all human players; triggered upon start of THIS player's
//turn
void ClueMainWindowClass::enableAllControls()
{
  actionOptionsFrame->setEnabled(true);
  okayButton->setEnabled(true);
}

//Function used for all human players; triggered only during THIS player's turn
//upon THIS player running out of moves
void ClueMainWindowClass::disableMovementControls()
{
  moveLeftOption->setEnabled(false);
  moveRightOption->setEnabled(false);
  moveUpOption->setEnabled(false);
  moveDownOption->setEnabled(false);
  rollDieOption->setEnabled(false);
  useSecretPassageOption->setEnabled(false);
  leaveRoomOption->setEnabled(false);
  doorNumberSpin->setEnabled(false);
}

//Function used for all human players; triggered only upon THIS player's turn
void ClueMainWindowClass::enableMovementControls()
{
  moveLeftOption->setEnabled(true);
  moveRightOption->setEnabled(true);
  moveUpOption->setEnabled(true);
  moveDownOption->setEnabled(true);
  rollDieOption->setEnabled(true);
  useSecretPassageOption->setEnabled(true);
  leaveRoomOption->setEnabled(true);
  doorNumberSpin->setEnabled(true);
}

//Function used for all human players; triggered only upon an invalid entries by
//THIS player
void ClueMainWindowClass::displayExceptionMessageBox(ExceptionClass
    exceptionText) const
{
  QMessageBox errorMessageBox;

  errorMessageBox.setText(exceptionText.getErrorMessage());
  errorMessageBox.setWindowTitle(exceptionText.getErrorTitle());
  errorMessageBox.exec();
}

//Function used for all human players; triggered only upon clicking the make
//accusation button during THIS player's turn
void ClueMainWindowClass::makePlayerAccusation()
{
  //Variable Declarations
  SuggestionClass accusation;                      //Accusation
  AccusationDialogClass accusationDialog(          //Accusation dialog
      &accusation, this);

  if(accusationDialog.exec() == QDialog::Accepted)
  {
    handleAccusation(accusation);
  }
}

//Function used for all human players; triggered only upon clicking the make
//suggestion button during THIS player's turn
void ClueMainWindowClass::makePlayerSuggestion()
{
  //Variable Declarations
  SuggestionClass suggestion;                //Suggestion
  SuggestionDialogClass suggestionDialog(    //Suggestion dialog
      &suggestion, this);

  disableAllControls();
  if(suggestionDialog.exec() == QDialog::Accepted)
  {
    handleSuggestion(suggestion);
    refreshDisplay();
  }
  enableAllControls();
}

//Function used for the host player only
void ClueMainWindowClass::dealCards(DeckClass &cardDeck)
{
  //Variable Declarations
  map<SuspectEnum, PlayerClass>::iterator playerBeingDealtTo =
      gameParticipants.begin();              //Iterator for player to deal to
  CardEnum randomCard;

  //Deal the cards to the players until there are no cards in the deck
  while(cardDeck.getDeckSize() > 0)
  {
    randomCard = cardDeck.drawRandomCard();
    playerBeingDealtTo->second.addCardToHand(randomCard);
    playerBeingDealtTo->second.addToDetectiveNotes(randomCard,
        playerBeingDealtTo->first);

    playerBeingDealtTo++;
    if(playerBeingDealtTo == gameParticipants.end())
    {
      playerBeingDealtTo = gameParticipants.begin();
    }
  }
}

//Function used for the host player only
void ClueMainWindowClass::setupGame()
{
  //Variable Declarations
  set<SuspectEnum> availableCharacters;
  SuspectEnum randomCharacter;
  DeckClass cardDeck;
  bool suspectAvailableFlag;
  int aiDifficulty = 0;
  QString aiNumber;

  aiMoveDelay = AI_DELAY;

  //Initialize availableCharacters
  for(SuspectEnum i = SCARLET; i < NUMBER_OF_SUSPECTS;
      i = SuspectEnum(int(i) + 1))
  {
    availableCharacters.insert(i);
  }

  //Make sure there are between 3 and 6 players
  if((humanPlayersSpin->value() + computerPlayersSpin->value()) < MIN_PLAYERS ||
      (humanPlayersSpin->value() + computerPlayersSpin->value() > MAX_PLAYERS))
  {
    throw(ExceptionClass("Error setting up game", "You must have a minimum of "
        "3 and a maximum of 6 players to start a game."));
  }

  //Add players to gameParticipants
  gameParticipants.clear();
  for(int i = 0; i < humanPlayersSpin->value() + computerPlayersSpin->value();
      i++)
  {
    //Assign a random character to the player
    do
    {
      randomCharacter = SuspectEnum(rand() % NUMBER_OF_SUSPECTS);

      if(availableCharacters.find(randomCharacter) != availableCharacters.end())
      {
        if(i == 0)
        {
          thisSuspect = randomCharacter;
        }
        else if(i >= humanPlayersSpin->value())
        {
          aiNumber.setNum(i - humanPlayersSpin->value() + 1);
          aiDifficulty = findChildren<QComboBox*>("ai" + aiNumber +
              "Difficulty").first()->currentIndex();
        }

        gameParticipants.insert(pair<SuspectEnum, PlayerClass>(randomCharacter,
            PlayerClass(i >= humanPlayersSpin->value(), i == 0,
            STARTING_LOCATIONS[randomCharacter],
            DifficultyEnum(aiDifficulty))));

        suspectAvailableFlag = true;
        availableCharacters.erase(randomCharacter);
      }
      else
      {
        suspectAvailableFlag = false;
      }
    }
    while(suspectAvailableFlag == false);
  }

  currentPlayerIter = gameParticipants.begin();

  //Make the case file and deal out the remaining cards
  caseFile = cardDeck.createCaseFile();
  dealCards(cardDeck);
}

//Function used for all players
void ClueMainWindowClass::startPlayerTurn()
{
  if(currentPlayerIter->second.getGameOverFlag() == true)
  {
    endTurn();
  }
  else
  {
    currentPlayerIter->second.resetLocationsThisTurn();
    currentPlayerIter->second.setMovedThisTurnFlag(false);
    refreshDisplay();

    //Current player is this player or this player is the host and current
    //player is an ai.
    if(currentPlayerIter->first == thisSuspect ||
        (currentPlayerIter->second.getAiFlag() == true &&
        gameParticipants.find(thisSuspect)->second.getHostFlag() == true))
    {
      //THIS player is not in any special situation (corner room or can make
      //suggestion).
      if(currentPlayerIter->second.getPlayerLocation().getTileType(
          CLUE_BOARD_IMAGE) != ROOM_TILE || (currentPlayerIter->second.
          getLastAction() == END_TURN && currentPlayerIter->second.
          getPlayerLocation().checkCornerRoom() == false))
      {
        continuePlayerTurn();
      }
      //Ai in a special situation (corner room or can make suggestion)
      else if(currentPlayerIter->second.getAiFlag() == true)
      {
        takeAiAction(currentPlayerIter->second.handlePrerollAi());
      }
    }
    //Other player or Ai controlled by other player is up
    else
    {
      receiveRemoteTurnInfo();
    }
  }
}

//Function used for all players; triggered only upon THIS player's turn and Ai
//turns if THIS player is host
void ClueMainWindowClass::continuePlayerTurn()
{
  //Roll die and display die roll
  if(currentPlayerIter->second.getLastAction() == END_TURN || (currentPlayerIter
      ->second.getLastAction() == MOVE && currentPlayerIter->second.
      getMovedThisTurnFlag() == false))
  {
    currentPlayerIter->second.rollDie();
    updateRollInfoText();
    currentPlayerIter->second.setLastAction(ROLL);
    ////////////////////////////////////////////////////////////////////////////
    //Network function call
    sendRemoteMoveInfo(ROLL, currentPlayerIter->second.getDieRoll());
    ////////////////////////////////////////////////////////////////////////////
  }

  //THIS player
  if(currentPlayerIter->first == thisSuspect)
  {
    refreshDisplay();
  }
  //Ai's
  else
  {
    takeAiAction(currentPlayerIter->second.handleAfterRollAi());
  }
}

//Function used for all players
void ClueMainWindowClass::handleSuggestion(const SuggestionClass
    &playerSuggestion)
{
  //Variable Declarations
  static SuggestionClass suggestion;              //Suggestion
  static map<SuspectEnum, PlayerClass>::iterator  //Iterator for the player
      playerIter = gameParticipants.end();        //handling the suggestion
  static QMessageBox suggestionMessage(this);     //Suggestion message box
  map<CardEnum, set<SuspectEnum> > playerHand;    //playerIter's hand
  CardEnum revealedCard;                          //Card revealed
  HandleSuggestionDialogClass                     //Dialog box for when human
      playerSuggestionDialog(this);               //player suggestion input
  QString playerName;                             //Name of the suspect handling
                                                  //the suggestion
  QString suggesterName = CARD_VALUES[getCard(    //Name of the suspect who made
      currentPlayerIter->first)];                 //the suggestion
      
  if(currentPlayerIter->first == thisSuspect)
  {
    suggesterName = "you";
  }

  //Set window flags for the player suggestion dialog.
  playerSuggestionDialog.setWindowFlags((playerSuggestionDialog.windowFlags()
      | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint);

  //Setup for the first time the current suggestion is handled      
  if(playerIter == gameParticipants.end())
  {
    //Set the last action of the current player to SUGGEST
    currentPlayerIter->second.setLastAction(SUGGEST);
    playerIter = currentPlayerIter;
    suggestion = playerSuggestion;

    ////////////////////////////////////////////////////////////////////////////
    //Network function call
    sendRemoteMoveInfo(SUGGEST, DEFAULT_DIE_ROLL, EMPTY_LOCATION,
        playerSuggestion);
    ////////////////////////////////////////////////////////////////////////////

    if(currentPlayerIter->first != thisSuspect && gameParticipants.find(
        thisSuspect)->second.getGameOverFlag() == false)
    {
      suggestionMessage.setWindowTitle("Suggestion");
      suggestionMessage.setText(CARD_VALUES[getCard(
          currentPlayerIter->first)] + " suggests that the crime was "
          "committed in the " + CARD_VALUES[getCard(suggestion.getRoom())]
          + " by " + CARD_VALUES[getCard(suggestion.getSuspect())] +
          " with the " + CARD_VALUES[getCard(suggestion.getWeapon())]);
      suggestionMessage.exec();
    }
    moveSuggestedSuspect(suggestion.getSuspect());
    if(suggestion.getSuspect() == thisSuspect)
    {
      refreshDisplay();
    }
  }

  //Move the player handling the suggestion to the next player
  playerIter++;
  if(playerIter == gameParticipants.end())
  {
    playerIter = gameParticipants.begin();
  }

  if(playerIter->first == thisSuspect)
  {
    playerName = "You";
  }
  else
  {
    playerName = CARD_VALUES[getCard(playerIter->first)];
  }
  
  //////////////////////////////////////////////////////////////////////////////
  //Network function call
  checkSuggestionAcknowledged();
  //////////////////////////////////////////////////////////////////////////////

  if(playerIter != currentPlayerIter)
  {
    playerHand = playerIter->second.getHand();
    
    //There is a card matching the suggestion in the player's hand
    if(suggestion == playerHand)
    {
      //THIS player is handling the suggestion
      if(playerIter->first == thisSuspect && gameParticipants.find(
          thisSuspect)->second.getGameOverFlag() == false)
      {
        //Setup and show the player suggestion dialog
        playerSuggestionDialog.setupDialogBox(&suggestion, &revealedCard);
        playerSuggestionDialog.exec();

        sendRevealedCard(revealedCard);
      }
      //Any other player is handling the suggestion
      else
      {
        //Ais are revealing a card if THIS player is host
        if(playerIter->second.getAiFlag() == true &&
            gameParticipants.find(thisSuspect)->second.getHostFlag() == true)
        {
          revealedCard = playerIter->second.handleSuggestionAi(suggestion,
              currentPlayerIter->first);
          sendRevealedCard(revealedCard);
        }
        //Remote players are revealing a card
        else
        {
          revealedCard = receiveRevealedCard();
        }

        if(gameParticipants.find(thisSuspect)->second.getGameOverFlag() ==
            false)
        {
          suggestionMessage.setWindowTitle("Card Revealed");
          suggestionMessage.setText(playerName + " reveals a card to " +
              suggesterName + ".");

          suggestionMessage.exec();
        }
      }

      //Add to and update detective notes and hand
      currentPlayerIter->second.addToDetectiveNotes(revealedCard,
          playerIter->first);
      playerIter->second.addSuspectShowed(revealedCard, currentPlayerIter->
          first);
      if(currentPlayerIter->first == thisSuspect)
      {
        updateDetectiveNotes(revealedCard);
      }

      playerIter = gameParticipants.end();
      finishMove();
    }
    //No cards matching the suggestion in the player's hand
    else
    {
      if(gameParticipants.find(thisSuspect)->second.getGameOverFlag() ==
        false)
      {
        suggestionMessage.setWindowTitle("No Card Shown");
        suggestionMessage.setText(playerName + " cannot disprove the "
            "suggestion.");
        suggestionMessage.exec();
      }
      handleSuggestion(suggestion);
    }
  }
  else
  {
    currentPlayerIter->second.setAiAccusation(playerSuggestion);
    finishMove();
    playerIter = gameParticipants.end();
  }
}

//Function used for all players
void ClueMainWindowClass::handleAccusation(const SuggestionClass
    playerAccusation)
{
  //Variable Declarations
  QMessageBox accusationMessage;                //Accusation message box
  QString playerName = CARD_VALUES[getCard(     //Player name of the accuser
      currentPlayerIter->first)];
  QString haveTense = "has";                    //Have tense
  bool activePlayersFlag = false;               //True = active human players;
                                                //False = no active humans
  map<SuspectEnum, PlayerClass>::const_iterator //Iterator used to go through
      playerIter = gameParticipants.begin();    //the game participants

  if(currentPlayerIter->first == thisSuspect)
  {
    playerName = "You";
    haveTense = "have";
  }

  accusationMessage.setWindowTitle("Game Over");

  //Accusation was correct
  if(playerAccusation == caseFile)
  {
    accusationMessage.setStandardButtons(QMessageBox::No | QMessageBox::Yes);

    accusationMessage.setText(playerName + " " + haveTense + " made a correct "
        "accusation and " + haveTense + " won the game!  Would you like to "
        "play again?");

    if(currentPlayerIter->first != thisSuspect)
    {
      accusationMessage.setText(playerName + " accused " + CARD_VALUES
          [getCard(playerAccusation.getSuspect())] + " of committing the "
          "crime in the " + CARD_VALUES[getCard(playerAccusation.getRoom())]
          + " with the " + CARD_VALUES[getCard(playerAccusation.
          getWeapon())] + ".<br>" + accusationMessage.text());
    }

    //THIS player was the accuser
    if(currentPlayerIter->first == thisSuspect)
    {
      accusationMessage.setWindowTitle("Congratulations!");
    }

    if(accusationMessage.exec() == QMessageBox::Yes)
    {
      setupNewBoard();
    }
    else
    {
      close();
    }
  }

  //Accusation was incorrect
  else
  {
    //If the player piece is in a doorway, move it into the room
    try
    {
      moveCurrentPlayer(DOOR_DIRECTIONS[currentPlayerIter->second.
          getPlayerLocation().getDoorIndex()]);
    }
    catch(ExceptionClass notAtADoor)
    {
      //Do nothing
    }

    accusationMessage.setStandardButtons(QMessageBox::Ok);

    accusationMessage.setText(playerName + " " + haveTense + " made an "
        "incorrect accusation and can make no further moves this game.");

    if(currentPlayerIter->first != thisSuspect)
    {
      accusationMessage.setWindowTitle("Player Eliminated");
      accusationMessage.setText(playerName + " accused " + CARD_VALUES
        [getCard(playerAccusation.getSuspect())] + " of committing the "
        "crime in the " + CARD_VALUES[getCard(playerAccusation.getRoom())] +
        " with the " + CARD_VALUES[getCard(playerAccusation.getWeapon())] +
        ".<br>" + accusationMessage.text());
    }

    currentPlayerIter->second.setAiFlag(true);
    currentPlayerIter->second.setGameOverFlag(true);

    accusationMessage.exec();

    //Check if there are still active human players
    while(playerIter != gameParticipants.end() && activePlayersFlag == false)
    {
      if(playerIter->second.getGameOverFlag() == false &&
          playerIter->second.getAiFlag() == false)
      {
        activePlayersFlag = true;
      }
      playerIter++;
    }
    
    //There are still active human players
    if(activePlayersFlag == true)
    {
      checkSuggestionAcknowledged();

      endTurn();
    }
    //No active human players left
    else
    {
      accusationMessage.setStandardButtons(QMessageBox::No | QMessageBox::Yes);

      if(gameParticipants.find(thisSuspect)->second.getGameOverFlag() == true)
      {
        accusationMessage.setText("All human players have been eliminated.  "
            "Would you like to watch the Ai players finish the game?");
      }

      if(accusationMessage.exec() == QMessageBox::Yes)
      {
        aiMoveDelay = AI_ONLY_DELAY;
        endTurn();
      }
      else
      {
        accusationMessage.setText("Would you like to play again?");
        if(accusationMessage.exec() == QMessageBox::Yes)
        {
          setupNewBoard();
        }
        else
        {
          close();
        }
      }
    }
  }
}

//Function used for all players
void ClueMainWindowClass::moveCurrentPlayer(const DirectionEnum &direction)
{
  //Variable Declarations
  BoardLocationClass oldLocation = currentPlayerIter->  //Old player location
      second.getPlayerLocation();

  try
  {
    //Draw the move
    currentPlayerIter->second.move(inProgressBoardImage, direction);
    drawMove(currentPlayerIter->first, oldLocation, currentPlayerIter->second.
        getPlayerLocation());

    if(currentPlayerIter->second.getPlayerLocation().getTileType(
        CLUE_BOARD_IMAGE) == ROOM_TILE)
    {
      clearVisitedTiles();
      refreshDisplay();
    }
    ////////////////////////////////////////////////////////////////////////////
    //Network function call
    sendRemoteMoveInfo(MOVE, DEFAULT_DIE_ROLL, currentPlayerIter->second.
        getPlayerLocation().getTileInDir(direction));
    ////////////////////////////////////////////////////////////////////////////

    finishMove();

  }
  catch(ExceptionClass newException)
  {
    errorMessageLabel->setText("<span style=' font-weight:600; color:#ff0000;'>"
        + newException.getErrorMessage() + "</span>");
  }
}

//Function used for all players
void ClueMainWindowClass::moveCurrentPlayerToSecretPassage()
{
  //Variable Declarations
  BoardLocationClass newLocation;    //New location of the player

  switch(currentPlayerIter->second.getPlayerLocation().getRoom())
  {
    case LOUNGE:
      newLocation = ROOM_PIECE_LOCATIONS[CONSERVATORY].getEmptyRoomTile(
          inProgressBoardImage);
      break;
    case KITCHEN:
      newLocation = ROOM_PIECE_LOCATIONS[STUDY].getEmptyRoomTile(
          inProgressBoardImage);
      break;
    case CONSERVATORY:
      newLocation = ROOM_PIECE_LOCATIONS[LOUNGE].getEmptyRoomTile(
          inProgressBoardImage);
      break;
    case STUDY:
      newLocation = ROOM_PIECE_LOCATIONS[KITCHEN].getEmptyRoomTile(
          inProgressBoardImage);
      break;
    default:
      break;
  }

  drawMove(currentPlayerIter->first, currentPlayerIter->second.
      getPlayerLocation(), newLocation);
  currentPlayerIter->second.setPlayerLocation(newLocation);
  //////////////////////////////////////////////////////////////////////////////
  //Network function call
  sendRemoteMoveInfo(USE_SECRET_PASSAGE, DEFAULT_DIE_ROLL,
      newLocation);
  //////////////////////////////////////////////////////////////////////////////
  finishMove();
  refreshDisplay();
}

//Function used for all players
void ClueMainWindowClass::moveSuggestedSuspect(SuspectEnum suggestedSuspect)
{
  //Variable Declarations
  map<SuspectEnum, PlayerClass>::iterator playerIter =   //Iterator pointing to
      gameParticipants.find(suggestedSuspect);           //the suggested suspect
  BoardLocationClass newLocation;                        //New location of the 
                                                         //suggested suspect

  //Player didn't suggest himself and the suggested suspect is not in the
  //current room
  if(gameParticipants.find(suggestedSuspect) != gameParticipants.end() &&
      playerIter->second.getGameOverFlag() == false && (playerIter->
      second.getPlayerLocation().getTileType(CLUE_BOARD_IMAGE) !=
      ROOM_TILE || playerIter->second.getPlayerLocation().getRoom() !=
      currentPlayerIter->second.getPlayerLocation().getRoom()))
  {
    newLocation = currentPlayerIter->second.getPlayerLocation().
        getEmptyRoomTile(inProgressBoardImage);
    drawMove(playerIter->first, playerIter->second.getPlayerLocation(),
        newLocation);
    playerIter->second.setPlayerLocation(newLocation);
  }
}

//Function used for all players
void ClueMainWindowClass::moveCurrentPlayerOutDoor(BoardLocationClass doorLoc)
{
  if(doorLoc.getTileType(inProgressBoardImage) == UNOCCUPIED_TILE)
  {
    drawMove(currentPlayerIter->first, currentPlayerIter->second.
        getPlayerLocation(), doorLoc);
    currentPlayerIter->second.setPlayerLocation(doorLoc);

    finishMove();
  refreshDisplay();
  }
  else
  {
    throw(ExceptionClass("This door is blocked.  Please select another door "
        "and try agan."));
  }
}

//Function used for all players
void ClueMainWindowClass::moveCurrentPlayerOutDoor(int doorNumber)
{
  //Check if the door number exists for the room
  if(doorNumber >
      NUMBER_OF_DOORS[currentPlayerIter->second.getPlayerLocation().getRoom()])
  {
    throw(ExceptionClass("The door you have selected does not exist for the "
        "room you are in.  Please select another door and try again."));
  }

  moveCurrentPlayerOutDoor(DOOR_LOCATIONS[getStartingDoorIndex(currentPlayerIter
      ->second.getPlayerLocation().getRoom()) + doorNumber - 1]);
}

//Function used for all players
void ClueMainWindowClass::finishMove()
{
  currentPlayerIter->second.decrementMovesLeft();
  //THIS player is the current player and either has no moves left, is blocked
  //and isn't in a corner room, or moved through a secret passage this turn
  if(currentPlayerIter->first == thisSuspect && (currentPlayerIter->second.
      getMovesLeft() <= 0 || (currentPlayerIter->second.getPlayerLocation().
      getTileType(CLUE_BOARD_IMAGE) == UNOCCUPIED_TILE && currentPlayerIter->
      second.getValidMoveDirections(inProgressBoardImage).empty()) ||
      (currentPlayerIter->second.getPlayerLocation().getTileType(
      CLUE_BOARD_IMAGE) == ROOM_TILE && ((currentPlayerIter->second.
      getValidExitDoors(inProgressBoardImage).empty() == true &&
      currentPlayerIter->second.getPlayerLocation().checkCornerRoom() == false)
      || currentPlayerIter->second.getMovedThisTurnFlag() == true))))
  {
    refreshDisplay();
  }
  else
  {
    updateRollInfoText();
  }
}

//Function used for all players
void ClueMainWindowClass::endTurn()
{
  currentPlayerIter->second.setMovesLeft(0);
  currentPlayerIter->second.setLastAction(END_TURN);
  clearVisitedTiles();
  currentPlayerIter->second.setDieRoll(0);
  
///// This block sends from client/host depending on whose turn it just was

// Print Send Block details
//     cout << "FIRST SEND BLOCK" << endl;
//     cout << "networkedPlayOption->isChecked() "
//       << networkedPlayOption->isChecked() << endl;
//     cout << "gameParticipants.find(thisSuspect)->second.getHostFlag() "
//       << gameParticipants.find(thisSuspect)->second.getHostFlag() << endl;
//     cout << "currentPlayerIter->second.getHostFlag() "
//       << currentPlayerIter->second.getHostFlag() << endl;
//     cout << "currentPlayerIter->second.getAiFlag() "
//       << currentPlayerIter->second.getAiFlag() << endl;

//   if(networkedPlayOption->isChecked() == true)
//     {
//       if (gameParticipants.find(thisSuspect)->second.getHostFlag() == true)
                                                                      // HOST
//       {
//         if (currentPlayerIter->second.getHostFlag() == true)
//         {
//           sendInfoFromHost();
//         }
//         else if (currentPlayerIter->second.getAiFlag() == true)
//         {
//           sendInfoFromHost();
//         }
//         else
//         {
//           //sendInfoFromClient();
//         }
//       }
//       else // The client
//       {
//         if (currentPlayerIter->second.getHostFlag() == true)
//         {
//           //sendInfoFromHost();
//         }
//         else if (currentPlayerIter->second.getAiFlag() == true)
//         {
//           //sendInfoFromHost();
//         }
//         else
//         {
//           sendInfoFromClient();
//         }
//       }
//     }
    
///// End networking block //////

  //Move the current player iterator to the next player or the first player if 
  //it is pointing at the end of the list of game participants
  currentPlayerIter++;
  if(currentPlayerIter == gameParticipants.end())
  {
    currentPlayerIter = gameParticipants.begin();
  }
  
///// Begin networking block //////
///// This block listens if it's not the turn of client/host

// Print Listen Block details
//   cout << "SECOND LISTEN BLOCK" << endl;
//   cout << "networkedPlayOption->isChecked() " 
//     << networkedPlayOption->isChecked() << endl;
//   cout << "gameParticipants.find(thisSuspect)->second.getHostFlag() "
//     << gameParticipants.find(thisSuspect)->second.getHostFlag() << endl;
//   cout << "currentPlayerIter->second.getHostFlag() "
//     << currentPlayerIter->second.getHostFlag() << endl;
//   cout << "currentPlayerIter->second.getAiFlag() "
//     << currentPlayerIter->second.getAiFlag() << endl;
    
    // Listen Block
//     if(networkedPlayOption->isChecked() == true)
//     {
//       if (gameParticipants.find(thisSuspect)->second.getHostFlag() == true)
                                                                    // HOST
//       {
//         if (currentPlayerIter->second.getHostFlag() == true)
//         {
//           //receiveInfoFromHost();
//         }
//         else if (currentPlayerIter->second.getAiFlag() == true)
//         {
//           //receiveInfoFromHost();
//         }
//         else
//         {
//           receiveInfoFromClient();
//         }
//       }
//       else // The client
//       {
//         if (currentPlayerIter->second.getHostFlag() == true)
//         {
//           receiveInfoFromHost();
//         }
//         else if (currentPlayerIter->second.getAiFlag() == true)
//         {
//           receiveInfoFromHost();
//         }
//         else
//         {
//           //receiveInfoFromClient();
//         }
//       }
//     }
    
///// End networking block //////

  startPlayerTurn();
}

//Function used for Ai players if THIS player is the host
void ClueMainWindowClass::takeAiAction(const ActionEnum action)
{
map<SuspectEnum, PlayerClass>::const_iterator aiPlayerIter =
    currentPlayerIter;
BoardLocationClass targetDoorLocation;

  switch(action)
  {
    case ROLL:
      continuePlayerTurn();
      break;
    case USE_SECRET_PASSAGE:
      moveCurrentPlayerToSecretPassage();
      QTest::qWait(aiMoveDelay);
      takeAiAction(SUGGEST);
      QTest::qWait(aiMoveDelay);
      break;
    case MOVE:
      try
      {
        //Get the target door location
        targetDoorLocation = currentPlayerIter->second.getAiTargetDoor();

        //Player is in a room
        if(currentPlayerIter->second.getPlayerLocation().getTileType(
            CLUE_BOARD_IMAGE) == ROOM_TILE)
        {
          moveCurrentPlayerOutDoor(currentPlayerIter->second.getAiExitDoor(
              inProgressBoardImage, targetDoorLocation));
        }

        //Move while there are moves left and the player hasn't reached the room
        while(currentPlayerIter->second.getMovesLeft() > 0 &&
            aiPlayerIter == currentPlayerIter && currentPlayerIter->second.
            getPlayerLocation().getTileType(CLUE_BOARD_IMAGE) ==
            UNOCCUPIED_TILE)
        {
          QTest::qWait(aiMoveDelay);
          moveCurrentPlayer(currentPlayerIter->second.getAiMove(
              inProgressBoardImage, targetDoorLocation));
          QTest::qWait(aiMoveDelay);
        }

        //Make a suggestion if the player is in a room
        if(aiPlayerIter == currentPlayerIter && currentPlayerIter->second.
            getPlayerLocation().getTileType(CLUE_BOARD_IMAGE) == ROOM_TILE)
        {
          takeAiAction(SUGGEST);
        }
        else
        {
          takeAiAction(ACCUSE);
        }
      }
      catch(ExceptionClass aiBlocked)
      {
        currentPlayerIter->second.setMovesLeft(0);
        takeAiAction(END_TURN);
      }
      break;
    case SUGGEST:
      handleSuggestion(currentPlayerIter->second.makeAiSuggestion());
      takeAiAction(ACCUSE);
      break;
    case ACCUSE:
      try
      {
        if(rand() % AI_GOD_MODE_ROLL == 0)
        {
          currentPlayerIter->second.setAiAccusation(caseFile);
        }
        currentPlayerIter->second.makeAiAccusation();
        handleAccusation(currentPlayerIter->second.getAiAccusation());
      }
      catch(ExceptionClass notReadyToAccuse)
      {
        takeAiAction(END_TURN);
      }
      break;
    case END_TURN:
      endTurn();
      break;
  }
}

void ClueMainWindowClass::sendRemoteMoveInfo(ActionEnum playerAction,
    int dieRoll, BoardLocationClass locationAfterMove, SuggestionClass
    suggestionMade)
{
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  //Send turn information from the host, one action at a time.
  //
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
}

// Sends info from host to client
void ClueMainWindowClass::sendInfoFromHost()
{
  // Open server socket
  ServerSocket server(socketPort);
  ServerSocket serverSock;


  string clientReply;
  PackageClass package;
  string gameParticipantsPkgStr, currentPlayerStr;
  
  // Wrap up package to pass to other networked game(s)
  gameParticipantsPkgStr = package.wrapSetupPkg(gameParticipants);
  currentPlayerStr = package.wrapCurrentPlayerPackage(currentPlayerIter->first);
  cout << gameParticipantsPkgStr << endl;
  cout << currentPlayerStr << endl;
  
  cout << "BEGIN SEND INFO FROM HOST" << endl;
  cout << "--------------------------" << endl;
  
  //cout << "Confirmation from client?" << endl;
  serverSock >> clientReply;
  cout << "Connection message: " << clientReply << endl;
  
  // Send game participants (package #1) to client
  cout << "Sending gameParticipantsPkgStr..." << endl;
  serverSock << gameParticipantsPkgStr; 
  
  serverSock >> clientReply;
  cout << "Received this reply: " << clientReply;
  
  cout << "Sending gameParticipantsPkgStr..." << endl;
  serverSock << currentPlayerStr;
  
  serverSock >> clientReply;
  
  cout << "END SEND INFO FROM HOST" << endl;
  cout << "--------------------------" << endl;
}

// Client receives info from host
void ClueMainWindowClass::receiveInfoFromHost()
{
  ClientSocket cliSock("localhost", socketPort);
  
  
  PackageClass package;
  string packageString;
  string gameParticipantsPkgStr, currentPlayerStr;
  SuspectEnum currentSuspectExchange;

  cout << "BEGIN RECEIVE INFO FROM HOST" << endl;
  cout << "--------------------------" << endl;
  
  // Send "I'm here message to server!"
  cout << "Sending message to server: " << endl;
  cliSock << "Connected!";
  
  cout << "Waiting for server packets" << endl;
  
  // Get game participants package (pack #1)
  cliSock >> gameParticipantsPkgStr;
  cout << "Received gameParticipantsPkgStr" << endl;
  
  gameParticipants = package.unwrapSetupPkg(gameParticipantsPkgStr);
  
  cliSock << "Send next packet!";
  
  // Get Suspect Enum (pack #2)
  cliSock >> currentPlayerStr;
  cout << "Received currentPlayerStr" << endl;
  
  currentSuspectExchange = 
    package.unwrapCurrentPlayerPackage(currentPlayerStr);
  
  currentPlayerIter = 
    gameParticipants.find(SuspectEnum(currentSuspectExchange));
  
  cliSock << "All done!";
  
  cout << "END RECIEVE INFO FROM HOST" << endl;
  cout << "--------------------------" << endl;
  
  refreshDisplay();
  
  // IF STILL HOSTS TURN
  // CALL REC INFO (go back into listen mode)
  
//       if (currentPlayerIter->second.getHostFlag() == true)
//         {
//           receiveInfoFromHost();
//         }
//       else if (currentPlayerIter->second.getAiFlag() == true)
//         {
//           receiveInfoFromHost();
//         }
//       else
//       {
//         //receiveInfoFromHost();
//       }
}

// Client sends info to host
void ClueMainWindowClass::sendInfoFromClient()
{

  ClientSocket cliSock("localhost", socketPort);
  
  
  PackageClass package;
  string packageString;
  string gameParticipantsPkgStr, currentPlayerStr;
  
  gameParticipantsPkgStr = package.wrapSetupPkg(gameParticipants);
  currentPlayerStr = package.wrapCurrentPlayerPackage(currentPlayerIter->first);
  cout << gameParticipantsPkgStr << endl;
  cout << currentPlayerStr << endl;
  
  cout << "BEGIN SEND INFO FROM CLIENT" << endl;
  cout << "--------------------------" << endl;
  
  cout << "Sending message to server: " << endl;
  cliSock << gameParticipantsPkgStr;
  
  cout << "Waiting for server packets" << endl;
  
  // Get game participants package (pack #1)
  cliSock >> gameParticipantsPkgStr;
  cout << "Sent gameParticipantsPkgStr" << endl;
  
  cliSock << "One more to go!";
  
  // Get game participants package (pack #2)
  cliSock >> currentPlayerStr;
  cout << "Sent currentPlayerStr" << endl;
  
  cliSock << "All done!";
  
  cout << "END SEND INFO FROM CLIENT" << endl;
  cout << "--------------------------" << endl;
}

// Host gets info from client
void ClueMainWindowClass::receiveInfoFromClient()
{
  ServerSocket server(socketPort);
  ServerSocket serverSock;

  
  cout << "BEGIN RECEIVE INFO FROM CLIENT" << endl;
  cout << "--------------------------" << endl;

  string clientReply;
  PackageClass package;
  string gameParticipantsPkgStr, currentPlayerStr;
  SuspectEnum currentSuspectExchange;
  
  // Wrap up package to pass to other networked game(s)
  
  
  // Hold for client to connect
  //cout << "Waiting for client to connect......." << endl;
  //server.accept(serverSock);
  //cout << "The client connected!" << endl;

  // Test stuff
  
  cout << "Waiting for message from client..." << endl;
  serverSock >> gameParticipantsPkgStr;
  cout << "Received message: gameParticipantsPkgStr" << endl;
  
  gameParticipants = package.unwrapSetupPkg(gameParticipantsPkgStr);
  
  serverSock << "OK, send the next one!"; 
  
  serverSock >> currentPlayerStr;
  cout << "Received message: currentPlayerStr" << endl;
  
  currentSuspectExchange = package.unwrapCurrentPlayerPackage(currentPlayerStr);

  currentPlayerIter = gameParticipants.find(SuspectEnum(
      currentSuspectExchange));
  
  serverSock << "All set!";
  
  serverSock >> clientReply;
  
  cout << "END RECEIVE INFO FROM CLIENT" << endl;
  cout << "--------------------------" << endl;

  
  refreshDisplay();
  
// IF STILL CLIENTS TURN
// CALL REC INFO (go back into listen mode)

//       if (currentPlayerIter->second.getAiFlag() == true)
//         {
//           //receiveInfoFromClient();
//         }
//       else if (currentPlayerIter->second.getHostFlag() == true)
//         {
//           //receiveInfoFromClient();
//         }
//       else
//       {
//         receiveInfoFromClient();
//       }
}


void ClueMainWindowClass::receiveRemoteTurnInfo()
{
  //Variable Declarations
  ActionEnum action;
  int dieRoll;
  BoardLocationClass locationAfterMove;
  SuggestionClass suggestionMade;

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  //Receive turn information from the host, one action at a time.
  //
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
}


void ClueMainWindowClass::sendRevealedCard(CardEnum revealedCard)
{

}

CardEnum ClueMainWindowClass::receiveRevealedCard()
{

}

void ClueMainWindowClass::checkSuggestionAcknowledged()
{

}

void ClueMainWindowClass::startGame()
{
  try
  {
    //Networked play - host
    if(gameHostCheck->isChecked() == true || localPlayOption->isChecked() ==
        true)
    {
      //Setup the game
      setupGame();

      if(networkedPlayOption->isChecked() == true &&
          humanPlayersSpin->value() > 1)
      {
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        //NETWORK
        //Send game setup data to clients:
        //gameParticipants
        //caseFile
        //SuspectEnum assigned to that player
        //
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////

        ServerSocket server(socketPort);
        ServerSocket serverSock;
        

        string clientReply;
        PackageClass package;
        string gameParticipantsPkgStr, caseFilePkgStr, suspectEnumPkgStr,
            iterString;
        
        // Wrap up package to pass to other networked game(s)
        gameParticipantsPkgStr = package.wrapSetupPkg(gameParticipants);
        caseFilePkgStr = package.wrapCaseFilePkg(caseFile);
        suspectEnumPkgStr = package.wrapSuspectEnum(thisSuspect);
        iterString = package.wrapCurrentPlayerPackage(currentPlayerIter->first);
        
//         TEST BLOCK
//         cout << gameParticipantsPkgStr << endl;
//         cout << "------" << endl;
//         cout << caseFilePkgStr << endl;
//         cout << "------" << endl;
//         cout << "This suspect is : " << thisSuspect << endl;
        
        // Hold for client to connect
        cout << "Waiting for client to connect......." << endl;
        server.accept(serverSock);
        cout << "The client connected!" << endl;
      
        // Test stuff
        cout << "Confirmation from client?" << endl;
        serverSock >> clientReply;
        cout << "Received message: " << clientReply << endl;
        
        // Send game participants (package #1) to client
        serverSock << gameParticipantsPkgStr; 
        
        serverSock >> clientReply;
        cout << "Done yet? " << clientReply << endl;
        
        // Send case file (package #2) to client
        serverSock << caseFilePkgStr;
        
        serverSock >> clientReply;
        cout << clientReply << endl;
        
        // Send suspect number (package #3) to client
        serverSock << suspectEnumPkgStr;
        
        serverSock >> clientReply;
        cout << clientReply << endl;
        
        cout << "Number of players is: " << (int) gameParticipants.size() <<
            endl;
        
//         TEST BLOCK
//         cout << gameParticipantsPkgStr << endl;
//         cout << caseFilePkgStr << endl;
//         cout << suspectEnumPkgStr << endl;      
      }
    }
    else
    {
      //////////////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////
      //NETWORK
      //Receive game setup data from host:
      //gameParticipants
      //caseFile
      //SuspectEnum for THIS player
      //
      //////////////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////
      
      // Fun things to create
    
      ClientSocket cliSock("localhost", socketPort);
      
      
      PackageClass package;
      string packageString;
      string gameParticipantsPkgStr, caseFilePkgStr, suspectEnumPkgStr,
          iterString;
      
      // Make sure this array is clear
      //gameParticipants.clear();
      
      // Trying to connect to server
      cout << "Attempting to connect to server" << endl;
      
      // Send "I'm here message to server!"
      cout << "Sending message to server: " << endl;
      cliSock << "I'm here!";
      
      cout << "Waiting for server packets" << endl;
      
      // Get game participants package (pack #1)
      cliSock >> gameParticipantsPkgStr;
      cout << gameParticipantsPkgStr << endl;
      
      gameParticipants = package.unwrapSetupPkg(gameParticipantsPkgStr);
      
      cliSock << "All done, send me #2!";
      
      // Get case file package (pack #2)
      cliSock >> caseFilePkgStr;
      cout << caseFilePkgStr << endl;
      
      caseFile = package.unwrapCaseFilePkg(caseFilePkgStr);
      
      cliSock << "All done, send me #3!";
      
      // Get THIS suspect's number package (pack #3)
      cliSock >> suspectEnumPkgStr;
      cout << suspectEnumPkgStr << endl;
      
      thisSuspect = package.unwrapSuspectEnum(suspectEnumPkgStr);
      
      cout << "Server suspect is " << thisSuspect << endl;
      
      // Cheap way of finding out the non-human, non-host player for now.
      // Get number of people in game
      int numPlayers = (int) gameParticipants.size();
      // Set iterator to first player (host)
      map< SuspectEnum, PlayerClass >::iterator partIter;
      partIter = gameParticipants.begin();
      PlayerClass player;
      int suspNum;
      int i;
      // Put inside loop?
      for (i = 0; i < numPlayers; i++)
      {
        suspNum = partIter->first;
        player = partIter->second;
        if (player.getHostFlag() == FALSE && player.getAiFlag() == FALSE )
        {
          thisSuspect = SuspectEnum(suspNum);
        }
      partIter++;
      }
      
      cout << "Client suspect is " << thisSuspect << endl;
      
      cliSock << "All done!";
      
//       TEST BLOCK
//       gameParticipantsPkgStr = package.wrapSetupPkg(gameParticipants);
//       caseFilePkgStr = package.wrapCaseFilePkg(caseFile);
//       suspectEnumPkgStr = package.wrapSuspectEnum(thisSuspect);
//       cout << gameParticipantsPkgStr << endl;
//       cout << caseFilePkgStr << endl;
//       cout << suspectEnumPkgStr << endl;

      currentPlayerIter = gameParticipants.begin();
      
    }
    
    cout << " Game setup completed" << endl;
    cout << " --------------------------------------" << endl;
    
    //Display gameplay interface
    displayCardsInHand();
    drawStartingPieces();
    displayGameInterface();
    displayDefaultOptions();
    
    
/// Test block for client/host going into listen mode after init
//     cout << "BLOCK JUST AFTER INIT" << endl;
//     cout << "networkedPlayOption->isChecked() "
//       << networkedPlayOption->isChecked() << endl;
//     cout << "gameParticipants.find(thisSuspect)->second.getHostFlag() "
//       << gameParticipants.find(thisSuspect)->second.getHostFlag() << endl;
//     cout << "currentPlayerIter->second.getHostFlag() "
//       << currentPlayerIter->second.getHostFlag() << endl;
//     cout << "currentPlayerIter->second.getAiFlag() "
//       << currentPlayerIter->second.getAiFlag() << endl;
    
    //Start the first turn
    startPlayerTurn();
    
//     if(networkedPlayOption->isChecked() == true)
//     {
//       if (gameParticipants.find(thisSuspect)->second.getHostFlag() == true)
                                                                      // HOST
//       {
//         if (currentPlayerIter->second.getHostFlag() == true)
//         {
//           //receiveInfoFromHost();
//         }
//         else if (currentPlayerIter->second.getAiFlag() == true)
//         {
//           //receiveInfoFromHost();
//         }
//         else
//         {
//           receiveInfoFromClient();
//         }
//       }
//       else // The client
//       {
//         if (currentPlayerIter->second.getHostFlag() == true)
//         {
//           receiveInfoFromHost();
//         }
//         else if (currentPlayerIter->second.getAiFlag() == true)
//         {
//           receiveInfoFromHost();
//         }
//         else
//         {
//           //receiveInfoFromClient();
//         }
//       }
//     }
    

  }
  catch(ExceptionClass newException)
  {
    displayExceptionMessageBox(newException);
  }
}

void ClueMainWindowClass::submitMove()
{
  try
  {
    errorMessageLabel->setText("");
    if(rollDieOption->isChecked() == true)
    {
      continuePlayerTurn();
    }
    else if(useSecretPassageOption->isChecked() == true)
    {
      moveCurrentPlayerToSecretPassage();
    }
    else if(moveUpOption->isChecked() == true)
    {
      moveCurrentPlayer(UP);
    }
    else if(moveDownOption->isChecked() == true)
    {
      moveCurrentPlayer(DOWN);
    }
    else if(moveLeftOption->isChecked() == true)
    {
      moveCurrentPlayer(LEFT);
    }
    else if(moveRightOption->isChecked() == true)
    {
      moveCurrentPlayer(RIGHT);
    }
    else if(leaveRoomOption->isChecked() == true)
    {
      moveCurrentPlayerOutDoor(doorNumberSpin->value());
    }
    else if(makeSuggestionOption->isChecked() == true)
    {
      makePlayerSuggestion();
    }
    else if(makeAccusationOption->isChecked() == true)
    {
      makePlayerAccusation();
    }
    else if(endTurnOption->isChecked() == true)
    {
      endTurn();
    }
  }
  catch(ExceptionClass newException)
  {
    errorMessageLabel->setText("<span style=' font-weight:600; color:#ff0000;'>"
        + newException.getErrorMessage() + "</span>");
  }
}



void ClueMainWindowClass::setNetworkOptVis()
{
  gameHostCheck->setVisible(true);
  if(gameHostCheck->isChecked() == true)
  {
    hostIPEdit->setVisible(false);
    hostIPLabel->setVisible(false);
    computerPlayersSpin->setVisible(true);
    computerPlayersLabel->setVisible(true);
    humanPlayersSpin->setVisible(true);
    humanPlayersLabel->setVisible(true);
    humanPlayersSpin->setEnabled(true);
    computerPlayersSpin->setEnabled(true);
    updateNumberOfAis(computerPlayersSpin->value());
  }
  else
  {
    hostIPEdit->setVisible(true);
    hostIPLabel->setVisible(true);
    computerPlayersSpin->setVisible(false);
    computerPlayersLabel->setVisible(false);
    humanPlayersSpin->setVisible(false);
    humanPlayersLabel->setVisible(false);
    updateNumberOfAis(0);
  }
}

void ClueMainWindowClass::setLocalOptVis()
{
  gameHostCheck->setVisible(false);
  hostIPEdit->setVisible(false);
  hostIPLabel->setVisible(false);
  computerPlayersSpin->setVisible(true);
  computerPlayersLabel->setVisible(true);
  humanPlayersSpin->setVisible(true);
  humanPlayersLabel->setVisible(true);

  computerPlayersSpin->setEnabled(true);
  humanPlayersSpin->setEnabled(false);
  humanPlayersSpin->setValue(1);
  ai1DifficultyLabel->setVisible(true);
  updateNumberOfAis(computerPlayersSpin->value());
}

void ClueMainWindowClass::updateNumberOfAis(int numberOfAis)
{
  QString aiNumber;

  for(int i = 0; i < MAX_AIS; i++)
  {
    aiNumber.setNum(i + 1);
    findChildren<QLabel*>("ai" + aiNumber + "DifficultyLabel").first()->
        setVisible(i < numberOfAis);
    findChildren<QComboBox*>("ai" + aiNumber + "Difficulty").first()->
        setVisible(i < numberOfAis);
  }

  gameSetupFrame->setGeometry(gameSetupFrame->x(), gameSetupFrame->y(),
      gameSetupFrame->width(), GAME_SETUP_FRAME_WIDTH + VERTICAL_GUI_SPACING *
      numberOfAis);
  startGameButton->setGeometry(startGameButton->x(), START_GAME_BUTTON_Y +
      VERTICAL_GUI_SPACING * numberOfAis, startGameButton->width(),
      startGameButton->height());
}

void ClueMainWindowClass::setClientSocket(ClientSocket &cliSock, int ipAddress,
    int portNumber)
{
  //ClientSocket *newSocket = new ClientSocket("localhost", 30000)
  //cliSock("localhost", 30000);
  return;
}
