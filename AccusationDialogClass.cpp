//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    AccusationDialogClass class definition

#include <QtGui>
#include "AccusationDialogClass.h"

AccusationDialogClass::AccusationDialogClass(SuggestionClass *accusationPtr,
    QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    setupUi(this);

    playerAccPtr = accusationPtr;

    //Signals/slots
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(setAccusation()));
}
