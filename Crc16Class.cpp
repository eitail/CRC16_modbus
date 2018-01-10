/*------------------------------
 * Version: 1.0.0
 * Author: Leo
 * Date: 2017.6.8
 ------------------------------*/
#include "Crc16Class.h"
#include <QDebug>
#include <QStringList>

using namespace Platform::Communication;

Crc16Class::Crc16Class(QObject *parent) : QObject(parent)
{

}
/*------------------------------
 * 程序计算crc16-modbus
 ------------------------------*/
unsigned short Crc16Class::crc16(unsigned char *CmmBuf, unsigned char Len)
{
    unsigned short crc = 0xFFFF;
    unsigned char i;
    unsigned char j;
   // if(Len > 78) return 1;
    if(Len > 200) return 1;
    j = 0;
    while(j < Len)
    {
        crc ^= CmmBuf[j];
        for(i=0; i < 8; i++)
        {
            if(crc & 0x01)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
          j++;
      }
//      CmmBuf[j] = crc % 0x100; //校验和从长度算起，长度为长度减3
//      CmmBuf[j + 1] = crc / 0x100;
//  qDebug() <<"MeterAgent::crc16: step 3：crc16校验"<< crc << "--" <<  crc % 0x100 << "---" << crc / 0x100;

    return crc;

}

    /*------------------------------
     * 检测crc是否正确
     ------------------------------*/
bool Crc16Class::crc_Checking(QString message)
{
    //将获取到底报文两个两个以空格分开
    QString strDis,temp_st;
    int data_frame_bit = 0;//数据帧到位数
    for(int i = 0;i < message.length(); i += 2)//填加空格
    {
        temp_st = message.mid(i,2);
        strDis += temp_st;
        strDis += " ";
        data_frame_bit ++;
    }
    if( data_frame_bit < 5 )
    {
        return false;
    }
    //将获取到底字符串报文，转成QByteArray格式，并用空格分隔获取到有多少个参数i
    QStringList strlist = strDis.split(" ");
    int data_frame_bit_crc = data_frame_bit-2;
    unsigned char crc_str[data_frame_bit_crc];
    QString crc_str_accept = "";//接收到的crc校验码
    QString crc_str_calculated = "";//计算出来的crc校验码大写
    QString crc_str_calculated_low = "";//计算出来的crc校验码小写
    for(int i = 0 ; i<data_frame_bit-2 ; i++)
    {
        //将数据帧接收到到crc检验吗去掉
        crc_str[i] = strlist.at(i).toInt(0,16);
    }

    //计算出crc16校验码
    int crc_reaout_int = this -> crc16(crc_str,data_frame_bit-2);
    crc_str_calculated = QString("%1").arg(crc_reaout_int, 4, 16, QChar('0')).toUpper();//窗口文件都是大写，所以计算出到字符串字母也要大写。
    crc_str_calculated_low =  QString("%1").arg(crc_reaout_int, 4, 16, QChar('0')).toLower();//小写
    //由于编号是0，1，2，3...所以重新拼接到crc校验吗应该从len-1开始，循环两次
    for(int i = data_frame_bit-1 ; i>data_frame_bit-3 ;i--)
    {
        //重新拼接接收到到crc16校验码
        crc_str_accept += strlist.at(i);
    }
    //qDebug() << crc_str_accept << "-------" << crc_str_calculated;
    //判断校验码是否正确，正确返回true,
    if( crc_str_calculated == crc_str_accept || crc_str_calculated_low == crc_str_accept )
    {
        return true;
    }
    else
    {
        return false;
    }
}

    /*------------------------------
     * 计算出crc的直
     * 将crc拼接到message后面
     ------------------------------*/
QString Crc16Class::crcCalculation(QString message)
{

    /*
     * 进来现将空格去掉
     * 进来不管有没有空格，先去掉，再按找下面标准执行。
     *
     */
    message =  message.replace(" ", "");
    /*
     * 这边传过来到没有crc校验，所以总长度不要减去2
     */
    //将获取到底报文两个两个以空格分开
    QString strDis,temp_st;
    int data_frame_bit = 0;//数据帧到位数
    for(int i = 0;i < message.length(); i += 2)//填加空格
    {
        temp_st = message.mid(i,2);
        strDis += temp_st;
        strDis += " ";
        data_frame_bit ++;
    }
    if( data_frame_bit < 5 )
    {
        return "false";
    }
    //将获取到底字符串报文，转成QByteArray格式，并用空格分隔获取到有多少个参数i
    QStringList strlist = strDis.split(" ");
    int data_frame_bit_crc = data_frame_bit;
    unsigned char crc_str[data_frame_bit_crc];

    QString crc_str_calculated_low = "";//计算出来的crc校验码小写
    for(int i = 0 ; i<data_frame_bit ; i++)
    {
        //将数据帧存放数组
        crc_str[i] = strlist.at(i).toInt(0,16);
    }

    //计算出crc16校验码
    int crc_reaout_int = this -> crc16(crc_str,data_frame_bit);
    crc_str_calculated_low =  QString("%1").arg(crc_reaout_int, 4, 16, QChar('0')).toLower();//小写

    QString strDis_crc;
    for(int i = 0;i < crc_str_calculated_low.length(); i += 2)//填加空格
    {
        temp_st = crc_str_calculated_low.mid(i,2);
        strDis_crc += temp_st;
        strDis_crc += " ";
    }
        QStringList strlist_crc = strDis_crc.split(" ");
        crc_str_calculated_low = strlist_crc.at(1)+strlist_crc.at(0);
        //qDebug() << "lo hi :" << crc_str_calculated_low ;

    /*
     * 将报文和校验拼接，形成带校验到报文，别且返回。
     */
    message = message + crc_str_calculated_low;

    return message;
}
