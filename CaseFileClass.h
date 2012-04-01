#ifndef _CASEFILECLASS_H_
#define _CASEFILECLASS_H_

#include "enums.h"
#include "DeckClass.h"
#include "SuggestionClass.h"

class CaseFileClass : public SuggestionClass
{
  public:
    //Constructors
    CaseFileClass()
    {
    }

    CaseFileClass(DeckClass &deck);

    void createCaseFile(DeckClass &deck);
};

#endif
