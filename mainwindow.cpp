#include "mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , game(nullptr)
{
    //Set up
    ui->setupUi(this);
    isPlaying = false;
    ui->Board->setMouseTracking(true); //Needed??
    ui->stackedWidget->setCurrentWidget(ui->MainMenu);

    //Navigation connections
    connect(ui->Exit, &QPushButton::clicked, this, &MainWindow::kill);
    connect(ui->SinglePlayer, &QPushButton::clicked, this, [this]() {
        ChangePage(ui->SinglePlayerMenu);
    });
    connect(ui->BackToMain, &QPushButton::clicked, this, [this]() { ChangePage(ui->MainMenu); });
    connect(ui->BackToMain_2, &QPushButton::clicked, this, [this]() { ChangePage(ui->MainMenu); });
    connect(ui->WinExit, &QPushButton::clicked, this, [this]() { ChangePage(ui->MainMenu); });

    connect(ui->Play, &QPushButton::clicked, this, [this]() { StartGame(); });
    connect(ui->ExitGame, &QPushButton::clicked, this, [this]() { ExitGame(); });


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

void MainWindow::ChangeCheckedStatus(bool isChecked)
{
    if (isChecked)
    {
        ui->Checked->setText("Check");
    }
    else
    {
        ui->Checked->setText("");
    }

}

void MainWindow::GameOver(EndStatus status)
{
    ChangePage(ui->WinScreen);
    QString endStatus;
    switch (status)
    {
    case EndStatus::BlackWins:
        endStatus = "White";
        break;
    case EndStatus::WhiteWins:
        endStatus = "Black";
        break;
    case EndStatus::InsufficientMaterialDraw:
        endStatus = "Insufficient Material Draw";
        break;
    case EndStatus::StaleMate:
        endStatus = "Stalemate";
        break;
    case EndStatus::DeadDraw:
        endStatus = "Dead Draw";
        break;
    case EndStatus::ThreefoldDraw:
        endStatus = "Threefold Draw";
        break;
    case EndStatus::FiftyMovesDraw:
        endStatus = "Fifty Move Draw";
        break;
    }

    ui->Winner->setText(endStatus);

    isPlaying = false;
    ui->Board->ResetBoard();

    if (game) {
        delete game;
        game = nullptr;
    }
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
    ChangePage(ui->PlayScreen);
    isPlaying = true;

    if (game) {
        delete game;
        game = nullptr;
    }

    game = new Game(ui->Board, this);

    ui->WhosTurn->setText("White");


    //Gameplay connections

    //Movements and eating
    connect(ui->Board, &ChessBoard::pieceSelected, game, &Game::handlePieceSelection);
    connect(game, &Game::pieceMoved, ui->Board, &ChessBoard::MovePiece);
    connect(game, &Game::EatPiece, ui->Board, &ChessBoard::EatingPiece);

    //Marking selection and available moves
    connect(game, &Game::SetSquareColor, ui->Board, &ChessBoard::SettingSquareColor);

    //Changing turn
    connect(ui->Board, &ChessBoard::endTurn, game, &Game::EndOfTurn);
    connect(game, &Game::UpdateUiToTurn, this, &MainWindow::ChangeTurnMark);
    connect(game, &Game::UpdateUiForCheck, this, &MainWindow::ChangeCheckedStatus);
    connect(game, &Game::UpdateUiForGameOver, this, &MainWindow::GameOver);


}

void MainWindow::ExitGame()
{
    ChangePage(ui->MainMenu);
    isPlaying = false;
    ui->Board->ResetBoard();

    if (game) {
        delete game;
        game = nullptr;
    }
}
