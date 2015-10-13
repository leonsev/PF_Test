/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "dialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QCheckBox>
#include <QtSerialPort/QSerialPort>
#include <QTime>

#include <QtSerialPort/QSerialPortInfo>

QT_USE_NAMESPACE

namespace
{
const quint8 delay_arr_size = 100;
quint8 delay_arr[delay_arr_size];
}

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , transactionCount(0)
    , serialPortLabel(new QLabel(tr("Serial port:")))
    , serialRxPortComboBox(new QComboBox())
    , serialTxPortComboBox(new QComboBox())
    , baudRateLabel(new QLabel(tr("Baud rate:")))
    //, waitResponseSpinBox(new QSpinBox())
    , baudRateValue(new QComboBox())
    , requestLabel(new QLabel(tr("Message:")))
    , requestChBox(new QCheckBox(tr("request")))
    , cyclicChBox(new QCheckBox(tr("cyclic")))
    , requestLineEdit(new QLineEdit(tr("FD 0E")))
    , stalusLabel(new QLabel(tr("Status")))
    , statusValue(new QLabel(tr("Not running")))
    , runButton(new QPushButton(tr("Open")))
    , sendButton(new QPushButton(tr("Send")))
    , cyclicButton(new QPushButton(tr("Cyclic")))
    , resultTable(new QStandardItemModel(6,5))
    , resultTableView(new QTreeView())
    , delayTable(new QStandardItemModel(100,2))
    , delayTableView(new QTreeView())
    , errorTable(new QStandardItemModel(6,4))
    , errorTableView(new QTreeView())
    , resultBox(new QGroupBox(tr("Result")))
    , controlBox(new QGroupBox(tr("Control")))
    , reply_counter(0)
    , error_counter(0)
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        serialRxPortComboBox->addItem(info.portName());

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        serialTxPortComboBox->addItem(info.portName());

    //requestChBox->setCheckState(Qt::Checked);

    baudRateValue->addItem("9600");
    baudRateValue->addItem("19200");
    baudRateValue->addItem("38400");
    baudRateValue->addItem("57600");
    baudRateValue->addItem("115200");


    resultTable->setHeaderData(0, Qt::Horizontal, QObject::tr("#"));
    resultTable->setHeaderData(1, Qt::Horizontal, QObject::tr("Time"));
    resultTable->setHeaderData(2, Qt::Horizontal, QObject::tr("Request"));
    resultTable->setHeaderData(3, Qt::Horizontal, QObject::tr("Reply"));
    resultTable->setHeaderData(4, Qt::Horizontal, QObject::tr("Delay"));

    delayTable->setHeaderData(0, Qt::Horizontal, QObject::tr("Delay, ms"));
    delayTable->setHeaderData(1, Qt::Horizontal, QObject::tr("Test counter"));

    for (quint8 i = 0; i < delayTable->rowCount(); i++)
    {
        delayTable->setData(delayTable->index(i, 0), QString::number(i));
        delayTable->setData(delayTable->index(i, 1), QString::number(0));
    }


    errorTable->setHeaderData(0, Qt::Horizontal, QObject::tr("#"));
    errorTable->setHeaderData(1, Qt::Horizontal, QObject::tr("Time"));
    errorTable->setHeaderData(2, Qt::Horizontal, QObject::tr("Err code"));
    errorTable->setHeaderData(3, Qt::Horizontal, QObject::tr("Err description"));

    QGridLayout *resultLayout = new QGridLayout();
    resultLayout->addWidget(resultTableView,0,0);
    resultLayout->addWidget(delayTableView,0,1);
    resultLayout->addWidget(errorTableView,0,2);
    resultBox->setLayout(resultLayout);

    QGridLayout *controlLayout = new QGridLayout;
    controlLayout->addWidget(serialPortLabel, 0, 0);
    controlLayout->addWidget(serialRxPortComboBox, 0, 1);
    controlLayout->addWidget(serialTxPortComboBox, 0, 2);
    controlLayout->addWidget(baudRateLabel, 1, 0);
    controlLayout->addWidget(baudRateValue, 1, 1);
    controlLayout->addWidget(runButton, 0, 3);
    controlLayout->addWidget(sendButton, 0, 4);
    controlLayout->addWidget(cyclicButton, 0, 5);
    controlLayout->addWidget(requestLabel, 2, 0);
    controlLayout->addWidget(requestChBox, 2, 4);
    //controlLayout->addWidget(cyclicChBox, 2, 3);
    controlLayout->addWidget(requestLineEdit, 2, 1);
    controlLayout->addWidget(stalusLabel, 3, 0);
    controlLayout->addWidget(statusValue, 4, 0);
    controlBox->setLayout(controlLayout);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(controlBox,0,0);
    mainLayout->addWidget(resultBox,1,0);
    setLayout(mainLayout);



    resultTableView->setModel(resultTable);
    resultTableView->setRootIsDecorated(false);
    resultTableView->setAlternatingRowColors(true);
    resultTableView->setColumnWidth(0,30);

    delayTableView->setModel(delayTable);
    delayTableView->setRootIsDecorated(false);
    delayTableView->setAlternatingRowColors(true);
    delayTableView->setColumnWidth(0,60);

    errorTableView->setModel(errorTable);
    errorTableView->setRootIsDecorated(false);
    errorTableView->setAlternatingRowColors(true);
    errorTableView->setColumnWidth(0,30);

    setWindowTitle(tr("PF_Test"));
    serialRxPortComboBox->setFocus();
    serialTxPortComboBox->setFocus();

    connect(runButton, SIGNAL(clicked()),
            this, SLOT(openport()));
    connect(sendButton, SIGNAL(clicked()),
            this, SLOT(sendprocessing()));
    connect(cyclicButton, SIGNAL(clicked()),
            this, SLOT(cyclicprocessing()));
