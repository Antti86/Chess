#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
    , board(nullptr)
{
    ui->setupUi(this);
    isPlaying = false;

    //Navigation connections
    connect(ui->Exit, &QPushButton::clicked, this, &MainWindow::kill);
    connect(ui->SinglePlayer, &QPushButton::clicked, this, [this] () {ChangePage(ui->SinglePlayerMenu);});
    connect(ui->BackToMain, &QPushButton::clicked, this, [this]() {ChangePage(ui->MainMenu);});
    connect(ui->BackToMain_2, &QPushButton::clicked, this, [this]() {ChangePage(ui->MainMenu);});

    connect(ui->Play, &QPushButton::clicked, this, [this]() {StartGame();});
    connect(ui->ExitGame, &QPushButton::clicked, this, [this](){GameOver();});

    //Set up
    ui->stackedWidget->setCurrentWidget(ui->MainMenu);

    //scene = new QGraphicsScene(this);
    //ui->Board->setScene(scene);



    //QPixmap pixmap(":/Images/Chessboard.png");

    //QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap);
    //scene->addItem(item);

    //ui->Board->fitInView(item, Qt::KeepAspectRatio);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete board;
}

void MainWindow::kill()
{
    QCoreApplication::quit();
}

void MainWindow::ChangePage(QWidget* widget)
{
    ui->stackedWidget->setCurrentWidget(widget);
}

void MainWindow::StartGame()
{
    ChangePage(ui->PlayScreen);
    isPlaying = true;
    ui->Board->setScene(scene);
    if (board)
    {
        delete board;
    }
    board = new ChessBoard(scene, Qt::white, Qt::gray);

}

void MainWindow::GameOver()
{
    ChangePage(ui->MainMenu);
    isPlaying = false;
    if (board)
    {
        delete board;
        board = nullptr;
    }

}





