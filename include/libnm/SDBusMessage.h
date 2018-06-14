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
		SDBusMessage( sd_bus_message* pMessage );
		SDBusMessage( const SDBusMessage& other );
		SDBusMessage( SDBusMessage&& other );
		SDBusMessage& operator=( const SDBusMessage& other );
		SDBusMessage& operator=( SDBusMessage&& other );
		~SDBusMessage();
		operator sd_bus_message**();
		template<typename... Ts>
		void read( Ts&... results );
	protected:
		sd_bus_message* pMessage_;
	};

} // end of namespace libnm

#include <stdexcept>
#include <systemd/sd-bus.h>
#include "libnm/TypeSignature.h"

template<typename... Ts>
void libnm::SDBusMessage::read( Ts&... returnValues )
{
	int result=sd_bus_message_read( pMessage_, TypeSignature<Ts...>::Type, &returnValues... );
	if( result<0 )
	{
		throw std::runtime_error("Failed to parse response message: "+std::string(strerror(-result)));
	}
}
namespace libnm
{
	template<> void SDBusMessage::read<std::string>( std::string& returnValue );
	template<> void SDBusMessage::read<std::string,std::string>( std::string& returnValue1, std::string& returnValue2 );
	template<> void SDBusMessage::read<bool>( bool& returnValue );
}
