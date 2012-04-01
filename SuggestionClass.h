#ifndef _SUGGESTIONCLASS_H_
#define _SUGGESTIONCLASS_H_

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
    SuggestionClass(int suspect = 0, int weapon = 0, int room = 0)
    {
      suspect = SuspectEnum(suspect);
      weapon = WeaponEnum(weapon);
      room = RoomEnum(room);
    }

    SuggestionClass(SuspectEnum suspect, WeaponEnum weapon, RoomEnum room)
    {
      suspect = suspect;
      weapon = weapon;
      room = room;
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
