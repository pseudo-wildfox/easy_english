#ifndef PROXYSINGLETON_H
#define PROXYSINGLETON_H
#include <QtWidgets>
#include "idb.h"


template <typename Impl>
class ProxySingleton : public IDB {
    IDB * db;
    ProxySingleton() {
        db = new Impl;
    }
public:
    static ProxySingleton& instance() {
        static ProxySingleton *singleton = new ProxySingleton;
        return *singleton;
    }
    int size() override {
        return db->size();
    }

    void  printAll() override {
        db->printAll();
    }
    QPair<QString, QString> findByWord(QString word) override {
        return db->findByWord(word);
    }
    QPair<QString, QString> at(int index) override {
        return db->at(index);
    }
    QMap<QString, QString>  getRand(int size) override {
        return db->getRand(size);
    }
    ~ProxySingleton() {
        delete db;
    }
    ProxySingleton(ProxySingleton& ps) = delete;
    ProxySingleton& operator= (ProxySingleton& ps) = delete;
};




#endif // PROXYSINGLETON_H
