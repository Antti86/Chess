#ifndef PROMOTIONDIALOG_H
#define PROMOTIONDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <Pieces/basepiece.h>
#include <qevent.h>

class PromotionDialog : public QDialog
{
    Q_OBJECT
public:
    PromotionDialog(bool isWhiteTurn, QWidget* parent = nullptr) : QDialog(parent)
    {
        setWindowTitle("Pawn Promotion");
        setGeometry(300, 300, 300, 300);

        QVBoxLayout* layout = new QVBoxLayout(this);

        QPushButton* queenButton = new QPushButton("Queen", this);
        QPixmap qPixmap = GetIcon(isWhiteTurn, "Queen");
        QIcon qIcon(qPixmap);
        queenButton->setIcon(qIcon);
        queenButton->setIconSize(qPixmap.rect().size());

        QPushButton* rookButton = new QPushButton("Rook", this);
        QPixmap rPixmap = GetIcon(isWhiteTurn, "Rook");
        QIcon rIcon(rPixmap);
        rookButton->setIcon(rIcon);
        rookButton->setIconSize(rPixmap.rect().size());

        QPushButton* bishopButton = new QPushButton("Bishop", this);
        QPixmap bPixmap = GetIcon(isWhiteTurn, "Bishop");
        QIcon bIcon(bPixmap);
        bishopButton->setIcon(bIcon);
        bishopButton->setIconSize(bPixmap.rect().size());

        QPushButton* knightButton = new QPushButton("Knight", this);
        QPixmap kPixmap = GetIcon(isWhiteTurn, "Knight");
        QIcon kIcon(kPixmap);
        knightButton->setIcon(kIcon);
        knightButton->setIconSize(kPixmap.rect().size());

        layout->addWidget(queenButton);
        layout->addWidget(rookButton);
        layout->addWidget(bishopButton);
        layout->addWidget(knightButton);

        connect(queenButton, &QPushButton::clicked, this, [this]() { selectedPiece = PieceType::Queen; accept(); });
        connect(rookButton, &QPushButton::clicked, this, [this]() { selectedPiece = PieceType::Rook; accept(); });
        connect(bishopButton, &QPushButton::clicked, this, [this]() { selectedPiece = PieceType::Bishop; accept(); });
        connect(knightButton, &QPushButton::clicked, this, [this]() { selectedPiece = PieceType::Knight; accept(); });
    }

    void closeEvent(QCloseEvent* event) override
    {
        event->ignore();
    }

    PieceType getSelectedPiece() const { return selectedPiece; }

private:
    PieceType selectedPiece;

    QPixmap GetIcon(bool isWhiteTurn, QString type)
    {
        QPixmap pixmap;
        if (type == "Queen")
        {
            pixmap = isWhiteTurn ? QPixmap(":/Images/w_queen.png") : QPixmap(":/Images/b_queen.png");
        }
        else if (type == "Rook")
        {
            pixmap = isWhiteTurn ? QPixmap(":/Images/w_rook.png") : QPixmap(":/Images/b_rook.png");
        }
        else if (type == "Knight")
        {
            pixmap = isWhiteTurn ? QPixmap(":/Images/w_knight.png") : QPixmap(":/Images/b_knight.png");
        }
        else if (type == "Bishop")
        {
            pixmap = isWhiteTurn ? QPixmap(":/Images/w_bishop.png") : QPixmap(":/Images/b_bishop.png");
        }

        QPixmap scaledPixmap = pixmap.scaled(Constants::SQUARE_SIZE - 10, Constants::SQUARE_SIZE - 10, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        return scaledPixmap;
    }
};


#endif // PROMOTIONDIALOG_H
