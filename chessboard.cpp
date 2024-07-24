#include "chessboard.h"


ChessBoard::ChessBoard(QGraphicsScene *scene, QBrush fColor, QBrush sColor)
    : scene(scene),
    fColor(fColor),
    sColor(sColor)
{
    textColor = Qt::black;

    DrawBoard();
    SetPiecesOnBoard();
    DrawPieces();
}

void ChessBoard::AddPiece(BasePiece *piece)
{
    pieces.append(piece);
    scene->addItem(piece);
}

void ChessBoard::DrawBoard()
{
    // Piirrä laudat
    for (int row = 0, revRow = 8; row < 8; ++row, --revRow)
    {
        for (int col = 0; col < 8; ++col)
        {
            QGraphicsRectItem *square = new QGraphicsRectItem(col * Constants::SQUARE_SIZE, row * Constants::SQUARE_SIZE,
                                                              Constants::SQUARE_SIZE, Constants::SQUARE_SIZE);
            if ((row + col) % 2 == 0)
            {
                square->setBrush(fColor);
            }
            else
            {
                square->setBrush(sColor);
            }

            if (col == 0)
            {
                QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem(QString::number(revRow));
                text->setBrush(Qt::black);
                text->setFont(QFont("Arial", 20));
                text->setPos(col * Constants::SQUARE_SIZE - 25, row * Constants::SQUARE_SIZE + 10);
                scene->addItem(text);
            }

            if (row == 7)
            {
                char c = 65 + col;
                QString t = QString(c);
                QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem(t);
                text->setBrush(Qt::black);
                text->setFont(QFont("Arial", 20));
                text->setPos(col * Constants::SQUARE_SIZE + 10, row * Constants::SQUARE_SIZE + 60);
                scene->addItem(text);
            }

            scene->addItem(square);

        }
    }

}

void ChessBoard::SetPiecesOnBoard()
{
    //Whites
    for (int y = 6; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (y == 6)
            {
                AddPiece(new Queen(Qt::white, QPoint(x, y), PieceType::Queen ));
            }
        }
    }
}

void ChessBoard::DrawPieces()
{
    for (auto& i : pieces)
    {
        i->Draw();
    }
}
