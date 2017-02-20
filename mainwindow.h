#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialportwidget.h"
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <string>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool initSerialPort();
    void sendData();
    void saveData();
    void Hex_To_String(QString &str);   //执行转换：例如"123"->"313233"
    void String_To_Hex(QString &str);   //执行转换：例如"313233"->"123"
    int hex_to_str(char *ptr,int index, char *buf,int len);      //执行转换：例如"123"->"313233"
    int str_to_hex(char *ch, char *cbuf, int len);               //执行转换：例如"313233"->"123"

protected:
     bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void ShowDataReceiveArea();

    void on_portNameRefreshBtn_clicked();

    void on_openSerialPortBtn_clicked();

    void on_showReceiveTimeCheckBox_clicked();

    void on_clearReceiveBtn_clicked();

    void on_saveDataBtn_clicked();

    void on_dataSaveToFileCheckBox_clicked();

    void on_sendBtn_clicked();

    void on_clearSendAreaBtn_clicked();

    void on_hexSendAreaCheckBox_clicked();

    void on_repeatSendCheckBox_clicked();

    void on_dataReceiveAreaTextBrowser_textChanged();

    void repeatSend();

    void on_loadFileBtn_clicked();

    void statusBarBtnClicked();

    void on_DTRcheckBox_clicked();

    void on_RTScheckBox_clicked();

private:
    Ui::MainWindow *ui;

    SerialPortWidget *serialPort;
    bool serialPortIsOpen;
    void getPortName();
    bool repeatSendFlag;
    QString fileName;
    QTimer *timer;
    bool showHex;
    void showStatusBar();
    int receiveDataLength;
    int sendDataLength;
    QLabel *statusBarLabel1;
    QLabel *statusBarLabel2;
    QPushButton *statusBarBtn;
    void get_prefix_suffix(QString &prefix,QString &suffix);
};

#endif // MAINWINDOW_H
