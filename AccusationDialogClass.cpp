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
