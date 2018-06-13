#include "libnm/SDBusError.h"
#include <systemd/sd-bus.h>

libnm::SDBusError::SDBusError()
	: error_(SD_BUS_ERROR_NULL)
{
}

libnm::SDBusError::~SDBusError()
{
	sd_bus_error_free( &error_ );
}

sd_bus_error* libnm::SDBusError::operator->()
{
	return &error_;
}

libnm::SDBusError::operator sd_bus_error*()
{
	return &error_;
}
