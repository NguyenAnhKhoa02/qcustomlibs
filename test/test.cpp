//
// Created by khoa.nguyen on 10/16/2026.
//

#include <QObject>

#include <QtTest/QtTest>
#include <QtConcurrent/QtConcurrent>
#include "qfuturewrapper.hpp"

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
    QFuture<int> future = QtConcurrent::run([]() {
        return 42;
    });

    QCustomLibs::QFutureWrapper<int>(&future)
        .ok([](const int result) {
            qDebug() << result;
        });

    // QCustomLibs::QFutureWrapper<int>(&future)
    //     .ok([]() {
    //
    //     });

    // QCustomLibs::QFutureWrapper<int>(&future)
    //         .ok([]() {
    //             qDebug() << "done";
    //         });

    // future.waitForFinished();
    //
    // QVERIFY(future.isFinished());
    // QCOMPARE(future.result(), 42);
}

void Test::cleanup() {
}

QTEST_MAIN(Test)

#include "test.moc"
