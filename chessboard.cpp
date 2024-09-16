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
    boardHeight = 8 * squareSize + 30;

    scene = new QGraphicsScene();
    this->setScene(scene);
    scene->setSceneRect(0, 0, boardWidth, boardHeight);

    textColor = Qt::black;

    DrawBoard();
    SetPiecesOnBoard();
    DrawPieces();
    ResetRecords();
}

ChessBoard::~ChessBoard()
{
    for (auto &i : pieces)
    {
        delete i;
    }
    pieces.clear();
    ResetRecords();
    scene->clear();
    delete scene;
}

const QVector<QPoint> ChessBoard::GetPopulatedAreas(QBrush color) const
{
    QVector<QPoint> populatedAreas;
    for (auto& p : pieces)
    {
        if (p->getColor() == color)
        {
            populatedAreas.append(p->getPos());
        }
    }
    return populatedAreas;
}

void ChessBoard::ResetBoard()
{
    for (auto &i : pieces)
    {
        delete i;
    }
    pieces.clear();
    scene->clear();
    DrawBoard();
    SetPiecesOnBoard();
    DrawPieces();
    ResetRecords();
}

bool ChessBoard::IsInsideBoard(const QPoint &pos) const
{
    return pos.x() >= 0 && pos.x() < 8 && pos.y() >= 0 && pos.y() < 8;
}

void ChessBoard::UpdateBoard()
{
    DrawPieces();
    scene->update();
}

BoardSquare* ChessBoard::GetSelectedSquare(const QPoint &pos) const
{
    auto items = scene->items();
    for (auto& i: items)
    {
        BoardSquare* b = qgraphicsitem_cast<BoardSquare*>(i);
        if (!b)
        {
            continue;
        }

        if (b->GetPos() == pos)
        {
            return b;
        }
    }
    return nullptr;
}

void ChessBoard::CheckPassantStatus(BasePiece *piece, const QPoint& from, const QPoint& to, bool isWhiteTurn)
{
    if (piece->getType() == PieceType::Pawn)
    {
        QPoint disCheck = QPoint(from.x(), from.y() - to.y());

        if (disCheck.y() == 2 || disCheck.y() == -2)
        {
            QBrush enemy = isWhiteTurn ? Qt::black : Qt::white;

            for (auto& p : pieces)
            {
                QPoint left = QPoint(to.x() - 1, to.y());
                QPoint right = QPoint(to.x() + 1, to.y());
                if (p->getType() == PieceType::Pawn && p->getColor() == enemy)
                {
                    Pawn *pawn = dynamic_cast<Pawn*>(p);

                    if (p->getPos() == left)
                    {
                        pawn->canPassantRight = true;
                    }
                    if (p->getPos() == right)
                    {
                        pawn->canPassantLeft = true;
                    }
                }
            }
        }
    }
}

void ChessBoard::RecordPiecePositions()
{
    QVector<PieceStateRecord> currentPos;
    for (auto* piece : pieces)
    {
        currentPos.push_back({ piece->getType(), piece->getPos(), piece->GetHashMoved() });
    }
    oldPositions.push_back(currentPos);
}

void ChessBoard::ResetRecords()
{
    moveCount = 0;
    oldPositions.clear();
}

const QVector<QPoint> ChessBoard::GetDangerAreas(bool isWhiteturn, const QVector<QPoint> &friendly, const QVector<QPoint> &enemy,
            const QPoint ignoredPiecePos) const
{

    QVector<QPoint> dangerAreas;
    QBrush enemyColor = isWhiteturn ? Qt::black : Qt::white;
    for (auto& p : pieces)
    {
        if (p->getColor() == enemyColor && ignoredPiecePos != p->getPos())
        {
            dangerAreas.append(p->GetThreateningMoves(enemy, friendly));
        }
    }
    return dangerAreas;
}

void ChessBoard::MovePiece(QPoint from, QPoint to, bool isWhiteTurn)
{

    for (auto &piece : pieces)
    {
        if (piece->getPos() == from)
        {
            piece->Move(to);
            CheckPassantStatus(piece, from, to, isWhiteTurn);
        }
        else
        {
            QBrush turn = isWhiteTurn ? Qt::white : Qt::black;
            if (piece->getType() == PieceType::Pawn && piece->getColor() == turn)
            {
                Pawn *pawn = dynamic_cast<Pawn*>(piece);
                pawn->canPassantLeft = false;
                pawn->canPassantRight = false;
            }
        }
    }
    RecordPiecePositions();
    UpdateBoard();
    emit endTurn();
}

void ChessBoard::SettingSquareColor(const QPoint &pos, const QVector<QPoint>& legalMoves, bool highlighting)
{
    BoardSquare* square = GetSelectedSquare(pos);
    if (square)
    {
        if (highlighting)
        {
            square->SetColor(Qt::green);
        }
        else
        {
            square->SetColor(square->GetColor());
        }
    }

    for (auto& point : legalMoves)
    {
        BoardSquare* square = GetSelectedSquare(point);
        if (square)
        {
            if (highlighting)
            {
                square->SetColor(Qt::blue);
            }
            else
            {
                square->SetColor(square->GetColor());
            }
        }
    }
}

