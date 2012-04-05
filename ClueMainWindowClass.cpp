#include <QtGui>
#include <QTest>
#include <string>
#include <set>
#include <stdlib.h>

#include "ClueMainWindowClass.h"
#include "constants.h"
#include "AccusationDialogClass.h"
#include "BoardLocationClass.h"
#include "HandleSuggestionDialogClass.h"
#include "CaseFileClass.h"
#include "ExceptionClass.h"
#include "SuggestionDialogClass.h"
#include "SuggestionClass.h"
#include "roomToCard.h"
#include "suspectToCard.h"
#include "weaponToCard.h"

using namespace std;

ClueMainWindowClass::ClueMainWindowClass(QWidget *parent)
{
  //Set up the GUI
  setupUi(this);

  cardsEnabledFlag = false;

  connect(networkedPlayOption, SIGNAL(clicked()), this,
      SLOT(setNetworkOptVis()));
  connect(localPlayOption, SIGNAL(clicked()), this, SLOT(setLocalOptVis()));
  connect(gameHostCheck, SIGNAL(clicked()), this, SLOT(setNetworkOptVis()));
  connect(doorNumberSpin, SIGNAL(valueChanged(int)), this,
      SLOT(toggleLeaveRoomOpt(int)));
  connect(okayButton, SIGNAL(clicked()), this, SLOT(submitMove()));
  connect(startGameButton, SIGNAL(clicked()), this, SLOT(startGame()));

  setupNewBoard();
}

