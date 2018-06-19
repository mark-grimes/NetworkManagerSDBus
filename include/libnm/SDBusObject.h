#pragma once
#include <string>
//
// Forward declarations
//
namespace libnm
{
	class SDBus;
}


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
		SDBusObject( const char* service, const char* path );
		SDBusObject( const SDBusObject& other ) = default;
		SDBusObject( SDBusObject&& other ) = default;
		SDBusObject& operator=( const SDBusObject& other ) = default;
		SDBusObject& operator=( SDBusObject&& other ) = default;
		~SDBusObject();

		const char* service() const;
		const char* path() const;
		template<typename return_type>
		void getParameter( libnm::SDBus& bus, const char* interface, const char* property, return_type& value ) const;
	protected:
		std::string service_;
		std::string path_;
	};

} // end of namespace libnm

#include "libnm/sd_bus_calls.h"
template<typename return_type>
void libnm::SDBusObject::getParameter( libnm::SDBus& bus, const char* interface, const char* property, return_type& value ) const
{
	return libnm::getParameter( bus, service_.c_str(), path_.c_str(), interface, property, value );
}

// Also specialise TypeSignature so that other code knows what to do with the class
#include "libnm/TypeSignature.h"
template<> struct libnm::TypeSignature<libnm::SDBusObject>
{
	using sequence=std::integer_sequence<char,'o'>;
	static constexpr char const* Type=&detail::ConvertToString<sequence>::string[0];
	using proxy_type=char*;
};
