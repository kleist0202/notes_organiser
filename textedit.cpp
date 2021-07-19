#include "textedit.hpp"

TextEdit::TextEdit(int w, int h, QWidget *parent)
    : m_width(w), m_height(h), QTextEdit(parent)
{
}

QSize TextEdit::sizeHint() const
{
    QSize size = QAbstractScrollArea::sizeHint();
    size.rheight() = m_width;
    size.rwidth() = m_height;
    return size;
}

