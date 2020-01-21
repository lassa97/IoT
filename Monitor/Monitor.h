#ifndef Monitor_h
#define Monitor_h

#include "Arduino.h"

#define MONITOR_MESSAGEID_UNDEF			0xFF
#define MONITOR_MESSAGEID_CONFIG			0x00

#define MONITOR_DESTADDRESS_UNDEF		0xFFFF
#define MONITOR_MOTEID_UNDEF			0xFFFF

#define MONITOR_PARAMETERID_UNDEF		0xFF
#define MONITOR_PARAMETERID_HUMPERIOD		0x01
#define MONITOR_PARAMETERID_TEMPPERIOD		0x02
#define MONITOR_PARAMETERID_LIGHTPERIOD	0x03

#define MONITOR_PARSEMSG_MAX			0x05

#define MONITOR_SERIALPORT_0			0x00
#define MONITOR_SERIALPORT_UNDEF			0xFF
#define MONITOR_SERIALTIMEOUT			500

class Monitor
{
  public:
    Monitor();
    boolean Init( void );
    void getSerialPort( byte *serialPort );
    void getDestAddress( unsigned int *destAddress );
    void getMoteID( unsigned int *moteID );
    void getParameterID( byte *parameterID );
    void getIntValue( unsigned int *value );
    boolean readMessage( String *msg );
    void writeMessage( String msg );
    void parseMessage( byte *msgId, String msg );
  private:
    byte _serialPort;
    unsigned int _destAddress;
    unsigned int _moteID;
    byte _parameterID;
    unsigned int _intValue;
};

#endif
