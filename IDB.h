#ifndef IDB_H
#define IDB_H

#include <QPair>
#include <QDebug>

class IDB {
public:
    virtual int size()
    {
        qDebug() << "idb::size";
        return 0;
    }
    virtual void printAll()
    {
        qDebug() << "idb::printAll";
    }
    virtual QPair<QString, QString> findByWord (QString word)
    {
        qDebug() << "idb::findByWord";
        return QPair<QString, QString> (word,"");
    }
    virtual QPair<QString, QString> at(int index)
    {
        qDebug() << "idb::at " << index;
        return QPair<QString, QString> ("","");
    }
    virtual QMap<QString, QString>  getRand(int size)
    {
        qDebug() << "idb::findByWord " << size;
        return QMap<QString, QString>();
    }

    virtual ~IDB() {

    }
};


#endif // IDB_H
