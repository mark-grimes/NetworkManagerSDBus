#include "libnm/SDBusMessage.h"
#include "libnm/TypeSignature.h"
#include <systemd/sd-bus.h>

libnm::SDBusMessage::SDBusMessage()
{
}

libnm::SDBusMessage::~SDBusMessage()
{
	sd_bus_message_unref( pMessage_ );
}

libnm::SDBusMessage::operator sd_bus_message**()
{
	return &pMessage_;
}

template<>
void libnm::SDBusMessage::read<std::string>( std::string& returnValue )
{
	char* pRawString;
	int result=sd_bus_message_read( pMessage_, TypeSignature<std::string>::Type, &pRawString );
	if( result<0 )
	{
		throw std::runtime_error("Failed to parse response message: "+std::string(strerror(-result)));
	}
	returnValue=std::string(pRawString);
}

template<>
void libnm::SDBusMessage::read<bool>( bool& returnValue )
{
	int32_t fourBytes;
	int result=sd_bus_message_read( pMessage_, TypeSignature<bool>::Type, &fourBytes );
	if( result<0 )
	{
		throw std::runtime_error("Failed to parse response message: "+std::string(strerror(-result)));
	}
	returnValue=fourBytes;
}
