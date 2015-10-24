#include <QString>
#include <QtTest>
#include <QByteArray>

#include "pf_rxprocessor.h"
#include "pf_mcontroller.h"
#include "pf_crc.h"

class PF_Test_UTTest : public QObject
{
    Q_OBJECT

public:
    PF_Test_UTTest();

private Q_SLOTS:
    void testCase1();
    void testCase2();
    void testCase3();

    void gotMessage(QByteArray message);

private:
    QByteArray result;

};

PF_Test_UTTest::PF_Test_UTTest()
{

}

void PF_Test_UTTest::gotMessage(QByteArray message)
{
    result = message;
}

void PF_Test_UTTest::testCase1()
{
    pf_rxprocessor rxproc;
    connect(&rxproc, SIGNAL(messageReceived(QByteArray)), this, SLOT(gotMessage(QByteArray)));

    char testdata[] = {START_CHAR, 0x01, 0x02, END_CHAR};
    char resdata[] = {0x01, 0x02};


    rxproc.insert(QByteArray(testdata, sizeof(testdata)));

    QCOMPARE(result, QByteArray(resdata, sizeof(resdata)));
}

void PF_Test_UTTest::testCase2()
{
    pf_rxprocessor rxproc;
    connect(&rxproc, SIGNAL(messageReceived(QByteArray)), this, SLOT(gotMessage(QByteArray)));

    char testdata[] = {START_CHAR, 0x01, 0x02, END_CHAR, START_CHAR, 0x01, START_CHAR, 0x01, START_CHAR, 0x01, 0x02, END_CHAR};
    char resdata[] = {0x01, 0x02};


    rxproc.insert(QByteArray(testdata, sizeof(testdata)));

    QCOMPARE(result, QByteArray(resdata, sizeof(resdata)));
}

void PF_Test_UTTest::testCase3()
{
    char testdata[] = {0x01, 0x02};

    QByteArray arr(testdata, 2);

    arr.append(pf_crc::get(arr));

    QDebug(QtDebugMsg) << "CRC:" << pf_crc::get(arr);

    QVERIFY(pf_crc::get(arr) == 0);
}

QTEST_APPLESS_MAIN(PF_Test_UTTest)

#include "tst_pf_test_uttest.moc"
