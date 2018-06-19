#include "libnm/SDBusMessage.h"
#include "libnm/TypeSignature.h"
#include <systemd/sd-bus.h>

libnm::SDBusMessage::SDBusMessage() : pMessage_(nullptr)
{
}

libnm::SDBusMessage::SDBusMessage( sd_bus_message* pMessage ) : pMessage_(pMessage)
{
	sd_bus_message_ref( pMessage_ );
}

libnm::SDBusMessage::SDBusMessage( const SDBusMessage& other )
{
	pMessage_=other.pMessage_;
	sd_bus_message_ref( pMessage_ );
}

libnm::SDBusMessage::SDBusMessage( SDBusMessage&& other )
{
	pMessage_=other.pMessage_;
	other.pMessage_=nullptr;
}

libnm::SDBusMessage& libnm::SDBusMessage::operator=( const SDBusMessage& other )
{
	sd_bus_message_unref( pMessage_ );
	pMessage_=other.pMessage_;
	sd_bus_message_ref( pMessage_ );
	return *this;
}

libnm::SDBusMessage& libnm::SDBusMessage::operator=( SDBusMessage&& other )
{
	sd_bus_message_unref( pMessage_ );
	pMessage_=other.pMessage_;
	other.pMessage_=nullptr;
	return *this;
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
void libnm::SDBusMessage::read<std::string,std::string>( std::string& returnValue1, std::string& returnValue2 )
{
	char* pRawString1;
	char* pRawString2;
	int result=sd_bus_message_read( pMessage_, TypeSignature<std::string,std::string>::Type, &pRawString1, &pRawString2 );
	if( result<0 )
	{
		throw std::runtime_error("Failed to parse response message: "+std::string(strerror(-result)));
	}
	returnValue1=std::string(pRawString1);
	returnValue2=std::string(pRawString2);
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
