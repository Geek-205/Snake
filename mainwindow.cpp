#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    setFixedSize(330, 370);

    m_gameField = new gameField();
    m_helpField = new helpField();
    connect(m_gameField, &gameField::ChangeTextSignal, m_helpField, &helpField::ChangeTextSlot);
    m_centralWidget = new QWidget();
    m_gridLayout = new QGridLayout();
    m_gridLayout -> addWidget(m_helpField, 1, 0);
    m_gridLayout -> addWidget(m_gameField, 0, 0);
    m_centralWidget -> setLayout(m_gridLayout);
    setCentralWidget(m_centralWidget);
    setStyleSheet("background: rgb(242, 232, 207);");
}

MainWindow::~MainWindow() {}
