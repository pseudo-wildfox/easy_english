#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtWidgets>
#include "xmlapi.h"
#include "dragwidget.h"
#include "quiz.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

private:
    Ui::MainWindow  *ui;
    QWidget     *window;
    DragWidget  *dragwin;
    Quiz        *quiz;

private slots:

    void actionPuzzle();
    void actionQuiz();
    void about();
    void actionExit();

};
#endif // MAINWINDOW_H
