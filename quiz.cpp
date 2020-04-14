#include "quiz.h"
#include <QtWidgets>
#include <QDebug>
#include "xmlapi.h"
#include "proxysingleton.h"

Quiz::Quiz(QWidget *parent)
    : QWidget(parent)
    , lineCount(4)
{
    shownWord = new QLabel();
    radio = new QRadioButton[lineCount];
    right = new QLabel();
    wrong = new QLabel();


    QVBoxLayout* layout = new QVBoxLayout(this);
    //layout->setSpacing(5);
    layout->setAlignment( Qt::AlignBaseline);
    this->setLayout(layout);


    right->setStyleSheet("color: rgb(0, 255, 0)");
    right->setAlignment(Qt::AlignLeft);
    right->setNum(0);

    wrong->setStyleSheet("color: rgb(124, 10, 2)");
    wrong->setAlignment(Qt::AlignRight);
    wrong->setNum(0);

    shownWord->setAutoFillBackground(true);
    shownWord->setAlignment(Qt::AlignHCenter);
    QFont font = shownWord->font();
    font.setPixelSize(20);
    shownWord->setFont(font);


    auto lineA = new QFrame(this);
    lineA->setFrameShape(QFrame::HLine);
    lineA->setFrameShadow(QFrame::Sunken);


    QPushButton* nextButton = new QPushButton("Next", this);
    connect(nextButton, &QPushButton::clicked, this, &Quiz::next);

    QHBoxLayout* childLayout = new QHBoxLayout();

    childLayout->addWidget(right);
    childLayout->addWidget(wrong);


    layout->addStretch();
    layout->addWidget(shownWord);
    layout->addStretch();
    layout->addWidget(lineA);
    for (int i=0; i<lineCount; i++) {
        layout->addWidget(&radio[i]);
    }
    layout->addStretch();
    layout->addWidget(nextButton);
    layout->addLayout(childLayout);


    emit next(); //инициализация строк

    this->resize(200, 200);
    setWindowTitle(tr("Quiz"));

}

Quiz::~Quiz() {
    delete shownWord;
    delete radio;
    delete right;
    delete wrong;
}

int Quiz::getIndexOfCheckedRadio()
{
    for (int i=0; i<lineCount; i++) {
        if (radio[i].isChecked() )
            return i;
    }
    throw -1;
}

void Quiz::next()
{
    static QMap<QString, QString> map;
    static QString key;

    QRandomGenerator rand = QRandomGenerator::securelySeeded();
    if (!key.isEmpty()) {
        QString str1 = map[key];
        QString str2 = radio[getIndexOfCheckedRadio()].text();

        if (QString::compare(str1, str2) == 0 ) {
            right->setNum(right->text().toInt() + 1);
        } else {
            wrong->setNum(wrong->text().toInt() + 1);
            QMessageBox msg;
            msg.setText(key + " - " + map[key]);
            msg.exec();
        }
    }
    map = ProxySingleton<XmlApi>::instance().getRand(lineCount);
    auto it = map.begin();
    key = (it+rand.bounded(lineCount)).key();

    shownWord->setText(key);

    for (int i=0; i<lineCount; i++, it++) {
        radio[i].setText(it.value() );
    }
}

