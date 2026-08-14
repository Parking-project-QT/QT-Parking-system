#include "mainwindow.h"
#include "serialcontroller.h"

#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QTime>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_serial(new SerialController(this))
{
    buildUi();

    connect(m_serial, &SerialController::connectedChanged,
            this, &MainWindow::setConnected);
    connect(m_serial, &SerialController::lineReceived, this,
            [this](const QString &line) { appendLog(QStringLiteral("RX"), line); });
    connect(m_serial, &SerialController::errorOccurred, this,
            [this](const QString &message) {
                appendLog(QStringLiteral("ERR"), message);
                m_statusLabel->setText(tr("Error"));
                m_statusLabel->setStyleSheet(QStringLiteral("color: #c62828; font-weight: bold;"));
            });

    refreshPorts();
    setConnected(false, QString());
}

void MainWindow::buildUi()
{
    setWindowTitle(tr("STM32 USART1 Controller"));
    resize(680, 560);

    auto *central = new QWidget(this);
    auto *rootLayout = new QVBoxLayout(central);

    auto *connectionGroup = new QGroupBox(tr("Serial connection"), central);
    auto *connectionLayout = new QHBoxLayout(connectionGroup);
    m_portCombo = new QComboBox(connectionGroup);
    auto *refreshButton = new QPushButton(tr("Refresh"), connectionGroup);
    m_connectButton = new QPushButton(tr("Connect"), connectionGroup);
    m_statusLabel = new QLabel(connectionGroup);
    connectionLayout->addWidget(new QLabel(tr("Port"), connectionGroup));
    connectionLayout->addWidget(m_portCombo, 1);
    connectionLayout->addWidget(refreshButton);
    connectionLayout->addWidget(m_connectButton);
    connectionLayout->addWidget(m_statusLabel);
    rootLayout->addWidget(connectionGroup);

    m_deviceControls = new QWidget(central);
    auto *controlsLayout = new QVBoxLayout(m_deviceControls);
    controlsLayout->setContentsMargins(0, 0, 0, 0);

    auto *ledGroup = new QGroupBox(tr("External LEDs (PB0 / PB1)"), m_deviceControls);
    auto *ledLayout = new QVBoxLayout(ledGroup);

    const auto addLedRow = [this, ledGroup, ledLayout](int ledNumber) {
        auto *row = new QHBoxLayout;
        auto *onButton = new QPushButton(tr("ON"), ledGroup);
        auto *offButton = new QPushButton(tr("OFF"), ledGroup);
        row->addWidget(new QLabel(tr("LED %1").arg(ledNumber), ledGroup));
        row->addStretch();
        row->addWidget(onButton);
        row->addWidget(offButton);
        ledLayout->addLayout(row);

        connect(onButton, &QPushButton::clicked, this,
                [this, ledNumber] {
                    sendCommand(QStringLiteral("LED %1 ON").arg(ledNumber));
                });
        connect(offButton, &QPushButton::clicked, this,
                [this, ledNumber] {
                    sendCommand(QStringLiteral("LED %1 OFF").arg(ledNumber));
                });
    };

    addLedRow(1);
    addLedRow(2);
    controlsLayout->addWidget(ledGroup);

    auto *motorGroup = new QGroupBox(tr("Motor PWM (TIM5, PA0 / PA1)"), m_deviceControls);
    auto *motorLayout = new QVBoxLayout(motorGroup);
    auto *dutyLayout = new QHBoxLayout;
    m_dutySlider = new QSlider(Qt::Horizontal, motorGroup);
    m_dutySlider->setRange(0, 100);
    m_dutySlider->setValue(50);
    m_dutySpinBox = new QSpinBox(motorGroup);
    m_dutySpinBox->setRange(0, 100);
    m_dutySpinBox->setSuffix(QStringLiteral(" %"));
    m_dutySpinBox->setValue(50);
    dutyLayout->addWidget(new QLabel(tr("Duty"), motorGroup));
    dutyLayout->addWidget(m_dutySlider, 1);
    dutyLayout->addWidget(m_dutySpinBox);
    motorLayout->addLayout(dutyLayout);

    auto *motorButtons = new QHBoxLayout;
    auto *cwButton = new QPushButton(tr("Clockwise"), motorGroup);
    auto *stopButton = new QPushButton(tr("STOP"), motorGroup);
    auto *ccwButton = new QPushButton(tr("Counter-clockwise"), motorGroup);
    stopButton->setStyleSheet(QStringLiteral("font-weight: bold; color: #b71c1c;"));
    motorButtons->addWidget(cwButton);
    motorButtons->addWidget(stopButton);
    motorButtons->addWidget(ccwButton);
    motorLayout->addLayout(motorButtons);
    controlsLayout->addWidget(motorGroup);
    rootLayout->addWidget(m_deviceControls);

    auto *logGroup = new QGroupBox(tr("Communication log"), central);
    auto *logLayout = new QVBoxLayout(logGroup);
    m_log = new QPlainTextEdit(logGroup);
    m_log->setReadOnly(true);
    m_log->setMaximumBlockCount(500);
    auto *clearButton = new QPushButton(tr("Clear"), logGroup);
    logLayout->addWidget(m_log, 1);
    logLayout->addWidget(clearButton, 0, Qt::AlignRight);
    rootLayout->addWidget(logGroup, 1);

    setCentralWidget(central);

    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::refreshPorts);
    connect(m_connectButton, &QPushButton::clicked, this, &MainWindow::toggleConnection);
    connect(clearButton, &QPushButton::clicked, m_log, &QPlainTextEdit::clear);
    connect(m_dutySlider, &QSlider::valueChanged, m_dutySpinBox, &QSpinBox::setValue);
    connect(m_dutySpinBox, &QSpinBox::valueChanged, m_dutySlider, &QSlider::setValue);
    connect(cwButton, &QPushButton::clicked, this,
            [this] { sendMotorCommand(QStringLiteral("CW")); });
    connect(ccwButton, &QPushButton::clicked, this,
            [this] { sendMotorCommand(QStringLiteral("CCW")); });
    connect(stopButton, &QPushButton::clicked, this,
            [this] { sendCommand(QStringLiteral("MOTOR STOP")); });
}

