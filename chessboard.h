#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QVector>
#include "Pieces/basepiece.h"
#include "qgraphicsitem.h"
#include "qgraphicsview.h"

class ChessBoard : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *widget = 0);
    ~ChessBoard();

    QGraphicsScene *scene;
    QVector<BasePiece *> pieces;

    const QVector<QPoint> &CheckPopulatedAreas() const; // TODO

public slots:
    void MovePiece(QPoint from, QPoint to);

signals:
    void pieceSelected(QPoint pos);
    void endTurn();
    void sendMousePoint(QPointF point);

protected:
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
