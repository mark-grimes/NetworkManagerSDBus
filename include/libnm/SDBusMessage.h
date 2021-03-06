#pragma once

#include <system_error>
//
// Forward declarations
//
struct sd_bus_message;

namespace libnm
{
	/** @brief Wrapper providing safe destroy behaviour for sd_bus_error struct.
	 * @author Mark Grimes (mark.grimes@rymapt.com)
	 * @date 12/Jun/2018
	 * @copyright Copyright 2018 Rymapt Ltd. Licence to be decided.
	 */
	class SDBusMessage
	{
	public:
		SDBusMessage();
		SDBusMessage( sd_bus_message* pMessage );
		SDBusMessage( const SDBusMessage& other );
		SDBusMessage( SDBusMessage&& other );
		SDBusMessage& operator=( const SDBusMessage& other );
		SDBusMessage& operator=( SDBusMessage&& other );
		~SDBusMessage();
		operator sd_bus_message**();
		template<typename... Ts>
		void read( Ts&... results );
		template<typename... Ts>
		void readNoThrow( std::error_code& error, Ts&... results );
	protected:
		sd_bus_message* pMessage_;
	};

} // end of namespace libnm

#include <stdexcept>
#include <vector>
#include <systemd/sd-bus.h>
#include "libnm/TypeSignature.h"

// Need a helper struct so that I can partially specialise the templates
namespace libnm::detail
{
	template<typename... Ts> struct ReadHelper;
}

template<typename... Ts>
void libnm::SDBusMessage::read( Ts&... returnValues )
{
	std::error_code error;
	readNoThrow( error, returnValues... );
	if( error ) throw std::system_error(error,"libnm::SDBusMessage::read()");
}

template<typename... Ts>
void libnm::SDBusMessage::readNoThrow( std::error_code& error, Ts&... returnValues )
{
	// Delegate to the ReadHelper struct because that can be partially specialised, whereas
	// functions can't.
	libnm::detail::ReadHelper<Ts...>::read( pMessage_, returnValues..., error );
}

template<typename... Ts>
struct libnm::detail::ReadHelper
{
	static void read( sd_bus_message* pMessage, Ts&... returnValues, std::error_code& error )
	{
		int result=sd_bus_message_read( pMessage, libnm::TypeSignature<Ts...>::Type, &returnValues... );
		if( result<0 )
		{
			error=std::error_code( -result, std::generic_category() );
			return;
		}
	}
};

template<typename value_type,typename... Ts>
struct libnm::detail::ReadHelper<std::vector<value_type,Ts...>>
{
	static void read( sd_bus_message* pMessage, std::vector<value_type,Ts...>& returnValue, std::error_code& error )
	{
		const char* sender=sd_bus_message_get_sender(pMessage);
		int result=sd_bus_message_enter_container( pMessage, 'a', TypeSignature<value_type>::Type );
		if( result<0 )
		{
			error=std::error_code( -result, std::generic_category() );
			return;
		}

		typename TypeSignature<value_type>::proxy_type proxy;
		while( result>0 )
		{
			result=sd_bus_message_read( pMessage, TypeSignature<value_type>::Type, &proxy );
			if( result<0 )
			{
				error=std::error_code( -result, std::generic_category() );
				return;
			}
			else if( result>0 ) returnValue.emplace_back( sender, proxy );
		}
		result=sd_bus_message_exit_container( pMessage );
		if( result<0 )
		{
			error=std::error_code( -result, std::generic_category() );
			return;
		}
	}
};

// Specialisations for particular types
template<> struct libnm::detail::ReadHelper<std::string>{ static void read( sd_bus_message* pMessage, std::string& returnValue, std::error_code& error ); };
template<> struct libnm::detail::ReadHelper<std::string,std::string>{ static void read( sd_bus_message* pMessage, std::string& returnValue1, std::string& returnValue2, std::error_code& error ); };
template<> struct libnm::detail::ReadHelper<bool>{ static void read( sd_bus_message* pMessage, bool& returnValue, std::error_code& error ); };
