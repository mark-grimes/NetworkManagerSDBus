#include "libnm/NetworkManager.h"
#include <cstdint>
#include <stdexcept>
#include "libnm/SDBusObject.h"
#include "libnm/SDBusMessage.h"
#include "libnm/SDBus.h"

std::ostream& operator<<( std::ostream& stream, const libnm::NetworkManager::ConnectivityState& state )
{
	switch( state )
	{
		case libnm::NetworkManager::ConnectivityState::UNKNOWN: stream << "ConnectivityState::UNKNOWN";break;
		case libnm::NetworkManager::ConnectivityState::NONE: stream << "ConnectivityState::NONE";break;
		case libnm::NetworkManager::ConnectivityState::PORTAL: stream << "ConnectivityState::PORTAL";break;
		case libnm::NetworkManager::ConnectivityState::LIMITED: stream << "ConnectivityState::LIMITED";break;
		case libnm::NetworkManager::ConnectivityState::FULL: stream << "ConnectivityState::FULL";break;
		// default: throw std::runtime_error("libnm::NetworkManager::ConnectivityState() unexpected state value");
	}
	return stream;
}

// Since the SDBusObject doesn't ever change state it's safe to use a single instance for everything.
// That way I can limit the includes in the header file.
libnm::SDBusObject globalNetworkManager( "org.freedesktop.NetworkManager", "/org/freedesktop/NetworkManager" );

void libnm::NetworkManager::CheckConnectivity( libnm::SDBus& bus, std::function<void(ConnectivityState)> callback ) const
{
	globalNetworkManager.callMethodAsync( bus, "org.freedesktop.NetworkManager", "CheckConnectivity",
		[userCallback=std::move(callback)]( libnm::SDBusMessage reply ){
			uint32_t state;
			reply.read( state );
			switch( state )
			{
				// case NM_CONNECTIVITY_UNKNOWN: return ConnectivityState::UNKNOWN;
				// case NM_CONNECTIVITY_NONE: return ConnectivityState::NONE;
				// case NM_CONNECTIVITY_PORTAL: return ConnectivityState::PORTAL;
				// case NM_CONNECTIVITY_LIMITED: return ConnectivityState::LIMITED;
				// case NM_CONNECTIVITY_FULL: return ConnectivityState::FULL;
				case 0: userCallback( ConnectivityState::UNKNOWN ); break;
				case 1: userCallback( ConnectivityState::NONE ); break;
				case 2: userCallback( ConnectivityState::PORTAL ); break;
				case 3: userCallback( ConnectivityState::LIMITED ); break;
				case 4: userCallback( ConnectivityState::FULL ); break;
				// default: throw std::runtime_error("libnm::NetworkManager::ConnectivityState() unexpected state value");
			}
		} );
}

void libnm::NetworkManager::Enable( libnm::SDBus& bus, bool enable, std::function<void()> callback )
{
	globalNetworkManager.callMethodAsync( bus, "org.freedesktop.NetworkManager", "Enable",
		[userCallback=std::move(callback)]( libnm::SDBusMessage reply ){
			userCallback();
		} );
}
