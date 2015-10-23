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

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include <QStandardItemModel>
#include <QGroupBox>
#include <QTreeView>
#include <QVector>

#include "pf_adaptor.h"
#include "pf_error.h"
#include "pf_reply.h"

QT_BEGIN_NAMESPACE

class QLabel;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QComboBox;
class QGroupBox;
class QCheckBox;
class QStandardItemModel;
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void openport();
    void sendprocessing();
    void cyclicprocessing();
    void refreshprocessing();
    void addprocessing();
    void resetprocessing();
    //void reply(QByteArray /*reply*/, QByteArray /*request*/, qint32 /*time*/ );
    void reply(pf_reply);
    void error(pf_error);

signals:

private:
    void setControlsEnabled(bool enable);
    void showReplies(QStandardItemModel& table, quint32 replies_to_show, bool insert);
    void showDelays();

private:
    int transactionCount;    
    quint32 error_counter;
    quint32 const max_reply;
    QList<pf_reply> replies;
    QVector<quint32> delays;

    QLabel *serialTxPortLabel;
    QComboBox *serialRxPortComboBox;
    QComboBox *serialTxPortComboBox;
    QLabel *serialRxPortLabel;
    QComboBox *baudRateValue;
    //QSpinBox *waitResponseSpinBox;
    QLabel *requestLabel;
    QCheckBox* requestChBox;
   // QCheckBox* cyclicChBox;
    QLineEdit *requestLineEdit;
    QLineEdit *periodLineEdit;
    QLineEdit *pauseLineEdit;
    QLabel *periodLabel;
    QLabel *pauseLabel;
   // QLabel *statusValue;
    QPushButton *runButton;
    QPushButton *sendButton;
    QPushButton *cyclicButton;
    QPushButton *refreshButton;
    QPushButton *addButton;
    QPushButton *resetButton;
    QStandardItemModel *resultTable;
    QTreeView *resultTableView;
    QStandardItemModel *delayTable;
    QTreeView *delayTableView;
    QStandardItemModel *errorTable;
    QTreeView *errorTableView;
    QGroupBox *resultBox;
    QGroupBox *controlBox;
    pf_adaptor pf_adapt;

    //quint32 reply_counter;

    //MasterThread thread;
};

#endif // DIALOG_H
