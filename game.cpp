#include "game.h"

gameField::gameField() {
    setFixedSize(300, 300);
    setFocusPolicy(Qt::StrongFocus);
    m_snakeItemSize = 10;
    m_fieldSaze = width()/m_snakeItemSize;
    StartNewGame();
}

void gameField::paintEvent(QPaintEvent *e) {

    Q_UNUSED(e)

    QBrush gameFieldBrush(QColor(56, 102, 65), Qt::SolidPattern);
    QBrush snakeBrush(QColor(106, 153, 78), Qt::SolidPattern);
    QBrush foodBrush(QColor(188, 71, 73), Qt::SolidPattern);

    QPainter painter;

    painter.begin(this);

    if(m_isGameOver) {
        painter.setPen(QColor(188, 71, 73));
        painter.setFont(QFont("Arial", 15, 700));
        painter.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, "ИГРА ПРОИГРАНА!\nсчет : " + QString::number(m_score));
        return;
    }

    //отрисовка игрового поля
    painter.setBrush(gameFieldBrush);
    painter.setPen(QColor(50,50,50));
    painter.drawRect(0, 0, width()-1, height()-1);

    // отрисовка окна
    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    painter.drawRect(0, 0, width() - 1, height() - 1);

    // отрисовка змейки
    painter.setBrush(snakeBrush);
    painter.setPen(QColor(0,0,0));
    for (int i = 0; i < m_snake -> m_snakeBody.size(); i++) {
        painter.drawEllipse(m_snake -> m_snakeBody[i] -> m_x * m_snakeItemSize, m_snake -> m_snakeBody[i] -> m_y * m_snakeItemSize, m_snakeItemSize, m_snakeItemSize);
    }

    //отрисовка еды
    painter.setBrush(foodBrush);
    painter.drawEllipse(m_food->m_x * m_snakeItemSize, m_food->m_y * m_snakeItemSize, m_snakeItemSize, m_snakeItemSize);

    painter.end();
    m_isMoveBlocked = false;
}

void gameField::keyPressEvent(QKeyEvent *e) {
    if(e->key() == Qt::Key_Space) {
        if(m_isGameOver) {
            StartNewGame();
            return;
        }
        m_isPause = !m_isPause;
        setGameStatus();
    }
    if(m_isMoveBlocked) return;
    if(e->key() == Qt::Key_Left && m_snake->m_snakeDirection != snake::snakeDirection::right)
        m_snake->m_snakeDirection = snake::snakeDirection::left;
    if(e->key() == Qt::Key_Right && m_snake->m_snakeDirection != snake::snakeDirection::left)
        m_snake->m_snakeDirection = snake::snakeDirection::right;
    if(e->key() == Qt::Key_Up && m_snake->m_snakeDirection != snake::snakeDirection::down)
        m_snake->m_snakeDirection = snake::snakeDirection::up;
    if(e->key() == Qt::Key_Down && m_snake->m_snakeDirection != snake::snakeDirection::up)
        m_snake->m_snakeDirection = snake::snakeDirection::down;
    m_isMoveBlocked = true;
}

void gameField::setGameStatus() {
    QString text;

    if(m_isPause) {
        m_moveSnakeTimer->stop();
        text = "счет : " + QString::number(m_score) + "\nпродолжить - ПРОБЕЛ";
    }
    else {
        m_moveSnakeTimer->start(125);
        text = "счет : " + QString::number(m_score) + "\nпауза - ПРОБЕЛ";
    }
    emit ChangeTextSignal(text);
}

void gameField::gameOver() {
    m_isGameOver = true;
    QString text = "\nНАЧАТЬ ЗАНОВО - ПРОБЕЛ";
    emit ChangeTextSignal(text);
    m_moveSnakeTimer->stop();
    delete m_snake;
    delete m_food;
    delete m_moveSnakeTimer;
}

void gameField::StartNewGame() {
    m_isPause = false;
    m_isMoveBlocked = false;
    m_isGameOver = false;
    m_snake = new snake();
    m_food = new snakeItem(m_fieldSaze/2, m_fieldSaze/2);
    m_moveSnakeTimer = new QTimer();
    connect(m_moveSnakeTimer, &QTimer::timeout, this, &gameField::moveSnakeSlot);
    m_moveSnakeTimer->start(125);
    m_score = 0;
    QString text = "счет : " + QString::number(m_score) + "\nпауза - ПРОБЕЛ";
    emit ChangeTextSignal(text);
}

void gameField::CreateFood() {
    m_food->m_x = QRandomGenerator::global()->bounded(0, m_fieldSaze - 1);
    m_food->m_y = QRandomGenerator::global()->bounded(0, m_fieldSaze - 1);
    for (int i = 0; i < m_snake->m_snakeBody.size(); i++) {
        if(m_food->m_x == m_snake->m_snakeBody[i]->m_x && m_food->m_y == m_snake->m_snakeBody[i]->m_y){
            return CreateFood();
        }
    }
}

void gameField::moveSnakeSlot() {
    snakeItem *newSnakeItem;
    switch(m_snake->m_snakeDirection) {

    case snake::snakeDirection::right:
        newSnakeItem = new snakeItem(m_snake->m_snakeBody[0]->m_x + 1, m_snake->m_snakeBody[0]->m_y);
        break;
    case snake::snakeDirection::left:
        newSnakeItem = new snakeItem(m_snake->m_snakeBody[0]->m_x - 1, m_snake->m_snakeBody[0]->m_y);
        break;
    case snake::snakeDirection::up:
        newSnakeItem = new snakeItem(m_snake->m_snakeBody[0]->m_x, m_snake->m_snakeBody[0]->m_y - 1);
        break;
    default:
        newSnakeItem = new snakeItem(m_snake->m_snakeBody[0]->m_x, m_snake->m_snakeBody[0]->m_y + 1);
        break;
    }

    //ограничение игрового поля
    if(newSnakeItem->m_x >= m_fieldSaze) newSnakeItem->m_x = 0;
    else if (newSnakeItem->m_x < 0) newSnakeItem->m_x = m_fieldSaze - 1;
    else if (newSnakeItem->m_y < 0) newSnakeItem->m_y = m_fieldSaze - 1;
    else if (newSnakeItem->m_y >= m_fieldSaze) newSnakeItem->m_y = 0;

    //проигрышь
    for(int i = 1; i < m_snake->m_snakeBody.size(); i++) {
        if(m_snake->m_snakeBody[0]->m_x == m_snake->m_snakeBody[i]->m_x && m_snake->m_snakeBody[0]->m_y == m_snake->m_snakeBody[i]->m_y){
            gameOver();
        }
    }

    //поедание
    if(newSnakeItem->m_x == m_food->m_x && newSnakeItem->m_y == m_food->m_y) {
        m_score++;
        CreateFood();
        QString text = "счет : " + QString::number(m_score) + "\nпауза - ПРОБЕЛ";
        emit ChangeTextSignal(text);
    }
    else {
        m_snake->m_snakeBody.removeLast();
    }

    m_snake->m_snakeBody.insert(0, newSnakeItem);
    repaint();
}

snakeItem::snakeItem(int x, int y) {
    m_x = x;
    m_y = y;
}

snake::snake() {
    m_snakeBeginSize = 3;
    for (int i = 0; i < m_snakeBeginSize; i++ ) {
        m_snakeBody.insert(0, new snakeItem(i, 15));
    }
    m_snakeDirection = snakeDirection::right;
}
