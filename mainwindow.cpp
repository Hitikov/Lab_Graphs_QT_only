#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    GraphWidget *widget = new GraphWidget(ui->graphicsView);

    connect(ui->userAddNode, &QPushButton::clicked, widget, &GraphWidget::AddNode);
    connect(ui->userDeleteNode, &QPushButton::clicked, widget, &GraphWidget::DeleteNodeSetup);
    connect(ui->userSetEdge, &QPushButton::clicked, widget, &GraphWidget::SetEdgeSetup1);
    connect(ui->userDeleteEdge, &QPushButton::clicked, widget, &GraphWidget::DeleteEdgeSetup);
    connect(ui->userShortestPath, &QPushButton::clicked, widget, &GraphWidget::ShortestPathSetup1);
    connect(ui->userKomiTask, &QPushButton::clicked, widget, &GraphWidget::KomiTask);

    connect(widget, &GraphWidget::SetNewCommentAct, ui->commentAct, &QLabel::setText);
    connect(widget, &GraphWidget::SetNewCommentPath, ui->commentPath, &QLabel::setText);
    connect(widget, &GraphWidget::SetNewPathList, this, &MainWindow::AddNewPathList);
    connect(widget, &GraphWidget::SetPathListClear, this, &MainWindow::PathListClear);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddNewPathList(QString argv1)
{
    ui->listPath->addItem(argv1);
}

void MainWindow::PathListClear()
{
    ui->listPath->clear();
}
