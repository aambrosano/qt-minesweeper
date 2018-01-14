#include <QApplication>
#include <QColor>
#include <QDebug>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

#include "minecell.h"

MineCell::MineCell(int size, QWidget *parent)
    : QPushButton(parent), opened(false), value(0), bomb(false), marked(false)
{
    this->cell = {0.0, 0.0, (double)size, (double)size};
    this->setMinimumSize(size, size);
    this->setMaximumSize(size, size);

    connect(this, SIGNAL(clicked(bool)), this, SLOT(onClicked(bool)));
}

void MineCell::addNeighbour(MineCell *neighbour)
{
    this->neighbours.push_back(neighbour);
    this->value += neighbour->isBomb() ? 1 : 0;
    connect(neighbour, SIGNAL(hasOpened()), this, SLOT(propagateOpen()));
}

bool MineCell::isBomb() { return this->bomb; }

void MineCell::setBomb() { this->bomb = true; }

bool MineCell::isOpened() { return this->opened; }

bool MineCell::isMarked() { return this->marked; }

void MineCell::setMarked()
{
    if (!this->marked)
        this->toggleMarked();
}

void MineCell::toggleMarked()
{
    this->marked = !this->marked;
    if (this->marked) {
        this->setText("*");
    } else {
        this->setText("");
    }
    emit sigMarked(this->marked);
    this->repaint();
}

int MineCell::getValue() { return this->value; }

void MineCell::paintEvent(QPaintEvent *event)
{
    if (this->opened) {
        QPainter painter(this);
        painter.drawRect(this->cell);
        painter.fillRect(this->cell, Qt::lightGray);
        if (this->value != 0) {
            painter.setFont(QFont(tr("Ubuntu"), this->cell.height() * 0.4));
            // Green with increasing intensity with bigger numbers
            painter.setPen(
                QPen(QColor::fromHsl(120 + 60 * this->value, 100 * 2.55,
                                     85 * 2.55 - 2.55 * 10 * this->value)));
            painter.drawText(this->cell, Qt::AlignCenter,
                             tr(std::to_string(this->value).c_str()));
        }
    } else {
        QPushButton::paintEvent(event);
    }
}

void MineCell::propagateOpen()
{
    if (!this->opened) {
        this->opened = true;
        if (this->value == 0)
            emit hasOpened();
        this->repaint();
    }
}

void MineCell::setOpened()
{
    if (!this->marked && !this->opened) {
        if (this->bomb) {
            emit hasExploded();
        } else {
            this->opened = true;
            if (this->value == 0)
                emit hasOpened();
            this->repaint();
            emit checkWin();
        }
    }
}

void MineCell::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!this->opened) {
            this->toggleMarked();
        } else {
            int count = 0;
            foreach (MineCell *mc, neighbours) {
                count += mc->marked ? 1 : 0;
            }
            if (count == this->value) {
                foreach (MineCell *mc, neighbours) {
                    mc->setOpened();
                }
            }
        }
        emit checkWin();
    } else if (event->button() == Qt::RightButton) {
        setOpened();
    }
    QPushButton::mouseReleaseEvent(event);
}
