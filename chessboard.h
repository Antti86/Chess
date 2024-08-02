#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsScene>
#include "qgraphicsitem.h"
#include <QVector>
#include "Pieces/basepiece.h"
#include "qgraphicsview.h"
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>



class ChessBoard : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ChessBoard(QBrush fColor, QBrush sColor, QWidget *widget = 0);
    ~ChessBoard();

    QGraphicsScene *scene;
    QVector<BasePiece*> pieces;

    const QVector<QPoint>& CheckPopulatedAreas() const; // TODO

public slots:
    void MovePiece(QPoint from, QPoint to);


signals:
    void pieceSelected(QPoint pos);
    void endTurn();
    void sendMousePoint(QPointF point);

public:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override; //TODO

private:
    void DrawBoard();
    void DrawPieces();
    void SetPiecesOnBoard();
    void AddPiece(BasePiece *piece);
    void UpdateBoard();

    //Square color
    QBrush fColor;
    QBrush sColor;

    //Coordinate font and color
    QBrush textColor;
    QFont font;

    //Testing for now..
    int squareSize = 70;
    int boardWidth = 8 * squareSize;
    int boardHeight = 8 * squareSize;
    QGraphicsRectItem fullBoardRect;

};


#endif // CHESSBOARD_H
