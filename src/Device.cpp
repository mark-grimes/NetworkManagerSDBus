#include "libnm/Device.h"
#include "libnm/SDBus.h"

libnm::Device::Device( const char* path )
	: libnm::SDBusObject("org.freedesktop.NetworkManager",path)
{
	// No operation besides the initialiser list
}

libnm::Device::Device( const char* service, const char* path )
	: libnm::SDBusObject(service,path)
{
	// No operation besides the initialiser list
}

libnm::Device::~Device()
{
	// No operation
}

std::string libnm::Device::Interface( libnm::SDBus& bus ) const
{
	std::string value;
	this->getParameter( bus, "org.freedesktop.NetworkManager.Device", "Interface", value );
	return value;
}
