#pragma once

#include <QFutureWatcher>
#include <QFuture>

#include "QFutureWrapper"
#include "qfuturewrapper.hpp"
#include "qfuturewrapper.hpp"

namespace QCustomLibs {
    template<typename T>
    QFutureWrapper<T>::QFutureWrapper(QObject *parent, QFuture<T> *future)
        : QObject(parent),
          m_future(future) {
        m_watcher = new QFutureWatcher<T>(this);
        m_watcher->setFuture(future);
    }

    template<typename T>
    QFutureWrapper<T>::QFutureWrapper(QFuture<T> *future)
        : m_future(future) {
        m_watcher = new QFutureWatcher<T>(this);
        m_watcher->setFuture(future);
    }

    template<typename T>
    QFutureWrapper<T>::QFutureWrapper(QFuture<T> future)
        : m_future(std::move(future)) {
        m_watcher = new QFutureWatcher<T>(this);
        m_watcher->setFuture(m_future);
    }

    template<typename T>
    template<class Function>
    auto QFutureWrapper<T>::then(Function &&callback) {
        using decay_func = std::decay_t<Function>;

        if constexpr (std::is_void_v<T>) {
            using return_type = std::invoke_result_t<decay_func>;

            auto next_future = m_future.then(
                [cb = std::forward<Function>(callback)]() mutable {
                    return cb();
                }
            );

            return QFutureWrapper<return_type>(next_future);
        } else {
            using return_type = std::invoke_result_t<decay_func, T>;
            auto next_future = m_future.then(
                [cb = std::forward<Function>(callback)](T value) mutable {
                    return cb(std::move(value));
                }
            );

            return QFutureWrapper<return_type>(next_future);
        }
    }

    template<typename T>
    QFutureWrapper<T> &QFutureWrapper<T>::waitForFinished() {
        m_future.waitForFinished();

        return *this;
    }

    // Helper to extract the argument type from a lambda/functor's operator()
    template<typename T>
    struct extract_callback_arg;

    template<typename C, typename R, typename Arg>
    struct extract_callback_arg<R (C::*)(Arg) const> {
        using type = Arg;
    };

    template<typename C, typename R, typename Arg>
    struct extract_callback_arg<R (C::*)(Arg)> {
        using type = Arg;
    };

    template<typename F>
    using extract_callback_arg_t = extract_callback_arg<decltype(&std::decay_t<F>::operator())>::type;

    template<typename T>
    template<class Function, typename>
    auto QFutureWrapper<T>::error(Function &&callback) {
        // using Arg = std::decay_t<extract_callback_arg_t<Function> >;

        auto next_future = m_future.onFailed(
            [cb = std::forward<Function>(callback)](const std::exception_ptr &ep) mutable {
                try {
                    if (ep) {
                        std::rethrow_exception(ep);
                    }
                } catch (const std::exception &e) {
                    cb(e);
                } catch (...) {
                    qWarning() << "QFutureWrapper::error: unknown exception thrown";
                    std::rethrow_exception(ep);
                }
            }
        );

        return QFutureWrapper(next_future);
    }
}
