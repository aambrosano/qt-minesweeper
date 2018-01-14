#ifndef MINEBOARD_H
#define MINEBOARD_H

#include <QBoxLayout>
#include <QGridLayout>
#include <QLCDNumber>
#include <QRandomGenerator>
#include <QRect>
#include <QVector>
#include <QWidget>

#include "minecell.h"

class MineBoard : public QWidget
{
    Q_OBJECT

public:
    explicit MineBoard(int rows, int cols, QWidget *parent = nullptr);
    //    ~MineBoard();

    void openAll();
    void initialize(); // Opens an empty space on the board

signals:
    void lost();
    void won();

public slots:
    void checkWin();
    void onMarked(bool);

private:
    QVector<MineCell *> bombCells;
    QVector<MineCell *> emptyCells;
    QVector<MineCell *> valueCells;

    QLCDNumber *bombLCD;

    QRandomGenerator rg;

    QGridLayout *grid;

    const int SIZE = 30;
    const int OFF_X = 20;
    const int OFF_Y = 20;

    int m_rows;
    int m_cols;

    bool m_won;
};

#endif // MINEBOARD_H
