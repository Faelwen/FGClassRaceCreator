#include "customtextedit.h"

// Reimplement copy/paste to paste the html source or plain text

bool CustomTextEdit::canInsertFromMimeData(const QMimeData *source) const
{
    return source->hasHtml() ||  QPlainTextEdit::canInsertFromMimeData(source);
}

void CustomTextEdit::insertFromMimeData(const QMimeData *source)
{
    QPlainTextEdit::insertFromMimeData(source);
}
