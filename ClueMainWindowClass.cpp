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
#include "roomToCard.h"
#include "suspectToCard.h"
#include "weaponToCard.h"
#include "getStartingDoorIndex.h"

using namespace std;

//Function used by all human players
ClueMainWindowClass::ClueMainWindowClass() : QWidget()
{
  //Set up the GUI
  setupUi(this);

  difficultySlider->setTickPosition(QSlider::TicksBothSides);

  //Signals and slots
  connect(networkedPlayOption, SIGNAL(clicked()), this,
      SLOT(setNetworkOptVis()));
  connect(localPlayOption, SIGNAL(clicked()), this, SLOT(setLocalOptVis()));
  connect(gameHostCheck, SIGNAL(clicked()), this, SLOT(setNetworkOptVis()));
  connect(doorNumberSpin, SIGNAL(valueChanged(int)), this,
      SLOT(toggleLeaveRoomOpt()));
  connect(okayButton, SIGNAL(clicked()), this, SLOT(submitMove()));
  connect(startGameButton, SIGNAL(clicked()), this, SLOT(startGame()));
  connect(difficultySlider, SIGNAL(valueChanged(int)), this,
      SLOT(updateDifficultyText(int)));
  connect(helpButton, SIGNAL(clicked()), this, SLOT(openClueRules()));

  //Setup the board
  setupNewBoard();
}

//Function used by all human players
void ClueMainWindowClass::setupNewBoard()
{
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

  scarletNameLabel->setVisible(false);
  mustardNameLabel->setVisible(false);
  whiteNameLabel->setVisible(false);
  greenNameLabel->setVisible(false);
  peacockNameLabel->setVisible(false);
  plumNameLabel->setVisible(false);
}

