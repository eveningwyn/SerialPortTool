#include "serialportwidget.h"
#include <QString>
#include <QMessageBox>
#include "language.h"
//#include <QThread>
#include <QtCore/QDebug>

SerialPortWidget::SerialPortWidget(QWidget *parent) : QWidget(parent)
{
    serial = new QSerialPort(this);
    byte = "";
    connect(serial,SIGNAL(readyRead()),this,SIGNAL(serialReadReady()));
}

SerialPortWidget::~SerialPortWidget()
{
    serial->close();
    delete serial;
}

void SerialPortWidget::setPortName(const QString &portName)      //配置端口号
{
    const QString name = portName.toUpper();
    serial->setPortName(name);
}
void SerialPortWidget::setBaudRate(int baudRate)      //配置波特率
{
    switch (baudRate) {
    case 115200:
        serial->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);
        break;
    case 57600:
        serial->setBaudRate(QSerialPort::Baud57600,QSerialPort::AllDirections);
        break;
    case 38400:
        serial->setBaudRate(QSerialPort::Baud38400,QSerialPort::AllDirections);
        break;
    case 19200:
        serial->setBaudRate(QSerialPort::Baud19200,QSerialPort::AllDirections);
        break;
    case 9600:
        serial->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);
        break;
    case 4800:
        serial->setBaudRate(QSerialPort::Baud4800,QSerialPort::AllDirections);
        break;
    case 2400:
        serial->setBaudRate(QSerialPort::Baud2400,QSerialPort::AllDirections);
        break;
    case 1200:
        serial->setBaudRate(QSerialPort::Baud1200,QSerialPort::AllDirections);
        break;
    default:
        break;
    }
}
void SerialPortWidget::setDataBits(int dataBit)       //配置数据位
{
    switch (dataBit) {
    case 5:
        serial->setDataBits(QSerialPort::Data5);
        break;
    case 6:
        serial->setDataBits(QSerialPort::Data6);
        break;
    case 7:
        serial->setDataBits(QSerialPort::Data7);
        break;
    case 8:
        serial->setDataBits(QSerialPort::Data8);
        break;
    default:
        QMessageBox::warning(this,tr("SerialPort"),tr("DataBit Error!"),QMessageBox::Ok);
        break;
    }
}
void SerialPortWidget::setParity(QString parityBit)         //配置校验位
{
    if(parityBit.isEmpty())
    {
        return;
    }

    QString str = parityBit.toLower();      //将字符串转换成小写字符串

    if(0<= str.indexOf("none",0))
        serial->setParity(QSerialPort::NoParity);
    else {
        if(0<= str.indexOf("odd",0))
            serial->setParity(QSerialPort::OddParity);
        else {
            if(0<= str.indexOf("even",0))
                serial->setParity(QSerialPort::EvenParity);
            else {
                if(0<= str.indexOf("mark",0))
                    serial->setParity(QSerialPort::MarkParity);
                else {
                    if(0<= str.indexOf("space",0))
                        serial->setParity(QSerialPort::SpaceParity);
                    else {
                        QMessageBox::warning(this,tr("SerialPort"),tr("ParityBit Error!"),QMessageBox::Ok);
                    }
                }
            }
        }
    }
}
void SerialPortWidget::setStopBits(QString stopBit)       //配置停止位
{
    if(stopBit.isEmpty())
    {
        return;
    }

    QString str = stopBit.toLower();      //将字符串转换成小写字符串

    if(0<= str.indexOf("1",0))
        serial->setStopBits(QSerialPort::OneStop);
    else {
        if(0<= str.indexOf("2",0))
            serial->setStopBits(QSerialPort::TwoStop);
        else {
            QMessageBox::warning(this,tr("SerialPort"),tr("StopBit Error!"),QMessageBox::Ok);
        }
    }
}
void SerialPortWidget::setDTR_RTS(bool setDTR,bool setRTS)
{
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setDataTerminalReady(setDTR);
    serial->setRequestToSend(setRTS);
}

/*参数：端口号、波特率、数据位、校验位、停止位、DTR和RTS*/
bool SerialPortWidget::openSerialPort(const QString portName,int baudRate,
                                      int dataBit,QString parityBit,
                                      QString stopBit,bool setDTR,bool setRTS)
{
    setPortName(portName);
    if (serial->open(QIODevice::ReadWrite))
    {
        setBaudRate(baudRate);
        setDataBits(dataBit);
        setParity(parityBit);
        setStopBits(stopBit);
        setDTR_RTS(setDTR,setRTS);
        return true;
    }
    else
    {
        QMessageBox::warning(this,tr("SerialPort"),tr("串口打开失败，该串口不存在或者被其他程序占用！"),QMessageBox::Ok);
        return false;
    }
}
void SerialPortWidget::closeSerialPort()
{
    serial->close();
}

/*串口读取数据*/
void SerialPortWidget::serialPortRead(QString &readString,QString prefix,QString suffix)
{
    if (serial->bytesAvailable()<=0)
    {
        return;
    }
    if(prefix.isEmpty() && suffix.isEmpty())
    {//如果无前缀无后缀，直接读取返回
        byte = serial->readAll();
        readString = QString(byte);
        byte = "";
        return;
    }
    QByteArray pre = prefix.toLatin1();     //获得前缀
    QByteArray suf = suffix.toLatin1();     //获得后缀
    QByteArray byteBuf;     //接收数据缓冲区

    byteBuf = serial->readAll();
    byte.append(byteBuf);
    /*判断是否接收完毕*/
    if(!prefix.isEmpty() && suffix.isEmpty())
    {   //如果有前缀无后缀
        if(byte.contains(pre))
        {
            readString = QString(byte);
            byte = "";
            return;
        }
    }
    else
        if(prefix.isEmpty() && !suffix.isEmpty())
        {   //如果无前缀有后缀
            if(byte.contains(suf))
            {
                readString = QString(byte);
                byte = "";
                return;
            }
        }
        else
            if(!prefix.isEmpty() && !suffix.isEmpty())
            {   //如果有前缀有后缀
                if(byte.contains(pre) && byte.contains(suf))
                {
                    readString = QString(byte);
                    byte = "";
                    return;
                }
            }
    byteBuf.clear();
}

/*串口发送数据*/
void SerialPortWidget::serialPortWrite(QString writeString)
{
    if(writeString.isEmpty())
        return;
    QByteArray byte = writeString.toLatin1();
    serial->write(byte);
}
