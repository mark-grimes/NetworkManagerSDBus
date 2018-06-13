#pragma once
#include <string>
#include <cstdint>

namespace libnm
{
	template<typename T> struct TypeSignature {};

	template<> struct TypeSignature<std::string>
	{
		static constexpr char const* Type="s";
	};

	template<> struct TypeSignature<bool>
	{
		static constexpr char const* Type="b";
	};

	template<> struct TypeSignature<uint32_t>
	{
		static constexpr char const* Type="u";
	};
} // end of namespace libnm
