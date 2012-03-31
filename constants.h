#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <QtGui>
#include <string>

#include "BoardLocationClass.h"

const QString CHARACTER_NAMES[6] = {"Miss Scarlet", "Colonel Mustard",
    "Mrs. White", "Mr. Green", "Ms. Peacock", "Professor Plum"};

//RGB value for transparency
const QRgb TRANSPARENT_RGB = qRgb(255, 255, 255);

//RGB values for empty tiles
const QRgb EMPTY_TILE_RGB[7] = {qRgb(255, 238, 170), qRgb(50, 103, 183),
    qRgb(139, 210, 91), qRgb(196, 117, 217), qRgb(213, 191, 102),
    qRgb(255, 254, 253), qRgb(234, 174, 174)};

//RGB values for rooms
const QRgb ROOM_TILE_RGB[9] = {qRgb(153, 153, 153), qRgb(145, 111, 124),
    qRgb(147, 167, 172), qRgb(205, 222, 135), qRgb(211, 95, 95),
    qRgb(233, 175, 198), qRgb(137, 160, 44), qRgb(160, 90, 44),
    qRgb(255, 212, 42)};

//RGB values for character pieces
const QRgb CHAR_PIECE_RGB[6] = {qRgb(160, 44, 44), qRgb(200, 171, 55),
    qRgb(244, 227, 215), qRgb(68, 120, 33), qRgb(33, 68, 120),
    qRgb(68, 22, 80)};

//Clue board image
const QImage CLUE_BOARD_IMAGE("images/board.png");

//Piece images
const QImage CHAR_PIECE_IMAGES[6] = {QImage("images/scarlet.png"),
    QImage("images/mustard.png"), QImage("images/white.png"),
    QImage("images/green.png"), QImage("images/peacock.png"),
    QImage("images/plum.png")};

//Door locations
const BoardLocationClass DOOR_LOCATIONS[16] = {BoardLocationClass(11, 7),
    BoardLocationClass(12, 7), BoardLocationClass(17, 7),
    BoardLocationClass(16, 9), BoardLocationClass(16, 11),
    BoardLocationClass(19, 17), BoardLocationClass(9, 16),
    BoardLocationClass(14, 16), BoardLocationClass(7, 19),
    BoardLocationClass(16, 19), BoardLocationClass(5, 19),
    BoardLocationClass(7, 15), BoardLocationClass(1, 11),
    BoardLocationClass(7, 8), BoardLocationClass(3, 11),
    BoardLocationClass(6, 4)};

//Piece starting locations
const BoardLocationClass STARTING_LOCATIONS[6] = {BoardLocationClass(16, 0),
    BoardLocationClass(23, 7), BoardLocationClass(14, 24),
    BoardLocationClass(9, 24), BoardLocationClass(0, 18),
    BoardLocationClass(0, 5)};

//Piece storage reference locations for each room
const BoardLocationClass ROOM_PIECE_LOCATIONS[9] = {BoardLocationClass(12, 0),
    BoardLocationClass(19, 0), BoardLocationClass(19, 13),
    BoardLocationClass(18, 22), BoardLocationClass(10, 22),
    BoardLocationClass(0, 19), BoardLocationClass(1, 15),
    BoardLocationClass(1, 6), BoardLocationClass(0, 0)};

//Number of doors per room
const int NUMBER_OF_DOORS[9] = {2, 1, 2, 1, 4, 1, 2, 2, 1};

const int NUMBER_OF_ROOMS = 9;
const int NUMBER_OF_SUSPECTS = 6;
const int NUMBER_OF_WEAPONS = 6;

const int BOARD_WIDTH = 24;
const int BOARD_HEIGHT = 25;

const int TILE_WIDTH = 25;
const int TILE_HEIGHT = 25;

const int ROOM_STORAGE_WIDTH = 3;

#endif
