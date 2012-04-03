#ifndef _SUGGESTIONCLASS_H_
#define _SUGGESTIONCLASS_H_

#include <set>

#include "enums.h"
#include "DeckClass.h"

class SuggestionClass
{
  protected:
    SuspectEnum suspect;
    WeaponEnum weapon;
    RoomEnum room;

  public:
    //Constructors
    SuggestionClass(int suspectSug = 0, int weaponSug = 0, int roomSug = 0)
    {
      suspect = SuspectEnum(suspectSug);
      weapon = WeaponEnum(weaponSug);
      room = RoomEnum(roomSug);
    }

    SuggestionClass(SuspectEnum suspectSug, WeaponEnum weaponSug,
        RoomEnum roomSug)
    {
      suspect = suspectSug;
      weapon = weaponSug;
      room = roomSug;
    }

    //Overloaded operators
    bool operator==(const SuggestionClass &rhs) const
    {
      return(suspect == rhs.suspect && weapon == rhs.weapon &&
          room == rhs.room);
    }

    bool operator!=(const SuggestionClass &rhs) const
    {
      return !(*this == rhs);
    }

    bool operator==(std::set<CardEnum> &playerHand) const;

    bool operator!=(std::set<CardEnum> &playerHand) const;

    //Inline functions
    const SuspectEnum getSuspect() const
    {
      return suspect;
    }
    const WeaponEnum getWeapon() const
    {
      return weapon;
    }
    const RoomEnum getRoom() const
    {
      return room;
    }
};

#endif
