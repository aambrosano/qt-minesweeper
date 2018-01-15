#ifndef MINECELL_H
#define MINECELL_H

#include <QPushButton>
#include <QRect>
#include <QWidget>

class MineCell : public QPushButton
{
    Q_OBJECT

public:
    MineCell(int size, QWidget *parent = nullptr);

    void addNeighbour(MineCell *neighbour);
    bool isBomb();
    void setBomb();
    bool isOpened();
    void setOpened();
    bool isMarked();
    void setMarked();
    void toggleMarked();
    int getValue();

signals:
    void hasExploded();
    void hasOpened();
    void rightClicked();
    void checkWin();
    void sigMarked(bool);

public slots:
    void propagateOpen();
    //    void onClicked(bool marked);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    //    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QVector<MineCell *> neighbours;
    QRectF cell;
    bool rightClick;

    // This is the model
    int value;

    bool bomb;
    bool marked;
    bool opened;
};

#endif // MINECELL_H
