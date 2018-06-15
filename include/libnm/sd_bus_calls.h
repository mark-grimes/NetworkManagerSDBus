#pragma once
#include "libnm/detail/async_detail.h"
//
// Forward declarations
//
namespace libnm
{
	class SDBusMessage;
}

namespace libnm
{
	template<typename return_type>
	void getParameter( libnm::SDBus& bus, const char* service, const char* object, const char* interface, const char* property, return_type& value );

	template<typename... arg_types>
	libnm::SDBusMessage callMethod( libnm::SDBus& bus, const char* service, const char* object, const char* interface, const char* method, arg_types&&... args );

	template<typename... arg_types>
	void callMethodAsync( libnm::SDBus& bus, const char* service, const char* object, const char* interface, const char* method, libnm::detail::CallbackData::callback_type callback, arg_types&&... args );
} // end of namespace libnm



#include <systemd/sd-bus.h>
#include "libnm/SDBusMessage.h"

template<typename return_type>
void libnm::getParameter( libnm::SDBus& bus, const char* service, const char* object, const char* interface, const char* property, return_type& value )
{
	libnm::SDBusError error;
	libnm::SDBusMessage message;
	int result=sd_bus_get_property( bus, service, object, interface, property, error, message, libnm::TypeSignature<return_type>::Type );
	if( result<0 )
	{
		throw std::runtime_error(std::string("Failed to issue parameter get: ")+error->message);
	}
	message.read( value );
}

template<typename... arg_types>
libnm::SDBusMessage libnm::callMethod( libnm::SDBus& bus, const char* service, const char* object, const char* interface, const char* method, arg_types&&... args )
{
	libnm::SDBusError error;
	libnm::SDBusMessage message;
	int result=sd_bus_call_method( bus, service, object, interface, method, error, message, libnm::TypeSignature<arg_types...>::Type, args... );
	if( result<0 )
	{
		throw std::runtime_error(std::string("Failed to issue method call: ")+error->message);
	}
	return message;
}

template<typename... arg_types>
void libnm::callMethodAsync( libnm::SDBus& bus, const char* service, const char* object, const char* interface, const char* method, libnm::detail::CallbackData::callback_type callback, arg_types&&... args )
{
	libnm::detail::CallbackData* pUserData=new libnm::detail::CallbackData;
	pUserData->callback=std::move(callback);
	int result=sd_bus_call_method_async( bus, nullptr, service, object, interface, method, &libnm::detail::messageHandler, pUserData, libnm::TypeSignature<arg_types...>::Type, args... );
	if( result<0 )
	{
		delete pUserData;
		throw std::runtime_error("Failed to issue method async call");
	}
}
