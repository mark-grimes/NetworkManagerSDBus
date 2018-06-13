#pragma once
#include <systemd/sd-bus.h>

namespace libnm
{
	/** @brief Wrapper providing safe destroy behaviour for sd_bus_error struct.
	 * @author Mark Grimes (mark.grimes@rymapt.com)
	 * @date 12/Jun/2018
	 * @copyright Copyright 2018 Rymapt Ltd. Licence to be decided.
	 */
	class SDBusError
	{
	public:
		SDBusError();
		SDBusError( const SDBusError& other ) = delete;
		SDBusError( SDBusError&& other ) = delete;
		SDBusError& operator=( const SDBusError& other ) = delete;
		SDBusError& operator=( SDBusError&& other ) = delete;
		~SDBusError();
		sd_bus_error* operator->();
		operator sd_bus_error*();
	protected:
		sd_bus_error error_;
	};

} // end of namespace libnm
