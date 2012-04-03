#include <QtGui>
#include <QLabel>

#ifndef _CLICKABLELABELCLASS_H_
#define _CLICKABLELABELCLASS_H_

class ClickableLabelClass : public QLabel
{
  Q_OBJECT
  public:
    ClickableLabelClass(const QString &text, QWidget *parent = 0, Qt::WindowFlags f = 0) : QLabel(text, parent, f)
    {
      connect(this, SIGNAL(clicked()), this, SLOT(clickedSlot()));
    }
    ClickableLabelClass(QWidget *parent = 0, Qt::WindowFlags f = 0) : QLabel(parent, f)
    {
      connect(this, SIGNAL(clicked()), this, SLOT(clickedSlot()));
    }

  signals:
    void clicked();

  public slots:
    void clickedSlot();

  protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif
