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

}

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    //, reply_counter(0)
    , transactionCount(0)
    , error_counter(0)
    , max_reply(1000)
    , replies()
    , delays(0)
    , serialTxPortLabel(new QLabel(tr("Tx Port:")))
    , serialRxPortComboBox(new QComboBox())
    , serialTxPortComboBox(new QComboBox())
    , serialRxPortLabel(new QLabel(tr("Rx Port:")))
    //, waitResponseSpinBox(new QSpinBox())
    , baudRateValue(new QComboBox())
    , requestLabel(new QLabel(tr("Message:")))
    , requestChBox(new QCheckBox(tr("request")))
   // , cyclicChBox(new QCheckBox(tr("cyclic")))
    , requestLineEdit(new QLineEdit(tr("FD 0E")))
    , periodLineEdit(new QLineEdit(tr("50")))
    , pauseLineEdit(new QLineEdit(tr("0")))
    , periodLabel(new QLabel(tr("Period:")))
    , pauseLabel(new QLabel(tr("Pause:")))
   // , statusValue(new QLabel(tr("Not running")))
    , runButton(new QPushButton(tr("Open")))
    , sendButton(new QPushButton(tr("Send")))
    , cyclicButton(new QPushButton(tr("Cyclic")))
    , refreshButton(new QPushButton(tr("Refresh")))
    , addButton(new QPushButton(tr("Add cyclic")))
    , resetButton(new QPushButton(tr("Reset cyclic")))
    , resultTable(new QStandardItemModel(max_reply,5))
    , resultTableView(new QTreeView())
    , delayTable(new QStandardItemModel(1,2))
    , delayTableView(new QTreeView())
    , errorTable(new QStandardItemModel(6,4))
    , errorTableView(new QTreeView())
    , resultBox(new QGroupBox(tr("Result")))
    , controlBox(new QGroupBox(tr("Control")))
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
    controlLayout->addWidget(serialTxPortLabel, 0, 0);
    controlLayout->addWidget(serialRxPortLabel, 1, 0);
    controlLayout->addWidget(serialRxPortComboBox, 1, 1);
    controlLayout->addWidget(serialTxPortComboBox, 0, 1);
    controlLayout->addWidget(baudRateValue, 2, 1);
    controlLayout->addWidget(runButton, 3, 1);
    controlLayout->addWidget(sendButton, 0, 5);
    controlLayout->addWidget(cyclicButton, 0, 6);
    controlLayout->addWidget(refreshButton, 7, 6);
    controlLayout->addWidget(addButton, 1, 5);
    controlLayout->addWidget(resetButton, 1, 6);
    controlLayout->addWidget(requestLabel, 0, 2);
    controlLayout->addWidget(requestChBox, 0, 4);
    //controlLayout->addWidget(cyclicChBox, 2, 3);
    controlLayout->addWidget(requestLineEdit, 0, 3);
    controlLayout->addWidget(periodLineEdit, 1, 3);
    controlLayout->addWidget(pauseLineEdit, 2, 3);
    controlLayout->addWidget(pauseLabel, 2, 2);
    controlLayout->addWidget(periodLabel, 1, 2);
    //controlLayout->addWidget(statusValue, 7, 1);
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
    connect(refreshButton, SIGNAL(clicked()),
            this, SLOT(refreshprocessing()));
    connect(addButton, SIGNAL(clicked()),
            this, SLOT(addprocessing()));
    connect(resetButton, SIGNAL(clicked()),
            this, SLOT(resetprocessing()));
//    connect(&pf_adapt, SIGNAL(reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ )),
//            this, SLOT(reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ )));
//    connect(&pf_adapt, SIGNAL(reply(pf_reply)),
//            this, SLOT(reply(pf_reply)));
//    connect(&pf_adapt, SIGNAL(reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ )),
//            this, SLOT(reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ )));

    connect(&pf_adapt, SIGNAL(reply(pf_reply)),
            this, SLOT(reply(pf_reply)));

    connect(&pf_adapt, SIGNAL(error(pf_error)),
            this, SLOT(error(pf_error)));

    replies.reserve(max_reply);
}

Dialog::~Dialog()
{

}

void Dialog::openport()
{
    setControlsEnabled(false);
//    statusValue->setText(tr("Status: Running, connected to port %1.")
//                         .arg(serialRxPortComboBox->currentText() +
//                              " " +
//                              serialTxPortComboBox->currentText()));

    emit (pf_adapt.open_serial(
                serialRxPortComboBox->currentText(),
                serialTxPortComboBox->currentText(),
                baudRateValue->currentText().toInt()));

}

void Dialog::cyclicprocessing()
{
    //QByteArray data(QByteArray::fromHex(requestLineEdit->text().toLocal8Bit()));
    //QDebug(QtDebugMsg) << "Executing Dialog::transmitt slot. Thread:" << this->thread();

    if(cyclicButton->isCheckable())
    {
        //cyclicButton->setChecked(false);
        cyclicButton->setCheckable(false);
        cyclicButton->setText("Cyclic");
        emit(pf_adapt.stop_cyclic());
        refreshprocessing();

    }
    else
    {
        cyclicButton->setText("Stop");
        //cyclicButton->setChecked(true);
        cyclicButton->setCheckable(true);
        QDebug(QtDebugMsg) << "Start cyclic";
        emit(pf_adapt.start_cyclic());
    }
}

void Dialog::refreshprocessing()
{
    showDelays();
    showReplies(*resultTable, max_reply, false);
}

void Dialog::addprocessing()
{
    QByteArray data(QByteArray::fromHex(requestLineEdit->text().toLocal8Bit()));
    //QDebug(QtDebugMsg) << "Executing Dialog::addprocessing slot";
    emit(pf_adapt.add_cyclic(data, periodLineEdit->text().toInt(), pauseLineEdit->text().toInt(), Qt::Checked == requestChBox->checkState()));
}

void Dialog::resetprocessing()
{
    QDebug(QtDebugMsg) << "Reset cyclic";
    emit(pf_adapt.reset_cyclic());
}

void Dialog::sendprocessing()
{
    QByteArray data(QByteArray::fromHex(requestLineEdit->text().toLocal8Bit()));
    //QDebug(QtDebugMsg) << "Executing Dialog::sendprocessing slot";

    emit(pf_adapt.request(data, Qt::Checked == requestChBox->checkState()));
}

void Dialog::reply(pf_reply reply_)
{
    replies.push_back(reply_);

    if(reply_.get_type() == pf_reply::TYPE_REQUEST)
    {
        // Increment reply distribution table
        while(delays.size() <= reply_.get_delay())
        {
            delays.push_back(0);
        }
        delays[reply_.get_delay()]++;
    }
}

void Dialog::error(pf_error err)
{
    QDebug(QtDebugMsg) << "Got error: " << err.get_txt() << " in thread: " << (int)this->thread();

//    statusValue->setText(tr("Got error: ") + QString(err.get_txt()));

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

void Dialog::showReplies(QStandardItemModel& table, quint32 replies_to_show, bool insert)
{
    quint32 i = 0;
    quint32 size = replies.size();

    //QDebug(QtDebugMsg) << "showReplies: " << size << (replies.end() == replies.begin());

    for (QList<pf_reply>::iterator it = replies.end()-1; i < replies_to_show && it >= replies.begin();i++ ,it--)
    {
        //QDebug(QtDebugMsg) << "showReplies 2: " << i;

        if(insert){resultTable->insertRow(i);}
        resultTable->setData(table.index(i, 0), QString::number(size - i));
        resultTable->setData(table.index(i, 1), it[0].get_timestamp().toString() + QString(" ") + QString::number(it[0].get_timestamp().msec()));
        resultTable->setData(table.index(i, 2), it[0].get_request().toHex());
        resultTable->setData(table.index(i, 3), it[0].get_reply().toHex());
        resultTable->setData(table.index(i, 4), QString::number(it[0].get_delay()));
    }
}

void Dialog::showDelays()
{
    if(delayTable->rowCount() < delays.size())
    {
        delayTable->setRowCount(delays.size());
    }

    //delayTable->clear();
    for(int i =0; i < delays.size(); i++)
    {
        //QDebug(QtDebugMsg) << "Set delay table: " << QString::number(i) << QString::number(delays[i]);
        delayTable->setData(delayTable->index(i, 0), QString::number(i));
        delayTable->setData(delayTable->index(i, 1), QString::number(delays[i]));
    }
}
