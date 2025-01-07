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
    botDifficulty = 2;
    ui->setupUi(this);
    isPlaying = false;
    ui->Board->setMouseTracking(true); //Needed??
    ui->stackedWidget->setCurrentWidget(ui->MainMenu);
    ui->Difficulty_Btn->setText("Easy");


    //Navigation connections
    connect(ui->Exit, &QPushButton::clicked, this, &MainWindow::kill);
    connect(ui->PlayerVsBot_Btn, &QPushButton::clicked, this, [this]() {
        ChangePage(ui->SinglePlayerMenu);
    });
    connect(ui->SettingsNav_Btn, &QPushButton::clicked, this, [this]() {ChangePage(ui->SettingsPage);});
    connect(ui->Difficulty_Btn, &QPushButton::clicked, this, [this] () {SetBotDifficulty();});
    connect(ui->BackToSingleMenu_Btn, &QPushButton::clicked, this, [this]() {ChangePage(ui->SinglePlayerMenu);});

    connect(ui->PlayerVsPlayer_Btn, &QPushButton::clicked, this, [this]() {ChangePage(ui->MultiplayerMenu);});

    connect(ui->BackToMain, &QPushButton::clicked, this, [this]() { ChangePage(ui->MainMenu); });
    connect(ui->BackToMain_2, &QPushButton::clicked, this, [this]() { ChangePage(ui->MainMenu); });

    connect(ui->PlayBot_Btn, &QPushButton::clicked, this, [this]() { StartGame(true); });
    connect(ui->PlayHuman_Btn, &QPushButton::clicked, this, [this]() { StartGame(false); });
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
    QString endStatus;
    switch (status)
    {
    case EndStatus::BlackWins:
        endStatus = "Black wins";
        break;
    case EndStatus::WhiteWins:
        endStatus = "White wins";
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


    ui->GameEnds_Lbl->setHidden(false);
    ui->EndStatus_Lbl->setHidden(false);
    ui->EndStatus_Lbl->setText(endStatus);
    ui->LastMove->setEnabled(false);

}

void MainWindow::kill()
{
    QCoreApplication::quit();
}

void MainWindow::ChangePage(QWidget *widget)
{
    ui->stackedWidget->setCurrentWidget(widget);
}

void MainWindow::SetBotDifficulty()
{
    botDifficulty++;
    if (botDifficulty > 4)
    {
        botDifficulty = 2;
    }

    if (botDifficulty == 2)
    {
        ui->Difficulty_Btn->setText("Easy");
    }
    else if (botDifficulty == 3)
    {
        ui->Difficulty_Btn->setText("Medium");
    }
    else if (botDifficulty == 4)
    {
        ui->Difficulty_Btn->setText("Hard");
    }

}

void MainWindow::StartGame(bool playingAgainstBot)
{
    ChangePage(ui->PlayScreen);
    isPlaying = true;
    ui->GameEnds_Lbl->setHidden(true);
    ui->EndStatus_Lbl->setHidden(true);
    ui->EndStatus_Lbl->setText("");
    ui->LastMove->setEnabled(true);

    if (game) {
        delete game;
        game = nullptr;
    }

    game = new Game(ui->Board, playingAgainstBot, botDifficulty, this);

    ui->WhosTurn->setText("White");
    ChangeCheckedStatus(false);

    //Gameplay connections, uses template wrapper.

    //Movements and eating
    trackConnection(ui->Board, &ChessBoard::pieceSelected, game, &Game::handlePieceSelection);
    trackConnection(game, &Game::pieceMoved, ui->Board, &ChessBoard::MovePiece);
    trackConnection(game, &Game::EatPiece, ui->Board, &ChessBoard::EatingPiece);
    trackConnection(game, &Game::PawnPromotion, ui->Board, &ChessBoard::PromotingPawn);

    //Marking selection and available moves
    trackConnection(game, &Game::SetSquareColor, ui->Board, &ChessBoard::SettingSquareColor);

    //Changing turn
    trackConnection(ui->Board, &ChessBoard::endTurn, game, &Game::EndOfTurn);
    trackConnection(game, &Game::UpdateUiToTurn, this, &MainWindow::ChangeTurnMark);
    trackConnection(game, &Game::UpdateUiForCheck, this, &MainWindow::ChangeCheckedStatus);
    trackConnection(game, &Game::UpdateUiForGameOver, this, &MainWindow::GameOver);


    trackConnection(ui->LastMove, &QPushButton::clicked, [this]() {
        ui->Board->ReverseMoveAndTurn(game->isPlayingAgainstBot());
    });
    trackConnection(ui->Board, &ChessBoard::HandleReverseMove, game, &Game::HandleReverseMove);


}

void MainWindow::ExitGame()
{
    ResetConnections();
    ChangePage(ui->MainMenu);
    isPlaying = false;
    ui->Board->ResetBoard();


    if (game) {
        delete game;
        game = nullptr;
    }
}

void MainWindow::ResetConnections()
{
    for (const auto &connection : connectionList)
    {
        disconnect(connection);
    }
    connectionList.clear();
}
