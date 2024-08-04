#include "chessboard.h"
#include "ConstantValues.h"
#include "Pieces/Bishop.h"
#include "Pieces/King.h"
#include "Pieces/Knight.h"
#include "Pieces/Pawn.h"
#include "Pieces/Queen.h"
#include "Pieces/Rook.h"

ChessBoard::ChessBoard(QWidget *widget)
    : QGraphicsView(widget)
    , fColor(Qt::white)
    , sColor(Qt::gray)
{
    boardWidth = 8 * squareSize;
    boardHeight = 8 * squareSize;

    scene = new QGraphicsScene();
    this->setScene(scene);
    scene->setSceneRect(0, 0, boardWidth, boardHeight);

    textColor = Qt::black;

    DrawBoard();
    SetPiecesOnBoard();
    DrawPieces();
}

ChessBoard::~ChessBoard()
{
    for (auto &i : pieces)
    {
        delete i;
    }
    pieces.clear();
    scene->clear();
    delete scene;
}

void ChessBoard::UpdateBoard()
{
    DrawPieces();
    scene->update();
}

void ChessBoard::MovePiece(QPoint from, QPoint to)
{
    for (auto &piece : pieces)
    {
        if (piece->getPos() == from)
        {
            piece->Move(to);
            break;
        }
    }
    UpdateBoard();
    emit endTurn();
}

void ChessBoard::mousePressEvent(QMouseEvent *event)
{
    QPointF pos = event->pos();
    QPoint boardPos(pos.x() / Constants::SQUARE_SIZE, pos.y() / Constants::SQUARE_SIZE);

    emit pieceSelected(boardPos);
}

void ChessBoard::mouseMoveEvent(QMouseEvent *event)
{
    QPointF movment;
    movment = mapToScene(event->pos());
    emit sendMousePoint(movment);
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
            QGraphicsRectItem *square = new QGraphicsRectItem(col * Constants::SQUARE_SIZE,
                                                              row * Constants::SQUARE_SIZE,
                                                              Constants::SQUARE_SIZE,
                                                              Constants::SQUARE_SIZE);
            if ((row + col) % 2 == 0)
            {
                square->setBrush(fColor);
            }
            else
            {
                square->setBrush(sColor);
            }

            // if (col == 0)
            // {
            //     QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem(QString::number(revRow));
            //     text->setBrush(Qt::black);
            //     text->setFont(QFont("Arial", 20));
            //     text->setPos(col * Constants::SQUARE_SIZE - 25, row * Constants::SQUARE_SIZE + 10);
            //     scene->addItem(text);
            // }

            // if (row == 7)
            // {
            //     char c = 65 + col;
            //     QString t = QString(c);
            //     QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem(t);
            //     text->setBrush(Qt::black);
            //     text->setFont(QFont("Arial", 20));
            //     text->setPos(col * Constants::SQUARE_SIZE + 10, row * Constants::SQUARE_SIZE + 60);
            //     scene->addItem(text);
            // }

            scene->addItem(square);
        }
    }
}

void ChessBoard::SetPiecesOnBoard()
{
    //Whites

    //Pawns
    for (int x = 0; x < 8; x++)
    {
        AddPiece(new Pawn(Qt::white, QPoint(x, 6), PieceType::Pawn));
    }

    //Rooks
    AddPiece(new Rook(Qt::white, QPoint(0, 7), PieceType::Rook));
    AddPiece(new Rook(Qt::white, QPoint(7, 7), PieceType::Rook));

    //Knights
    AddPiece(new Knight(Qt::white, QPoint(1, 7), PieceType::Knight));
    AddPiece(new Knight(Qt::white, QPoint(6, 7), PieceType::Knight));

    //Bishops
    AddPiece(new Bishop(Qt::white, QPoint(2, 7), PieceType::Bishop));
    AddPiece(new Bishop(Qt::white, QPoint(5, 7), PieceType::Bishop));

    //Queen
    AddPiece(new Queen(Qt::white, QPoint(3, 7), PieceType::Queen));

    //King
    AddPiece(new King(Qt::white, QPoint(4, 7), PieceType::King));

    //Blacks

    //Pawns
    for (int x = 0; x < 8; x++)
    {
        AddPiece(new Pawn(Qt::black, QPoint(x, 1), PieceType::Pawn));
    }

    //Rooks
    AddPiece(new Rook(Qt::black, QPoint(0, 0), PieceType::Rook));
    AddPiece(new Rook(Qt::black, QPoint(7, 0), PieceType::Rook));

    //Knights
    AddPiece(new Knight(Qt::black, QPoint(1, 0), PieceType::Knight));
    AddPiece(new Knight(Qt::black, QPoint(6, 0), PieceType::Knight));

    //Bishops
    AddPiece(new Bishop(Qt::black, QPoint(2, 0), PieceType::Bishop));
    AddPiece(new Bishop(Qt::black, QPoint(5, 0), PieceType::Bishop));

    //Queen
    AddPiece(new Queen(Qt::black, QPoint(3, 0), PieceType::Queen));

    //King
    AddPiece(new King(Qt::black, QPoint(4, 0), PieceType::King));
}

void ChessBoard::DrawPieces()
{
    for (auto &i : pieces)
    {
        i->Draw();
    }
}
