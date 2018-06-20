#include "libnm/SDBusASIOHandler.h"
#include <functional>
#include <iostream> // TODO - add a proper logging class
#include <systemd/sd-bus.h>

libnm::SDBusASIOHandler::SDBusASIOHandler( ASIO_NAMESPACE::io_service& service )
	: busFileDescriptor_( service, bus_.fileDescriptor() )
{
	// Bind the callback for all possible operation on the file descriptor
	busFileDescriptor_.async_wait(
		ASIO_NAMESPACE::posix::stream_descriptor::wait_read,
		std::bind(&SDBusASIOHandler::handler,this,ASIO_NAMESPACE::posix::stream_descriptor::wait_read,std::placeholders::_1)
	);
	busFileDescriptor_.async_wait(
		ASIO_NAMESPACE::posix::stream_descriptor::wait_write,
		std::bind(&SDBusASIOHandler::handler,this,ASIO_NAMESPACE::posix::stream_descriptor::wait_write,std::placeholders::_1)
	);
	busFileDescriptor_.async_wait(
		ASIO_NAMESPACE::posix::stream_descriptor::wait_error,
		std::bind(&SDBusASIOHandler::handler,this,ASIO_NAMESPACE::posix::stream_descriptor::wait_error,std::placeholders::_1)
	);
}

libnm::SDBusASIOHandler::~SDBusASIOHandler()
{
	// Let sd-bus handle freeing the file descriptor
	busFileDescriptor_.release();
}

libnm::SDBus& libnm::SDBusASIOHandler::bus()
{
	return bus_;
}

void libnm::SDBusASIOHandler::handler( ASIO_NAMESPACE::posix::stream_descriptor::wait_type waitType, const std::error_code& error )
{
	if( error ) std::cerr << "Got error in sd_bus handler " << error.message() << std::endl;
	else
	{
		while( true )
		{
			int result=sd_bus_process( bus_, nullptr );
			if( result<0 )
			{
				std::cout << "Error processing sd-bus loop " << strerror(-result) << std::endl;
				break;
			}
			else if( result==0 ) break; // Nothing left to do, so return control to ASIO
		}
		busFileDescriptor_.async_wait(
			waitType,
			std::bind(&SDBusASIOHandler::handler,this,waitType,std::placeholders::_1)
		);
	}
}
