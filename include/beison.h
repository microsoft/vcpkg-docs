#pragma once

#include <stddef.h>
#include <stdint.h>

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

#if !defined(BEISON_EXPORT)
#define BEISON_EXPORT
#endif

namespace beison
{
	enum class ValueKind : int
	{
		Null,
		Boolean,
		Integer,
		Number,
		String,
		Array,
		Object
	};

	namespace impl
	{
		struct ValueImpl;
	}

	struct Array;
	struct Object;

	struct Value
	{
		BEISON_EXPORT
		Value() noexcept; // equivalent to Value::null()
		BEISON_EXPORT
		Value(Value&&) noexcept;
		BEISON_EXPORT
		Value(const Value&);
		BEISON_EXPORT
		Value& operator=(Value&&) noexcept;
		BEISON_EXPORT
		Value& operator=(const Value&);
		BEISON_EXPORT
		~Value();

		BEISON_EXPORT
		ValueKind kind() const noexcept;

		BEISON_EXPORT
		bool is_null() const noexcept;
		BEISON_EXPORT
		bool is_boolean() const noexcept;
		BEISON_EXPORT
		bool is_integer() const noexcept;
		// either integer _or_ number
		BEISON_EXPORT
		bool is_number() const noexcept;
		BEISON_EXPORT
		bool is_string() const noexcept;
		BEISON_EXPORT
		bool is_array() const noexcept;
		BEISON_EXPORT
		bool is_object() const noexcept;

		// a.x() asserts when !a.is_x()
		BEISON_EXPORT
		bool boolean() const noexcept;
		BEISON_EXPORT
		int64_t integer() const noexcept;
		BEISON_EXPORT
		double number() const noexcept;
		BEISON_EXPORT
		std::string_view string() const noexcept;

		BEISON_EXPORT
		const Array& array() const& noexcept;
		BEISON_EXPORT
		Array& array() & noexcept;
		BEISON_EXPORT
		Array&& array() && noexcept;

		BEISON_EXPORT
		const Object& object() const& noexcept;
		BEISON_EXPORT
		Object& object() & noexcept;
		BEISON_EXPORT
		Object&& object() && noexcept;

		BEISON_EXPORT
		static Value null(std::nullptr_t) noexcept;
		BEISON_EXPORT
		static Value boolean(bool) noexcept;
		BEISON_EXPORT
		static Value integer(int64_t i) noexcept;
		BEISON_EXPORT
		static Value number(double d) noexcept;
		BEISON_EXPORT
		static Value string(std::string s) noexcept;
		BEISON_EXPORT
		static Value array(Array&&) noexcept;
		BEISON_EXPORT
		static Value array(const Array&) noexcept;
		BEISON_EXPORT
		static Value object(Object&&) noexcept;
		BEISON_EXPORT
		static Value object(const Object&) noexcept;

		BEISON_EXPORT
		friend bool operator==(const Value& lhs, const Value& rhs);
		friend bool operator!=(const Value& lhs, const Value& rhs) { return !(lhs == rhs); }

	private:
		friend struct impl::ValueImpl;
		std::unique_ptr<impl::ValueImpl> underlying_;
	};

	struct Array
	{
	private:
		using underlying_t = std::vector<Value>;

	public:
		Array() = default;
		Array(Array const&) = default;
		Array(Array&&) = default;
		Array& operator=(Array const&) = default;
		Array& operator=(Array&&) = default;
		~Array() = default;

		using iterator = underlying_t::iterator;
		using const_iterator = underlying_t::const_iterator;

		BEISON_EXPORT
		Value& push_back(Value&& value);
		BEISON_EXPORT
		Object& push_back(Object&& value);
		BEISON_EXPORT
		Array& push_back(Array&& value);
		BEISON_EXPORT
		Value& insert_before(iterator it, Value&& value);
		BEISON_EXPORT
		Object& insert_before(iterator it, Object&& value);
		BEISON_EXPORT
		Array& insert_before(iterator it, Array&& value);

		std::size_t size() const noexcept { return this->underlying_.size(); }

		// asserts idx < size
		Value& operator[](size_t idx) noexcept
		{
			if (idx > this->size()) std::abort();
			return this->underlying_[idx];
		}
		const Value& operator[](size_t idx) const noexcept
		{
			if (idx > this->size()) std::abort();
			return this->underlying_[idx];
		}

		iterator begin() { return underlying_.begin(); }
		iterator end() { return underlying_.end(); }
		const_iterator begin() const { return cbegin(); }
		const_iterator end() const { return cend(); }
		const_iterator cbegin() const { return underlying_.cbegin(); }
		const_iterator cend() const { return underlying_.cend(); }

		BEISON_EXPORT
		friend bool operator==(const Array& lhs, const Array& rhs);
		friend bool operator!=(const Array& lhs, const Array& rhs) { return !(lhs == rhs); }

	private:
		underlying_t underlying_;
	};

	struct Object
	{
	private:
		using value_type = std::pair<std::string, Value>;
		using underlying_t = std::vector<value_type>;

		BEISON_EXPORT
		underlying_t::const_iterator internal_find_key(std::string_view key) const noexcept;

