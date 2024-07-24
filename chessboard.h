#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsScene>
#include "qgraphicsitem.h"
#include <QVector>
#include "Pieces/basepiece.h"


class ChessBoard
{
public:
    ChessBoard(QGraphicsScene *scene, QBrush fColor, QBrush sColor);

    void UpdateBoard(); //To Do



    QVector<BasePiece*> pieces;

    const QVector<QPoint>& CheckPopulatedAreas() const; // ???


private:
    void DrawBoard();
    void DrawPieces();
    void SetPiecesOnBoard(); //To Do
    void AddPiece(BasePiece *piece);

    QGraphicsScene *scene;
    //Square color
    QBrush fColor;
    QBrush sColor;

    //Coordinate font and color
    QBrush textColor;
    QFont font;

};

#endif // CHESSBOARD_H
