#ifndef MINESWIFFER_H
#define MINESWIFFER_H

#include <QMainWindow>

#include "mineboard.h"

class MineWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MineWindow(QWidget *parent = nullptr);
    ~MineWindow();

    void reset();

private slots:
    void lost();
    void won();

    void checkButtons(QAbstractButton *btn);

private:
    MineBoard *board;
};

#endif // MINESWIFFER_H
