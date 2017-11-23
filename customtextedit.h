#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QPlainTextEdit>


class CustomTextEdit : public QPlainTextEdit
{
public:
     bool canInsertFromMimeData(const QMimeData* source) const;
     void insertFromMimeData(const QMimeData* source);
};

#endif // CUSTOMTEXTEDIT_H
