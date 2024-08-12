#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include    "game.h"
#include    "helpfield.h"
#include    <QGridLayout>
#include    <QMainWindow>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    gameField *m_gameField;
    helpField *m_helpField;
    QWidget *m_centralWidget;
    QGridLayout *m_gridLayout;
};
#endif // MAINWINDOW_H
