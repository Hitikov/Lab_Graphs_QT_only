#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphwidget.h"

//Инициализация главного окна
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget::setWindowTitle("Graph graphics");
    GraphWidget *widget = new GraphWidget(ui->graphicsView);

    //Связывание сигналов кнопок с соответствующими слотами графического виджета
    connect(ui->userAddNode, &QPushButton::clicked,
            widget, &GraphWidget::AddNode);
    connect(ui->userDeleteNode, &QPushButton::clicked,
            widget, &GraphWidget::DeleteNodeSetup);
    connect(ui->userSetEdge, &QPushButton::clicked,
            widget, &GraphWidget::SetEdgeSetup1);
    connect(ui->userDeleteEdge, &QPushButton::clicked,
            widget, &GraphWidget::DeleteEdgeSetup);
    connect(ui->userShortestPath, &QPushButton::clicked,
            widget, &GraphWidget::ShortestPathSetup1);
    connect(ui->userKomiTask, &QPushButton::clicked,
            widget, &GraphWidget::KomiTask);

    //Связывание сигналов графического виджета с соответствующими слотами полей
    connect(widget, &GraphWidget::SetNewCommentAct,
            ui->commentAct, &QLabel::setText);
    connect(widget, &GraphWidget::SetNewCommentPath,
            ui->commentPath, &QLabel::setText);
    connect(widget, &GraphWidget::SetNewPathList,
            this, &MainWindow::AddNewPathList);
    connect(widget, &GraphWidget::SetPathListClear,
            this, &MainWindow::PathListClear);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Добавление нового элемента с поле путей
void MainWindow::AddNewPathList(QString argv1)
{
    ui->listPath->addItem(argv1);
}

//Отчистка поля путей
void MainWindow::PathListClear()
{
    ui->listPath->clear();
}
