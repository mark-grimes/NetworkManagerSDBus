#pragma once
#include <string>
#include <cstdint>
#include <utility>

namespace libnm
{
	namespace detail
	{
		/** @brief Concatenates integer_sequence together
		 */
		template<typename... Sequences> struct SequenceCat;
		template<typename T,T... values1,T... values2>
		struct SequenceCat<std::integer_sequence<T,values1...>,std::integer_sequence<T,values2...>>
		{
			using values = std::integer_sequence<T,values1...,values2...>;
		};
		template<typename T,T... values1,T... values2,typename... Sequences>
		struct SequenceCat<std::integer_sequence<T,values1...>,std::integer_sequence<T,values2...>,Sequences...>
		{
			using values = typename SequenceCat<std::integer_sequence<T,values1...,values2...>,Sequences...>::values;
		};

		/* @brief Convert a std::integer_sequence<char,...> to a null terminated array of chars. */
		template<typename T> struct ConvertToString;
		template<char... Chars>
		struct ConvertToString<std::integer_sequence<char,Chars...>>
		{
			static constexpr char string[]={Chars...,0};
		};
	} // end of namespace detail

	template<typename... Ts> struct TypeSignature {};

	template<> struct TypeSignature<std::string>
	{
		using sequence=std::integer_sequence<char,'s'>;
		static constexpr char const* Type=&detail::ConvertToString<sequence>::string[0];
	};

	template<> struct TypeSignature<bool>
	{
		using sequence=std::integer_sequence<char,'b'>;
		static constexpr char const* Type=&detail::ConvertToString<sequence>::string[0];
	};

	template<> struct TypeSignature<uint32_t>
	{
		using sequence=std::integer_sequence<char,'u'>;
		static constexpr char const* Type=&detail::ConvertToString<sequence>::string[0];
	};

	template<> struct TypeSignature<>
	{
		using sequence=std::integer_sequence<char>;
		static constexpr char const* Type=&detail::ConvertToString<sequence>::string[0];
	};

	template<typename T,typename... Ts> struct TypeSignature<T,Ts...>
	{
		using sequence=typename detail::SequenceCat<typename TypeSignature<T>::sequence,typename TypeSignature<Ts>::sequence...>::values;
		static constexpr char const* Type=&detail::ConvertToString<sequence>::string[0];
	};
} // end of namespace libnm