//    connect(&pf_adapt, SIGNAL(reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ )),
//            this, SLOT(reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ )));
    connect(&pf_adapt, SIGNAL(reply(pf_reply)),
            this, SLOT(reply(pf_reply)));
    connect(&pf_adapt, SIGNAL(reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ )),
            this, SLOT(reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ )));
    connect(&pf_adapt, SIGNAL(error(pf_error)),
            this, SLOT(error(pf_error)));
}

Dialog::~Dialog()
{

}

void Dialog::openport()
{
    setControlsEnabled(false);
    statusValue->setText(tr("Status: Running, connected to port %1.")
                         .arg(serialRxPortComboBox->currentText() +
                              " " +
                              serialTxPortComboBox->currentText()));

    emit (pf_adapt.open_serial(
                serialRxPortComboBox->currentText(),
                serialTxPortComboBox->currentText(),
                baudRateValue->currentText().toInt()));

}

void Dialog::cyclicprocessing()
{
    QByteArray data(QByteArray::fromHex(requestLineEdit->text().toLocal8Bit()));
    QDebug(QtDebugMsg) << "Executing Dialog::transmitt slot. Thread:" << this->thread();

    if(cyclicButton->isCheckable())
    {
        //cyclicButton->setChecked(false);
        cyclicButton->setCheckable(false);
        cyclicButton->setText("Cyclic");
        emit(pf_adapt.cyclic_stop());

    }
    else
    {
        cyclicButton->setText("Stop");
        //cyclicButton->setChecked(true);
        cyclicButton->setCheckable(true);
        emit(pf_adapt.cyclic_request(data, 0));
    }
}

void Dialog::sendprocessing()
{
    QByteArray data(QByteArray::fromHex(requestLineEdit->text().toLocal8Bit()));
    QDebug(QtDebugMsg) << "Executing Dialog::transmitt slot. Thread:" << this->thread();

    emit(pf_adapt.request(data, Qt::Checked == requestChBox->checkState()));
    if(Qt::Checked != requestChBox->checkState())
    {
       QByteArray empty("");
       emit(reply(pf_reply(empty,data, (qint32)-1)));
    }
}
//void Dialog::reply(QByteArray reply, QByteArray request, qint32 time)
void Dialog::reply(pf_reply reply_)
{
    //QDebug(QtDebugMsg) << "Got reply: " << reply << " timeout: " << time << " in thread: " << (int)this->thread();;

    statusValue->setText(tr("Got reply: ") + QString(reply_.get_reply().toHex()) + tr("\r\n")
                         + tr("timeout: ") + QString::number(reply_.get_delay()));

    resultTable->insertRow(reply_counter);
    resultTable->setData(resultTable->index(reply_counter, 0), QString::number(reply_counter+1));
    resultTable->setData(resultTable->index(reply_counter, 1), QString::number(reply_.get_timestamp().msec()));
    resultTable->setData(resultTable->index(reply_counter, 2), reply_.get_request().toHex());
    resultTable->setData(resultTable->index(reply_counter, 3), reply_.get_reply().toHex());
    resultTable->setData(resultTable->index(reply_counter, 4), QString::number(reply_.get_delay()));
    reply_counter++;
}

void Dialog::error(pf_error err)
{
    QDebug(QtDebugMsg) << "Got error: " << err.get_txt() << " in thread: " << (int)this->thread();

    statusValue->setText(tr("Got error: ") + QString(err.get_txt()));

    errorTable->insertRow(error_counter);
    errorTable->setData(errorTable->index(error_counter, 0), QString::number(error_counter+1));
    errorTable->setData(errorTable->index(error_counter, 1), QString::number(QTime::currentTime().msec()));
    errorTable->setData(errorTable->index(error_counter, 2), QString::number(err.get_err_no()));
    errorTable->setData(errorTable->index(error_counter, 3), err.get_txt());
    error_counter++;
}

void Dialog::setControlsEnabled(bool enable)
{
    runButton->setEnabled(enable);
    serialRxPortComboBox->setEnabled(enable);
    serialTxPortComboBox->setEnabled(enable);
    baudRateValue->setEnabled(enable);
    //waitResponseSpinBox->setEnabled(enable);
}
