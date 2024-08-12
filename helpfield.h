#ifndef HELPFIELD_H
#define HELPFIELD_H

#include <QPainter>
#include <QWidget>

class helpField : public QWidget {
public:
    helpField();

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    QString m_text;

public slots:
    void ChangeTextSlot(QString text);
};

#endif // HELPFIELD_H
