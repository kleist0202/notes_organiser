#ifndef TEXTEDIT_HPP 
#define TEXTEDIT_HPP

#include <QTextEdit>

class TextEdit : public QTextEdit
{
    Q_OBJECT

public:
    explicit TextEdit(int w, int h, QWidget *parent = nullptr);

    QSize sizeHint() const override;
private:
    int m_width;
    int m_height;
};

#endif // TEXTEDIT_HPP
