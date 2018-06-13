#pragma once

//
// Forward declarations
//
struct sd_bus;


namespace libnm
{
	/** @brief Wrapper providing safe copy/move/destroy behaviour for sd_bus pointer.
	 * @author Mark Grimes (mark.grimes@rymapt.com)
	 * @date 12/Jun/2018
	 * @copyright Copyright 2018 Rymapt Ltd. Licence to be decided.
	 */
	class SDBus
	{
	public:
		SDBus();
		SDBus( const SDBus& other );
		SDBus( SDBus&& other );
		SDBus& operator=( const SDBus& other );
		SDBus& operator=( SDBus&& other );
		~SDBus();
		operator sd_bus*();
	protected:
		sd_bus* pBus_;
	};

} // end of namespace libnm