//Function used by all human players; only applies to member variables for THIS
//player
void ClueMainWindowClass::displayCardsInHand()
{
  //Variable Declarations
  set<CardEnum>hand = gameParticipants.find(thisSuspect)->second.getHand();
                            //THIS player's hand
  set<CardEnum>::iterator currentCardIter = hand.begin();
                            //Iterator used to progress through the hand
  QLabel *cardPtr;          //Pointer to the QLabel on the form to show the card

  for(int i = 1; currentCardIter != hand.end(); i++, currentCardIter++)
  {
    switch(i)
    {
      case 1:
        cardPtr = cardInHand1;
        break;
      case 2:
        cardPtr = cardInHand2;
        break;
      case 3:
        cardPtr = cardInHand3;
        break;
      case 4:
        cardPtr = cardInHand4;
        break;
      case 5:
        cardPtr = cardInHand5;
        break;
      case 6:
        cardPtr = cardInHand6;
        break;
    }

    cardPtr->setPixmap(QPixmap::fromImage(CARD_IMAGES[*currentCardIter]));

    updateDetectiveNotes(*currentCardIter);
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

  for(playerIterator = gameParticipants.begin(); playerIterator !=
      gameParticipants.end(); playerIterator++)
  {
    drawPieceToBoard(playerIterator->first,
        STARTING_LOCATIONS[playerIterator->first]);
    gameBoard->setPixmap(QPixmap::fromImage(inProgressBoardImage));
    switch(playerIterator->first)
    {
      case SCARLET:
        scarletNameLabel->setVisible(true);
        break;
      case MUSTARD:
        mustardNameLabel->setVisible(true);
        break;
      case WHITE:
        whiteNameLabel->setVisible(true);
        break;
      case GREEN:
        greenNameLabel->setVisible(true);
        break;
      case PEACOCK:
        peacockNameLabel->setVisible(true);
        break;
      case PLUM:
        plumNameLabel->setVisible(true);
        break;
      default:
        break;
    }
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
  //the borner
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
  cardInHand1->setVisible(true);
  cardInHand2->setVisible(true);
  cardInHand3->setVisible(true);
  cardInHand4->setVisible(true);
  cardInHand5->setVisible(true);
  cardInHand6->setVisible(true);
  youAreText->setVisible(true);

  //Make game setup interface invisible
  gameWelcomeLabel->setVisible(false);
  gameSetupFrame->setVisible(false);
  startGameButton->setVisible(false);
}

//Function used for all human players
void ClueMainWindowClass::refreshDisplay()
{
  //Variable Declarations
  map<SuspectEnum, PlayerClass>::iterator nextPlayerIter;

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
      if(currentPlayerIter->second.getPlayerLocation().checkCornerRoom() == true)
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
     if((currentPlayerIter->second.getLastAction() == MOVE && currentPlayerIter->
         second.getMovedThisTurnFlag() == false) || currentPlayerIter->second.
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
      inProgressBoardImage).empty()) || (currentPlayerIter->
      second.getPlayerLocation().getTileType(CLUE_BOARD_IMAGE) == ROOM_TILE &&
      ((currentPlayerIter->second.getValidExitDoors(inProgressBoardImage).empty()
      == true && currentPlayerIter->second.getPlayerLocation().checkCornerRoom()
      == false) || currentPlayerIter->second.getMovedThisTurnFlag() == true)))
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
  while(nextPlayerIter->second.getGameOverFlag() == true);

  //Current player is this player.
  if(currentPlayerIter->first == thisSuspect)
  {
    enableAllControls();
    currentTurnText->setText("It is your turn.  " +
        CARD_VALUES[suspectToCard(nextPlayerIter->first)] +
        "'s turn is next.");
  }
  else
  {
    if(nextPlayerIter->first == thisSuspect)
    {
      currentTurnText->setText("It is " +
          CARD_VALUES[suspectToCard(currentPlayerIter->first)]
          + "'s turn.  Your turn is next.");
    }
    else
    {
      currentTurnText->setText("It is " +
          CARD_VALUES[suspectToCard(currentPlayerIter->first)]
          + "'s turn.  " + CARD_VALUES[suspectToCard(nextPlayerIter->first)]
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
  QString textManip;
  QString suspectName = "You";
  QString pluralString = "";
  QString haveTense = "have";
  QString isTense = "are";

  if(currentPlayerIter->first != thisSuspect)
  {
    suspectName = CARD_VALUES[suspectToCard(currentPlayerIter->first)];
    haveTense = "has";
    isTense = "is";
  }

  if(currentPlayerIter->second.getMovesLeft() != 1)
  {
    pluralString = "s";
  }

  if(currentPlayerIter->second.getPlayerLocation().getTileType(CLUE_BOARD_IMAGE)
      == ROOM_TILE && currentPlayerIter->second.getLastAction() == MOVE)
  {
    rollInfoText->setText(suspectName + " just entered the " +
        CARD_VALUES[roomToCard(currentPlayerIter->second.
        getPlayerLocation().getRoom())] + " and " + haveTense +
        " no more moves left.");
  }
  else if(currentPlayerIter->second.getPlayerLocation().getTileType(
      CLUE_BOARD_IMAGE) == ROOM_TILE && currentPlayerIter->second.
      getMovedThisTurnFlag() == false && currentPlayerIter->second.
      getLastAction() != ROLL)
  {
    rollInfoText->setText(suspectName + " " + isTense + " in the " +
        CARD_VALUES[roomToCard(currentPlayerIter->second.getPlayerLocation().
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
  QLabel *textToUpdatePtr;

  switch(updatedCard)
  {
    case SCARLET_CARD:
      textToUpdatePtr = scarletText;
      break;
    case MUSTARD_CARD:
      textToUpdatePtr = mustardText;
      break;
    case WHITE_CARD:
      textToUpdatePtr = whiteText;
      break;
    case GREEN_CARD:
      textToUpdatePtr = greenText;
      break;
    case PEACOCK_CARD:
      textToUpdatePtr = peacockText;
      break;
    case PLUM_CARD:
      textToUpdatePtr = plumText;
      break;
    case KNIFE_CARD:
      textToUpdatePtr = knifeText;
      break;
    case CANDLESTICK_CARD:
      textToUpdatePtr = candlestickText;
      break;
    case REVOLVER_CARD:
      textToUpdatePtr = revolverText;
      break;
    case ROPE_CARD:
      textToUpdatePtr = ropeText;
      break;
    case LEAD_PIPE_CARD:
      textToUpdatePtr = leadPipeText;
      break;
    case WRENCH_CARD:
      textToUpdatePtr = wrenchText;
      break;
    case HALL_CARD:
      textToUpdatePtr = hallText;
      break;
    case LOUNGE_CARD:
      textToUpdatePtr = loungeText;
      break;
    case DINING_ROOM_CARD:
      textToUpdatePtr = diningRoomText;
      break;
    case KITCHEN_CARD:
      textToUpdatePtr = kitchenText;
      break;
    case BALLROOM_CARD:
      textToUpdatePtr = ballroomText;
      break;
    case CONSERVATORY_CARD:
      textToUpdatePtr = conservatoryText;
      break;
    case BILLIARD_ROOM_CARD:
      textToUpdatePtr = billiardRoomText;
      break;
    case LIBRARY_CARD:
      textToUpdatePtr = libraryText;
      break;
    case STUDY_CARD:
      textToUpdatePtr = studyText;
      break;
  }
  textToUpdatePtr->setText("<span style='color:#ff0000;'>" +
      CARD_VALUES[suspectToCard(gameParticipants.find(thisSuspect)->second.
      getDetectiveNotes(updatedCard))] + "</span>");
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

//  endTurnOption->toggle();
//  endTurnOption->setFocus();
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
  SuggestionClass accusation;
  AccusationDialogClass accusationDialog(&accusation, this);

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
  SuggestionClass suggestion;
  SuggestionDialogClass suggestionDialog(&suggestion, this);

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
      gameParticipants.begin();  //Iterator for the player to deal a card to
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

  aiMoveDelay = HOST_AI_DELAY;

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
//    throw(ExceptionClass("Error setting up game", "You must have a minimum of "
//        "3 and a maximum of 6 players to start a game."));
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
        gameParticipants.insert(pair<SuspectEnum, PlayerClass>(randomCharacter,
            PlayerClass(i >= humanPlayersSpin->value(), i == 0,
            STARTING_LOCATIONS[randomCharacter], DifficultyEnum(
            difficultySlider->value()))));

        if(i == 0)
        {
          thisSuspect = randomCharacter;
          youAreText->setText("You are " + CARD_VALUES[randomCharacter] + ".");
        }
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
        takeAiAction(currentPlayerIter->second.handlePrerollAi(
            inProgressBoardImage));
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
  QPoint pos;

  //Variable Declarations
  static SuggestionClass suggestion;
  static map<SuspectEnum, PlayerClass>::iterator
      playerIter = gameParticipants.end();
  static QMessageBox suggestionMessage(this);
  set<CardEnum> playerHand;
  CardEnum revealedCard;
  HandleSuggestionDialogClass playerSuggestionDialog(this);
  QString revealer;
  QString suggester = CARD_VALUES[suspectToCard(currentPlayerIter->first)];

  playerSuggestionDialog.setWindowFlags((playerSuggestionDialog.windowFlags()
      | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint);
  currentPlayerIter->second.setLastAction(SUGGEST);

  if(playerIter == gameParticipants.end())
  {
    playerIter = currentPlayerIter;
  }

  if(playerIter == currentPlayerIter)
  {
    suggestion = playerSuggestion;

    if(currentPlayerIter->first != thisSuspect && gameParticipants.find(
        thisSuspect)->second.getGameOverFlag() == false)
    {
      suggestionMessage.setWindowTitle("Suggestion");
      suggestionMessage.setText(CARD_VALUES[suspectToCard(
          currentPlayerIter->first)] + " suggests that the crime was "
          "committed in the " + CARD_VALUES[roomToCard(suggestion.getRoom())]
          + " by " + CARD_VALUES[suspectToCard(suggestion.getSuspect())] +
          " with the " + CARD_VALUES[weaponToCard(suggestion.getWeapon())]);
      suggestionMessage.exec();
    }
    moveSuggestedSuspect(suggestion.getSuspect());
    if(suggestion.getSuspect() == thisSuspect)
    {
      refreshDisplay();
    }
  }

  playerIter++;
  if(playerIter == gameParticipants.end())
  {
    playerIter = gameParticipants.begin();
  }

  checkSuggestionAcknowledged();

  revealer = CARD_VALUES[suspectToCard(playerIter->first)];

  if(playerIter != currentPlayerIter)
  {
    playerHand = playerIter->second.getHand();

    if(suggestion == playerHand)
    {
      pos = suggestionMessage.pos();

      //THIS player is receiving the suggestion
      if(playerIter->first == thisSuspect && gameParticipants.find(
          thisSuspect)->second.getGameOverFlag() == false)
      {
        playerSuggestionDialog.setupDialogBox(&suggestion, &revealedCard);
        playerSuggestionDialog.exec();

        sendRevealedCard(revealedCard);
      }
      else
      {
        //Ais are revealing a card if THIS player is host
        if(playerIter->second.getAiFlag() == true &&
            gameParticipants.find(thisSuspect)->second.getHostFlag() == true)
        {
          revealedCard = playerIter->second.handleSuggestionAi(suggestion);
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

          if(currentPlayerIter->first != thisSuspect)
          {
            suggestionMessage.setText(revealer + " reveals a card to " +
                suggester + ".");
          }
          else
          {
            suggestionMessage.setText(revealer + " reveals the " +
                CARD_VALUES[revealedCard] + " card to you.");
          }

          suggestionMessage.exec();
        }

        //Add to and update detective notes
        currentPlayerIter->second.addToDetectiveNotes(revealedCard,
            playerIter->first);
        if(currentPlayerIter->first == thisSuspect)
        {
          updateDetectiveNotes(revealedCard);
        }
      }

      playerIter = gameParticipants.end();
      finishMove();
    }
    else if(gameParticipants.find(thisSuspect)->second.getGameOverFlag() ==
        false)
    {
      suggestionMessage.setWindowTitle("No Card Shown");
      //THIS player
      if(playerIter->first == thisSuspect)
      {
        suggestionMessage.setText("You cannot disprove the suggestion.");
      }
      //All other players/ais
      else
      {
        suggestionMessage.setText(CARD_VALUES[int(suspectToCard(
            playerIter->first))] + " cannot disprove the suggestion.");
      }
      //Comment to test Ai
      suggestionMessage.exec();
      handleSuggestion(suggestion);
    }
  }
  else
  {
    finishMove();
    playerIter = gameParticipants.end();
  }
}

//Function used for all players
void ClueMainWindowClass::handleAccusation(const SuggestionClass
    playerAccusation)
{
  //Variable Declarations
  QMessageBox accusationMessage;
  QString playerName = CARD_VALUES[suspectToCard(currentPlayerIter->first)];
  QString haveTense = "has";
  bool activePlayersFlag = false;
  map<SuspectEnum, PlayerClass>::iterator playerIter = gameParticipants.begin();

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
          [suspectToCard(playerAccusation.getSuspect())] + " of committing the "
          "crime in the " + CARD_VALUES[roomToCard(playerAccusation.getRoom())]
          + " with the " + CARD_VALUES[weaponToCard(playerAccusation.
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

    accusationMessage.setWindowTitle("Player Eliminated");
    accusationMessage.setText(playerName + " " + haveTense + " made an "
        "incorrect accusation and can make no further moves this game.");

    if(currentPlayerIter->first != thisSuspect)
    {
      accusationMessage.setText("Game Over");
      accusationMessage.setText(playerName + " accused " + CARD_VALUES
        [suspectToCard(playerAccusation.getSuspect())] + " of committing the "
        "crime in the " + CARD_VALUES[roomToCard(playerAccusation.getRoom())] +
        " with the " + CARD_VALUES[weaponToCard(playerAccusation.getWeapon())] +
        ".<br>" + accusationMessage.text());
    }

    currentPlayerIter->second.setAiFlag(true);
    currentPlayerIter->second.setGameOverFlag(true);

    accusationMessage.exec();

    while(playerIter != gameParticipants.end() && activePlayersFlag == false)
    {
      if(playerIter->second.getGameOverFlag() == false &&
          playerIter->second.getAiFlag() == false)
      {
        activePlayersFlag = true;
      }
      playerIter++;
    }

    if(activePlayersFlag == true)
    {
      checkSuggestionAcknowledged();

      endTurn();
    }
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
  BoardLocationClass oldLocation = currentPlayerIter->second.
      getPlayerLocation();

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
  BoardLocationClass newLocation;

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

  finishMove();
  refreshDisplay();
}

//Function used for all players
void ClueMainWindowClass::moveSuggestedSuspect(SuspectEnum suggestedSuspect)
{
  map<SuspectEnum, PlayerClass>::iterator playerIter = gameParticipants.
      find(suggestedSuspect);
  BoardLocationClass newLocation;

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

  currentPlayerIter++;
  if(currentPlayerIter == gameParticipants.end())
  {
    currentPlayerIter = gameParticipants.begin();
  }

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
              inProgressBoardImage));
        }

        //Move while there are moves left and the player hasn't reached the room
        while(currentPlayerIter->second.getMovesLeft() > 0 &&
            aiPlayerIter == currentPlayerIter && currentPlayerIter->second.
            getPlayerLocation().getTileType(CLUE_BOARD_IMAGE) == UNOCCUPIED_TILE)
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
        handleAccusation(currentPlayerIter->second.makeAiAccusation());
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

void ClueMainWindowClass::sendRemoteTurnInfo(ActionEnum playerAction,
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
        
        // Open server socket
        ServerSocket server(30000);
        ServerSocket serverSock;
  
        string clientReply;
        PackageClass package;
        string gameParticipantsPkgStr, caseFilePkgStr, suspectEnumPkgStr;
        
        // Wrap up package to pass to other networked game(s)
        gameParticipantsPkgStr = package.wrapSetupPkg(gameParticipants);
        caseFilePkgStr = package.wrapCaseFilePkg(caseFile);
        suspectEnumPkgStr = package.wrapSuspectEnum(thisSuspect);
        
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
        
        cout << "Number of players is: " << (int) gameParticipants.size() << endl;
        
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
      PackageClass package;
      string packageString;
      string gameParticipantsPkgStr, caseFilePkgStr, suspectEnumPkgStr;
      
      // Make sure this array is clear
      gameParticipants.clear();
      
      // Trying to connect to server
      cout << "Attempting to connect to server" << endl;
      // Right now on localhost, replace with IP
      ClientSocket cliSock("localhost", 30000);
      
      // packageString = package.wrapPackage();
      
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
      
      cout << "This suspect is " << thisSuspect << endl;
      
      cliSock << "All done, send me #4!";
      
//       TEST BLOCK
//       gameParticipantsPkgStr = package.wrapSetupPkg(gameParticipants);
//       caseFilePkgStr = package.wrapCaseFilePkg(caseFile);
//       suspectEnumPkgStr = package.wrapSuspectEnum(thisSuspect);
//       cout << gameParticipantsPkgStr << endl;
//       cout << caseFilePkgStr << endl;
//       cout << suspectEnumPkgStr << endl;

      currentPlayerIter = gameParticipants.begin();
    }

    //Display gameplay interface
    displayCardsInHand();
    drawStartingPieces();
    displayGameInterface();
    displayDefaultOptions();

    //Start the first turn
    startPlayerTurn();
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
    difficultySlider->setVisible(true);
    aiDifficultyLabel->setVisible(true);

    humanPlayersSpin->setEnabled(true);
    computerPlayersSpin->setEnabled(true);
  }
  else
  {
    hostIPEdit->setVisible(true);
    hostIPLabel->setVisible(true);
    computerPlayersSpin->setVisible(false);
    computerPlayersLabel->setVisible(false);
    humanPlayersSpin->setVisible(false);
    humanPlayersLabel->setVisible(false);
    difficultySlider->setVisible(false);
    aiDifficultyLabel->setVisible(false);
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
  difficultySlider->setVisible(true);

  computerPlayersSpin->setEnabled(true);
  humanPlayersSpin->setEnabled(false);
  humanPlayersSpin->setValue(1);
  aiDifficultyLabel->setVisible(true);
}

void ClueMainWindowClass::updateDifficultyText(int sliderPosition)
{
  switch(DifficultyEnum(sliderPosition))
  {
    case VERY_EASY:
      aiDifficultyText->setText("Very Easy");
      break;
    case EASY:
      aiDifficultyText->setText("Easy");
      break;
    case MEDIUM:
      aiDifficultyText->setText("Medium");
      break;
    case HARD:
      aiDifficultyText->setText("Hard");
      break;
    case EXPERT:
      aiDifficultyText->setText("Expert");
      break;
  }
}
