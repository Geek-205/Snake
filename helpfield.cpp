#include "helpfield.h"

helpField::helpField() {
    setFixedSize(300, 50);
    m_text = "счет : 0\nпауза - ПРОБЕЛ";
}

void helpField::paintEvent(QPaintEvent *e) {

    Q_UNUSED(e)

    QPainter painter;

    painter.begin(this);
    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    painter.drawRect(2, 3, width() - 5, height() - 6);
    painter.setFont(QFont("Arial", 10, 700));
    painter.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, m_text);
    painter.end();
}

void helpField::ChangeTextSlot(QString text) {
    m_text = text;
    repaint();
}
