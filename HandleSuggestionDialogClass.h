#ifndef _HANDLESUGGESTIONDIALOGCLASS_H_
#define _HANDLESUGGESTIONDIALOGCLASS_H_

#include <QtGui>
#include "ui_HandleSuggestionDialogClass.h"
#include "SuggestionClass.h"
#include "ClueMainWindowClass.h"
#include "enums.h"

class HandleSuggestionDialogClass : public QDialog,
    private Ui::handleSuggestionDialog
{
  Q_OBJECT

  public:
    HandleSuggestionDialogClass(ClueMainWindowClass *parent = 0, Qt::WindowFlags f = 0);

    void setupDialogBox(SuggestionClass *suggestionPtr,
        CardEnum *cardPtr);

  private:
    SuggestionClass *suggPtr;
    CardEnum *cardToRevealPtr;
    SuspectEnum currentPlayerChar;
    std::set<CardEnum> hand;

  public slots:
    void showCard();
};

#endif
