#include "libnm/TypeSignature.h"
#include "catch.hpp"

SCENARIO( "Test that TypeSignature gives correct values", "[TypeSignature]" )
{
	WHEN( "Testing single value signatures" )
	{
		// char array and char* do not compare equal in catch.hpp even if they refer to the
		// same data, so use string view suffix instead (e.g. '"s"sv')
		using namespace std::literals;
		// For some reason get linker errors if using the expression directly in the CHECK
		// statement. Pretty sure it's an issue with catch.hpp, see
		// https://github.com/catchorg/Catch2/issues/1134. Oddly multiple template parameters
		// work fine.
		auto fooString=libnm::TypeSignature<std::string>::Type;
		auto fooBool=libnm::TypeSignature<bool>::Type;
		auto fooU32=libnm::TypeSignature<uint32_t>::Type;
		auto fooNone=libnm::TypeSignature<>::Type;
		CHECK( fooString == "s"sv );
		CHECK( fooBool   == "b"sv );
		CHECK( fooU32    == "u"sv );
		CHECK( fooNone   == ""sv );
	}
	WHEN( "Test multi value signatures" )
	{
		using namespace std::literals;
		CHECK( libnm::TypeSignature<std::string,std::string>::sequence::size()==2 );
		CHECK( libnm::TypeSignature<std::string,std::string>::Type=="ss"sv );
		CHECK( libnm::TypeSignature<bool,bool>::Type=="bb"sv );
		CHECK( libnm::TypeSignature<std::string,bool,uint32_t>::Type=="sbu"sv );
	}
} // end of 'SCENARIO ... TypeSignature'
