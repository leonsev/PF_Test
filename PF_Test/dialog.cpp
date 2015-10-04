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
#include <QtSerialPort/QSerialPort>

#include <QtSerialPort/QSerialPortInfo>

QT_USE_NAMESPACE

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , transactionCount(0)
    , serialPortLabel(new QLabel(tr("Serial port:")))
    , serialRxPortComboBox(new QComboBox())
    , serialTxPortComboBox(new QComboBox())
    , waitResponseLabel(new QLabel(tr("Wait response, msec:")))
    , waitResponseSpinBox(new QSpinBox())
    , requestLabel(new QLabel(tr("Request:")))
    , requestLineEdit(new QLineEdit(tr("")))
    , trafficLabel(new QLabel(tr("No traffic.")))
    , statusLabel(new QLabel(tr("Status: Not running.")))
    , runButton(new QPushButton(tr("Start")))
    , sendButton(new QPushButton(tr("Send")))
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        serialRxPortComboBox->addItem(info.portName());

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        serialTxPortComboBox->addItem(info.portName());

    waitResponseSpinBox->setRange(0, 10000);
    waitResponseSpinBox->setValue(1000);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(serialPortLabel, 0, 0);
    mainLayout->addWidget(serialRxPortComboBox, 0, 1);
    mainLayout->addWidget(serialTxPortComboBox, 0, 2);
    mainLayout->addWidget(waitResponseLabel, 1, 0);
    mainLayout->addWidget(waitResponseSpinBox, 1, 1);
    mainLayout->addWidget(runButton, 0, 3);
    mainLayout->addWidget(sendButton, 0, 4);
    mainLayout->addWidget(requestLabel, 2, 0);
    mainLayout->addWidget(requestLineEdit, 2, 1, 1, 3);
    mainLayout->addWidget(trafficLabel, 3, 0, 1, 4);
    mainLayout->addWidget(statusLabel, 4, 0, 1, 5);
    setLayout(mainLayout);

    setWindowTitle(tr("PF_Test"));
    serialRxPortComboBox->setFocus();
    serialTxPortComboBox->setFocus();

    requestLineEdit->setText("010203");

    connect(runButton, SIGNAL(clicked()),
            this, SLOT(openport()));
    connect(sendButton, SIGNAL(clicked()),
            this, SLOT(sendprocessing()));
//    connect(&thread, SIGNAL(response(QString)),
//            this, SLOT(showResponse(QString)));
//    connect(&thread, SIGNAL(error(QString)),
//            this, SLOT(processError(QString)));
//    connect(&thread, SIGNAL(timeout(QString)),
//            this, SLOT(processTimeout(QString)));
}

void Dialog::openport()
{
    setControlsEnabled(false);
    statusLabel->setText(tr("Status: Running, connected to port %1.")
                         .arg(serialRxPortComboBox->currentText() +
                              " " +
                              serialTxPortComboBox->currentText()));

    pf_adapt.open(
                serialRxPortComboBox->currentText(),
                serialTxPortComboBox->currentText());

//    thread.transaction(serialPortComboBox->currentText(),
//                       waitResponseSpinBox->value(),
//                       requestLineEdit->text());
}

void Dialog::sendprocessing()
{
    QByteArray data(QByteArray::fromHex(requestLineEdit->text().toLocal8Bit()));
    QDebug(QtDebugMsg) << "Executing Dialog::transmitt slot. Thread:" << this->thread() << "Data: ";
    //pf->broadcast(data);
    emit(pf_adapt.send_broadcast(data));
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

void Dialog::setControlsEnabled(bool enable)
{
    runButton->setEnabled(enable);
    serialRxPortComboBox->setEnabled(enable);
    serialTxPortComboBox->setEnabled(enable);
    waitResponseSpinBox->setEnabled(enable);
}
