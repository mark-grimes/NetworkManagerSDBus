#include "libnm/SDBusObject.h"

libnm::SDBusObject::SDBusObject( const char* path )
	: path_(path)
{
	// No operation besides the initialiser list
}

libnm::SDBusObject::~SDBusObject()
{
	// No operation
}

const char* libnm::SDBusObject::path() const
{
	return path_.c_str();
}
