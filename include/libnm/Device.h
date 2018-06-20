#pragma once
#include "libnm/SDBusObject.h"

namespace libnm
{
	/** Object to represent NetworkManager devices
	 *
	 * @author Mark Grimes (mark.grimes@rymapt.com)
	 * @date 20/Jun/2018
	 * @copyright Copyright 2018 Rymapt Ltd. Released under the MIT licence.
	 */
	class Device : public libnm::SDBusObject
	{
	public:
		Device( const char* path );
		Device( const char* service, const char* path );
		Device( const Device& other ) = default;
		Device( Device&& other ) = default;
		Device& operator=( const Device& other ) = default;
		Device& operator=( Device&& other ) = default;
		virtual ~Device();

		std::string Interface( libnm::SDBus& bus ) const;
	protected:
	};
} // end of namespace libnm

// Also specialise TypeSignature so that other code knows what to do with the class
#include "libnm/TypeSignature.h"
template<> struct libnm::TypeSignature<libnm::Device>
{
	using sequence=std::integer_sequence<char,'o'>;
	static constexpr char const* Type=&detail::ConvertToString<sequence>::string[0];
	using proxy_type=char*;
};
