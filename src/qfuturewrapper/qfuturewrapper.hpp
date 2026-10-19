//
// Created by khoa.nguyen on 10/16/2026.
//

#pragma once

#ifndef QCUSTOMLIBS_QFUTUREWRAPPER_HPP
#define QCUSTOMLIBS_QFUTUREWRAPPER_HPP
#include <QObject>
#include <QFuture>
#include <QFutureWatcher>

namespace QCustomLibs {
    template<typename T>
    class QFutureWrapper : public QObject {
    public:
        explicit QFutureWrapper(QObject *parent = nullptr, QFuture<T> *future = nullptr);
        explicit QFutureWrapper(QFuture<T> *future = nullptr);
        explicit QFutureWrapper(QFuture<T> future);

        template<class Function>
        auto then(Function &&callback);

        template<class Function,
            typename = std::enable_if_t<!QtPrivate::ArgResolver<Function>::HasExtraArgs> >
        auto error(Function &&callback);

        QFutureWrapper& waitForFinished();

    private:
        QFuture<T> m_future;
        QFutureWatcher<T> *m_watcher = nullptr;
    };
} // QCustomLibs

#include "qfuturewrapper.ipp"

#endif //QCUSTOMLIBS_QFUTUREWRAPPER_H
