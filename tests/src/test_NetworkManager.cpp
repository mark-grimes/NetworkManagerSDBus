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
			while( !callbackReturned ) sd_bus_process( bus, nullptr ); // Run the event loop until the callback occurs
			// I have no way of knowing what the network state on the test device is. I'll
			// just assume it has full connectivity for now. At least the error message will
			// give an indication if the call fails or just that the network is in a different
			// state.
			CHECK( state==libnm::NetworkManager::ConnectivityState::FULL );
		}
	}
} // end of SCENARIO NetworkManager
