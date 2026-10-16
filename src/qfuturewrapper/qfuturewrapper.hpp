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

        template<class Function>
        QFutureWrapper& ok(Function &&callback);

    private:
        QFuture<T> *m_future = nullptr;
        QFutureWatcher<T> *m_watcher = nullptr;
    };
} // QCustomLibs

#include "qfuturewrapper.ipp"

#endif //QCUSTOMLIBS_QFUTUREWRAPPER_H
