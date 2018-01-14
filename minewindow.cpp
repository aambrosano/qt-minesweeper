#include "minewindow.h"
#include "mineboard.h"

#include <QMessageBox>

MineWindow::MineWindow(QWidget *parent) : QMainWindow(parent), board(nullptr)
{
    reset();
}

void MineWindow::reset()
{
    board = new MineBoard(25, 25, this);
    setCentralWidget(board);
    resize(board->size());
    setMaximumSize(board->width(), board->height());

    connect(board, SIGNAL(lost()), this, SLOT(lost()));
    connect(board, SIGNAL(won()), this, SLOT(won()));

    board->initialize();
}

void MineWindow::checkButtons(QAbstractButton *btn)
{
    if (btn->text() == tr("Restart")) {
        this->reset();
    } else if (btn->text() == tr("Close")) {
        this->close();
    }
}

void MineWindow::lost()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("You lost the game.");
    msgBox.setText("You lost the game.");
    msgBox.addButton(tr("Restart"), QMessageBox::ResetRole);
    msgBox.addButton(tr("Close"), QMessageBox::DestructiveRole);
    connect(&msgBox, SIGNAL(buttonClicked(QAbstractButton *)), this,
            SLOT(checkButtons(QAbstractButton *)));
    msgBox.exec();
}

void MineWindow::won()
{
    board->openAll();

    QMessageBox msgBox;
    msgBox.setWindowTitle("You won.");
    msgBox.setText("You won.");
    msgBox.addButton(tr("Restart"), QMessageBox::ResetRole);
    msgBox.addButton(tr("Close"), QMessageBox::DestructiveRole);
    connect(&msgBox, SIGNAL(buttonClicked(QAbstractButton *)), this,
            SLOT(checkButtons(QAbstractButton *)));
    msgBox.exec();
}

MineWindow::~MineWindow()
{
    // delete ui;
    // delete board;
}
