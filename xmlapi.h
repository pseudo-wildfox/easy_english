#ifndef XMLAPI_H1
#define XMLAPI_H1

#include <QDomDocument>
#include <QtWidgets>
#include "idb.h"

class XmlApi : public IDB
{
private:
    QDomNode root;

public:
     XmlApi();
                       int  size()                   override;
                      void  printAll()               override;
    QPair<QString, QString> findByWord(QString word) override;
    QPair<QString, QString> at(int index)            override;
    QMap<QString, QString>  getRand(int size)        override;
    virtual ~XmlApi() {}
};





#endif // XMLAPI_H1
