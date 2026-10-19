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
    auto future = QtConcurrent::run([]() {
        throw std::runtime_error("Error");
    });
    bool errorCalled = false;
    future.onFailed(
        [&](const std::exception &e) {
            qDebug() << "Exception " << e.what(); // Sẽ IN RA ở đây!
            errorCalled = true;
        }
    );
    // auto wrapper = QCustomLibs::QFutureWrapper(future)
    //     .error([&](const std::exception &e) {
    //         qDebug() << "Exception " << e.what(); // Sẽ IN RA ở đây!
    //         errorCalled = true;
    //     });
    QTRY_VERIFY_WITH_TIMEOUT(errorCalled, 2000);
    // QCustomLibs::QFutureWrapper(future)
    //         .error([](const std::exception &e) {
    //             qDebug() << "Exception " << e.what();
    //         })
    //         .then([]() {
    //             qDebug() << "Done 2";
    //             return 2;
    //         })
    //         .then([](const int value) {
    //             qDebug() << "Done 2" << value;
    //         })
    //         .waitForFinished();
}

void Test::cleanup() {
}

QTEST_MAIN(Test)

#include "test.moc"
