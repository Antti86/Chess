#ifndef PROMOTIONDIALOG_H
#define PROMOTIONDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <Pieces/basepiece.h>

class PromotionDialog : public QDialog
{
    Q_OBJECT
public:
    PromotionDialog(QWidget* parent = nullptr) : QDialog(parent)
    {
        setWindowTitle("Pawn Promotion");
        QVBoxLayout* layout = new QVBoxLayout(this);

        QPushButton* queenButton = new QPushButton("Queen", this);
        QPushButton* rookButton = new QPushButton("Rook", this);
        QPushButton* bishopButton = new QPushButton("Bishop", this);
        QPushButton* knightButton = new QPushButton("Knight", this);

        layout->addWidget(queenButton);
        layout->addWidget(rookButton);
        layout->addWidget(bishopButton);
        layout->addWidget(knightButton);

        connect(queenButton, &QPushButton::clicked, this, [this]() { selectedPiece = PieceType::Queen; accept(); });
        connect(rookButton, &QPushButton::clicked, this, [this]() { selectedPiece = PieceType::Rook; accept(); });
        connect(bishopButton, &QPushButton::clicked, this, [this]() { selectedPiece = PieceType::Bishop; accept(); });
        connect(knightButton, &QPushButton::clicked, this, [this]() { selectedPiece = PieceType::Knight; accept(); });
    }

    PieceType getSelectedPiece() const { return selectedPiece; }

private:
    PieceType selectedPiece;
};


#endif // PROMOTIONDIALOG_H