void ClueMainWindowClass::displayCardsInHand()
{
  //Variable Declarations
  set<CardEnum>hand = thisPlayerPtr->getHand();
  set<CardEnum>::iterator currentCardIter = hand.begin();
  QLabel *cardPtr;

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

    cardPtr->setPixmap(QPixmap::fromImage(
        CARD_IMAGES[*currentCardIter]));
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

void ClueMainWindowClass::drawPieceToBoard(const list<PlayerClass>::iterator playerIter,
    const BoardLocationClass &tile)
{
  //Variable Declarations
  QPoint boardLocTopLeft = tile.getTopLeftPixel();

  for(int i = 0; i < TILE_WIDTH; i++)
  {
    for(int j = 0; j < TILE_HEIGHT; j++)
    {
      if(CHAR_PIECE_IMAGES[playerIter->getCharacter()].pixel(i, j) != TRANSPARENT_RGB)
      {
        inProgressBoardImage.setPixel(boardLocTopLeft + QPoint(i, j),
            CHAR_PIECE_IMAGES[playerIter->getCharacter()].pixel(i, j));
      }
    }
  }
}

void ClueMainWindowClass::clearVisitedTiles()
{
  //Variable Declarations
  set<BoardLocationClass> visitedLocations =
      currentPlayerIter->getLocationsThisTurn();
  set<BoardLocationClass>::iterator visitedLocationIter =
      visitedLocations.begin();
  QPoint tileTopLeftPixel;
  BoardLocationClass currentPlayerLoc = currentPlayerIter->getPlayerLocation();

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
  currentPlayerIter->resetLocationsThisTurn();
  gameBoard->setPixmap(QPixmap::fromImage(inProgressBoardImage));
}

void ClueMainWindowClass::clearPiece(const list<PlayerClass>::iterator
    playerIter, const BoardLocationClass &tile)
{
  //Variable Declarations
  QPoint boardLocTopLeft = tile.getTopLeftPixel();
  int drawOffset[NUMBER_OF_DIRECTIONS] = {1, 1, 1, 1};
  TileTypeEnum tileType;

  if(tile.getTileType(CLUE_BOARD_IMAGE) == ROOM_TILE)
  {
    for(DirectionEnum direction = UP; direction <= RIGHT; direction =
        DirectionEnum(int(direction) + 1))
    {
      tileType = tile.getTileInDir(direction).getTileType(inProgressBoardImage);
      if(tileType != OCCUPIED_TILE)
      {
        drawOffset[direction] = 0;
      }
    }
  }

  for(int i = drawOffset[LEFT]; i < TILE_WIDTH - drawOffset[RIGHT]; i++)
  {
    for(int j = drawOffset[UP]; j < TILE_HEIGHT - drawOffset[DOWN]; j++)
    {
      if(tile.getTileType(CLUE_BOARD_IMAGE) != ROOM_TILE &&
          playerIter == currentPlayerIter)
      {
        inProgressBoardImage.setPixel(boardLocTopLeft + QPoint(i, j),
            VISITED_TILE_RGB[playerIter->getCharacter()]);
      }
      else
      {
        inProgressBoardImage.setPixel(boardLocTopLeft + QPoint(i, j),
            CLUE_BOARD_IMAGE.pixel(boardLocTopLeft + QPoint(i, j)));
      }
    }
  }

  QColor a = inProgressBoardImage.pixel(tile.getMiddlePixel());
}

void ClueMainWindowClass::drawMove(const BoardLocationClass &oldLocation,
    const BoardLocationClass &newLocation, const list<PlayerClass>::iterator &playerIter)
{
  clearPiece(playerIter, oldLocation);
  drawPieceToBoard(playerIter, newLocation);

  playerIter->addToLocationsThisTurn(newLocation);

  gameBoard->setPixmap(QPixmap::fromImage(inProgressBoardImage));
}

void ClueMainWindowClass::dealCards()
{
  //Variable Declarations
  list<PlayerClass>::iterator playerBeingDealtTo = gameParticipants.begin();

  while(cardDeck.getDeckSize() > 0)
  {
    playerBeingDealtTo->addCardToHand(cardDeck.drawRandomCard());

    playerBeingDealtTo++;
    if(playerBeingDealtTo == gameParticipants.end())
    {
      playerBeingDealtTo = gameParticipants.begin();
    }
  }
}

void ClueMainWindowClass::setupGame()
{
  //Variable Declarations
  list<SuspectEnum> availableCharacters;
  list<SuspectEnum>::iterator charIterator;
  list<PlayerClass>::iterator participantIterator;
  int randomCharacterNumber;
  QString playerName;
  int defaultNameCounter = 1;

  //Initialize availableCharacters
  for(SuspectEnum i = SuspectEnum(0); i < NUMBER_OF_SUSPECTS;
      i = SuspectEnum(int(i) + 1))
  {
    availableCharacters.push_front(i);
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
    randomCharacterNumber = rand() % availableCharacters.size();
    for(charIterator = availableCharacters.begin();
        randomCharacterNumber > 0; charIterator++)
    {
      randomCharacterNumber--;
    }
    //Get to the right place in gameParticipants
    for(participantIterator = gameParticipants.begin();
        (*charIterator > participantIterator->getCharacter()) &&
        (participantIterator != gameParticipants.end()); participantIterator++)
    {
    }

    //Add the player to gameParticipants.  Human participants (and the host) are
    //added first.
    if(i == 0)
    {
      playerName = playerNameText->text();
    }
    else if(i < humanPlayersSpin->value())
    {
      //Networked player names
    }
    else
    {
      playerName.setNum(defaultNameCounter);
      playerName.prepend("Player ");
      defaultNameCounter++;
    }
    gameParticipants.insert(participantIterator, PlayerClass(playerName,
        *charIterator, i >= humanPlayersSpin->value(), i == 0));

    //Set thisPlayerChar
    if(i == 0)
    {
      participantIterator--;
      thisPlayerPtr = &*participantIterator;
      youAreText->setText("You are " + CARD_VALUES[int(suspectToCard(
          thisPlayerPtr->getCharacter()))] + ".");
    }

    //Erase the character in availableCharacters
    availableCharacters.erase(charIterator);
  }

  currentPlayerIter = gameParticipants.begin();

  //Make the case file and deal out the remaining cards
  cardDeck.resetDeck();
  caseFile.createCaseFile(cardDeck);
  dealCards();

  displayCardsInHand();

  drawStartingPieces();
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
  list<PlayerClass>::iterator nextPlayerIter;

  if(thisPlayerPtr->getPlayerLocation().getTileType(CLUE_BOARD_IMAGE) == ROOM_TILE)
  {
    //Player is in a corner room
    if(thisPlayerPtr->getPlayerLocation().checkCornerRoom() == true)
    {
      displayCornerRoomOptions();
    }
    else
    {
      displayRoomOptions();
    }
  }
  else
  {
    displayDefaultOptions();
  }

  if(thisPlayerPtr->getMovedSinceLastTurnFlag() == true ||
      thisPlayerPtr->getEnteredRoomThisMoveFlag() == true)
  {
    makeSuggestionOption->setEnabled(true);
  }
  else
  {
    makeSuggestionOption->setEnabled(false);
  }

  //Check if the current player is this player
  nextPlayerIter = currentPlayerIter;
  nextPlayerIter++;
  if(nextPlayerIter == gameParticipants.end())
  {
    nextPlayerIter = gameParticipants.begin();
  }

  //Current player is this player.
  if(&*currentPlayerIter == &*thisPlayerPtr)
  {
    enableAllControls();
    currentTurnText->setText("It is your turn.  " +
        CARD_VALUES[int(suspectToCard(nextPlayerIter->getCharacter()))] +
        "'s (" + nextPlayerIter->getName() + ") turn is next.");
  }
  else
  {
    if(&*nextPlayerIter == &*thisPlayerPtr)
    {
      currentTurnText->setText("It is " +
          CARD_VALUES[int(suspectToCard(currentPlayerIter->getCharacter()))]
          + "'s (" + currentPlayerIter->getName() + ") turn.  "
          "Your turn is next.");
    }
    else
    {
      currentTurnText->setText("It is " +
          CARD_VALUES[int(suspectToCard(currentPlayerIter->getCharacter()))]
          + "'s (" + currentPlayerIter->getName() + ") turn.  " +
          CARD_VALUES[int(suspectToCard(nextPlayerIter->getCharacter()))] +
          "'s (" + nextPlayerIter->getName() + ") turn is next.");
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
      CARD_VALUES[int(suspectToCard(currentPlayerIter->getDetectiveNotes(
      updatedCard)))] + "</span>");
}

//Handles all player events
void ClueMainWindowClass::startPlayerTurn()
{
  SuggestionClass aiSuggestion;
  AiActionEnum aiAction;
  queue<DirectionEnum> aiMoveList;
  int aiExitDoor;

  refreshDisplay();

  if(gameOver == true)
  {
    //endGame();
  }
  else
  {
    //Current player is this player or this player is the host and current
    //player is an ai.
    if(&*currentPlayerIter == &*thisPlayerPtr ||
        (currentPlayerIter->getAiFlag() == true &&
        thisPlayerPtr->getHostFlag() == true))
    {
      //THIS player is not in any special situation (corner room or can make
      //suggestion).
      if((currentPlayerIter->getPlayerLocation().getTileType(CLUE_BOARD_IMAGE)
          != ROOM_TILE || currentPlayerIter->getPlayerLocation().
          getTileType(CLUE_BOARD_IMAGE) == false ||
          (currentPlayerIter->getPlayerLocation().checkCornerRoom() == false
          && currentPlayerIter->getMovedSinceLastTurnFlag() == false)) &&
          &*currentPlayerIter == &*thisPlayerPtr)
      {
        continuePlayerTurn();
      }
      //Ai's
      else if(currentPlayerIter->getAiFlag() == true)
      {
        //Ai can either make suggestions or is in a corner room
        if(currentPlayerIter->getPlayerLocation().getTileType(CLUE_BOARD_IMAGE)
            == ROOM_TILE && currentPlayerIter->getPlayerLocation().
            getTileType(CLUE_BOARD_IMAGE) == true &&
            (currentPlayerIter->getPlayerLocation().checkCornerRoom() == true
            || currentPlayerIter->getMovedSinceLastTurnFlag() == true))
        {
          aiAction = currentPlayerIter->handlePrerollAi(
              inProgressBoardImage, aiSuggestion);
          takeAiAction(aiAction, aiSuggestion, aiMoveList, aiExitDoor);
          currentPlayerIter->setMovedSinceLastTurnFlag(false);
        }

        continuePlayerTurn();
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

void ClueMainWindowClass::takeAiAction(AiActionEnum action,
    SuggestionClass &aiSuggestion, queue<DirectionEnum> &aiMoves,
    int aiExitDoorNumber)
{
  int j = currentPlayerIter->getDieRoll();
  BoardLocationClass lastLocation = currentPlayerIter->getPlayerLocation();
  list<PlayerClass>::const_iterator aiPlayerIter= currentPlayerIter;

  switch(action)
  {
    case ROLL:
      continuePlayerTurn();
      break;
    case USE_SECRET_PASSAGE:
      moveCurrentPlayerToSecretPassage();
      takeAiAction(SUGGEST, aiSuggestion, aiMoves, aiExitDoorNumber);
      break;
    case MOVE:
      if(currentPlayerIter->getPlayerLocation().getTileType(CLUE_BOARD_IMAGE) == ROOM_TILE)
      {
        moveCurrentPlayerOutDoor(aiExitDoorNumber);
      }

      while(aiMoves.size() > 0)
      {
        QTest::qWait(AI_DELAY);
        moveCurrentPlayer(aiMoves.front());
        lastLocation = currentPlayerIter->getPlayerLocation();
        aiMoves.pop();
      }

      if(aiPlayerIter == currentPlayerIter)
      {
        currentPlayerIter->setMovesLeft(0);
        finishMove();
      }

      if(currentPlayerIter->getPlayerLocation().getTileType(CLUE_BOARD_IMAGE) == ROOM_TILE)
      {
        currentPlayerIter->setEnteredRoomThisMoveFlag(false);
        currentPlayerIter->setMovesLeft(0);
        takeAiAction(SUGGEST, aiSuggestion, aiMoves, aiExitDoorNumber);
      }
      break;
    case SUGGEST:
      aiSuggestion = currentPlayerIter->makeSuggestionAi();
      handleSuggestion(aiSuggestion);
      break;
    case ACCUSE:
      break;
    case END_TURN:
      break;
  }
}

//Only applies to THIS player and Ai's if THIS player is the host
void ClueMainWindowClass::continuePlayerTurn()
{
  SuggestionClass aiSuggestion;
  AiActionEnum aiAction;
  queue<DirectionEnum> aiMoveList;
  int aiExitDoor;

  //Roll die and display die roll
  if(currentPlayerIter->getDieRoll() == 0)
  {
    currentPlayerIter->rollDie();
  }

  //THIS player
  if(&*currentPlayerIter == &*thisPlayerPtr)
  {
    refreshDisplay();
  }
  //Ai's
  else
  {
    aiAction = currentPlayerIter->handleAfterRollAi(inProgressBoardImage,
        aiSuggestion, aiMoveList, aiExitDoor);
    takeAiAction(aiAction, aiSuggestion, aiMoveList, aiExitDoor);
  }

  updateRollInfoText();
}

void ClueMainWindowClass::updateRollInfoText()
{
  QString textManip;
  QString suspectName =
      CARD_VALUES[int(suspectToCard(currentPlayerIter->getCharacter()))];

  switch(currentPlayerIter->getEnteredRoomThisMoveFlag())
  {
    case true:
      rollInfoText->setText("You just entered the " +
          CARD_VALUES[int(roomToCard(currentPlayerIter->getPlayerLocation().
          getRoom()))] + " and have no moves left.");
      break;
    case false:
      textManip.setNum(currentPlayerIter->getDieRoll());
      if(&*currentPlayerIter == &*thisPlayerPtr)
      {
        rollInfoText->setText("You rolled a " + textManip + ".  You have ");
        textManip.setNum(currentPlayerIter->getMovesLeft());
        rollInfoText->setText(rollInfoText->text() + textManip + " moves left.");
      }
      else
      {
        rollInfoText->setText(suspectName + " rolled a " + textManip + ".  " +
            suspectName);
        if(currentPlayerIter->getMovesLeft() == 1)
        {
          rollInfoText->setText(rollInfoText->text() + " have ");
        }
        else
        {
          rollInfoText->setText(rollInfoText->text() + " has ");
        }
        textManip.setNum(currentPlayerIter->getMovesLeft());

        rollInfoText->setText(rollInfoText->text() + textManip + " moves left.");
      }
      break;
  }
}

void ClueMainWindowClass::displayCornerRoomOptions()
{
  displayRoomOptions();

  if(currentPlayerIter->getEnteredRoomThisMoveFlag() == false)
  {
    useSecretPassageOption->setVisible(true);
  }
}

void ClueMainWindowClass::displayRoomOptions()
{
  useSecretPassageOption->setVisible(false);
  makeSuggestionOption->setEnabled(true);

  if(currentPlayerIter->getEnteredRoomThisMoveFlag() == true)
  {
    rollDieOption->setVisible(false);
    leaveRoomOption->setVisible(false);
    doorNumberSpin->setVisible(false);
    moveDownOption->setVisible(true);
    moveUpOption->setVisible(true);
    moveLeftOption->setVisible(true);
    moveRightOption->setVisible(true);
    moveDownOption->setEnabled(false);
    moveUpOption->setEnabled(false);
    moveLeftOption->setEnabled(false);
    moveRightOption->setEnabled(false);

    makeSuggestionOption->setFocus();
    makeSuggestionOption->toggle();
  }
  else
  {
    if(currentPlayerIter->getDieRoll() == 0)
    {
      rollDieOption->setVisible(true);
      leaveRoomOption->setVisible(false);
      doorNumberSpin->setVisible(false);
      moveDownOption->setVisible(false);
      moveUpOption->setVisible(false);
      moveLeftOption->setVisible(false);
      moveRightOption->setVisible(false);

      rollDieOption->setFocus();
      rollDieOption->toggle();
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

      leaveRoomOption->setFocus();
      leaveRoomOption->toggle();
    }
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
  moveDownOption->setEnabled(true);
  moveUpOption->setEnabled(true);
  moveLeftOption->setEnabled(true);
  moveRightOption->setEnabled(true);
  makeSuggestionOption->setEnabled(false);

  moveUpOption->setFocus();
  moveUpOption->toggle();
}

void ClueMainWindowClass::disableAllControls()
{
  actionOptionsFrame->setEnabled(false);
  okayButton->setEnabled(false);
}

void ClueMainWindowClass::enableAllControls()
{
  actionOptionsFrame->setEnabled(true);
  okayButton->setEnabled(true);
}

void ClueMainWindowClass::drawStartingPieces()
{
  QPainter verticalTextPainter(this);
  QPaintEvent *test;
  test = new QPaintEvent(QRect(0, 0, 100, 100));
  paintEvent(test);

  for(list<PlayerClass>::iterator playerIterator = gameParticipants.begin();
      playerIterator != gameParticipants.end(); playerIterator++)
  {
    drawPieceToBoard(playerIterator,
        STARTING_LOCATIONS[int(playerIterator->getCharacter())]);
    gameBoard->setPixmap(QPixmap::fromImage(inProgressBoardImage));
    switch(playerIterator->getCharacter())
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
      currentPlayerIter->setAiFlag(true);
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

  currentPlayerIter->setEnteredRoomThisMoveFlag(false);


  if(suggestionDialog.exec() == QDialog::Accepted)
  {
    handleSuggestion(suggestion);
  }

  refreshDisplay();
}

//Handles all player's actions
void ClueMainWindowClass::handleSuggestion(SuggestionClass &playerSuggestion)
{
  static SuggestionClass suggestion;
  static list<PlayerClass>::iterator playerIter = gameParticipants.end();
  set<CardEnum> playerHand;
  CardEnum revealedCard;
  QMessageBox suggestionMessage(this);
  HandleSuggestionDialogClass playerSuggestionDialog(this);
  PlayerClass a;
  PlayerClass b;

  playerSuggestionDialog.setWindowFlags((playerSuggestionDialog.windowFlags()
      | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint);

  if(playerIter == gameParticipants.end())
  {
    playerIter = currentPlayerIter;
  }

  if(playerIter == currentPlayerIter)
  {
    suggestion = playerSuggestion;

    if(&*currentPlayerIter != &*thisPlayerPtr)
    {
      a = *currentPlayerIter;
      b = *thisPlayerPtr;

      suggestionMessage.setWindowTitle("Suggestion");
      suggestionMessage.setText(CARD_VALUES[int(suspectToCard(
          currentPlayerIter->getCharacter()))] + " suggests that the crime was "
          "committed in the " + CARD_VALUES[int(roomToCard(suggestion.getRoom()))]
          + " by " + CARD_VALUES[int(suspectToCard(suggestion.getSuspect()))] +
          " with the " + CARD_VALUES[int(weaponToCard(suggestion.getWeapon()))]);
      suggestionMessage.exec();
    }
    moveSuggestedSuspect(suggestion.getSuspect());
  }

  playerIter++;
  if(playerIter == gameParticipants.end())
  {
    playerIter = gameParticipants.begin();
  }

  if(playerIter != currentPlayerIter)
  {
    playerHand = playerIter->getHand();

    if(suggestion == playerHand)
    {
      //THIS player is receiving the suggestion
      if(&*playerIter == &*thisPlayerPtr)
      {
        playerSuggestionDialog.setupDialogBox(&suggestion, &revealedCard);
        playerSuggestionDialog.exec();
      }
      else
      {
        //Ai's if THIS player is the host
        if(playerIter->getAiFlag() == true &&
            thisPlayerPtr->getHostFlag() == true)
        {
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
        //Dummy AI code
        revealedCard = playerIter->handleSuggestionAi(suggestion);
  ///////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////
        }
        //Other players or ais if THIS player isn't the host
        else if(&*playerIter != &*thisPlayerPtr)
        {
  /*****************************************************************************/
          //Wait to get card revealed from the house
          //revealedCard =
  /*****************************************************************************/
        }

        if(&*currentPlayerIter == &*thisPlayerPtr)
        {
          suggestionMessage.setWindowTitle("Card Revealed");
          suggestionMessage.setText(CARD_VALUES[int(suspectToCard(playerIter->
              getCharacter()))] + " reveals the " +
              CARD_VALUES[int(revealedCard)] + " card to you.");
          suggestionMessage.exec();

          currentPlayerIter->addToDetectiveNotes(revealedCard,
              playerIter->getCharacter());
          updateDetectiveNotes(revealedCard);
        }
        else
        {
          suggestionMessage.setWindowTitle("Card Revealed");
          suggestionMessage.setText(CARD_VALUES[int(suspectToCard(playerIter->
              getCharacter()))] + " reveals a card to " + CARD_VALUES[int(
              suspectToCard(currentPlayerIter->getCharacter()))]);
          suggestionMessage.exec();
        }
      }

      playerIter = gameParticipants.end();
      finishMove();
    }
    else
    {
      //THIS player
      if(&*playerIter == &*thisPlayerPtr)
      {
        suggestionMessage.setText("You cannot disprove the suggestion.");
      }
      //All other players/ais
      else
      {
        suggestionMessage.setText(CARD_VALUES[int(suspectToCard(playerIter->
            getCharacter()))] + " cannot disprove the suggestion.");
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
  list<PlayerClass>::iterator playerIter = currentPlayerIter;
  BoardLocationClass newLocation;

  playerIter++;
  if(playerIter == gameParticipants.end())
  {
    playerIter = gameParticipants.begin();
  }

  while(playerIter != currentPlayerIter)
  {
    if(playerIter->getCharacter() == suggestedSuspect)
    {
      if(playerIter->getPlayerLocation().getTileType(inProgressBoardImage) == ROOM_TILE)
      {
        if(playerIter->getPlayerLocation().getRoom() ==
            currentPlayerIter->getPlayerLocation().getRoom())
        {
          newLocation = currentPlayerIter->getPlayerLocation().
              getEmptyRoomTile(inProgressBoardImage);
          drawMove(playerIter->getPlayerLocation(), newLocation, playerIter);
          playerIter->setPlayerLocation(newLocation);
          playerIter->setMovedSinceLastTurnFlag(true);
        }
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

void ClueMainWindowClass::moveCurrentPlayerOutDoor(int doorNumber)
{
  //Variable Declarations
  int firstDoor = 0;

  //Check if the door number exists for the room
  if(doorNumber >
      NUMBER_OF_DOORS[currentPlayerIter->getPlayerLocation().getRoom()])
  {
    throw(ExceptionClass("The door you have selected does not exist for the "
        "room you are in.  Please select another door and try again."));
  }

  for(int i = 0; i < int(currentPlayerIter->getPlayerLocation().getRoom()); i++)
  {
    firstDoor += NUMBER_OF_DOORS[i];
  }

  drawMove(currentPlayerIter->getPlayerLocation(),
      DOOR_LOCATIONS[firstDoor + doorNumber - 1], currentPlayerIter);
  currentPlayerIter->setPlayerLocation(DOOR_LOCATIONS
      [firstDoor + doorNumber - 1]);

  currentPlayerIter->setEnteredRoomThisMoveFlag(false);
  finishMove();
  refreshDisplay();
}

void ClueMainWindowClass::finishMove()
{
  currentPlayerIter->decrementMovesLeft();
  if(currentPlayerIter->getMovesLeft() <= 0)
  {
    if(currentPlayerIter->getEnteredRoomThisMoveFlag() == false)
    {
      currentPlayerIter->setDieRoll(0);
      clearVisitedTiles();

      currentPlayerIter++;
      if(currentPlayerIter == gameParticipants.end())
      {
        currentPlayerIter = gameParticipants.begin();
      }
      startPlayerTurn();
    }
    refreshDisplay();
  }
  updateRollInfoText();
}

void ClueMainWindowClass::moveCurrentPlayer(const DirectionEnum &direction)
{
  //Variable Declarations
  BoardLocationClass oldLocation = currentPlayerIter->getPlayerLocation();

  if(&*currentPlayerIter == &*thisPlayerPtr)
  {
    oldLocation = currentPlayerIter->getPlayerLocation();
  }


  try
  {
    //Draw the move
    currentPlayerIter->move(inProgressBoardImage, direction);
    drawMove(oldLocation, currentPlayerIter->getPlayerLocation(), currentPlayerIter);

    if(currentPlayerIter->getPlayerLocation().getTileType(CLUE_BOARD_IMAGE)
        == ROOM_TILE)
    {
      currentPlayerIter->setEnteredRoomThisMoveFlag(true);
      currentPlayerIter->setMovesLeft(0);
      refreshDisplay();
    }
    else
    {
      //
    }
    finishMove();
  }
  catch(ExceptionClass exception)
  {
    errorMessageLabel->setText("<span style=' font-weight:600; color:#ff0000;'>"
        + exception.getErrorMessage() + "</span>");
  }
}

void ClueMainWindowClass::moveCurrentPlayerToSecretPassage()
{
  BoardLocationClass newLocation;

  switch(currentPlayerIter->getPlayerLocation().getRoom())
  {
    case LOUNGE:
      newLocation = ROOM_PIECE_LOCATIONS[CONSERVATORY].getEmptyRoomTile(inProgressBoardImage);
      break;
    case KITCHEN:
      newLocation = ROOM_PIECE_LOCATIONS[STUDY].getEmptyRoomTile(inProgressBoardImage);
      break;
    case CONSERVATORY:
      newLocation = ROOM_PIECE_LOCATIONS[LOUNGE].getEmptyRoomTile(inProgressBoardImage);
      break;
    case STUDY:
      newLocation = ROOM_PIECE_LOCATIONS[KITCHEN].getEmptyRoomTile(inProgressBoardImage);
      break;
    default:
      break;
  }

  drawMove(currentPlayerIter->getPlayerLocation(), newLocation, currentPlayerIter);
  currentPlayerIter->setPlayerLocation(newLocation);

  currentPlayerIter->setEnteredRoomThisMoveFlag(true);
  currentPlayerIter->setMovesLeft(0);
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
      currentPlayerIter->setMovesLeft(0);
      currentPlayerIter->setEnteredRoomThisMoveFlag(false);
      finishMove();
    }
  }
  catch(ExceptionClass exception)
  {
    errorMessageLabel->setText("<span style=' font-weight:600; color:#ff0000;'>"
        + exception.getErrorMessage() + "</span>");
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

      //Display gameplay interface
      displayGameInterface();

      //Start the first turn
      startPlayerTurn();
    }
    //Networked play - client
    else
    {
      //Code
    }
  }
  catch(ExceptionClass exception)
  {
    displayExceptionMessageBox(exception);
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
  }
  else
  {
    hostIPEdit->setVisible(true);
    hostIPLabel->setVisible(true);
    computerPlayersSpin->setVisible(false);
    computerPlayersLabel->setVisible(false);
    humanPlayersSpin->setVisible(false);
    humanPlayersLabel->setVisible(false);
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
}

RoomEnum ClueMainWindowClass::getCurrentPlayerRoom()
{
  return currentPlayerIter->getPlayerLocation().getRoom();
}


//Dummy AI code
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