void MainWindow::refreshPorts()
{
    const QString selectedPort = m_portCombo->currentText();
    m_portCombo->clear();
    m_portCombo->addItems(SerialController::availablePortNames());

    const int previousIndex = m_portCombo->findText(selectedPort);
    if (previousIndex >= 0) {
        m_portCombo->setCurrentIndex(previousIndex);
    }
}

void MainWindow::toggleConnection()
{
    if (m_serial->isConnected()) {
        m_serial->disconnectPort();
        return;
    }

    if (m_portCombo->currentText().isEmpty()) {
        QMessageBox::information(this, tr("Serial port"),
                                 tr("Connect the USB-to-UART adapter and refresh the port list."));
        return;
    }

    if (m_serial->connectPort(m_portCombo->currentText())) {
        sendCommand(QStringLiteral("PING"));
    }
}

void MainWindow::setConnected(bool connected, const QString &portName)
{
    m_portCombo->setEnabled(!connected);
    m_connectButton->setText(connected ? tr("Disconnect") : tr("Connect"));
    m_deviceControls->setEnabled(connected);

    if (connected) {
        m_statusLabel->setText(tr("Connected: %1").arg(portName));
        m_statusLabel->setStyleSheet(QStringLiteral("color: #2e7d32; font-weight: bold;"));
        appendLog(QStringLiteral("SYS"), tr("Connected to %1 at 115200 8-N-1").arg(portName));
    } else {
        m_statusLabel->setText(tr("Disconnected"));
        m_statusLabel->setStyleSheet(QStringLiteral("color: #616161;"));
        if (!portName.isEmpty()) {
            appendLog(QStringLiteral("SYS"), tr("Disconnected from %1").arg(portName));
        }
    }
}

void MainWindow::sendCommand(const QString &command)
{
    if (!m_serial->isConnected()) {
        return;
    }

    appendLog(QStringLiteral("TX"), command);
    m_serial->sendCommand(command);
}

void MainWindow::sendMotorCommand(const QString &direction)
{
    sendCommand(QStringLiteral("MOTOR %1 %2")
                    .arg(direction)
                    .arg(m_dutySpinBox->value()));
}

void MainWindow::appendLog(const QString &direction, const QString &text)
{
    m_log->appendPlainText(QStringLiteral("[%1] %2  %3")
                               .arg(QTime::currentTime().toString(QStringLiteral("HH:mm:ss.zzz")))
                               .arg(direction)
                               .arg(text));
}
