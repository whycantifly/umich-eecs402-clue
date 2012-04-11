#ifndef _SUGGESTIONCLASS_H_
#define _SUGGESTIONCLASS_H_

#include <map>
#include <set>

#include "enums.h"

class SuggestionClass
{
  protected:
    SuspectEnum suspect;
    WeaponEnum weapon;
    RoomEnum room;

  public:
    //Constructors

    SuggestionClass(SuspectEnum suspectSug, WeaponEnum weaponSug,
        RoomEnum roomSug)
    {
      suspect = suspectSug;
      weapon = weaponSug;
      room = roomSug;
    }

    SuggestionClass(int suspectSug = 0, int weaponSug = 0, int roomSug = 0)
    {
      suspect = SuspectEnum(suspectSug);
      weapon = WeaponEnum(weaponSug);
      room = RoomEnum(roomSug);
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

    bool operator==(std::map<CardEnum, std::set<SuspectEnum> > &playerHand)
        const;

    bool operator!=(std::map<CardEnum, std::set<SuspectEnum> > &playerHand)
        const;

    bool operator==(CardEnum card) const;

    bool operator!=(CardEnum card) const;

    //Inline functions
    SuspectEnum getSuspect() const
    {
      return suspect;
    }
    WeaponEnum getWeapon() const
    {
      return weapon;
    }
    RoomEnum getRoom() const
    {
      return room;
    }
};

#endif
