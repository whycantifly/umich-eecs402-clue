#ifndef _ENUMS_H_
#define _ENUMS_H_

//All cards (include suspects, weapons, rooms)
enum CardEnum{SCARLET, MUSTARD, WHITE, GREEN, PEACOCK, PLUM, KNIFE,
    CANDLESTICK, REVOLVER, ROPE, LEAD_PIPE, WRENCH, HALL, LOUNGE, DINING_ROOM,
    KITCHEN, BALLROOM, CONSERVATORY, BILLIARD_ROOM, LIBRARY, STUDY};

//Card types
enum CardTypeEnum{SUSPECT_CARD, WEAPON_CARD, ROOM_CARD};

//Tile types
enum TileTypeEnum{UNOCCUPIED_TILE, ROOM_TILE, OCCUPIED_TILE,
    OUT_OF_BOUNDS_TILE};

//Directions
enum DirectionEnum{UP, DOWN, LEFT, RIGHT};

#endif
