#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
    , board(new ChessBoard(scene, Qt::white, Qt::gray))
{
    ui->setupUi(this);

    //Navigation connections
    connect(ui->Exit, &QPushButton::clicked, this, &MainWindow::kill);
    connect(ui->SinglePlayer, &QPushButton::clicked, this, [this] () {ChangePage(ui->SinglePlayerMenu);});
    connect(ui->BackToMain, &QPushButton::clicked, this, [this]() {ChangePage(ui->MainMenu);});
    connect(ui->BackToMain_2, &QPushButton::clicked, this, [this]() {ChangePage(ui->MainMenu);});
    connect(ui->Play, &QPushButton::clicked, this, [this]() {ChangePage(ui->PlayScreen);});

    //Set up
    ui->stackedWidget->setCurrentWidget(ui->MainMenu);

    //scene = new QGraphicsScene(this);
    ui->Board->setScene(scene);



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





