#pragma once

#include <functional>
#include "libnm/SDBusObject.h"
//
// Forward declarations
//
namespace libnm
{
	class SDBus;
	class Device;
}

namespace libnm
{
	class NetworkManager : public libnm::SDBusObject
	{
	public:
		enum class ConnectivityState { UNKNOWN=0, NONE=1, PORTAL=2, LIMITED=3, FULL=4 };
		NetworkManager();
		NetworkManager( const NetworkManager& other ) = default;
		NetworkManager( NetworkManager&& other ) = default;
		NetworkManager& operator=( const NetworkManager& other ) = default;
		NetworkManager& operator=( NetworkManager&& other ) = default;
		virtual ~NetworkManager();
		void CheckConnectivity( libnm::SDBus& bus, std::function<void(ConnectivityState)> callback ) const;
		void Enable( libnm::SDBus& bus, bool enable, std::function<void()> callback );
		void GetAllDevices( libnm::SDBus& bus, std::function<void(std::vector<libnm::Device>&&)> callback );
	protected:
	};
} // end of namespace libnm

// Operator overload to enable printing ConnectivityState enums
#include <iosfwd>
std::ostream& operator<<( std::ostream& stream, const libnm::NetworkManager::ConnectivityState& state );
