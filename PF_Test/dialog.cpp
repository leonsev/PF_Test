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

#include <QtSerialPort/QSerialPortInfo>

QT_USE_NAMESPACE

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
    , requestLineEdit(new QLineEdit(tr("FD 0E")))
    , trafficLabel(new QLabel(tr("Result")))
    , statusLabel(new QLabel(tr("Status: Not running.")))
    , runButton(new QPushButton(tr("Open")))
    , sendButton(new QPushButton(tr("Send")))
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

//    waitResponseSpinBox->setRange(0, 10000);
//    waitResponseSpinBox->setValue(1000);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(serialPortLabel, 0, 0);
    mainLayout->addWidget(serialRxPortComboBox, 0, 1);
    mainLayout->addWidget(serialTxPortComboBox, 0, 2);
    mainLayout->addWidget(baudRateLabel, 1, 0);
    //mainLayout->addWidget(waitResponseSpinBox, 1, 1);
    mainLayout->addWidget(baudRateValue, 1, 1);
    mainLayout->addWidget(runButton, 0, 3);
    mainLayout->addWidget(sendButton, 0, 4);;
    mainLayout->addWidget(requestLabel, 2, 0);
    mainLayout->addWidget(requestChBox, 2, 4);
    mainLayout->addWidget(requestLineEdit, 2, 1, 1, 3);
    mainLayout->addWidget(trafficLabel, 3, 0, 1, 4);
    mainLayout->addWidget(statusLabel, 4, 0, 1, 5);
    setLayout(mainLayout);

    setWindowTitle(tr("PF_Test"));
    serialRxPortComboBox->setFocus();
    serialTxPortComboBox->setFocus();

    connect(runButton, SIGNAL(clicked()),
            this, SLOT(openport()));
    connect(sendButton, SIGNAL(clicked()),
            this, SLOT(sendprocessing()));
    connect(&pf_adapt, SIGNAL(reply_s(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ )),
            this, SLOT(reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ )));
//    connect(&thread, SIGNAL(response(QString)),
//            this, SLOT(showResponse(QString)));
//    connect(&thread, SIGNAL(error(QString)),
//            this, SLOT(processError(QString)));
//    connect(&thread, SIGNAL(timeout(QString)),
    //            this, SLOT(processTimeout(QString)));
}

Dialog::~Dialog()
{

}

void Dialog::openport()
{
    setControlsEnabled(false);
    statusLabel->setText(tr("Status: Running, connected to port %1.")
                         .arg(serialRxPortComboBox->currentText() +
                              " " +
                              serialTxPortComboBox->currentText()));

    emit (pf_adapt.open_serial_s(
                serialRxPortComboBox->currentText(),
                serialTxPortComboBox->currentText(),
                baudRateValue->currentText().toInt()));

//    thread.transaction(serialPortComboBox->currentText(),
//                       waitResponseSpinBox->value(),
//                       requestLineEdit->text());
}

void Dialog::sendprocessing()
{
    QByteArray data(QByteArray::fromHex(requestLineEdit->text().toLocal8Bit()));
    QDebug(QtDebugMsg) << "Executing Dialog::transmitt slot. Thread:" << this->thread();

    emit(pf_adapt.request_sl(data, Qt::Checked == requestChBox->checkState()));
}

void Dialog::showResponse(const QString &s)
{
    setControlsEnabled(true);
    trafficLabel->setText(tr("Traffic, transaction #%1:"
                             "\n\r-request: %2"
                             "\n\r-response: %3")
                          .arg(++transactionCount).arg(requestLineEdit->text()).arg(s));
}

void Dialog::processError(const QString &s)
{
    setControlsEnabled(true);
    statusLabel->setText(tr("Status: Not running, %1.").arg(s));
    trafficLabel->setText(tr("No traffic."));
}

void Dialog::processTimeout(const QString &s)
{
    setControlsEnabled(true);
    statusLabel->setText(tr("Status: Running, %1.").arg(s));
    trafficLabel->setText(tr("No traffic."));
}

void Dialog::reply(QByteArray reply, QByteArray request, qint32 time)
{
    QDebug(QtDebugMsg) << "Got reply: " << reply << " timeout: " << time << " in thread: " << (int)this->thread();;

    statusLabel->setText(tr("Got reply: ") + QString(reply.toHex()) + tr("\r\n")
                         + tr("timeout: ") + QString::number(time));
}

void Dialog::setControlsEnabled(bool enable)
{
    runButton->setEnabled(enable);
    serialRxPortComboBox->setEnabled(enable);
    serialTxPortComboBox->setEnabled(enable);
    baudRateValue->setEnabled(enable);
    //waitResponseSpinBox->setEnabled(enable);
}
