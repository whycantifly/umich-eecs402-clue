#ifndef _EXCEPTIONCLASS_H_
#define _EXCEPTIONCLASS_H_

#include <QString>

class ExceptionClass
{
  private:
    QString errorTitle;     //Error title (for message box)
    QString errorMessage;   //Error text

  public:
    //Constructors

    //Constructor(); initializes the title and message to empty strings.
    ExceptionClass()
    {
      errorTitle = "";
      errorMessage = "";
    }

    //Constructor(QString); initializes the object based on:
    //exceptionText (error message)
    ExceptionClass(const QString &exceptionText)
    {
      errorTitle = "";
      errorMessage = exceptionText;
    }

    //Constructor(QString, QString); initializes the object based on:
    //exceptionTitle (error message title)
    //exceptionText (error message)
    ExceptionClass(const QString &exceptionTitle, const QString &exceptionText)
    {
      errorTitle = exceptionTitle;
      errorMessage = exceptionText;
    }

    //Inline Functions

    //Gets the error title.
    QString getErrorTitle() const
    {
      return errorTitle;
    }

    //Gets the error message.
    QString getErrorMessage() const
    {
      return errorMessage;
    }
};

#endif
