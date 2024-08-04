#include "mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , board(nullptr)
    , game(nullptr)
{
    ui->setupUi(this);
    isPlaying = false;
    firstTime = true;

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
    // QWidget::connect(ui->Board2,
    //                  SIGNAL(sendMousePoint(QPointF)),
    //                  this,
    //                  SLOT(setMousePoint(QPointF)));

    //Set up
    ui->stackedWidget->setCurrentWidget(ui->MainMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete board;
    delete game;
}

void MainWindow::ChangeTurnMark(bool isWhiteTrun)
{
    QString turn = isWhiteTrun ? "White" : "Black";
    ui->WhosTurn->setText(turn);
}

void MainWindow::setMousePoint(QPointF point)
{
    int x = 0;
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
    if (board) {
        delete board;
        board = nullptr;
    }
    if (game) {
        delete game;
        game = nullptr;
    }

    board = new ChessBoard();
    game = new Game(board, this);

    ui->WhosTurn->setText("White");
    // ui->Board2->setScene(board->scene);

    // ui->Board2->setViewport(board);

    // ui->Board2 = board;

    connect(ui->Board2, &ChessBoard::pieceSelected, game, &Game::handlePieceSelection);
    connect(game, &Game::pieceMoved, ui->Board2, &ChessBoard::MovePiece);
    connect(ui->Board2, &ChessBoard::endTurn, game, &Game::EndOfTurn);
    connect(game, &Game::UpdateUiToTurn, this, &MainWindow::ChangeTurnMark);
}

void MainWindow::GameOver()
{
    ChangePage(ui->MainMenu);
    isPlaying = false;
    if (board) {
        delete board;
        board = nullptr;
    }
    if (game) {
        delete game;
        game = nullptr;
    }
}
