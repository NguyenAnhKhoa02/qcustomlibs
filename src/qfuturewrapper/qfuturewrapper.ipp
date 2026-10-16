#pragma once

#include <QFutureWatcher>
#include <QFuture>

namespace QCustomLibs {
    template<typename T>
    QFutureWrapper<T>::QFutureWrapper(QObject *parent, QFuture<T> *future)
        : QObject(parent)
          , m_future(future) {
        m_watcher = new QFutureWatcher<T>(this);
    }

    template<typename T>
    QFutureWrapper<T>::QFutureWrapper(QFuture<T> *future)
        : m_future(future) {
        m_watcher = new QFutureWatcher<T>(this);
    }

    template<typename T>
    template<class Function>
    QFutureWrapper<T> & QFutureWrapper<T>::ok(Function &&callback) {
        m_future->then(callback);

        return *this;
    }
}
