/** @file
 * @brief A class that allows the ASIO event loop to process sd-bus events
 *
 * IMPORTANT: Note that the corresponding cpp file is only compiled into LibNM if the BUILD_WITH_ASIO
 * CMake option is set. If it is not and you try to use this class you will probably get undefined
 * symbol linker errors.
 */
#pragma once

#include "libnm/SDBus.h"

#ifdef ASIO_STANDALONE
#	include <asio.hpp>
	namespace ASIO_NAMESPACE=asio;
#else
#	include <boost/asio.hpp>
	namespace ASIO_NAMESPACE=boost::asio;
#endif

namespace libnm
{
	/** @brief Class to integrate sd-bus event processing into the ASIO event loop
	 *
	 * All sd-bus processing of the member bus instance will happen in the event loop of the
	 * asio::io_service provided at creation time. To get the bus instance use the "bus()" method.
	 *
	 * @author Mark Grimes (mark.grimes@rymapt.com)
	 * @date 15/Jun/2018
	 * @copyright Copyright 2018 Rymapt Ltd. Licence to be decided.
	 */
	class SDBusASIOHandler
	{
	public:
		SDBusASIOHandler( ASIO_NAMESPACE::io_service& service );
		SDBusASIOHandler( const SDBusASIOHandler& other ) = delete;
		SDBusASIOHandler( SDBusASIOHandler&& other ) = delete;
		SDBusASIOHandler& operator=( const SDBusASIOHandler& other ) = delete;
		SDBusASIOHandler& operator=( SDBusASIOHandler&& other ) = delete;
		~SDBusASIOHandler();

		libnm::SDBus& bus();
	protected:
		libnm::SDBus bus_;
		ASIO_NAMESPACE::posix::stream_descriptor busFileDescriptor_;
		void handler( ASIO_NAMESPACE::posix::stream_descriptor::wait_type waitType, const std::error_code& error );
	}; // end of class SDBusASIOHandler
} // end of namespace iotwificonfig
