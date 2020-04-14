
#include "dragwidget.h"
#include <QDomDocument>
#include <QDebug>
#include <QPainter>
#include "xmlapi.h"
#include "proxysingleton.h"



static QLabel *createDragLabel(const QString &text, QWidget *parent)
{
    QLabel *label = new QLabel(text, parent);
    label->setAutoFillBackground(true);
    label->setFrameShape(QFrame::Panel);
    label->setFrameShadow(QFrame::Raised);
    QFont font = label->font();
    font.setPixelSize(16);
    label->setFont(font);
    return label;
}

static QString hotSpotMimeDataKey() { return QStringLiteral("application/x-hotspot"); }

DragWidget::DragWidget(QWidget *parent)
    : QWidget(parent)
{
    x = 5;
    y = 5;
    ProxySingleton<XmlApi> & dataBase = ProxySingleton<XmlApi>::instance();
    for(int i=0; i < dataBase.size(); i++) {
        QString word;
        word = dataBase.at(i).first;
        descSet.insert(dataBase.at(i).second);
        if (!word.isEmpty()) {
            QLabel *wordLabel = createDragLabel(word, this);
            wordLabel->move(x, y);
            wordLabel->show();
            wordLabel->setAttribute(Qt::WA_DeleteOnClose);
            x += wordLabel->width() + 2;
            if (x >= 245) {
                x = 5;
                y += wordLabel->height() + 2;
            }
        }
    }



    this->shownDesc = new QLabel(this);
    shownDesc->move(100, y+70);
    shownDesc->setMaximumWidth(250);
    shownDesc->setAlignment(Qt::AlignHCenter);
    shownDesc->setWordWrap(true);
    shownDesc->setText(*descSet.begin());

    QFont font = shownDesc->font();
    font.setPixelSize(20);
    shownDesc->setFont(font);

    timeLabel = new QLabel(this);
    timeLabel->move(120, y+35);
    timeLabel->setFont(font);
    timeLabel->setFrameShape(QFrame::Panel);
    QTimer * timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    timer->start(1000);

    setAcceptDrops(true);
    setMinimumSize(260, 310);
    setWindowTitle(tr("Draggable Text"));
}

DragWidget::~DragWidget() {
    delete shownDesc;
    delete timeLabel;
}

void DragWidget::slotTimerAlarm() {
    static auto startTime = QTime::currentTime();
    int difference = startTime.secsTo(QTime::currentTime());
    QTime time(0,0);
    time = time.addSecs(difference);

    timeLabel->setText(time.toString("hh:mm:ss"));
}

bool DragWidget::nextStep(QString str) {
    if(!str.isEmpty()) {
        QString question = shownDesc->text();
        auto dragedWord = ProxySingleton<XmlApi>::instance().findByWord(str);
        if (dragedWord.second == question) {
            descSet.remove(question);
            if (descSet.size()) {
                shownDesc->setText(*descSet.begin());
                shownDesc->adjustSize();
                shownDesc->setAlignment(Qt::AlignHCenter);

            }
            else
                shownDesc->setText("The end");

            return true;
        }
    }
    return false;
}

void DragWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText()) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void DragWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasText()) {
        const QMimeData *mime = event->mimeData();
        QStringList pieces = mime->text().split(QRegularExpression(QStringLiteral("\\s+")),
                                                QString::SkipEmptyParts);
        QPoint position = event->pos();
        QPoint hotSpot;

        QByteArrayList hotSpotPos = mime->data(hotSpotMimeDataKey()).split(' ');
        if (hotSpotPos.size() == 2) {
            hotSpot.setX(hotSpotPos.first().toInt());
            hotSpot.setY(hotSpotPos.last().toInt());
        }

        for (const QString &piece : pieces) {
            QLabel *newLabel = createDragLabel(piece, this);
            newLabel->move(position - hotSpot);
            newLabel->show();
            newLabel->setAttribute(Qt::WA_DeleteOnClose);

            position += QPoint(newLabel->width(), 0);
        }

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }

        if (event->pos().y() > y + 50) {
            QLabel *child = qobject_cast<QLabel*>(childAt(event->pos()));
            if (child) {
                if (emit nextStep(child->text()) ) {
                    child->close();
                }

            }
        }

    } else {
        event->ignore();
    }
    for (QWidget *widget : findChildren<QWidget *>()) {
        if (!widget->isVisible())
            widget->deleteLater();
    }

}

void DragWidget::mousePressEvent(QMouseEvent *event)
{
    QLabel *child = qobject_cast<QLabel*>(childAt(event->pos()));
    if (!child)
        return;

    QPoint hotSpot = event->pos() - child->pos();

    QMimeData *mimeData = new QMimeData;
    mimeData->setText(child->text());
    mimeData->setData(hotSpotMimeDataKey(),
                      QByteArray::number(hotSpot.x()) + ' ' + QByteArray::number(hotSpot.y()));

    qreal dpr = window()->windowHandle()->devicePixelRatio();
    QPixmap pixmap(child->size() * dpr);
    pixmap.setDevicePixelRatio(dpr);
    child->render(&pixmap);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(hotSpot);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);

    if (dropAction == Qt::MoveAction)
        child->close();
}

void DragWidget::paintEvent(QPaintEvent *event)
{
    event->setAccepted(true);
    QPainter p(this);
    p.drawLine(0, y+50, 120, y+50);
    p.drawLine(207, y+50, 350, y+50);
}