	public:
		// these are here for better diagnostics
		Object() = default;
		Object(Object const&) = default;
		Object(Object&&) = default;
		Object& operator=(Object const&) = default;
		Object& operator=(Object&&) = default;
		~Object() = default;

		// asserts if the key is found
		BEISON_EXPORT
		Value& insert(std::string key, Value&& value);
		BEISON_EXPORT
		Value& insert(std::string key, const Value& value);
		BEISON_EXPORT
		Object& insert(std::string key, Object&& value);
		BEISON_EXPORT
		Object& insert(std::string key, const Object& value);
		BEISON_EXPORT
		Array& insert(std::string key, Array&& value);
		BEISON_EXPORT
		Array& insert(std::string key, const Array& value);

		// replaces the value if the key is found, otherwise inserts a new
		// value.
		BEISON_EXPORT
		Value& insert_or_replace(std::string key, Value&& value);
		BEISON_EXPORT
		Value& insert_or_replace(std::string key, const Value& value);
		BEISON_EXPORT
		Object& insert_or_replace(std::string key, Object&& value);
		BEISON_EXPORT
		Object& insert_or_replace(std::string key, const Object& value);
		BEISON_EXPORT
		Array& insert_or_replace(std::string key, Array&& value);
		BEISON_EXPORT
		Array& insert_or_replace(std::string key, const Array& value);

		// returns whether the key existed
		BEISON_EXPORT
		bool remove(std::string_view key) noexcept;

		// asserts on lookup failure
		Value& operator[](std::string_view key) noexcept
		{
			auto res = this->get(key);
			if (!res) std::abort();
			return *res;
		}
		const Value& operator[](std::string_view key) const noexcept
		{
			auto res = this->get(key);
			if (!res) std::abort();
			return *res;
		}

		BEISON_EXPORT
		Value* get(std::string_view key) noexcept;
		BEISON_EXPORT
		const Value* get(std::string_view key) const noexcept;

		bool contains(std::string_view key) const noexcept { return this->get(key); }

		bool is_empty() const noexcept { return size() == 0; }
		std::size_t size() const noexcept { return this->underlying_.size(); }

		struct const_iterator
		{
			using value_type = std::pair<std::string_view, const Value&>;
			using reference = value_type;
			using iterator_category = std::forward_iterator_tag;

			value_type operator*() const noexcept { return *underlying_; }
			const_iterator& operator++() noexcept
			{
				++underlying_;
				return *this;
			}
			const_iterator operator++(int) noexcept
			{
				auto res = *this;
				++underlying_;
				return res;
			}

			bool operator==(const_iterator other) const noexcept { return this->underlying_ == other.underlying_; }
			bool operator!=(const_iterator other) const noexcept { return !(this->underlying_ == other.underlying_); }

		private:
			friend struct Object;
			explicit const_iterator(const underlying_t::const_iterator& it) : underlying_(it) { }
			underlying_t::const_iterator underlying_;
		};
		using iterator = const_iterator;

		const_iterator begin() const noexcept { return this->cbegin(); }
		const_iterator end() const noexcept { return this->cend(); }
		const_iterator cbegin() const noexcept { return const_iterator{this->underlying_.begin()}; }
		const_iterator cend() const noexcept { return const_iterator{this->underlying_.end()}; }

		BEISON_EXPORT
		friend bool operator==(const Object& lhs, const Object& rhs);
		friend bool operator!=(const Object& lhs, const Object& rhs) { return !(lhs == rhs); }

	private:
		underlying_t underlying_;
	};

	enum class json_parse_errc
	{
		NoError = 0,

		// string parse errors
		EofInString,
		ControlCharacterInString,
		SurrogateCharacterInString,
		EofAfterEscape,
		InvalidCharacterAfterEscape,
		EofInUnicodeEscape,
		InvalidCharacterInUnicodeEscape,

		// number parse errors
		EofAfterMinusSign,
		DigitsAfterLeadingZero,
		NoDigitsAfterDecimalPoint,
		FloatingPointNumberTooBig,
		InvalidFloatingPointConstant,

		// keyword parse errors
		EofInKeyword,
		InvalidCharacterInKeyword,

		// array parse errors
		EofInArray,
		TrailingCommaInArray,
		ExpectedCommaOrArrayEnd,

		// object parse errors
		EofInObject,
		TrailingCommaInObject,
		ExpectedCommaOrObjectEnd,
		ExpectedPropertyName,
		ExpectedColon,

		// value parse errors
		InvalidCharacterForValueStart,

		// other errors
		ExpectedEofAfterTopLevelValue,
	};

	BEISON_EXPORT
	const std::error_category& json_parse_category() noexcept;

	inline std::error_code make_error_code(json_parse_errc err) noexcept
	{
		return std::error_code(static_cast<int>(err), json_parse_category());
	}

	BEISON_EXPORT
	Value parse(std::string_view text, std::error_code& ec) noexcept;

	BEISON_EXPORT
	std::string stringify(const Value&);
	BEISON_EXPORT
	std::string stringify(const Object&);
	BEISON_EXPORT
	std::string stringify(const Array&);
}

template <>
struct std::is_error_code_enum<beison::json_parse_errc> : std::true_type {};
