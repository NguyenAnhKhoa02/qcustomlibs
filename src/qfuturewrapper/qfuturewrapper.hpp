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
        // explicit QFutureWrapper(QObject *parent = nullptr, QFuture<T> *future = nullptr);
        // explicit QFutureWrapper(QFuture<T> *future = nullptr);
        // explicit QFutureWrapper(QFuture<T> future);

        static QFutureWrapper<T> *create(const QFuture<T> &future);

        template<class Function>
        QFutureWrapper<T> *ok(Function &&callback);

        template<class Function>
        QFutureWrapper<T> *progressing(Function &&callback);

        template<class Function,
            typename = std::enable_if_t<!QtPrivate::ArgResolver<Function>::HasExtraArgs> >
        QFutureWrapper<T> *error(Function &&callback);

    private:
        explicit QFutureWrapper(QFuture<T> future);

        QFuture<T> m_future;
        QFutureWatcher<T> *m_watcher = nullptr;

        void handleFinished();

        std::function<void(T)> m_okCallback;
        std::function<void(std::exception_ptr)> m_errorCallback;
        std::function<void(int)> m_valueChangedCallback;
    };
} // QCustomLibs

#include "qfuturewrapper.ipp"

#endif //QCUSTOMLIBS_QFUTUREWRAPPER_H