void ChessBoard::EatingPiece(QPoint eatingPos)
{
    ResetRecords();
    auto it = std::remove_if(pieces.begin(), pieces.end(), [&eatingPos](BasePiece* piece)
    {
        if (piece->getPos() == eatingPos)
        {
            delete piece;
            return true;
        }
        return false;
    });
    pieces.erase(it, pieces.end());
}


void ChessBoard::mousePressEvent(QMouseEvent *event)
{
    QPointF pos = event->pos();
    QPoint boardPos((pos.x() - 30) / Constants::SQUARE_SIZE, pos.y() / Constants::SQUARE_SIZE);
    emit pieceSelected(boardPos);
}

void ChessBoard::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void ChessBoard::AddPiece(BasePiece *piece)
{
    pieces.append(piece);
    scene->addItem(piece);
}

void ChessBoard::DrawBoard()
{
    for (int row = 0, revRow = 8; row < 8; ++row, --revRow)
    {
        for (int col = 0; col < 8; ++col)
        {
            QBrush color;
            if ((row + col) % 2 == 0)
            {
                color = Qt::white;
            }
            else
            {
                color = Qt::gray;
            }

            if (col == 0)
            {
                QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem(QString::number(revRow));
                text->setBrush(Qt::black);
                text->setFont(QFont("Arial", 20));
                text->setPos(col * Constants::SQUARE_SIZE - 20, row * Constants::SQUARE_SIZE + 25);
                scene->addItem(text);
            }

            if (row == 7)
            {
                char c = 65 + col;
                QString t = QString(c);
                QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem(t);
                text->setBrush(Qt::black);
                text->setFont(QFont("Arial", 20));
                text->setPos(col * Constants::SQUARE_SIZE + 25, row * Constants::SQUARE_SIZE + 75);
                scene->addItem(text);
            }

            BoardSquare* square = new BoardSquare(color, QPoint(col, row));

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

    // AddPiece(new Pawn(Qt::white, QPoint(0, 4), PieceType::Pawn));
    // AddPiece(new Pawn(Qt::white, QPoint(3, 4), PieceType::Pawn));
    // AddPiece(new Pawn(Qt::white, QPoint(5, 4), PieceType::Pawn));
    // AddPiece(new Pawn(Qt::white, QPoint(7, 4), PieceType::Pawn));



    //Rooks
    AddPiece(new Rook(Qt::white, QPoint(0, 7), PieceType::Rook));
    AddPiece(new Rook(Qt::white, QPoint(7, 7), PieceType::Rook));

    //Knights
    // AddPiece(new Knight(Qt::white, QPoint(4, 4), PieceType::Knight));
    AddPiece(new Knight(Qt::white, QPoint(1, 7), PieceType::Knight));
    AddPiece(new Knight(Qt::white, QPoint(6, 7), PieceType::Knight));

    //Bishops
    AddPiece(new Bishop(Qt::white, QPoint(2, 7), PieceType::Bishop));
    AddPiece(new Bishop(Qt::white, QPoint(5, 7), PieceType::Bishop));

    //Queen
    AddPiece(new Queen(Qt::white, QPoint(3, 7), PieceType::Queen));

    //King
    AddPiece(new King(Qt::white, QPoint(4, 7), PieceType::King));
    // AddPiece(new King(Qt::black, QPoint(4, 7), PieceType::King));

    //Blacks

    // Pawns
    for (int x = 0; x < 8; x++)
    {
        AddPiece(new Pawn(Qt::black, QPoint(x, 1), PieceType::Pawn));
    }

    // AddPiece(new Pawn(Qt::black, QPoint(0, 3), PieceType::Pawn));
    // AddPiece(new Pawn(Qt::black, QPoint(3, 3), PieceType::Pawn));
    // AddPiece(new Pawn(Qt::black, QPoint(5, 2), PieceType::Pawn));
    // AddPiece(new Pawn(Qt::black, QPoint(7, 2), PieceType::Pawn));

    //Rooks
    AddPiece(new Rook(Qt::black, QPoint(0, 0), PieceType::Rook));
    AddPiece(new Rook(Qt::black, QPoint(7, 0), PieceType::Rook));

    // Knights
    AddPiece(new Knight(Qt::black, QPoint(1, 0), PieceType::Knight));
    AddPiece(new Knight(Qt::black, QPoint(6, 0), PieceType::Knight));

    //Bishops
    AddPiece(new Bishop(Qt::black, QPoint(2, 0), PieceType::Bishop));
    AddPiece(new Bishop(Qt::black, QPoint(5, 0), PieceType::Bishop));

    //Queen
    AddPiece(new Queen(Qt::black, QPoint(3, 0), PieceType::Queen));

    //King
    AddPiece(new King(Qt::black, QPoint(4, 0), PieceType::King));
    // AddPiece(new King(Qt::white, QPoint(4, 0), PieceType::King));
}

void ChessBoard::DrawPieces()
{
    for (auto &i : pieces)
    {
        i->Draw();
    }
}
