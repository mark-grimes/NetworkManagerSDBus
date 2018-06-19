#include "libnm/SDBusObject.h"

libnm::SDBusObject::SDBusObject( const char* service, const char* path )
	: service_(service), path_(path)
{
	// No operation besides the initialiser list
}

libnm::SDBusObject::~SDBusObject()
{
	// No operation
}

const char* libnm::SDBusObject::service() const
{
	return service_.c_str();
}

const char* libnm::SDBusObject::path() const
{
	return path_.c_str();
}
