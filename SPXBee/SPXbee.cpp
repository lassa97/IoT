//SPXBee
//Version: 2.0

#include "Arduino.h"
#include "SPXbee.h"

SPXbee::SPXbee( void )
{
  _serialPort = SPXBEE_SERIALPORT_UNDEF;
  _opChannel = SPXBEE_CHANNEL_UNDEF;
  _extPAN = SPXBEE_PAN_UNDEF;
  _opPAN = SPXBEE_PAN_UNDEF;
  _nwkAddress = SPXBEE_NWKADDRESS_UNDEF;
  _destAddressH = SPXBEE_DESTADDRESSH_UNDEF;
  _destAddressL = SPXBEE_DESTADDRESSL_UNDEF;
}

void SPXbee::SerialInit( void )
{
  //Initialize serial communication
  Serial.begin( 9600 );
  _serialPort = SPXBEE_SERIALPORT_0;
}

boolean SPXbee::Init( void )
{
  //Declarations
  String paramValue = "";
  String paramValue1 = "";
    
  //Initialize serial communication
  Serial.begin( 9600 );
  _serialPort = SPXBEE_SERIALPORT_0;

  //Wait network association
  while( !getAssoc() )
  {
    delay( random( 500, 2500 ) );
  }

  //Read operation channel
  while( !getOpChannel( &paramValue ) )
  {
    delay( random( 10, 500 ) );
  }
  _opChannel = ( byte )strtol( paramValue.c_str(), NULL , 16 );

  //Read extended PAN
  while( !getExtPAN( &paramValue ) )
  {
    delay( random( 10, 500 ) );
  }
  _extPAN = ( int )strtol( paramValue.c_str(), NULL , 16 );

  //Read operation PAN
  while( !getOpPAN( &paramValue ) )
  {
    delay( random( 10, 500 ) );
  }
  _opPAN = ( int )strtol( paramValue.c_str(), NULL , 16 );
  
  //Read network address
  while( !getNwkAddress( &paramValue ) )
  {
    delay( random( 10, 500 ) );
  }
  _nwkAddress = ( int )strtol( paramValue.c_str(), NULL , 16 );

  //Read destination address
  while( !getDestinationAddress( &paramValue, &paramValue1 ) )
  {
    delay( random( 10, 500 ) );
  }
  _destAddressH = ( int )strtol( paramValue.c_str(), NULL, 16 );
  _destAddressL = ( int )strtol( paramValue1.c_str(), NULL, 16 );

  return true;
}

void SPXbee::getSerialPort( byte *serialPort )
{
  //Read serial port communication with xbee module
  serialPort[ 0 ] = _serialPort;
}

void SPXbee::getChannel( byte *channel )
{
  //Read operation channel of xbee module
  channel[ 0 ] = _opChannel;
}

void SPXbee::getExtendedPAN( int *PAN )
{
  //Read extended PAN
  PAN[ 0 ] = _extPAN;
}

void SPXbee::getOperationPAN( int *PAN )
{
  //Read operation PAN
  PAN[ 0 ] = _opPAN;
}

void SPXbee::getSrcAddress( unsigned int *srcAddress )
{
  //Read network address
  srcAddress[ 0 ] = _nwkAddress;
}

void SPXbee::getDestAddress( unsigned int *destAddressH, unsigned int *destAddressL )
{
  //Read destination address
  destAddressH[ 0 ] = _destAddressH;
  destAddressL[ 0 ] = _destAddressL;
}

boolean SPXbee::setExtendedPAN( int *PAN )
{
  //Declarations
  String paramValue = "";

  //Write extended PAN of xbee module
  paramValue = String( PAN[ 0 ], HEX );
  while( !setExtPAN( &paramValue ) )
  {
    delay( random( 10, 500 ) );
  }

  _extPAN = ( int )strtol( paramValue.c_str(), NULL , 16 );
  return true;
}

void SPXbee::readData( char *c )
{
  while( Serial.available() == 0 )
  {}

  c[ 0 ] = Serial.read();
}

void SPXbee::readData( byte str_length, String *str )
{
	byte num_bytes_read;
	char c;
	
	*str = String( "" );
	if( str_length > 0 )
	{
		num_bytes_read = 0;
		while( num_bytes_read < str_length )
		{
			this->readData( &c );
			*str += String( c );
			num_bytes_read++;
		}
	}
}

void SPXbee::writeData( char c )
{
  Serial.write( c );
}

void SPXbee::writeData( byte str_length, String str )
{
  byte num_bytes;

  num_bytes = min( str_length, str.length() );
  for( byte i = 0; i < num_bytes; i++ )
  {
    this->writeData( str.charAt( i ) );
  }
}

boolean SPXbee::getAssoc( void )
{
  //Declarations
  String paramValue = "";
  String assocResult = "";
  
  //Read association indication
  if( !readParameter( SPXBEE_ASSOCIND, &paramValue ) )
  {
    return false;
  }
  
  //Create association indication in xx format
  switch( paramValue.length() )
  {
    case 1:
    assocResult = "0" + paramValue;
    break;
    default:
    break;
  }
  
  //Verify association indication result
  if( assocResult == "00" )
  {
    return true;
  }
  else
  {
    return false;
  }
}

