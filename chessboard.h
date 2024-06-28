#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsScene>
#include "qgraphicsitem.h"

class ChessBoard
{
public:
    ChessBoard(QGraphicsScene *scene, QBrush fColor, QBrush sColor);



private:
    void DrawBoard();
    void DrawCoordinates();

    QGraphicsScene *scene;
    QBrush fColor;
    QBrush sColor;

};

#endif // CHESSBOARD_H
