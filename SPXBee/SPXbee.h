//SPXBee
//Version: 2.0

#ifndef SPXbee_h
#define SPXbee_h

#include "Arduino.h"

#define SPXBEE_SERIALPORT_UNDEF		0xFF
#define SPXBEE_SERIALPORT_0			0x00
#define SPXBEE_SERIAL_TIMEOUT		500

#define SPXBEE_CHANNEL_UNDEF		0xFF
#define SPXBEE_PAN_UNDEF			0xFFFF
#define SPXBEE_NWKADDRESS_UNDEF		0xFFFF
#define SPXBEE_DESTADDRESSH_UNDEF	0xFFFF
#define SPXBEE_DESTADDRESSL_UNDEF	0xFFFF

#define SPXBEE_DESTADDRH			1
#define SPXBEE_DESTADDRL   			2
#define SPXBEE_NWKADDR     			3
#define SPXBEE_OPCHANNEL   			4
#define SPXBEE_SCANCHANNEL			5
#define SPXBEE_EXTPANID    			6
#define SPXBEE_OPEXTPANID  			7
#define SPXBEE_ASSOCIND    			8

class SPXbee
{
  public:
    SPXbee();
    void SerialInit( void );
    boolean Init( void );
    void getSerialPort( byte *serialPort );
    void getChannel( byte *channel );
    void getOperationPAN( int *PAN );
    void getExtendedPAN( int *PAN );
    void getSrcAddress( unsigned int *srcAddress );
    void getDestAddress( unsigned int *destAddressH, unsigned int *destAddressL );
    boolean setExtendedPAN( int *PAN );
    void readData( char *c );
	void readData( byte str_length, String *str );
    void writeData( char c );
	void writeData( byte str_length, String str );
  private:
    byte _serialPort;
    byte _opChannel;
    int _opPAN;
    int _extPAN;
    unsigned int _nwkAddress;
    unsigned int _destAddressH;
    unsigned int _destAddressL;
    boolean getAssoc( void );
    boolean getOpChannel( String *opChannel );
    boolean getOpPAN( String *opPAN );
    boolean getExtPAN( String *extPAN );
    boolean getNwkAddress( String *nwkAddr );
    boolean getDestinationAddress( String *destAddrH, String *destAddrL );
    boolean setExtPAN( String *extPAN );
    boolean readParameter( byte Id, String *Value );
    boolean writeParameter( byte Id, String *Value );
    boolean enterCommandMode( void );
    boolean exitCommandMode ( void );
    boolean readCommand( String *Cmd );
};

#endif
