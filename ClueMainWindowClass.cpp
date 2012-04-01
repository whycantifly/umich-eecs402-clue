#include <QtGui> 
#include <string>
#include <stdlib.h>

#include "ClueMainWindowClass.h"
#include "constants.h"
#include "AccusationDialogClass.h"
#include "BoardLocationClass.h"
#include "CaseFileClass.h"
#include "ExceptionClass.h"
#include "SuggestionDialogClass.h"

using namespace std;

ClueMainWindowClass::ClueMainWindowClass(QWidget *parent)
{
  //Set up the GUI
  setupUi(this);

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
  list<CardEnum>hand = thisPlayerPtr->getHand();
  list<CardEnum>::iterator currentCardIter = hand.begin();
  QLabel *cardDisplayPtr;
  CardEnum card;

  for(int i = 1; currentCardIter != hand.end(); i++, currentCardIter++)
  {
    switch(i)
    {
      case 1:
        cardDisplayPtr = cardInHand1;
        break;
      case 2:
        cardDisplayPtr = cardInHand2;
        break;
      case 3:
        cardDisplayPtr = cardInHand3;
        break;
      case 4:
        cardDisplayPtr = cardInHand4;
        break;
      case 5:
        cardDisplayPtr = cardInHand5;
        break;
      case 6:
        cardDisplayPtr = cardInHand6;
        break;
    }
    card = *currentCardIter;
    cardDisplayPtr->setPixmap(QPixmap::fromImage(
        CARD_IMAGES[int(*currentCardIter)]));
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
  cardInHand1->setVisible(false);
  cardInHand2->setVisible(false);
  cardInHand3->setVisible(false);
  cardInHand4->setVisible(false);
  cardInHand5->setVisible(false);
  cardInHand6->setVisible(false);

  //Make game setup interface visible
  gameWelcomeLabel->setVisible(true);
  gameSetupFrame->setVisible(true);
  startGameButton->setVisible(true);

  networkedPlayOption->toggle();
  networkedPlayOption->setFocus();

  gameHostCheck->setChecked(true);
  hostIPEdit->setVisible(false);
  hostIPLabel->setVisible(false);
}

void ClueMainWindowClass::drawPieceToBoard(const CardEnum &character,
    const BoardLocationClass &tile)
{
  //Variable Declarations
  QPoint boardLocTopLeft = tile.getTopLeftPixel();

  for(int i = 0; i < TILE_WIDTH; i++)
  {
    for(int j = 0; j < TILE_HEIGHT; j++)
    {
      if(CHAR_PIECE_IMAGES[int(character)].pixel(i, j) != TRANSPARENT_RGB)
      {
        inProgressBoardImage.setPixel(boardLocTopLeft + QPoint(i, j),
            CHAR_PIECE_IMAGES[int(character)].pixel(i, j));
      }
    }
  }
}

void ClueMainWindowClass::eraseTileContents(const BoardLocationClass &tile)
{
  //Variable Declarations
  QPoint boardLocTopLeft = tile.getTopLeftPixel();

  for(int i = 0; i < TILE_WIDTH; i++)
  {
    for(int j = 0; j < TILE_HEIGHT; j++)
    {
      inProgressBoardImage.setPixel(boardLocTopLeft + QPoint(i, j),
          CLUE_BOARD_IMAGE.pixel(boardLocTopLeft + QPoint(i, j)));
    }
  }
}

void ClueMainWindowClass::drawMove(const BoardLocationClass &oldLocation,
    const BoardLocationClass &newLocation)
{
  drawPieceToBoard(currentPlayerIter->getCharacter(), newLocation);
  eraseTileContents(oldLocation);
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
  list<CardEnum> availableCharacters;
  list<CardEnum>::iterator charIterator;
  list<PlayerClass>::iterator participantIterator;
  int randomCharacterNumber;

  //Initialize availableCharacters
  for(CardEnum i = CardEnum(0); i < NUMBER_OF_SUSPECTS;
      i = CardEnum(int(i) + 1))
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
    gameParticipants.insert(participantIterator, PlayerClass("Some name",
        *charIterator, i >= humanPlayersSpin->value(), i == 0));

    //Set thisPlayerChar
    if(i == 0)
    {
      participantIterator--;
      thisPlayerPtr = &*participantIterator;
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

  //Make game setup interface invisible
  gameWelcomeLabel->setVisible(false);
  gameSetupFrame->setVisible(false);
  startGameButton->setVisible(false);
}

void ClueMainWindowClass::startPlayerTurn()
{
  int dieRoll;

  if(gameOver == true)
  {
    //endGame();
  }
  else
  {
    if(currentPlayerIter->getPlayerLocation().getTileType(inProgressBoardImage)
        == ROOM_TILE)
    {
      //Player is in a corner room
      if(currentPlayerIter->getPlayerLocation().getRoom() == LOUNGE ||
          currentPlayerIter->getPlayerLocation().getRoom() == KITCHEN ||
          currentPlayerIter->getPlayerLocation().getRoom() == CONSERVATORY ||
          currentPlayerIter->getPlayerLocation().getRoom() == STUDY)
      {
        displayCornerRoomOptions();
      }
      else
      {
        displayDefaultOptions();
        dieRoll = getAndDisplayDieRoll();
      }
    }
    else
    {
      displayDefaultOptions();
      dieRoll = getAndDisplayDieRoll();
    }
  }
}

const int ClueMainWindowClass::getAndDisplayDieRoll() const
{
  int dieRoll;
  QString dieRollText;
  QString dieRollString;

  dieRoll = currentPlayerIter->rollDie();
  dieRollString.setNum(dieRoll);
  dieRollText = "You rolled a " + dieRollString + ".  You have " +
      dieRollString + " moves left.";
  rollInfoText->setText(dieRollText);

  return dieRoll;
}

void ClueMainWindowClass::displayCornerRoomOptions()
{
  rollDieOption->setVisible(true);
  useSecretPassageOption->setVisible(true);
  moveUpOption->setVisible(false);
  moveDownOption->setVisible(false);
  moveLeftOption->setVisible(false);
  moveRightOption->setVisible(false);
  leaveRoomOption->setVisible(false);
  doorNumberSpin->setVisible(false);
  makeSuggestionOption->setVisible(false);

}

void ClueMainWindowClass::displayDefaultOptions()
{
  rollDieOption->setVisible(false);
  useSecretPassageOption->setVisible(false);
  moveUpOption->setVisible(true);
  moveDownOption->setVisible(true);
  moveLeftOption->setVisible(true);
  moveRightOption->setVisible(true);
}

void ClueMainWindowClass::drawStartingPieces()
{
  for(list<PlayerClass>::iterator playerIterator = gameParticipants.begin();
      playerIterator != gameParticipants.end(); playerIterator++)
  {
    drawPieceToBoard(playerIterator->getCharacter(),
        STARTING_LOCATIONS[int(playerIterator->getCharacter())]);
    gameBoard->setPixmap(QPixmap::fromImage(inProgressBoardImage));
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
      currentPlayerIter->setAi(true);
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

void ClueMainWindowClass::makePlayerSuggestion()
{
  //Variable Declarations
  SuggestionClass suggestion;
  SuggestionDialogClass suggestionDialog(&suggestion, this);

  if(suggestionDialog.exec() == QDialog::Accepted)
  {

  }
}

void ClueMainWindowClass::movePlayerOutDoor(int doorNumber)
{
  //Variable Declarations
  int firstDoor = 0;

  //Check if the door number exists for the room
  if(doorNumber >
        NUMBER_OF_DOORS[currentPlayerIter->getPlayerLocation().getRoom() -
        NUMBER_OF_SUSPECTS - NUMBER_OF_WEAPONS])
  {
    throw(ExceptionClass("The door you have selected does not exist for the "
        "room you are in.  Please select another door and try again."));
  }

  for(int i = 0; i < int(currentPlayerIter->getPlayerLocation().getRoom()) -
      NUMBER_OF_SUSPECTS - NUMBER_OF_WEAPONS; i++)
  {
    firstDoor += NUMBER_OF_DOORS[i];
  }

  drawMove(currentPlayerIter->getPlayerLocation(),
      DOOR_LOCATIONS[firstDoor + doorNumber - 1]);
  currentPlayerIter->setPlayerLocation(DOOR_LOCATIONS
      [firstDoor + doorNumber - 1]);
}

void ClueMainWindowClass::movePlayer(const DirectionEnum &direction)
{
  //Variable Declarations
  BoardLocationClass newLocation = currentPlayerIter->getPlayerLocation();

  const QString CARD_VALUES[21] = {"Miss Scarlet", "Col. Mustard", "Mrs. White",
      "Mr. Green", "Mrs. Peacock", "Prof. Plum", "Knife", "Candlestick",
      "Revolver", "Rope", "Lead Pipe", "Wrench", "Hall", "Lounge", "Dining Room",
      "Kitchen", "Ballroom", "Conservatory", "Billiard Room", "Library", "Study"};
  try
  {
    newLocation.move(direction);

    //Check if the move is valid
    checkIfValidMove(newLocation);

    //Draw the move
    try
    {
      newLocation.getRoom();
      newLocation = getEmptyRoomTile(newLocation);
      drawMove(currentPlayerIter->getPlayerLocation(), newLocation);
      currentPlayerIter->setPlayerLocation(newLocation);
    }
    catch(ExceptionClass exception)
    {
      drawMove(currentPlayerIter->getPlayerLocation(), newLocation);
      currentPlayerIter->move(direction);
    }
  }
  catch(ExceptionClass exception)
  {
    errorMessageLabel->setText("<span style=' font-weight:600; color:#ff0000;'>"
        + exception.getErrorMessage() + "</span>");
  }
}

void ClueMainWindowClass::checkIfValidMove(BoardLocationClass &newLocation)
{
  //Variable Declarations
  TileTypeEnum currentTileType = newLocation.getTileType(inProgressBoardImage);
  TileTypeEnum originalTileType = newLocation.getTileType(CLUE_BOARD_IMAGE);
  int totalNumberOfDoors = 0;
  int i;
  int j;
  int runningNumberOfDoors = 0;
  bool isDoorTile = false;

  if(originalTileType == OUT_OF_BOUNDS_TILE)
  {
    throw(ExceptionClass("That tile is not within the bounds of the "
        "board.  Please try another move."));
  }
  else if(originalTileType == ROOM_TILE)
  {
    if(currentPlayerIter->getPlayerLocation().getRoomDoor() !=
        newLocation.getRoom())
    {
      throw(ExceptionClass("That tile is on the other side of a wall.  You "
          "must use a door to enter a room.  Please try another move."));
    }
  }
  else if(currentTileType == OCCUPIED_TILE)
  {
    throw(ExceptionClass("That tile is occupied by another player.  "
        "Please try another move."));
  }
}

void ClueMainWindowClass::movePlayerToSecretPassage()
{
  BoardLocationClass newLocation;

  switch(currentPlayerIter->getPlayerLocation().getRoom())
  {
    case LOUNGE:
      newLocation = getEmptyRoomTile(CONSERVATORY);
      break;
    case KITCHEN:
      newLocation = getEmptyRoomTile(STUDY);
      break;
    case CONSERVATORY:
      newLocation = getEmptyRoomTile(LOUNGE);
      break;
    case STUDY:
      newLocation = getEmptyRoomTile(KITCHEN);
      break;
  }

  drawMove(currentPlayerIter->getPlayerLocation(), newLocation);
  currentPlayerIter->setPlayerLocation(newLocation);
}

void ClueMainWindowClass::displayExceptionMessageBox(ExceptionClass
    exceptionText)
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
      getAndDisplayDieRoll();
    }
    else if(useSecretPassageOption->isChecked() == true)
    {
      movePlayerToSecretPassage();
    }
    else if(moveUpOption->isChecked() == true)
    {
      movePlayer(UP);
    }
    else if(moveDownOption->isChecked() == true)
    {
      movePlayer(DOWN);
    }
    else if(moveLeftOption->isChecked() == true)
    {
      movePlayer(LEFT);
    }
    else if(moveRightOption->isChecked() == true)
    {
      movePlayer(RIGHT);
    }
    else if(leaveRoomOption->isChecked() == true)
    {
      movePlayerOutDoor(doorNumberSpin->value());
    }
    else if(makeSuggestionOption->isChecked() == true)
    {
      makePlayerSuggestion();
    }
    else if(makeAccusationOption->isChecked() == true)
    {
      makePlayerAccusation();
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


const BoardLocationClass ClueMainWindowClass::getEmptyRoomTile(
    const BoardLocationClass &boardLocation)
{
  return getEmptyRoomTile(boardLocation.getRoom());
}

const BoardLocationClass ClueMainWindowClass::getEmptyRoomTile(
    const CardEnum &room)
{
  //Variable Declarations
  bool emptyTileFlag = false;
  BoardLocationClass currentLocation;
  int i;
  int j;

  i = 0;
  while(i < ROOM_STORAGE_WIDTH && emptyTileFlag == false)
  {
    j = 0;
    while(j < NUMBER_OF_SUSPECTS / ROOM_STORAGE_WIDTH && emptyTileFlag == false)
    {
      currentLocation =
          BoardLocationClass(ROOM_PIECE_LOCATIONS[int(room) -
          NUMBER_OF_SUSPECTS - NUMBER_OF_WEAPONS].getXCoord() + i,
          ROOM_PIECE_LOCATIONS[int(room) -
          NUMBER_OF_SUSPECTS - NUMBER_OF_WEAPONS].getYCoord() + j);
      if(currentLocation.getTileType(inProgressBoardImage) == ROOM_TILE)
      {
        emptyTileFlag = true;
      }
      j++;
    }
    i++;
  }

  return currentLocation;
}

CardEnum ClueMainWindowClass::getCurrentPlayerRoom()
{
  return currentPlayerIter->getPlayerLocation().getRoom();
}
