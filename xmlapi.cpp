

#include "xmlapi.h"

#include <QDebug>
#include <QtWidgets>
#include <QRandomGenerator>


XmlApi::XmlApi(){
    QDomDocument doc("dictionary");

    QFile file(":/etc/dictionary.xml");
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }

    file.close();
    root = doc.namedItem("Dictionary");
}

int XmlApi::size()
{
    return root.childNodes().count();
}


void XmlApi::printAll()
{
    qDebug() << root.attributes().count();
    qDebug() << root.attributes().namedItem("Language").nodeValue();

    QDomNodeList nodeList = root.childNodes();



    for(int iDx = 0;iDx < nodeList.count(); iDx++) {
        QString word(nodeList.at(iDx).attributes().namedItem("word").nodeValue());
        QString desc(nodeList.at(iDx).attributes().namedItem("desc").nodeValue());

        qDebug() << "Word: " << word  << " Desc: " << desc << endl;
    }



    // print out the element names of all elements that are direct children
    // of the outermost element.
    //QDomElement docElem = doc.documentElement();

    //QDomNode n = docElem.firstChild();
    //while(!n.isNull()) {
    //    QDomElement e = n.toElement(); // try to convert the node to an element.
    //    if(!e.isNull()) {
    //        qDebug() << qPrintable(e.tagName()) << endl; // the node really is an element.
    //    }
    //    n = n.nextSibling();
    //}

    // Here we append a new element to the end of the document
    //QDomElement elem = doc.createElement("img");
    //elem.setAttribute("src", "myimage.png");
    //docElem.appendChild(elem);

}

QPair<QString, QString> XmlApi::findByWord(QString word)
{
    QPair<QString, QString> pair;
    QDomNodeList nodeList = root.childNodes();
    for(int item=0; item<nodeList.count(); item++) {
        if (nodeList.at(item).attributes().namedItem("word").nodeValue() == word) {
            pair.first = nodeList.at(item).attributes().namedItem("word").nodeValue();
            pair.second = nodeList.at(item).attributes().namedItem("desc").nodeValue();
            return pair;
        }
    }
    throw -1;
}

QPair<QString, QString> XmlApi::at(int index)
{
    QDomNodeList nodeList = root.childNodes();
    while (index >= nodeList.count())
        index = index - nodeList.count();

    QPair<QString, QString> pair;
    pair.first = nodeList.at(index).attributes().namedItem("word").nodeValue();
    pair.second = nodeList.at(index).attributes().namedItem("desc").nodeValue();
    return pair;
}

QMap<QString, QString> XmlApi::getRand(int size)
{   if (size > root.childNodes().count())
        size = root.childNodes().count();

    QRandomGenerator rand = QRandomGenerator::securelySeeded();
    int limit = root.childNodes().count();
    QMap<QString, QString> map;
    while (map.size() < size) {
        auto value = this->at(rand.bounded(limit) );
        map.insert(value.first, value.second);
    }
    return map;
}

