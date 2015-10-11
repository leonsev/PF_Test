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
    , cyclicChBox(new QCheckBox(tr("cyclic")))
    , requestLineEdit(new QLineEdit(tr("FD 0E")))
    , stalusLabel(new QLabel(tr("Status")))
    , statusValue(new QLabel(tr("Not running")))
    , runButton(new QPushButton(tr("Open")))
    , sendButton(new QPushButton(tr("Send")))
    , resultTable(new QStandardItemModel(6,4))
    , resultTableView(new QTreeView())
    , resultBox(new QGroupBox(tr("Result")))
    , controlBox(new QGroupBox(tr("Control")))
    , reply_counter(0)
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
    //resultTable->setColumnWidth(0,100);
    resultTable->setHeaderData(1, Qt::Horizontal, QObject::tr("Request"));
    resultTable->setHeaderData(2, Qt::Horizontal, QObject::tr("Reply"));
    resultTable->setHeaderData(3, Qt::Horizontal, QObject::tr("Delay"));

    QHBoxLayout *resultLayout = new QHBoxLayout;
    resultLayout->addWidget(resultTableView);
    resultBox->setLayout(resultLayout);

    QGridLayout *controlLayout = new QGridLayout;
    controlLayout->addWidget(serialPortLabel, 0, 0);
    controlLayout->addWidget(serialRxPortComboBox, 0, 1);
    controlLayout->addWidget(serialTxPortComboBox, 0, 2);
    controlLayout->addWidget(baudRateLabel, 1, 0);
    controlLayout->addWidget(baudRateValue, 1, 1);
    controlLayout->addWidget(runButton, 0, 3);
    controlLayout->addWidget(sendButton, 0, 4);;
    controlLayout->addWidget(requestLabel, 2, 0);
    controlLayout->addWidget(requestChBox, 2, 4);
    controlLayout->addWidget(cyclicChBox, 2, 3);
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

    setWindowTitle(tr("PF_Test"));
    serialRxPortComboBox->setFocus();
    serialTxPortComboBox->setFocus();

    connect(runButton, SIGNAL(clicked()),
            this, SLOT(openport()));
    connect(sendButton, SIGNAL(clicked()),
            this, SLOT(sendprocessing()));
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

void Dialog::sendprocessing()
{
    QByteArray data(QByteArray::fromHex(requestLineEdit->text().toLocal8Bit()));
    QDebug(QtDebugMsg) << "Executing Dialog::transmitt slot. Thread:" << this->thread();


    if(sendButton->isCheckable())
    {
        sendButton->setCheckable(false);
        sendButton->setChecked(false);
        emit(pf_adapt.cyclic_stop());
    }
    else
    {
        if (Qt::Checked == cyclicChBox->checkState())
        {
            //sendButton->setText("Stop");
            sendButton->setCheckable(true);
            sendButton->setChecked(true);
            sendButton->isChecked();
            emit(pf_adapt.cyclic_request(data, 0));
        }
        else
        {
            emit(pf_adapt.request(data, Qt::Checked == requestChBox->checkState()));
            if(Qt::Checked != requestChBox->checkState())
            {
               emit(reply(QByteArray(""),data, -1));
            }
        }
    }
}

void Dialog::reply(QByteArray reply, QByteArray request, qint32 time)
{
    QDebug(QtDebugMsg) << "Got reply: " << reply << " timeout: " << time << " in thread: " << (int)this->thread();;

    statusValue->setText(tr("Got reply: ") + QString(reply.toHex()) + tr("\r\n")
                         + tr("timeout: ") + QString::number(time));

    resultTable->insertRow(reply_counter);
    resultTable->setData(resultTable->index(reply_counter, 0), QString::number(reply_counter+1));
    resultTable->setData(resultTable->index(reply_counter, 1), request.toHex());
    resultTable->setData(resultTable->index(reply_counter, 2), reply.toHex());
    resultTable->setData(resultTable->index(reply_counter, 3), QString::number(time));
    reply_counter++;
}

void Dialog::error(pf_error err)
{
    QDebug(QtDebugMsg) << "Got error: " << err.get_txt() << " in thread: " << (int)this->thread();

    statusValue->setText(tr("Got error: ") + QString(err.get_txt()));
}

void Dialog::setControlsEnabled(bool enable)
{
    runButton->setEnabled(enable);
    serialRxPortComboBox->setEnabled(enable);
    serialTxPortComboBox->setEnabled(enable);
    baudRateValue->setEnabled(enable);
    //waitResponseSpinBox->setEnabled(enable);
}
