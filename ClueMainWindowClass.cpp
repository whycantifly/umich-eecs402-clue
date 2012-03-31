#include <QtGui> 
#include <string>
#include <stdlib.h>

#include "ClueMainWindowClass.h"
#include "constants.h"
#include "BoardLocationClass.h"
#include "ExceptionClass.h"

using namespace std;

ClueMainWindowClass::ClueMainWindowClass(QWidget *parent)
{
  //Set up the GUI
  setupUi(this);

  //Assign signals/slots
  connect(networkedPlayOption, SIGNAL(toggled(bool)), gameHostCheck,
      SLOT(setVisible(bool)));
  connect(networkedPlayOption, SIGNAL(toggled(bool)), humanPlayersSpin,
      SLOT(setEnabled(bool)));
  connect(gameHostCheck, SIGNAL(toggled(bool)), humanPlayersSpin,
      SLOT(setEnabled(bool)));
  connect(gameHostCheck, SIGNAL(toggled(bool)), computerPlayersSpin,
      SLOT(setEnabled(bool)));
  connect(localPlayOption, SIGNAL(pressed()), computerPlayersSpin,
      SLOT(enable()));
  connect(localPlayOption, SIGNAL(pressed()), this,
      SLOT(resetHumanPlayersSpin()));
  connect(okayButton, SIGNAL(clicked()), this, SLOT(submitMove()));
  connect(startGameButton, SIGNAL(clicked()), this, SLOT(startGame()));

  setupNewBoard();
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

void ClueMainWindowClass::makeCaseFile()
{
  //Code
}

void ClueMainWindowClass::dealCards()
{
  //Code
}

void ClueMainWindowClass::setupGame()
{
  list<CardEnum> availableCharacters;
  list<CardEnum>::iterator charIterator;
  list<PlayerClass>::iterator participantIterator;
  int randomCharacterNumber;
  QMessageBox hi;

  //Initialize availableCharacters
  for(CardEnum i = CardEnum(0); i < NUMBER_OF_SUSPECTS;
      i = CardEnum(int(i) + 1))
  {
    availableCharacters.push_front(i);
  }

  //Make sure there are between 3 and 6 players
  if((humanPlayersSpin->value() + computerPlayersSpin->value()) < 3 ||
      (humanPlayersSpin->value() + computerPlayersSpin->value() > 6))
  {
//    throw(ExceptionClass("Error setting up game", "You must have a minimum of "
//        "3 and a maximum of 6 players to start a game."));
  }

  //Add players to gameParticipants
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
    gameParticipants.insert(participantIterator,
        PlayerClass("Some name", *charIterator, i >= humanPlayersSpin->value(),
            i == 0));

    //Set thisPlayerChar
    if(i == 0)
    {
      thisPlayerChar = *charIterator;
    }

    //Erase the character in availableCharacters
    availableCharacters.erase(charIterator);
  }

  currentPlayerIter = gameParticipants.begin();
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
  //Code
}
void ClueMainWindowClass::makePlayerSuggestion()
{
  //Code
}
void ClueMainWindowClass::movePlayerToDoor(int doorNumber)
{
  //Code
}
void ClueMainWindowClass::movePlayer(const DirectionEnum &direction)
{
  //Variable Declarations
  BoardLocationClass newLocation = currentPlayerIter->getPlayerLocation();

  try
  {
    newLocation.move(direction);

    //Check if the move is valid
    checkIfValidMove(newLocation);

    //Draw the move
    drawMove(currentPlayerIter->getPlayerLocation(), newLocation);

    //Update the player's current location
    currentPlayerIter->move(direction);
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
  bool isDoorTile = false;

  //Get the totalNumberOfDoors
  for(i = 0; i < NUMBER_OF_ROOMS; i++)
  {
    totalNumberOfDoors += NUMBER_OF_DOORS[i];
  }

  if(originalTileType == OUT_OF_BOUNDS_TILE)
  {
    throw(ExceptionClass("That tile is not within the bounds of the "
        "board.  Please try another move."));
  }
  else if(originalTileType == ROOM_TILE)
  {
    i = 0;
    while(i < totalNumberOfDoors && isDoorTile == false)
    {
      if(newLocation == DOOR_LOCATIONS[i])
      {
        isDoorTile = true;
      }
      i++;
    }

    if(isDoorTile == false)
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
      movePlayerToDoor(doorNumberSpin->value());
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