boolean SPXbee::getOpChannel( String *opChannel )
{
  //Declarations
  String paramValue = "";
  
  //Read operating channel
  if( !readParameter( SPXBEE_OPCHANNEL, &paramValue ) )
  {
    return false;
  }
  
  //Create operating channel in xx format
  switch( paramValue.length() )
  {
    case 1:
    *opChannel = "0" + paramValue;
    break;
    case 2:
    *opChannel = paramValue;
    break;
    default:
    break;
  }
  
  return true;
}

boolean SPXbee::getExtPAN( String *extPAN )
{
  //Declarations
  String paramValue = "";
  
  //Read extended PAN Id
  if( !readParameter( SPXBEE_EXTPANID, &paramValue ) )
  {
    return false;
  }
  
  //Create extended PAN Id in xxxxxxxxxxxxxxxx format
  switch( paramValue.length() )
  {
    case 1:
    *extPAN = "000000000000000" + paramValue;
    break;
    case 2:
    *extPAN = "00000000000000" + paramValue;
    break;
    case 3:
    *extPAN = "0000000000000" + paramValue;
    break;
    case 4:
    *extPAN = "000000000000" + paramValue;
    break;
    case 5:
    *extPAN = "00000000000" + paramValue;
    break;
    case 6:
    *extPAN = "0000000000" + paramValue;
    break;
    case 7:
    *extPAN = "000000000" + paramValue;
    break;
    case 8:
    *extPAN = "00000000" + paramValue;
    break;
    case 9:
    *extPAN = "0000000" + paramValue;
    break;
    case 10:
    *extPAN = "000000" + paramValue;
    break;
    case 11:
    *extPAN = "00000" + paramValue;
    break;
    case 12:
    *extPAN = "0000" + paramValue;
    break;
    case 13:
    *extPAN = "000" + paramValue;
    break;
    case 14:
    *extPAN = "00" + paramValue;
    break;
    case 15:
    *extPAN = "0" + paramValue;
    break;
    case 16:
    *extPAN = paramValue;
    break;
    default:
    break;
  }
  
  return true;
}

boolean SPXbee::getOpPAN( String *opPAN )
{
  //Declarations
  String paramValue = "";
  
  //Read operating extended PAN Id
  if( !readParameter( SPXBEE_OPEXTPANID, &paramValue ) )
  {
    return false;
  }
  
  //Create operating extended PAN Id in xxxxxxxxxxxxxxxx format
  switch( paramValue.length() )
  {
    case 1:
    *opPAN = "000000000000000" + paramValue;
    break;
    case 2:
    *opPAN = "00000000000000" + paramValue;
    break;
    case 3:
    *opPAN = "0000000000000" + paramValue;
    break;
    case 4:
    *opPAN = "000000000000" + paramValue;
    break;
    case 5:
    *opPAN = "00000000000" + paramValue;
    break;
    case 6:
    *opPAN = "0000000000" + paramValue;
    break;
    case 7:
    *opPAN = "000000000" + paramValue;
    break;
    case 8:
    *opPAN = "00000000" + paramValue;
    break;
    case 9:
    *opPAN = "0000000" + paramValue;
    break;
    case 10:
    *opPAN = "000000" + paramValue;
    break;
    case 11:
    *opPAN = "00000" + paramValue;
    break;
    case 12:
    *opPAN = "0000" + paramValue;
    break;
    case 13:
    *opPAN = "000" + paramValue;
    break;
    case 14:
    *opPAN = "00" + paramValue;
    break;
    case 15:
    *opPAN = "0" + paramValue;
    break;
    case 16:
    *opPAN = paramValue;
    break;
    default:
    break;
  }
  
  return true;
}

boolean SPXbee::getNwkAddress( String *nwkAddr )
{
  //Declarations
  String paramValue = "";
  
  //Read network address
  if( !readParameter( SPXBEE_NWKADDR, &paramValue ) )
  {
    return false;
  }
  
  //Create network address in xxxx format
  switch( paramValue.length() )
  {
    case 1:
    *nwkAddr = "000" + paramValue;
    break;
    case 2:
    *nwkAddr = "00" + paramValue;
    break;
    case 3:
    *nwkAddr = "0" + paramValue;
    break;
    case 4:
    *nwkAddr = paramValue;
    break;
    default:
    break;
  }
  
  return true;
}

