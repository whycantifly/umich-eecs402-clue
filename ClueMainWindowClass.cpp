#include <QtGui>
#include <QTest>
#include <map>
#include <string>
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

  //Setup the board
  setupNewBoard();
}

//Initial board setup functions

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

void ClueMainWindowClass::setupNewBoard()
{
  gameOver = false;

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

void ClueMainWindowClass::clearVisitedTiles()
{
  //Variable Declarations
  set<BoardLocationClass> visitedLocations =                //Locations visited
      currentPlayerIter->second.getLocationsThisTurn();            //this turn
  set<BoardLocationClass>::iterator visitedLocationIter =   //Iterator to the
      visitedLocations.begin();                             //locations visited
  QPoint tileTopLeftPixel;  //Location of top left pixel of *visitedLocationIter
  BoardLocationClass currentPlayerLoc = currentPlayerIter->second.
      getPlayerLocation();                      //Current location of the player

  //Reset the tiles in the list of locations visited to the original color,
  //with the exception of the player's current location
  while(visitedLocationIter != visitedLocations.end())
  {
    if(*visitedLocationIter != currentPlayerLoc)
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

//Applies to all players
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

//Applies only if THIS player is the host
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

//Only applies if THIS player is the host
void ClueMainWindowClass::setupGame()
{
  //Variable Declarations
  set<SuspectEnum> availableCharacters;
  SuspectEnum randomCharacter;
  DeckClass cardDeck;
  bool suspectAvailableFlag;

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

void ClueMainWindowClass::refreshDisplay()
{
  map<SuspectEnum, PlayerClass>::iterator nextPlayerIter;
  map<SuspectEnum, PlayerClass>::const_iterator thisPlayerIter = gameParticipants.
      find(thisSuspect);

  if(currentPlayerIter->first == thisSuspect)
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

  //Check if the current player is this player
  nextPlayerIter = currentPlayerIter;
  nextPlayerIter++;
  if(nextPlayerIter == gameParticipants.end())
  {
    nextPlayerIter = gameParticipants.begin();
  }

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

void ClueMainWindowClass::updateDetectiveNotes(CardEnum updatedCard)
{
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

//Handles all player events
void ClueMainWindowClass::startPlayerTurn()
{
  if(gameOver == true)
  {
    //endGame();
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
/*****************************************************************************/
      //Do network stuff
/*****************************************************************************/
    }
  }
}

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
      QTest::qWait(AI_DELAY);
      takeAiAction(SUGGEST);
      break;
    case MOVE:
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
        QTest::qWait(AI_DELAY);
        try
        {
          moveCurrentPlayer(currentPlayerIter->second.getAiMove(
              inProgressBoardImage, targetDoorLocation));
        }
        catch(ExceptionClass noValidMovesLeft)
        {
          currentPlayerIter->second.setMovesLeft(0);
          finishMove();
        }
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
      break;
    case SUGGEST:
      handleSuggestion(currentPlayerIter->second.makeAiSuggestion());
      takeAiAction(ACCUSE);
      break;
    case ACCUSE:
      try
      {
        currentPlayerIter->second.makeAiAccusation();
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

//Only applies to THIS player and Ai's if THIS player is the host
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

void ClueMainWindowClass::updateRollInfoText()
{
  QString textManip;
  QString suspectName = "You";
  QString pluralString = "";
  QString haveTense = "have";
  QString isTense = "is";

  if(currentPlayerIter->first != thisSuspect)
  {
    suspectName = CARD_VALUES[suspectToCard(currentPlayerIter->first)];
    haveTense = "has";
    isTense = "are";
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

void ClueMainWindowClass::displayCornerRoomOptions()
{
  displayRoomOptions();

  useSecretPassageOption->setVisible(true);
}

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

void ClueMainWindowClass::disableAllControls()
{
  actionOptionsFrame->setEnabled(false);
  okayButton->setEnabled(false);
}

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

void ClueMainWindowClass::enableAllControls()
{
  actionOptionsFrame->setEnabled(true);
  okayButton->setEnabled(true);
}

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

void ClueMainWindowClass::makePlayerAccusation()
{
  //Variable Declarations
  SuggestionClass accusation;
  QMessageBox endGame;
  AccusationDialogClass accusationDialog(&accusation, this);

  if(accusationDialog.exec() == QDialog::Accepted)
  {
    endGame.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    if(accusation != caseFile)
    {
      endGame.setWindowTitle("Game Over");
      endGame.setText("You have made an incorrect accusation and can make "
          "no further moves this game. Would you like to play again?");
      currentPlayerIter->second.setAiFlag(true);
    }
    else
    {
      endGame.setWindowTitle("Congratulations!");
      endGame.setText("You have made a correct accusation and have won the "
          "game!  Would you like to play again?");
    }
    if(endGame.exec() == QMessageBox::Yes)
    {
      setupNewBoard();
    }
    else
    {
      close();
    }
  }
}

//Only applies to THIS player
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

//Handles all player's actions
void ClueMainWindowClass::handleSuggestion(const SuggestionClass
    &playerSuggestion)
{

  QPoint pos;

  //Variable Declarations
  static SuggestionClass suggestion;
  static map<SuspectEnum, PlayerClass>::iterator
      playerIter = gameParticipants.end();
  set<CardEnum> playerHand;
  CardEnum revealedCard;
  QMessageBox suggestionMessage(this);
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

    if(currentPlayerIter->first != thisSuspect)
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
  }

  playerIter++;
  if(playerIter == gameParticipants.end())
  {
    playerIter = gameParticipants.begin();
  }

  revealer = CARD_VALUES[suspectToCard(playerIter->first)];

  if(playerIter != currentPlayerIter)
  {
    playerHand = playerIter->second.getHand();

    if(suggestion == playerHand)
    {
      pos = suggestionMessage.pos();
      //THIS player is receiving the suggestion
      if(playerIter->first == thisSuspect)
      {
        playerSuggestionDialog.setupDialogBox(&suggestion, &revealedCard);
        playerSuggestionDialog.exec();
      }
      else
      {
        //Ai's are receiving the suggestion if THIS player is the host
        if(playerIter->second.getAiFlag() == true &&
            gameParticipants.find(thisSuspect)->second.getHostFlag() == true)
        {
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
          //Dummy AI code
          revealedCard = playerIter->second.handleSuggestionAi(suggestion);
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
        }
        //Other players or Ai's are receivingthe suggestion if
        //THIS player isn't the host
        else if(playerIter->first != thisSuspect)
        {
  /*****************************************************************************/
          //Wait to get card revealed from the house
          //revealedCard =
  /*****************************************************************************/
        }
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
        if(currentPlayerIter->first == thisSuspect || (gameParticipants.find(
            thisSuspect)->second.getHostFlag() == true && currentPlayerIter->
            second.getAiFlag() == true))
        {
          currentPlayerIter->second.addToDetectiveNotes(revealedCard,
              playerIter->first);
          if(currentPlayerIter->first == thisSuspect)
          {
            updateDetectiveNotes(revealedCard);
          }
        }
      }

      playerIter = gameParticipants.end();
      finishMove();
    }
    else
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

void ClueMainWindowClass::moveSuggestedSuspect(SuspectEnum suggestedSuspect)
{
  map<SuspectEnum, PlayerClass>::iterator playerIter = currentPlayerIter;
  BoardLocationClass newLocation;

  playerIter++;
  if(playerIter == gameParticipants.end())
  {
    playerIter = gameParticipants.begin();
  }

  while(playerIter != currentPlayerIter)
  {
    if(playerIter->first == suggestedSuspect)
    {
      if(playerIter->second.getPlayerLocation().getTileType(CLUE_BOARD_IMAGE) !=
          ROOM_TILE || playerIter->second.getPlayerLocation().getRoom() !=
          currentPlayerIter->second.getPlayerLocation().getRoom())
      {
        newLocation = currentPlayerIter->second.getPlayerLocation().
            getEmptyRoomTile(inProgressBoardImage);
        drawMove(playerIter->first, playerIter->second.getPlayerLocation(),
            newLocation);
        playerIter->second.setPlayerLocation(newLocation);
      }
      playerIter = currentPlayerIter;
    }
    else
    {
      playerIter++;
      if(playerIter == gameParticipants.end())
      {
        playerIter = gameParticipants.begin();
      }
    }
  }
}

void ClueMainWindowClass::moveCurrentPlayerOutDoor(BoardLocationClass doorLoc)
{
  drawMove(currentPlayerIter->first, currentPlayerIter->second.
      getPlayerLocation(), doorLoc);
  currentPlayerIter->second.setPlayerLocation(doorLoc);

  finishMove();
  refreshDisplay();
}

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

void ClueMainWindowClass::finishMove()
{
  currentPlayerIter->second.decrementMovesLeft();
  if(currentPlayerIter->first == thisSuspect && currentPlayerIter->second.
      getMovesLeft() == 0)
  {
    refreshDisplay();
  }
  else
  {
    updateRollInfoText();
  }
}

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
    else
    {
      //
    }
    finishMove();
  }
  catch(ExceptionClass newException)
  {
    errorMessageLabel->setText("<span style=' font-weight:600; color:#ff0000;'>"
        + newException.getErrorMessage() + "</span>");
  }
}

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

void ClueMainWindowClass::displayExceptionMessageBox(ExceptionClass
    exceptionText) const
{
  QMessageBox errorMessageBox;

  errorMessageBox.setText(exceptionText.getErrorMessage());
  errorMessageBox.setWindowTitle(exceptionText.getErrorTitle());
  errorMessageBox.exec();
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

RoomEnum ClueMainWindowClass::getCurrentPlayerRoom()
{
  return currentPlayerIter->second.getPlayerLocation().getRoom();
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
