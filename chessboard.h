#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsScene>
#include "qgraphicsitem.h"
#include <QVector>
#include "Pieces/basepiece.h"
#include "qgraphicsview.h"
#include <QMouseEvent>


class ChessBoard : public QGraphicsView
{
    Q_OBJECT
public:
    ChessBoard(QGraphicsScene *scene, QBrush fColor, QBrush sColor, QGraphicsView *view, QWidget *parent = nullptr);


    QVector<BasePiece*> pieces;

    const QVector<QPoint>& CheckPopulatedAreas() const; // TODO

public slots:
    void MovePiece(QPoint from, QPoint to);

signals:
    void pieceSelected(QPoint pos);
    void endTurn();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    void DrawBoard();
    void DrawPieces();
    void SetPiecesOnBoard();
    void AddPiece(BasePiece *piece);
    void UpdateBoard();
    QGraphicsScene *scene;
    QGraphicsView *view;
    //Square color
    QBrush fColor;
    QBrush sColor;

    //Coordinate font and color
    QBrush textColor;
    QFont font;

};


#endif // CHESSBOARD_H
