#include <iostream>
#include "libnm/version.h"
#include <stdio.h>
#include <stdlib.h>
#include <systemd/sd-bus.h>
#include "libnm/SDBus.h"
#include "libnm/SDBusMessage.h"
#include "libnm/SDBusError.h"
#include "libnm/TypeSignature.h"

template<typename return_type>
void getParameter( libnm::SDBus& bus, const char* service, const char* object, const char* interface, const char* property, return_type& value )
{
	libnm::SDBusError error;
	libnm::SDBusMessage message;
	int result=sd_bus_get_property( bus, service, object, interface, property, error, message, libnm::TypeSignature<return_type>::Type );
	if( result<0 )
	{
		throw std::runtime_error(std::string("Failed to issue method call: ")+error->message);
	}
	message.read( value );
}

int main( int argc, char* argv[] )
{
	std::cout << "LibNM program with version " << libnm::version::GitDescribe << "\n";
	libnm::SDBus bus;
	std::string version;
	bool wirelessEnabled;
	uint32_t state;

	getParameter( bus, "org.freedesktop.NetworkManager", "/org/freedesktop/NetworkManager", "org.freedesktop.NetworkManager", "Version", version );
	getParameter( bus, "org.freedesktop.NetworkManager", "/org/freedesktop/NetworkManager", "org.freedesktop.NetworkManager", "WirelessEnabled", wirelessEnabled );
	getParameter( bus, "org.freedesktop.NetworkManager", "/org/freedesktop/NetworkManager", "org.freedesktop.NetworkManager", "State", state );
	std::cout << "Version is " << version << "\n";
	std::cout << "wirelessEnabled is " << wirelessEnabled << "\n";
	std::cout << "state is " << state << "\n";

	return 0;
}
