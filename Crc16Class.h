/*------------------------------
 * Version: 1.0.0
 * Author: Leo
 * Date: 2017.6.8
 ------------------------------*/
#ifndef CRC16CLASS_H
#define CRC16CLASS_H

#include <QObject>

namespace Platform
{
    namespace Communication
    {
        class Crc16Class : public QObject
        {
            Q_OBJECT
            public:
                explicit    Crc16Class(QObject *parent = 0);
                unsigned short crc16(unsigned char *CmmBuf, unsigned char Len);
                bool        crc_Checking(QString);
                QString     crcCalculation(QString message);

            protected:


            signals:

            public slots:
        };
    }
}

#endif // CRC16CLASS_H
