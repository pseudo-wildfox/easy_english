#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    window = new QWidget(this);
    quiz = new Quiz();
    dragwin = nullptr;
    setWindowTitle(tr("Easy English"));


    setCentralWidget(window);

    QVBoxLayout* layout = new QVBoxLayout(window);
    layout->setContentsMargins(15, 10, 15, 0);
    layout->setSpacing(5);

    window->setLayout(layout);

    QPushButton* puzzle = new QPushButton("Puzzle", window);
    QPushButton* quiz = new QPushButton("Quiz", window);
    QPushButton* exitButton = new QPushButton("Exit", window);

    layout->addWidget(quiz);
    layout->addWidget(puzzle);


    auto lineA = new QFrame(window);
    lineA->setFrameShape(QFrame::HLine);
    lineA->setFrameShadow(QFrame::Sunken);
    layout->addWidget(lineA);

    layout->addWidget(exitButton);


    QMenu *file = menuBar()->addMenu("App");
    QAction * about = new QAction(tr("About app"), this);
    QAction * exit = new QAction(tr("Exit"), this);
    file->addAction(about);
    file->addAction(exit);


    connect(about, SIGNAL(triggered()), this, SLOT(about()));
    connect(exit, SIGNAL(triggered()), this, SLOT(actionExit()));
    connect(quiz, SIGNAL(clicked()), this, SLOT(actionQuiz()));
    connect(puzzle, SIGNAL(clicked()), this, SLOT(actionPuzzle()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(actionExit()));
/*

    QFile dictionaryFile(QStringLiteral(":/dictionary/words.txt"));
    dictionaryFile.open(QIODevice::ReadOnly);
    QTextStream inputStream(&dictionaryFile);

    while (!inputStream.atEnd()) {
        QString word;
        inputStream >> word;

*/
}

MainWindow::~MainWindow()
{
    delete ui;
    delete window;
    if (dragwin) delete dragwin;
    if (quiz) delete quiz;
}

void MainWindow::actionPuzzle() {
    if (dragwin != nullptr) {
        if (dragwin->isHidden()) {
            dragwin->show();
        }
    } else {
        dragwin = new DragWidget();
        dragwin->show();
    }
}

void MainWindow::actionQuiz() {
    if (quiz->isHidden()) {
        quiz->show();
    }
}

void MainWindow::about() {
    QMessageBox msg;
    msg.setText("[EasyEnglish]\nVersion: 0.01\nAuthor: Akhmetov Timur");
    msg.exec();
}

void MainWindow::actionExit() {
    close();
}
