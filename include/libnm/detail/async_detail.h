#pragma once
#include <functional>
#include <systemd/sd-bus.h>
//
// Forward declarations
//
namespace libnm
{
	class SDBusMessage;
}

namespace libnm::detail
{
	struct CallbackData
	{
		typedef std::function<void(libnm::SDBusMessage message)> callback_type;
		callback_type callback;
	};

	int messageHandler( sd_bus_message* pMessage, void *pUserData, sd_bus_error* pError );
} // end of namespace libnm::detail
