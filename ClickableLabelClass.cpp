#include <QLabel>
#include <QtGui>

#include "ClickableLabelClass.h"

void ClickableLabelClass::clickedSlot()
{
  qDebug()<<"Clicked";
}

void ClickableLabelClass::mousePressEvent(QMouseEvent *event)
{
  emit clicked();
}
