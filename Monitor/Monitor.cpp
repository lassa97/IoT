#include "Arduino.h"
#include "Monitor.h"

Monitor::Monitor( void )
{
  _serialPort = MONITOR_SERIALPORT_UNDEF;
  _destAddress = MONITOR_DESTADDRESS_UNDEF;
  _moteID = MONITOR_MOTEID_UNDEF;
  _parameterID = MONITOR_PARAMETERID_UNDEF;
}

boolean Monitor::Init( void )
{
  //Initialize serial port
  Serial.begin( 9600 );
  _serialPort = MONITOR_SERIALPORT_0;

}

void Monitor::getSerialPort( byte *serialPort )
{
  //Read serial port communication
  serialPort[ 0 ] = _serialPort;
}

void Monitor::getDestAddress( unsigned int *destAddress )
{
  //Read monitor destination address
  destAddress[ 0 ] = _destAddress;
}

void Monitor::getMoteID( unsigned int *moteID )
{
  //Read monitor mote id
  moteID[ 0 ] = _moteID;
}

void Monitor::getParameterID( byte *parameterID )
{
  //Read monitor parameter id
  parameterID [ 0 ] = _parameterID ;
}

void Monitor::getIntValue( unsigned int *value )
{
  //Read monitor int parameter value
  value[ 0 ] = _intValue;
}

boolean Monitor::readMessage( String *msg )
{
  //Declarations
  char c;
  
  //Initializations
  c = 0x00;
  *msg = "";
  
  //Verify reception of bytes
  if( Serial.available() > 0 )
  {
    //Read bytes until carriage return
    while( c != '\r' )
    {
      if( Serial.available() )
      {
        c = Serial.read();
          *msg += c;
      }
    }
    
    return true;
  }
  
  return false;
}

void Monitor::parseMessage( byte *msgId, String msg )
{
  //Declarations
  byte index;
  String parseMsg[ MONITOR_PARSEMSG_MAX ];
  
  //Initializations
  msgId[ 0 ] = MONITOR_MESSAGEID_UNDEF;
//  _parameterID = MONITOR_PARAMETERID_UNDEF;

  //Remove any leading and trailing whitespace
  msg.trim();
  index = 0;

  while( ( msg.length() > 0 ) && ( index < MONITOR_PARSEMSG_MAX ) )
  {
    if( msg.indexOf( ' ' ) >= 0 )
    {
      parseMsg[ index ] = msg.substring( 0, msg.indexOf( ' ' ) );
      msg = msg.substring( msg.indexOf( ' ' ) );
      msg.trim();
    }
    else
    {
      parseMsg[ index ] = msg;
      msg = "";
    }
    
    index++;
  }

  if( msg.length() == 0 )
  {
    //Verify monitor message id
    if( parseMsg[ 0 ].equalsIgnoreCase( "CONFIG" ) )
    {
      msgId[ 0 ] = MONITOR_MESSAGEID_CONFIG;

      //Store monitor destination address
      _destAddress = ( unsigned int )strtol( parseMsg[ 1 ].c_str(), NULL , 16 );

      //Store monitor mote id
      _moteID = ( unsigned int )strtol( parseMsg[ 2 ].c_str(), NULL, 16 );

      //Verify monitor parameter id
      if( parseMsg[ 3 ].equalsIgnoreCase( "HUM_PERIOD" ) )
      {
        _parameterID = MONITOR_PARAMETERID_HUMPERIOD;
      }
      else
      {
        if( parseMsg[ 3 ].equalsIgnoreCase( "TEMP_PERIOD" ) )
        {
          _parameterID = MONITOR_PARAMETERID_TEMPPERIOD;
        }
        else
        {
          if( parseMsg[ 3 ].equalsIgnoreCase( "LIGHT_PERIOD" ) )
          {
            _parameterID = MONITOR_PARAMETERID_LIGHTPERIOD;
          }
          else
          {
            _parameterID = MONITOR_PARAMETERID_UNDEF;
          }         
        }
      }

      //Store monitor parameter value
      _intValue = ( unsigned int )strtol( parseMsg[ 4 ].c_str(), NULL, 10 );
    }
  }
}

void Monitor::writeMessage( String msg )
{
  Serial.print( msg );
}