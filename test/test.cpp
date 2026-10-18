//
// Created by khoa.nguyen on 10/16/2026.
//

#include <QObject>

#include <QtTest/QtTest>
#include <QtConcurrent/QtConcurrent>
#include <QFutureWrapper>

namespace {
    class Test : public QObject {
        Q_OBJECT

    private slots:
        static void init();

        static void testFunc();

        static void cleanup();
    };
}

void Test::init() {
}

void Test::testFunc() {
    QPromise<void> promise;
    promise.start();

    promise.setProgressRange(0, 9);

    QFuture<void> future = QtConcurrent::run([promise = std::move(promise)]() mutable {
        for (int i = 0; i < 10; i++) {
            promise.setProgressValue(i);
            // Simulate work
        }
        // 5. Mark the promise as finished
        promise.finish();
    });

    QCustomLibs::QFutureWrapper<void>::create(future)
        ->progressing([](int process) {

        });
}

void Test::cleanup() {
}

QTEST_MAIN(Test)

#include "test.moc"
