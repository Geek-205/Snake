#ifndef GAME_H
#define GAME_H

#include <QPainter>
#include <QWidget>
#include <QList>
#include <QTimer>
#include <QKeyEvent>
#include <QRandomGenerator>

class snakeItem;
class snake;
class gameField;

class snakeItem {

    friend snake;
    friend gameField;
public:
    snakeItem(int x, int y);
private:
    int m_x;
    int m_y;
};

class snake {
    friend gameField;
public:
    snake();
private:
    QList<snakeItem*> m_snakeBody;
    int m_snakeBeginSize;
    enum snakeDirection {
        up,
        left,
        down,
        right
    };
    snakeDirection m_snakeDirection;
};

class gameField : public QWidget {

    Q_OBJECT

public:
    gameField();

protected:
    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

private:
    snake *m_snake;
    snakeItem *m_food;
    int m_snakeItemSize;
    QTimer *m_moveSnakeTimer;
    int m_fieldSaze;
    int m_score;
    bool m_isPause;
    bool m_isMoveBlocked;
    bool m_isGameOver;
    void setGameStatus();
    void gameOver();
    void StartNewGame();
    void CreateFood();

private slots:
    void moveSnakeSlot();

signals:
    void ChangeTextSignal(QString text);
};

#endif // GAME_H
