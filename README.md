# CRC16_modbus
代码是QT 下的 CRC16（Modbus）校验类，拿过去直接调用类中函数即可(注意命名空间)，crc16(...)：计算crc 校验码。crc_Checking(...):将有crc校验码的报文传入该函数，会检测你的报文中crc校验码是否正确。crcCalculation(...):将字符串传入该函数，自动计算出crc校验码并且添加到报文之后，返回携带crc校验码的新报文。
