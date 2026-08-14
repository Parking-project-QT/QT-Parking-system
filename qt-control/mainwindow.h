#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QComboBox;
class QLabel;
class QPlainTextEdit;
class QPushButton;
class QSlider;
class QSpinBox;
class QWidget;
class SerialController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void buildUi();
    void refreshPorts();
    void toggleConnection();
    void setConnected(bool connected, const QString &portName);
    void sendCommand(const QString &command);
    void sendMotorCommand(const QString &direction);
    void appendLog(const QString &direction, const QString &text);

    SerialController *m_serial;
    QComboBox *m_portCombo;
    QPushButton *m_connectButton;
    QLabel *m_statusLabel;
    QWidget *m_deviceControls;
    QSlider *m_dutySlider;
    QSpinBox *m_dutySpinBox;
    QPlainTextEdit *m_log;
};

#endif

