#include "libnm/NetworkManager.h"
#include "libnm/SDBus.h"
#include "catch.hpp"
#include <systemd/sd-bus.h>

SCENARIO( "Test that NetworkManager works as expected", "[NetworkManager]" )
{
	GIVEN( "An instance of NetworkManager" )
	{
		libnm::SDBus bus;
		libnm::NetworkManager manager;

		WHEN( "Checking the state" )
		{
			bool callbackReturned=false;
			libnm::NetworkManager::ConnectivityState state;
			CHECK_NOTHROW( manager.CheckConnectivity( bus,
				[&callbackReturned,&state](libnm::NetworkManager::ConnectivityState newState){
					state=newState;
					callbackReturned=true;
				}
			) );
			while( !callbackReturned ) sd_bus_process( bus, nullptr );
			CHECK( state==libnm::NetworkManager::ConnectivityState::FULL );
		}
	}
} // end of SCENARIO NetworkManager
