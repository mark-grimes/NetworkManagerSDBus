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

void libnm::detail::ReadHelper<std::string>::read( sd_bus_message* pMessage, std::string& returnValue, std::error_code& error )
{
	char* pRawString;
	int result=sd_bus_message_read( pMessage, TypeSignature<std::string>::Type, &pRawString );
	if( result<0 )
	{
		error=std::error_code( -result, std::generic_category() );
		return;
	}
	returnValue=std::string(pRawString);
}

void libnm::detail::ReadHelper<std::string,std::string>::read( sd_bus_message* pMessage, std::string& returnValue1, std::string& returnValue2, std::error_code& error )
{
	char* pRawString1;
	char* pRawString2;
	int result=sd_bus_message_read( pMessage, TypeSignature<std::string,std::string>::Type, &pRawString1, &pRawString2 );
	if( result<0 )
	{
		error=std::error_code( -result, std::generic_category() );
		return;
	}
	returnValue1=std::string(pRawString1);
	returnValue2=std::string(pRawString2);
}

void libnm::detail::ReadHelper<bool>::read( sd_bus_message* pMessage, bool& returnValue, std::error_code& error )
{
	int32_t fourBytes;
	int result=sd_bus_message_read( pMessage, TypeSignature<bool>::Type, &fourBytes );
	if( result<0 )
	{
		error=std::error_code( -result, std::generic_category() );
		return;
	}
	returnValue=fourBytes;
}
