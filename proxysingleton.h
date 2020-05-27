#ifndef PROXYSINGLETON_H
#define PROXYSINGLETON_H
#include <QtWidgets>
#include "IDB.h"


template <typename Impl>
class ProxySingleton : public IDB {
private:
    IDB * db;
    static ProxySingleton * singleton;

    ProxySingleton()
        : db (new Impl)
    {}
    ProxySingleton(ProxySingleton& ps) = delete;
    ProxySingleton& operator= (ProxySingleton& ps) = delete;
public:
    static ProxySingleton& instance() {
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
        if (singleton) delete singleton;
    }
};

template <typename Impl>
ProxySingleton<Impl> *ProxySingleton<Impl>::singleton = new ProxySingleton;


#endif // PROXYSINGLETON_H
