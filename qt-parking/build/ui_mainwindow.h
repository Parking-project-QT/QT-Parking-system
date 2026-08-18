/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *mainLayout;
    QFrame *headerFrame;
    QHBoxLayout *headerLayout;
    QLabel *lblTitle;
    QSpacerItem *headerSpacer;
    QLabel *lblPortCaption;
    QComboBox *cmbPort;
    QPushButton *btnRefreshPort;
    QPushButton *btnConnect;
    QLabel *lblConnState;
    QHBoxLayout *bodyLayout;
    QGroupBox *grpCamera;
    QVBoxLayout *cameraLayout;
    QLabel *lblCamera;
    QLabel *lblCameraStatus;
    QVBoxLayout *sideLayout;
    QGroupBox *grpSensor;
    QVBoxLayout *sensorLayout;
    QFormLayout *sensorForm;
    QLabel *lblSensorStateCaption;
    QLabel *lblSensorState;
    QLabel *lblDistanceCaption;
    QLabel *lblDistance;
    QLabel *lblInOutCaption;
    QLabel *lblInOutState;
    QProgressBar *barDistance;
    QLabel *lblLedState;
    QPushButton *btnTestDetect;
    QGroupBox *grpStatus;
    QVBoxLayout *statusLayout;
    QFormLayout *statusForm;
    QLabel *lblParkedCaption;
    QLabel *lblParkedCount;
    QLabel *lblEmptyCaption;
    QLabel *lblEmptyCount;
    QProgressBar *barOccupancy;
    QPushButton *btnVehicleList;
    QPushButton *btnPrepay;
    QSpacerItem *sideSpacer;
    QGroupBox *grpLog;
    QHBoxLayout *logLayout;
    QPlainTextEdit *txtLog;
    QPushButton *btnClearLog;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1120, 800);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QVBoxLayout(centralwidget);
        mainLayout->setObjectName("mainLayout");
        headerFrame = new QFrame(centralwidget);
        headerFrame->setObjectName("headerFrame");
        headerFrame->setFrameShape(QFrame::StyledPanel);
        headerLayout = new QHBoxLayout(headerFrame);
        headerLayout->setObjectName("headerLayout");
        lblTitle = new QLabel(headerFrame);
        lblTitle->setObjectName("lblTitle");
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        lblTitle->setFont(font);

        headerLayout->addWidget(lblTitle);

        headerSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        headerLayout->addItem(headerSpacer);

        lblPortCaption = new QLabel(headerFrame);
        lblPortCaption->setObjectName("lblPortCaption");

        headerLayout->addWidget(lblPortCaption);

        cmbPort = new QComboBox(headerFrame);
        cmbPort->setObjectName("cmbPort");
        cmbPort->setMinimumSize(QSize(130, 32));

        headerLayout->addWidget(cmbPort);

        btnRefreshPort = new QPushButton(headerFrame);
        btnRefreshPort->setObjectName("btnRefreshPort");
        btnRefreshPort->setMinimumSize(QSize(0, 32));

        headerLayout->addWidget(btnRefreshPort);

        btnConnect = new QPushButton(headerFrame);
        btnConnect->setObjectName("btnConnect");
        btnConnect->setMinimumSize(QSize(90, 32));

        headerLayout->addWidget(btnConnect);

        lblConnState = new QLabel(headerFrame);
        lblConnState->setObjectName("lblConnState");
        lblConnState->setMinimumSize(QSize(110, 0));
        QFont font1;
        font1.setBold(true);
        lblConnState->setFont(font1);

        headerLayout->addWidget(lblConnState);


        mainLayout->addWidget(headerFrame);

        bodyLayout = new QHBoxLayout();
        bodyLayout->setObjectName("bodyLayout");
        grpCamera = new QGroupBox(centralwidget);
        grpCamera->setObjectName("grpCamera");
        QFont font2;
        font2.setPointSize(11);
        font2.setBold(true);
        grpCamera->setFont(font2);
        cameraLayout = new QVBoxLayout(grpCamera);
        cameraLayout->setObjectName("cameraLayout");
        lblCamera = new QLabel(grpCamera);
        lblCamera->setObjectName("lblCamera");
        lblCamera->setMinimumSize(QSize(640, 480));
        QFont font3;
        font3.setPointSize(11);
        font3.setBold(false);
        lblCamera->setFont(font3);
        lblCamera->setStyleSheet(QString::fromUtf8("background-color: #1b1b1b; color: #cccccc; border: 1px solid #555;"));
        lblCamera->setAlignment(Qt::AlignCenter);

        cameraLayout->addWidget(lblCamera);

        lblCameraStatus = new QLabel(grpCamera);
        lblCameraStatus->setObjectName("lblCameraStatus");
        QFont font4;
        font4.setPointSize(9);
        font4.setBold(false);
        lblCameraStatus->setFont(font4);

        cameraLayout->addWidget(lblCameraStatus);


        bodyLayout->addWidget(grpCamera);

        sideLayout = new QVBoxLayout();
        sideLayout->setObjectName("sideLayout");
        grpSensor = new QGroupBox(centralwidget);
        grpSensor->setObjectName("grpSensor");
        grpSensor->setMinimumSize(QSize(340, 0));
        grpSensor->setFont(font2);
        sensorLayout = new QVBoxLayout(grpSensor);
        sensorLayout->setObjectName("sensorLayout");
        sensorForm = new QFormLayout();
        sensorForm->setObjectName("sensorForm");
        lblSensorStateCaption = new QLabel(grpSensor);
        lblSensorStateCaption->setObjectName("lblSensorStateCaption");
        lblSensorStateCaption->setFont(font3);

        sensorForm->setWidget(0, QFormLayout::ItemRole::LabelRole, lblSensorStateCaption);

        lblSensorState = new QLabel(grpSensor);
        lblSensorState->setObjectName("lblSensorState");
        QFont font5;
        font5.setPointSize(12);
        font5.setBold(true);
        lblSensorState->setFont(font5);

        sensorForm->setWidget(0, QFormLayout::ItemRole::FieldRole, lblSensorState);

        lblDistanceCaption = new QLabel(grpSensor);
        lblDistanceCaption->setObjectName("lblDistanceCaption");
        lblDistanceCaption->setFont(font3);

        sensorForm->setWidget(1, QFormLayout::ItemRole::LabelRole, lblDistanceCaption);

        lblDistance = new QLabel(grpSensor);
        lblDistance->setObjectName("lblDistance");
        lblDistance->setFont(font5);

        sensorForm->setWidget(1, QFormLayout::ItemRole::FieldRole, lblDistance);

        lblInOutCaption = new QLabel(grpSensor);
        lblInOutCaption->setObjectName("lblInOutCaption");
        lblInOutCaption->setFont(font3);

        sensorForm->setWidget(2, QFormLayout::ItemRole::LabelRole, lblInOutCaption);

        lblInOutState = new QLabel(grpSensor);
        lblInOutState->setObjectName("lblInOutState");
        lblInOutState->setFont(font5);

        sensorForm->setWidget(2, QFormLayout::ItemRole::FieldRole, lblInOutState);


        sensorLayout->addLayout(sensorForm);

        barDistance = new QProgressBar(grpSensor);
        barDistance->setObjectName("barDistance");
        barDistance->setMinimumSize(QSize(0, 22));
        barDistance->setMaximum(100);
        barDistance->setValue(0);

        sensorLayout->addWidget(barDistance);

        lblLedState = new QLabel(grpSensor);
        lblLedState->setObjectName("lblLedState");
        lblLedState->setMinimumSize(QSize(0, 34));
        lblLedState->setFont(font2);
        lblLedState->setAlignment(Qt::AlignCenter);

        sensorLayout->addWidget(lblLedState);

        btnTestDetect = new QPushButton(grpSensor);
        btnTestDetect->setObjectName("btnTestDetect");
        btnTestDetect->setMinimumSize(QSize(0, 44));
        btnTestDetect->setFont(font2);

        sensorLayout->addWidget(btnTestDetect);


        sideLayout->addWidget(grpSensor);

        grpStatus = new QGroupBox(centralwidget);
        grpStatus->setObjectName("grpStatus");
        grpStatus->setMinimumSize(QSize(340, 0));
        grpStatus->setFont(font2);
        statusLayout = new QVBoxLayout(grpStatus);
        statusLayout->setObjectName("statusLayout");
        statusForm = new QFormLayout();
        statusForm->setObjectName("statusForm");
        lblParkedCaption = new QLabel(grpStatus);
        lblParkedCaption->setObjectName("lblParkedCaption");
        lblParkedCaption->setFont(font3);

        statusForm->setWidget(0, QFormLayout::ItemRole::LabelRole, lblParkedCaption);

        lblParkedCount = new QLabel(grpStatus);
        lblParkedCount->setObjectName("lblParkedCount");
        QFont font6;
        font6.setPointSize(14);
        font6.setBold(true);
        lblParkedCount->setFont(font6);

        statusForm->setWidget(0, QFormLayout::ItemRole::FieldRole, lblParkedCount);

        lblEmptyCaption = new QLabel(grpStatus);
        lblEmptyCaption->setObjectName("lblEmptyCaption");
        lblEmptyCaption->setFont(font3);

        statusForm->setWidget(1, QFormLayout::ItemRole::LabelRole, lblEmptyCaption);

        lblEmptyCount = new QLabel(grpStatus);
        lblEmptyCount->setObjectName("lblEmptyCount");
        lblEmptyCount->setFont(font6);

        statusForm->setWidget(1, QFormLayout::ItemRole::FieldRole, lblEmptyCount);


        statusLayout->addLayout(statusForm);

        barOccupancy = new QProgressBar(grpStatus);
        barOccupancy->setObjectName("barOccupancy");
        barOccupancy->setMinimumSize(QSize(0, 22));
        barOccupancy->setMaximum(10);
        barOccupancy->setValue(0);

        statusLayout->addWidget(barOccupancy);

        btnVehicleList = new QPushButton(grpStatus);
        btnVehicleList->setObjectName("btnVehicleList");
        btnVehicleList->setMinimumSize(QSize(0, 44));
        btnVehicleList->setFont(font2);

        statusLayout->addWidget(btnVehicleList);

        btnPrepay = new QPushButton(grpStatus);
        btnPrepay->setObjectName("btnPrepay");
        btnPrepay->setMinimumSize(QSize(0, 44));
        btnPrepay->setFont(font2);

        statusLayout->addWidget(btnPrepay);


        sideLayout->addWidget(grpStatus);

        sideSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sideLayout->addItem(sideSpacer);


        bodyLayout->addLayout(sideLayout);


        mainLayout->addLayout(bodyLayout);

        grpLog = new QGroupBox(centralwidget);
        grpLog->setObjectName("grpLog");
        grpLog->setMaximumSize(QSize(16777215, 170));
        QFont font7;
        font7.setPointSize(10);
        font7.setBold(true);
        grpLog->setFont(font7);
        logLayout = new QHBoxLayout(grpLog);
        logLayout->setObjectName("logLayout");
        txtLog = new QPlainTextEdit(grpLog);
        txtLog->setObjectName("txtLog");
        QFont font8;
        font8.setFamilies({QString::fromUtf8("Consolas")});
        font8.setPointSize(9);
        font8.setBold(false);
        txtLog->setFont(font8);
        txtLog->setReadOnly(true);
        txtLog->setMaximumBlockCount(500);

        logLayout->addWidget(txtLog);

        btnClearLog = new QPushButton(grpLog);
        btnClearLog->setObjectName("btnClearLog");
        btnClearLog->setMaximumSize(QSize(90, 16777215));
        QFont font9;
        font9.setBold(false);
        btnClearLog->setFont(font9);

        logLayout->addWidget(btnClearLog);


        mainLayout->addWidget(grpLog);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\354\243\274\354\260\250 \352\264\200\353\246\254 \354\213\234\354\212\244\355\205\234", nullptr));
        lblTitle->setText(QCoreApplication::translate("MainWindow", "\354\243\274\354\260\250 \352\264\200\353\246\254 \354\213\234\354\212\244\355\205\234", nullptr));
        lblPortCaption->setText(QCoreApplication::translate("MainWindow", "\355\217\254\355\212\270", nullptr));
        btnRefreshPort->setText(QCoreApplication::translate("MainWindow", "\354\203\210\353\241\234\352\263\240\354\271\250", nullptr));
        btnConnect->setText(QCoreApplication::translate("MainWindow", "\354\227\260\352\262\260", nullptr));
        lblConnState->setText(QCoreApplication::translate("MainWindow", "\354\227\260\352\262\260 \354\225\210 \353\220\250", nullptr));
        grpCamera->setTitle(QCoreApplication::translate("MainWindow", "\354\213\244\354\213\234\352\260\204 \354\271\264\353\251\224\353\235\274 / \353\262\210\355\230\270\355\214\220 \354\235\270\354\213\235 \354\230\201\354\227\255", nullptr));
        lblCamera->setText(QCoreApplication::translate("MainWindow", "\354\271\264\353\251\224\353\235\274 \353\214\200\352\270\260 \354\244\221\342\200\246", nullptr));
        lblCameraStatus->setText(QCoreApplication::translate("MainWindow", "\354\271\264\353\251\224\353\235\274 \354\203\201\355\203\234 : --", nullptr));
        grpSensor->setTitle(QCoreApplication::translate("MainWindow", "\354\204\274\354\204\234 \354\203\201\355\203\234", nullptr));
        lblSensorStateCaption->setText(QCoreApplication::translate("MainWindow", "\352\260\220\354\247\200 \354\203\201\355\203\234 :", nullptr));
        lblSensorState->setText(QCoreApplication::translate("MainWindow", "\353\214\200\352\270\260", nullptr));
        lblDistanceCaption->setText(QCoreApplication::translate("MainWindow", "\352\261\260\353\246\254 :", nullptr));
        lblDistance->setText(QCoreApplication::translate("MainWindow", "--- cm", nullptr));
        lblInOutCaption->setText(QCoreApplication::translate("MainWindow", "\352\265\254\353\266\204 :", nullptr));
        lblInOutState->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        barDistance->setFormat(QCoreApplication::translate("MainWindow", "%v cm", nullptr));
        lblLedState->setText(QCoreApplication::translate("MainWindow", "\353\263\264\353\223\234 LED : --", nullptr));
        btnTestDetect->setText(QCoreApplication::translate("MainWindow", "\354\260\250\353\237\211 \352\260\220\354\247\200", nullptr));
        grpStatus->setTitle(QCoreApplication::translate("MainWindow", "\354\243\274\354\260\250\354\236\245 \355\230\204\355\231\251", nullptr));
        lblParkedCaption->setText(QCoreApplication::translate("MainWindow", "\354\243\274\354\260\250 \354\260\250\353\237\211 :", nullptr));
        lblParkedCount->setText(QCoreApplication::translate("MainWindow", "0 / 10 \353\214\200", nullptr));
        lblEmptyCaption->setText(QCoreApplication::translate("MainWindow", "\353\271\210 \354\243\274\354\260\250 \352\263\265\352\260\204 :", nullptr));
        lblEmptyCount->setText(QCoreApplication::translate("MainWindow", "10 \353\214\200", nullptr));
        barOccupancy->setFormat(QCoreApplication::translate("MainWindow", "%v / %m", nullptr));
        btnVehicleList->setText(QCoreApplication::translate("MainWindow", "\354\243\274\354\260\250 \354\260\250\353\237\211 \354\241\260\355\232\214", nullptr));
        btnPrepay->setText(QCoreApplication::translate("MainWindow", "\354\202\254\354\240\204\354\240\225\354\202\260", nullptr));
        grpLog->setTitle(QCoreApplication::translate("MainWindow", "\355\206\265\354\213\240 \353\241\234\352\267\270", nullptr));
        btnClearLog->setText(QCoreApplication::translate("MainWindow", "\354\247\200\354\232\260\352\270\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
