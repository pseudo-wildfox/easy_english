#ifndef QUIZ_H
#define QUIZ_H

#include <QtWidgets>



class Quiz : public QWidget
{
    Q_OBJECT
private:
    QLabel    *shownWord;
    QRadioButton  *radio;
    QLabel        *right;
    QLabel        *wrong;
    int        lineCount;

public:
    explicit Quiz(QWidget *parent = nullptr);
    virtual ~Quiz();
    int getRadioIndex();
private slots:
    void next();
};

#endif // QUIZ_H
