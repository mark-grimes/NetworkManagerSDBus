#include "libnm/SDBusASIOHandler.h"
#include <catch.hpp>

SCENARIO( "Test that SDBusASIOHandler works as expected", "[SDBusASIOHandler]" )
{
	GIVEN( "An SDBusASIOHandler instance" )
	{
		asio::io_service service;
		// Need to wrap in a pointer so that I can put the constructor in a REQUIRE block
		std::unique_ptr<libnm::SDBusASIOHandler> pWorkHandler;
		REQUIRE_NOTHROW( pWorkHandler.reset(new libnm::SDBusASIOHandler(service)) );

		// For now all I'm doing is making sure instantiating an instance doesn't throw exceptions
		// TODO Add some actual tests
		libnm::SDBus* pBus=nullptr;
		CHECK_NOTHROW( pBus=&pWorkHandler->bus() );
		CHECK( pBus!=nullptr );
	} // end of GIVEN An SDBusASIOHandler instance
}
