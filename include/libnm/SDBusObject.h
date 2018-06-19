#pragma once
#include <string>
#include <functional>
#include <system_error>
//
// Forward declarations
//
namespace libnm
{
	class SDBus;
}


namespace libnm
{
	/** @brief Class to represent the path of an object in sd-bus
	 * @author Mark Grimes (mark.grimes@rymapt.com)
	 * @date 16/Jun/2018
	 * @copyright Copyright 2018 Rymapt Ltd. Licence to be decided.
	 */
	class SDBusObject
	{
	public:
		SDBusObject( const char* service, const char* path );
		SDBusObject( const SDBusObject& other ) = default;
		SDBusObject( SDBusObject&& other ) = default;
		SDBusObject& operator=( const SDBusObject& other ) = default;
		SDBusObject& operator=( SDBusObject&& other ) = default;
		~SDBusObject();

		const char* service() const;
		const char* path() const;
		template<typename return_type>
		void getParameter( libnm::SDBus& bus, const char* interface, const char* property, return_type& value ) const;
		template<typename return_type>
		void getParameterAsync( libnm::SDBus& bus, const char* interface, const char* property, std::function<void(return_type&&,std::error_code)> callback ) const;
	protected:
		std::string service_;
		std::string path_;
	};

} // end of namespace libnm

#include "libnm/sd_bus_calls.h"
#include <iostream> // TODO - remove once I've put in a proper logging system
#include <cerrno>

template<typename return_type>
void libnm::SDBusObject::getParameter( libnm::SDBus& bus, const char* interface, const char* property, return_type& value ) const
{
	return libnm::getParameter( bus, service_.c_str(), path_.c_str(), interface, property, value );
}

template<typename return_type>
void libnm::SDBusObject::getParameterAsync( libnm::SDBus& bus, const char* interface, const char* property, std::function<void(return_type&&,std::error_code)> callback ) const
{
	libnm::callMethodAsync( bus, service_.c_str(), path_.c_str(), "org.freedesktop.DBus.Properties", "Get",
		[userCallback=std::move(callback)]( libnm::SDBusMessage message ){
			return_type value;
			std::error_code error;
			try
			{
				// I don't particularly understand this section of code since sd-bus has almost no documentation,
				// it just follows the libsystemd code for sd_bus_get_property
				// https://github.com/systemd/systemd/blob/v238/src/libsystemd/sd-bus/bus-convenience.c#L316
				// but changes the sd_bus_call_method for the asynchronous one.
				sd_bus_message* pMessage=*message;
				int result=sd_bus_message_enter_container(pMessage, 'v', libnm::TypeSignature<return_type>::Type );
				if( result<0 ) error=std::make_error_code(std::errc::invalid_argument);
				else message.readNoThrow(error,value);
			}
			catch( const std::exception& exception )
			{
				// TODO Figure out a proper logging system
				std::cerr << "Error in SDBusObject::getParameterAsync callback: " << exception.what() << std::endl;
				error=std::make_error_code(std::errc::invalid_argument);
			}

			try { userCallback( std::move(value), error ); }
			catch( const std::exception& exception )
			{
				// TODO Figure out a proper logging system
				std::cerr << "Error in SDBusObject::getParameterAsync, user callback threw exception: " << exception.what() << std::endl;
			}
		}, interface, property );
}

// Also specialise TypeSignature so that other code knows what to do with the class
#include "libnm/TypeSignature.h"
template<> struct libnm::TypeSignature<libnm::SDBusObject>
{
	using sequence=std::integer_sequence<char,'o'>;
	static constexpr char const* Type=&detail::ConvertToString<sequence>::string[0];
	using proxy_type=char*;
};
