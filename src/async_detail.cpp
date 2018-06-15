#include "libnm/detail/async_detail.h"
#include "libnm/SDBusMessage.h"

int libnm::detail::messageHandler( sd_bus_message *pMessage, void *pUserData, sd_bus_error* pError )
{
	libnm::SDBusMessage message(pMessage);
	if( pUserData )
	{
		libnm::detail::CallbackData* pMyUserData=reinterpret_cast<libnm::detail::CallbackData*>(pUserData);
		pMyUserData->callback( std::move(message) );
		delete pMyUserData;
	}
}
