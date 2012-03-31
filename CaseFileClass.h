#ifndef _CASEFILECLASS_H_
#define _CASEFILECLASS_H_

#include "enums.h"
#include "DeckClass.h"

class CaseFileClass
{
  private:
    CardEnum suspectCard;
    CardEnum weaponCard;
    CardEnum roomCard;

  public:
    //Constructors
    CaseFileClass()
    {
    }

    CaseFileClass(DeckClass &deck);

    void createCaseFile(DeckClass &deck);

    //Inline functions
    const CardEnum getSuspectCard() const
    {
      return suspectCard;
    }
    const CardEnum getWeaponCard() const
    {
      return weaponCard;
    }
    const CardEnum getRoomCard() const
    {
      return roomCard;
    }
};

#endif
