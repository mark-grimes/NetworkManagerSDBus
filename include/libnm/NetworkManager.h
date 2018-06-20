#pragma once

#include <functional>
//
// Forward declarations
//
namespace libnm
{
	class SDBus;
}

namespace libnm
{
	class NetworkManager
	{
	public:
		enum class ConnectivityState { UNKNOWN=0, NONE=1, PORTAL=2, LIMITED=3, FULL=4 };
		void CheckConnectivity( libnm::SDBus& bus, std::function<void(ConnectivityState)> callback ) const;
		void Enable( libnm::SDBus& bus, bool enable, std::function<void()> callback );
	protected:
	};
} // end of namespace libnm

// Operator overload to enable printing ConnectivityState enums
#include <iosfwd>
std::ostream& operator<<( std::ostream& stream, const libnm::NetworkManager::ConnectivityState& state );
