#include "quiz.h"
#include <QtWidgets>
#include <QDebug>
#include "xmlapi.h"
#include "proxysingleton.h"

Quiz::Quiz(QWidget *parent)
    : QWidget(parent)
    , lineCount(4)
{
    //setMinimumSize(150, 200);
    this->resize(200, 200);
    setWindowTitle(tr("Quiz"));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(5);
    layout->setAlignment( Qt::AlignBaseline);

    this->setLayout(layout);

    shownWord = new QLabel();
    radio = new QRadioButton[lineCount];
    right = new QLabel();
    wrong = new QLabel();

    right->setStyleSheet("color: rgb(0, 255, 0)");
    right->setAlignment(Qt::AlignLeft);
    right->setNum(0);

    wrong->setStyleSheet("color: rgb(124, 10, 2)");
    wrong->setAlignment(Qt::AlignRight);
    wrong->setNum(0);

    shownWord->setAutoFillBackground(true);
    shownWord->setAlignment(Qt::AlignHCenter);

    layout->addStretch();
    layout->addWidget(shownWord);
    layout->addStretch();

    auto lineA = new QFrame(this);
    lineA->setFrameShape(QFrame::HLine);
    lineA->setFrameShadow(QFrame::Sunken);
    layout->addWidget(lineA);



    for (int i=0; i<lineCount; i++) {
        layout->addWidget(&radio[i]);
    }
    //radio->setChecked(true);

    layout->addStretch();

    QPushButton* nextButton = new QPushButton("Next", this);
    connect(nextButton, &QPushButton::clicked, this, &Quiz::next);
    layout->addWidget(nextButton);

    QHBoxLayout* childLayout = new QHBoxLayout();
    layout->addLayout(childLayout);
    childLayout->addWidget(right);
    childLayout->addWidget(wrong);

    emit next(); //инициализация строк


}

Quiz::~Quiz() {
    delete shownWord;
    delete radio;
    delete right;
    delete wrong;
}

int Quiz::getRadioIndex()
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
        QString str2 = radio[getRadioIndex()].text();
        //qDebug() << str1 << ' ' << str2;
        if (QString::compare(str1, str2) == 0 ) {
            right->setNum(right->text().toInt() + 1);
            //qDebug() << " !key.isEmpty(map[key])";
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

    shownWord->setText( key );

    for (int i=0; i<lineCount; i++, it++) {
        radio[i].setText(it.value() );
    }
}