boolean SPXbee::getDestinationAddress( String *destAddrH, String *destAddrL )
{
  //Declarations
  String paramValue;
  
  //Read destination address high parameter
  if( !readParameter( SPXBEE_DESTADDRH, &paramValue ) )
  {
    return false;
  }

  //Convert destination address high string to xxxxxxxx format
  switch( paramValue.length() )
  {
    case 1:
    *destAddrH = "0000000" + paramValue;
    break;
    case 2:
    *destAddrH = "000000" + paramValue;
    break;
    case 3:
    *destAddrH = "00000" + paramValue;
    break;
    case 4:
    *destAddrH = "0000" + paramValue;
    break;
    case 5:
    *destAddrH = "000" + paramValue;
    break;
    case 6:
    *destAddrH = "00" + paramValue;
    break;
    case 7:
    *destAddrH = "0" + paramValue;
    break;
    case 8:
    *destAddrH = paramValue;
    break;
    default:
    break;
  }
    
  //Read destination address low parameter
  if( !readParameter( SPXBEE_DESTADDRL, &paramValue ) )
  {
    return false;
  }
  
  //Convert destination address low string to xxxxxxxx format
  switch( paramValue.length() )
  {
    case 1:
    *destAddrL = "0000000" + paramValue;
    break;
    case 2:
    *destAddrL = "000000" + paramValue;
    break;
    case 3:
    *destAddrL = "00000" + paramValue;
    break;
    case 4:
    *destAddrL = "0000" + paramValue;
    break;
    case 5:
    *destAddrL = "000" + paramValue;
    break;
    case 6:
    *destAddrL = "00" + paramValue;
    break;
    case 7:
    *destAddrL = "0" + paramValue;
    break;
    case 8:
    *destAddrL = paramValue;
    break;
    default:
    break;
  }
  
  return true;
}

boolean SPXbee::setExtPAN( String *extPAN )
{
  //Write extended PAN Id
  if( !writeParameter( SPXBEE_EXTPANID, extPAN ) )
  {
    return false;
  }

  return true;
}

boolean SPXbee::readParameter( byte Id, String *Value )
{
  //Enter xbee module to command mode
  if( enterCommandMode() )
  {
    //Verify Id parameter
    switch( Id )
    {
      case SPXBEE_DESTADDRH:
      Serial.print("ATDH\r");
      break;
      case SPXBEE_DESTADDRL:
      Serial.print("ATDL\r");
      break;
      case SPXBEE_NWKADDR:
      Serial.print("ATMY\r");
      break;
      case SPXBEE_OPCHANNEL:
      Serial.print("ATCH\r");
      break;
      case SPXBEE_SCANCHANNEL:
      Serial.print("ATSC\r");
      break;
      case SPXBEE_OPEXTPANID:
      Serial.print("ATOP\r");
      break;
      case SPXBEE_EXTPANID:
      Serial.print("ATID\r");
      break;
      case SPXBEE_ASSOCIND:
      Serial.print("ATAI\r");
      break;
      default:
      break;
    }
    
    //Read parameter value
    readCommand( Value );
    
    //Exit xbee module from command mode
    if( exitCommandMode() )
    {
      return true;
    }
  }

  return false;
}

boolean SPXbee::writeParameter( byte Id, String *Value )
{
  //Declarations
  String Cmd = "";

  //Enter xbee module to command mode
  if( enterCommandMode() )
  {
    //Verify Id parameter
    switch( Id )
    {
      case SPXBEE_EXTPANID:
      Cmd = String( "ATID " ) + Value[ 0 ] + String( "\r" );
      Serial.print( Cmd );
      break;
      default:
      break;
    }

    //Read and verify command
    readCommand( &Cmd );
    if( Cmd == "OK" )
    {
      //Write in permanent memory
      Cmd = String( "ATWR\r" );
      Serial.print( Cmd );
      delay( 500 );

      //Read command
      readCommand( &Cmd );
      if( Cmd == "OK" )
      {
        //Exit xbee module from command mode
        if( exitCommandMode() )
        {
          return true;
        }
      }
    }
  }
  
  return false;
}

boolean SPXbee::enterCommandMode( void )
{
  //Declarations
  String Cmd = "";

  //Delay Guard Time
  delay( 1000 );
  
  //Send Command Sequence Character
  Serial.write( '+' );
  Serial.write( '+' );
  Serial.write( '+' );
  
  //Delay Guard Time
  delay( 1000 );
  
  //Read and verify command
  if( readCommand( &Cmd ) )
  {
    if( Cmd == "OK" )
    {
      return true;
    }
  }

  return false;
}

boolean SPXbee::exitCommandMode( void )
{
  //Declarations
  String Cmd = "";

  //Send exit command
  Serial.print( "ATCN\r" );
  
  //Read and verify command
  if( readCommand( &Cmd ) )
  {
    if( Cmd == "OK" )
    {
      return true;
    }
  }

  return false;
}

boolean SPXbee::readCommand( String *Cmd )
{
  //declarations
  char c;
  long timeStart;
  long timeDif;
  
  //Initializations
  c = 0x00;
  *Cmd = "";
  timeDif = 0;
  timeStart = millis();
  
  //Read bytes until end of command
  //End of command character non included
  while( ( c != '\r' ) && (timeDif < SPXBEE_SERIAL_TIMEOUT ) )
  {
    if( Serial.available() )
    {
      c = Serial.read();
      if( c != '\r' )
      {
        *Cmd += c;
      }
      
      //Update timeout variables
      timeStart = millis();
    }
    
    //Update timeout variables
    timeDif = millis() - timeStart;
  }
  
  if( c == '\r' )
  {
    return true;
  }
  else
  {
    return false;
  }
}
