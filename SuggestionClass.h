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

    SuggestionClass(SuspectEnum suspectSug = UNKNOWN_SUSPECT, WeaponEnum
        weaponSug = UNKNOWN_WEAPON, RoomEnum roomSug = UNKNOWN_ROOM)
    {
      suspect = suspectSug;
      weapon = weaponSug;
      room = roomSug;
    }

    SuggestionClass(int suspectSug, int weaponSug, int roomSug)
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

    bool operator==(const std::map<CardEnum, std::set<SuspectEnum> > &playerHand)
        const;

    bool operator!=(const std::map<CardEnum, std::set<SuspectEnum> > &playerHand)
        const;

    bool operator==(const CardEnum card) const;

    bool operator!=(const CardEnum card) const;

    //Inline functions
    SuspectEnum getSuspect() const
    {
      return suspect;
    }

    void setSuspect(const SuspectEnum newSuspect)
    {
      suspect = newSuspect;
    }

    WeaponEnum getWeapon() const
    {
      return weapon;
    }

    void setWeapon(const WeaponEnum newWeapon)
    {
      weapon = newWeapon;
    }

    RoomEnum getRoom() const
    {
      return room;
    }

    void setRoom(const RoomEnum newRoom)
    {
      room = newRoom;
    }
};

#endif
