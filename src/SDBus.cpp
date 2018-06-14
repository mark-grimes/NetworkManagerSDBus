#include "libnm/SDBus.h"
#include <stdexcept>
#include <systemd/sd-bus.h>

libnm::SDBus::SDBus()
{
	int result=sd_bus_open_system(&pBus_);
	if( result<0 )
	{
		throw std::runtime_error("Failed to connect to system bus: "+std::string(strerror(-result)));
	}
}

libnm::SDBus::SDBus( const SDBus& other )
{
	pBus_=other.pBus_;
	if( pBus_ ) sd_bus_ref( pBus_ );
}

libnm::SDBus::SDBus( SDBus&& other )
{
	pBus_=other.pBus_;
	other.pBus_=nullptr;
}

libnm::SDBus& libnm::SDBus::operator=( const SDBus& other )
{
	pBus_=other.pBus_;
	if( pBus_ ) sd_bus_ref( pBus_ );
	return *this;
}

libnm::SDBus& libnm::SDBus::operator=( SDBus&& other )
{
	pBus_=other.pBus_;
	other.pBus_=nullptr;
	return *this;
}

libnm::SDBus::~SDBus()
{
	if( pBus_ ) sd_bus_unref( pBus_ );
}

libnm::SDBus::operator sd_bus*()
{
	return pBus_;
}

int libnm::SDBus::fileDescriptor() const
{
	return sd_bus_get_fd( pBus_ );
}
