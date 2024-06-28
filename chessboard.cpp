#include "chessboard.h"


ChessBoard::ChessBoard(QGraphicsScene *scene, QBrush fColor, QBrush sColor)
    : scene(scene),
    fColor(fColor),
    sColor(sColor)
{
    DrawBoard();
}

void ChessBoard::DrawBoard()
{
    int squareSize = 50;
    // Piirrä laudat
    for (int row = 0, revRow = 8; row < 8; ++row, --revRow)
    {
        for (int col = 0; col < 8; ++col)
        {
            QGraphicsRectItem *square = new QGraphicsRectItem(col * squareSize, row * squareSize, squareSize, squareSize);
            if ((row + col) % 2 == 0) {
                square->setBrush(fColor);
            } else {
                square->setBrush(sColor);
            }

            if (col == 0)
            {
                QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem(QString::number(revRow));
                text->setBrush(Qt::black);
                text->setFont(QFont("Arial", 20));
                text->setPos(col * squareSize - 25, row * squareSize + 10);
                scene->addItem(text);
            }

            if (row == 7)
            {
                char c = 65 + col;
                QString t = QString(c);
                QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem(t);
                text->setBrush(Qt::black);
                text->setFont(QFont("Arial", 20));
                text->setPos(col * squareSize + 10, row * squareSize + 60);
                scene->addItem(text);
            }

            scene->addItem(square);

        }
    }

}

void ChessBoard::DrawCoordinates()
{

}
