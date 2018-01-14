#include <QLCDNumber>
#include <QPainter>
#include <QTime>

#include "mineboard.h"

MineBoard::MineBoard(int rows, int cols, QWidget *parent)
    : QWidget(parent), m_rows(rows), m_cols(cols), m_won(false)
{
    qsrand(static_cast<quint64>(QTime::currentTime().msecsSinceStartOfDay()));

    QVBoxLayout *box = new QVBoxLayout(this);
    bombLCD = new QLCDNumber(this);
    box->addWidget(bombLCD);

    bombLCD->setSegmentStyle(QLCDNumber::Filled);
    bombLCD->setMinimumHeight(50);
    bombLCD->setDigitCount(3);

    grid = new QGridLayout(this);
    grid->setVerticalSpacing(0);
    grid->setHorizontalSpacing(0);

    // Creating all the cells
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            MineCell *cell = new MineCell(SIZE, this);
            if (qrand() % 100 > 88) {
                cell->setBomb();
                bombLCD->display(bombLCD->intValue() + 1);
            }
            connect(cell, SIGNAL(hasExploded()), this, SIGNAL(lost()));
            connect(cell, SIGNAL(checkWin()), this, SLOT(checkWin()));
            connect(cell, SIGNAL(sigMarked(bool)), this, SLOT(onMarked(bool)));
            grid->addWidget(cell, i, j, Qt::AlignCenter);
        }
    }

    grid->setSizeConstraint(QGridLayout::SetMinAndMaxSize);
    box->addLayout(grid);
    setMaximumWidth(box->sizeHint().width());
    setMaximumHeight(box->sizeHint().height());
    setLayout(box);

    // Setting up connections
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            MineCell *curr = (MineCell *)grid->itemAtPosition(i, j)->widget();
            QLayoutItem *item;
            if ((item = grid->itemAtPosition(i - 1, j - 1)) != nullptr)
                curr->addNeighbour((MineCell *)item->widget());
            if ((item = grid->itemAtPosition(i - 1, j)) != nullptr)
                curr->addNeighbour((MineCell *)item->widget());
            if ((item = grid->itemAtPosition(i - 1, j + 1)) != nullptr)
                curr->addNeighbour((MineCell *)item->widget());
            if ((item = grid->itemAtPosition(i, j - 1)) != nullptr)
                curr->addNeighbour((MineCell *)item->widget());
            if ((item = grid->itemAtPosition(i, j + 1)) != nullptr)
                curr->addNeighbour((MineCell *)item->widget());
            if ((item = grid->itemAtPosition(i + 1, j - 1)) != nullptr)
                curr->addNeighbour((MineCell *)item->widget());
            if ((item = grid->itemAtPosition(i + 1, j)) != nullptr)
                curr->addNeighbour((MineCell *)item->widget());
            if ((item = grid->itemAtPosition(i + 1, j + 1)) != nullptr)
                curr->addNeighbour((MineCell *)item->widget());
        }
    }
}
void MineBoard::checkWin()
{
    // Policy for winning is that either all and only the bombs are marked or
    // only all the bombs are closed
    bool allMarked = true;
    bool allClosed = true;

    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            MineCell *mc
                = (MineCell *)this->grid->itemAtPosition(i, j)->widget();
            if (!mc->isOpened() && !mc->isBomb())
                allClosed = false;
            if ((mc->isBomb() && !mc->isMarked())
                || (!mc->isBomb() && mc->isMarked()))
                allMarked = false;
            if (!allMarked && !allClosed)
                return;
        }
    }

    if (!m_won) {
        m_won = true;
        emit won();
    }
}

void MineBoard::onMarked(bool marked)
{
    bombLCD->display(bombLCD->intValue() + (marked ? -1 : 1));
}

void MineBoard::openAll()
{
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            MineCell *mc
                = (MineCell *)this->grid->itemAtPosition(i, j)->widget();
            if (!mc->isBomb())
                mc->setOpened();
            else if (!mc->isMarked())
                mc->setMarked();
        }
    }
}

void MineBoard::initialize()
{
    while (true) {
        int r = rg.generate() % m_rows;
        int c = rg.generate() % m_cols;

        MineCell *mc = (MineCell *)this->grid->itemAtPosition(r, c)->widget();
        if (mc->getValue() == 0 && !mc->isBomb()) {
            mc->setOpened();
            break;
        }
    }
}
