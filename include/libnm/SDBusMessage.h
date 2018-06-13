#pragma once

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
		SDBusMessage( const SDBusMessage& other ) = delete;
		SDBusMessage( SDBusMessage&& other ) = delete;
		SDBusMessage& operator=( const SDBusMessage& other ) = delete;
		SDBusMessage& operator=( SDBusMessage&& other ) = delete;
		~SDBusMessage();
		operator sd_bus_message**();
		template<typename T>
		void read( T& result );
	protected:
		sd_bus_message* pMessage_;
	};

} // end of namespace libnm

#include <stdexcept>
#include <systemd/sd-bus.h>
#include "libnm/TypeSignature.h"

template<typename T>
void libnm::SDBusMessage::read( T& returnValue )
{
	int result=sd_bus_message_read( pMessage_, TypeSignature<T>::Type, &returnValue );
	if( result<0 )
	{
		throw std::runtime_error("Failed to parse response message: "+std::string(strerror(-result)));
	}
}
namespace libnm
{
	template<> void SDBusMessage::read<std::string>( std::string& returnValue );
	template<> void SDBusMessage::read<bool>( bool& returnValue );
}
