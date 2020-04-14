
#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QWidget>
#include <QtWidgets>


QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

class DragWidget : public QWidget
{
    Q_OBJECT
    QSet<QString>  set;
    QLabel      *label;
    QLabel  *timeLabel;

    int x;
    int y;

public:
    explicit DragWidget(QWidget *parent = nullptr);
    virtual ~DragWidget() override {
        delete label;
        delete timeLabel;
    }

private slots:
    void slotTimerAlarm();
    bool nextStep(QString str);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

};

#endif // DRAGWIDGET_H
