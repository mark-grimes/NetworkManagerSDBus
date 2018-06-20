#include "libnm/Device.h"
#include "libnm/SDBus.h"
#include "catch.hpp"
#include <systemd/sd-bus.h>

SCENARIO( "Test that Device works as expected", "[Device]" )
{
	GIVEN( "An instance of Device" )
	{
		libnm::SDBus bus;
		// Loop back device is usually device 1
		libnm::Device loopbackDevice("/org/freedesktop/NetworkManager/Devices/1");

		WHEN( "Checking the interface name" )
		{
			std::string interface;
			CHECK_NOTHROW( interface=loopbackDevice.Interface(bus) );
			CHECK( interface=="lo" );
		}
	}
} // end of SCENARIO Device
