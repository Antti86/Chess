#include "mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , game(nullptr)
{
    ui->setupUi(this);
    isPlaying = false;

    //Navigation connections
    connect(ui->Exit, &QPushButton::clicked, this, &MainWindow::kill);
    connect(ui->SinglePlayer, &QPushButton::clicked, this, [this]() {
        ChangePage(ui->SinglePlayerMenu);
    });
    connect(ui->BackToMain, &QPushButton::clicked, this, [this]() { ChangePage(ui->MainMenu); });
    connect(ui->BackToMain_2, &QPushButton::clicked, this, [this]() { ChangePage(ui->MainMenu); });

    connect(ui->Play, &QPushButton::clicked, this, [this]() { StartGame(); });
    connect(ui->ExitGame, &QPushButton::clicked, this, [this]() { GameOver(); });
    connect(ui->ExitGame2, &QPushButton::clicked, this, [this]() { GameOver(); });

    ui->Board2->setMouseTracking(true);

    //Set up
    ui->stackedWidget->setCurrentWidget(ui->MainMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete game;
}

void MainWindow::ChangeTurnMark(bool isWhiteTrun)
{
    QString turn = isWhiteTrun ? "White" : "Black";
    ui->WhosTurn->setText(turn);
}

void MainWindow::kill()
{
    QCoreApplication::quit();
}

void MainWindow::ChangePage(QWidget *widget)
{
    ui->stackedWidget->setCurrentWidget(widget);
}

void MainWindow::StartGame()
{
    // ChangePage(ui->PlayScreen);
    ChangePage(ui->Testi);
    isPlaying = true;

    if (game) {
        delete game;
        game = nullptr;
    }

    game = new Game(ui->Board2, this);

    ui->WhosTurn->setText("White");

    connect(ui->Board2, &ChessBoard::pieceSelected, game, &Game::handlePieceSelection);
    connect(game, &Game::pieceMoved, ui->Board2, &ChessBoard::MovePiece);
    connect(ui->Board2, &ChessBoard::endTurn, game, &Game::EndOfTurn);
    connect(game, &Game::UpdateUiToTurn, this, &MainWindow::ChangeTurnMark);
    connect(game, &Game::SetSquareColor, ui->Board2, &ChessBoard::SettingSquareColor);
    connect(game, &Game::EatPiece, ui->Board2, &ChessBoard::EatingPiece);
}

void MainWindow::GameOver()
{
    ChangePage(ui->MainMenu);
    isPlaying = false;
    ui->Board2->ResetBoard();

    if (game) {
        delete game;
        game = nullptr;
    }
}
