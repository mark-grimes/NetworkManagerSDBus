#pragma once
#include <string>

namespace libnm
{
	/** @brief Class to represent the path of an object in sd-bus
	 * @author Mark Grimes (mark.grimes@rymapt.com)
	 * @date 16/Jun/2018
	 * @copyright Copyright 2018 Rymapt Ltd. Licence to be decided.
	 */
	class SDBusObject
	{
	public:
		SDBusObject( const char* path );
		SDBusObject( const SDBusObject& other ) = default;
		SDBusObject( SDBusObject&& other ) = default;
		SDBusObject& operator=( const SDBusObject& other ) = default;
		SDBusObject& operator=( SDBusObject&& other ) = default;
		~SDBusObject();

		const char* path() const;
	protected:
		std::string path_;
	};

} // end of namespace libnm

// Also specialise TypeSignature so that other code knows what to do with the class
#include "libnm/TypeSignature.h"
template<> struct libnm::TypeSignature<libnm::SDBusObject>
{
	using sequence=std::integer_sequence<char,'o'>;
	static constexpr char const* Type=&detail::ConvertToString<sequence>::string[0];
	using proxy_type=char*;
};
