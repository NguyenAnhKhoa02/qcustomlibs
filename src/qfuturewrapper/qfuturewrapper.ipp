#pragma once

#include <QFutureWatcher>
#include <QFuture>

#include "qfuturewrapper.hpp"

namespace QCustomLibs {
    template<typename T>
    QFutureWrapper<T>::QFutureWrapper(QFuture<T> future)
        : m_future(std::move(future)) {
        m_watcher = new QFutureWatcher<T>(this);
        m_watcher->setFuture(m_future);
        connect(
            m_watcher,
            &QFutureWatcher<T>::finished,
            this,
            &QFutureWrapper<T>::handleFinished
        );
        connect(
            m_watcher,
            &QFutureWatcher<T>::progressValueChanged,
            this,
            [this](const int progress) {
                m_valueChangedCallback(progress);
            }
        );
    }

    template<typename T>
    void QFutureWrapper<T>::handleFinished() {
        std::exception_ptr ep;
        std::optional<T> value;

        try {
            value = m_future.result();
        }catch (const QUnhandledException &e) {
            ep = e.exception();
            if (!ep) ep = std::current_exception();
        }catch (...) {
            ep = std::current_exception();
        }

        if (ep) {
            if (m_errorCallback) m_errorCallback(ep);
            else qWarning() << "QFutureWrapper: unhandled exception";
        } else if (m_okCallback) {
            m_okCallback(*value);
        }

        deleteLater();
    }

    template<typename T>
    QFutureWrapper<T> * QFutureWrapper<T>::create(const QFuture<T> &future) {
        return new QFutureWrapper(future);
    }

    template<typename T>
    template<class Function>
    QFutureWrapper<T>* QFutureWrapper<T>::ok(Function &&callback) {
        m_okCallback = [callback = std::forward<Function>(callback)](T result) {
            callback(result);
        };

        return this;
    }

    template<typename T>
    template<class Function>
    QFutureWrapper<T> * QFutureWrapper<T>::progressing(Function &&callback) {
        m_valueChangedCallback = [callback = std::forward<Function>(callback)](T result) {
            callback(result);
        };

        return this;
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
    QFutureWrapper<T>* QFutureWrapper<T>::error(Function &&callback) {
        using Arg = std::decay_t<extract_callback_arg_t<Function>>;

        m_errorCallback = [cb = std::forward<Function>(callback)](const std::exception_ptr &ep) {
            if (!ep) return;
            try {
                std::rethrow_exception(ep);
            } catch (const Arg &e) {
                cb(e);
            } catch (...) {
                qWarning() << "QFutureWrapper: unhandled exception type";
            }
        };
        return this;
    }
}
