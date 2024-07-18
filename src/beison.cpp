#include <beison.h>
#include <beicode.h>

#include <inttypes.h>

#include <charconv>
#include <regex>
#include <utility>
#include <cmath>

namespace beison
{
	using VK = ValueKind;

	// struct Value {
	namespace impl
	{
		// TODO: add a value_kind value template once we get rid of VS2015 support
		template<ValueKind Vk>
		using ValueKindConstant = std::integral_constant<ValueKind, Vk>;

		struct ValueImpl
		{
			VK tag;
			union
			{
				std::nullptr_t null;
				bool boolean;
				int64_t integer;
				double number;
				std::string string;
				Array array;
				Object object;
			};

			ValueImpl(ValueKindConstant<VK::Null> vk, std::nullptr_t) : tag(vk), null() { }
			ValueImpl(ValueKindConstant<VK::Boolean> vk, bool b) : tag(vk), boolean(b) { }
			ValueImpl(ValueKindConstant<VK::Integer> vk, int64_t i) : tag(vk), integer(i) { }
			ValueImpl(ValueKindConstant<VK::Number> vk, double d) : tag(vk), number(d) { }
			ValueImpl(ValueKindConstant<VK::String> vk, std::string&& s) : tag(vk), string(std::move(s)) { }
			ValueImpl(ValueKindConstant<VK::String> vk, const std::string& s) : tag(vk), string(s) { }
			ValueImpl(ValueKindConstant<VK::Array> vk, Array&& arr) : tag(vk), array(std::move(arr)) { }
			ValueImpl(ValueKindConstant<VK::Array> vk, const Array& arr) : tag(vk), array(arr) { }
			ValueImpl(ValueKindConstant<VK::Object> vk, Object&& obj) : tag(vk), object(std::move(obj)) { }
			ValueImpl(ValueKindConstant<VK::Object> vk, const Object& obj) : tag(vk), object(obj) { }

			ValueImpl& operator=(ValueImpl&& other) noexcept
			{
				switch (other.tag)
				{
					case VK::Null: return internal_assign(VK::Null, &ValueImpl::null, other);
					case VK::Boolean: return internal_assign(VK::Boolean, &ValueImpl::boolean, other);
					case VK::Integer: return internal_assign(VK::Integer, &ValueImpl::integer, other);
					case VK::Number: return internal_assign(VK::Number, &ValueImpl::number, other);
					case VK::String: return internal_assign(VK::String, &ValueImpl::string, other);
					case VK::Array: return internal_assign(VK::Array, &ValueImpl::array, other);
					case VK::Object: return internal_assign(VK::Object, &ValueImpl::object, other);
				}
			}

			~ValueImpl() { destroy_underlying(); }

		private:
			template<class T>
			ValueImpl& internal_assign(ValueKind vk, T ValueImpl::*mp, ValueImpl& other) noexcept
			{
				if (tag == vk)
				{
					this->*mp = std::move(other.*mp);
				}
				else
				{
					destroy_underlying();
					auto* address = &(this->*mp);
					new (address) T(std::move(other.*mp));
					tag = vk;
				}

				return *this;
			}

			void destroy_underlying() noexcept
			{
				switch (tag)
				{
					case VK::String: string.~basic_string(); break;
					case VK::Array: array.~Array(); break;
					case VK::Object: object.~Object(); break;
					default: break;
				}
				new (&null) std::nullptr_t();
				tag = VK::Null;
			}
		};
	}

	using impl::ValueImpl;
	using impl::ValueKindConstant;

	VK Value::kind() const noexcept
	{
		if (underlying_)
		{
			return underlying_->tag;
		}
		else
		{
			return VK::Null;
		}
	}

	bool Value::is_null() const noexcept { return kind() == VK::Null; }
	bool Value::is_boolean() const noexcept { return kind() == VK::Boolean; }
	bool Value::is_integer() const noexcept { return kind() == VK::Integer; }
	bool Value::is_number() const noexcept
	{
		auto k = kind();
		return k == VK::Integer || k == VK::Number;
	}
	bool Value::is_string() const noexcept { return kind() == VK::String; }
	bool Value::is_array() const noexcept { return kind() == VK::Array; }
	bool Value::is_object() const noexcept { return kind() == VK::Object; }

	bool Value::boolean() const noexcept
	{
		if (!is_boolean()) std::abort();
		return underlying_->boolean;
	}
	int64_t Value::integer() const noexcept
	{
		if (!is_integer()) std::abort();
		return underlying_->integer;
	}
	double Value::number() const noexcept
	{
		auto k = kind();
		if (k == VK::Number)
		{
			return underlying_->number;
		}
		else
		{
			return static_cast<double>(integer());
		}
	}
	std::string_view Value::string() const noexcept
	{
		if (!is_string()) std::abort();
		return underlying_->string;
	}

	const Array& Value::array() const& noexcept
	{
		if (!is_array()) std::abort();
		return underlying_->array;
	}
	Array& Value::array() & noexcept
	{
		if (!is_array()) std::abort();
		return underlying_->array;
	}
	Array&& Value::array() && noexcept { return std::move(this->array()); }

	const Object& Value::object() const& noexcept
	{
		if (!is_object()) std::abort();
		return underlying_->object;
	}
	Object& Value::object() & noexcept
	{
		if (!is_object()) std::abort();
		return underlying_->object;
	}
	Object&& Value::object() && noexcept { return std::move(this->object()); }

	Value::Value() noexcept = default;
	Value::Value(Value&&) noexcept = default;
	Value& Value::operator=(Value&&) noexcept = default;

	Value::Value(const Value& other)
	{
		switch (other.kind())
		{
			case ValueKind::Null: return; // default construct underlying_
			case ValueKind::Boolean:
				underlying_.reset(new ValueImpl(ValueKindConstant<VK::Boolean>(), other.underlying_->boolean));
				break;
			case ValueKind::Integer:
				underlying_.reset(new ValueImpl(ValueKindConstant<VK::Integer>(), other.underlying_->integer));
				break;
			case ValueKind::Number:
				underlying_.reset(new ValueImpl(ValueKindConstant<VK::Number>(), other.underlying_->number));
				break;
			case ValueKind::String:
				underlying_.reset(new ValueImpl(ValueKindConstant<VK::String>(), other.underlying_->string));
				break;
			case ValueKind::Array:
				underlying_.reset(new ValueImpl(ValueKindConstant<VK::Array>(), other.underlying_->array));
				break;
			case ValueKind::Object:
				underlying_.reset(new ValueImpl(ValueKindConstant<VK::Object>(), other.underlying_->object));
				break;
			default: std::abort();
		}
	}

	Value& Value::operator=(const Value& other)
	{
		switch (other.kind())
		{
			case ValueKind::Null: underlying_.reset(); break;
			case ValueKind::Boolean:
				underlying_.reset(new ValueImpl(ValueKindConstant<VK::Boolean>(), other.underlying_->boolean));
				break;
			case ValueKind::Integer:
				underlying_.reset(new ValueImpl(ValueKindConstant<VK::Integer>(), other.underlying_->integer));
				break;
			case ValueKind::Number:
				underlying_.reset(new ValueImpl(ValueKindConstant<VK::Number>(), other.underlying_->number));
				break;
			case ValueKind::String:
				underlying_.reset(new ValueImpl(ValueKindConstant<VK::String>(), other.underlying_->string));
				break;
			case ValueKind::Array:
				underlying_.reset(new ValueImpl(ValueKindConstant<VK::Array>(), other.underlying_->array));
				break;
			case ValueKind::Object:
				underlying_.reset(new ValueImpl(ValueKindConstant<VK::Object>(), other.underlying_->object));
				break;
			default: std::abort();
		}

		return *this;
	}

	Value::~Value() = default;

	Value Value::null(std::nullptr_t) noexcept { return Value(); }
	Value Value::boolean(bool b) noexcept
	{
		Value val;
		val.underlying_ = std::make_unique<ValueImpl>(ValueKindConstant<VK::Boolean>(), b);
		return val;
	}
	Value Value::integer(int64_t i) noexcept
	{
		Value val;
		val.underlying_ = std::make_unique<ValueImpl>(ValueKindConstant<VK::Integer>(), i);
		return val;
	}
	Value Value::number(double d) noexcept
	{
		if (!std::isfinite(d)) std::abort();
		Value val;
		val.underlying_ = std::make_unique<ValueImpl>(ValueKindConstant<VK::Number>(), d);
		return val;
	}
	Value Value::string(std::string s) noexcept
	{
		if (!beicode::utf8_is_valid_string(s.data(), s.data() + s.size()))
		{
			std::abort();
		}
		Value val;
		val.underlying_ = std::make_unique<ValueImpl>(ValueKindConstant<VK::String>(), std::move(s));
		return val;
	}
	Value Value::array(Array&& arr) noexcept
	{
		Value val;
		val.underlying_ = std::make_unique<ValueImpl>(ValueKindConstant<VK::Array>(), std::move(arr));
		return val;
	}
	Value Value::array(const Array& arr) noexcept
	{
		Value val;
		val.underlying_ = std::make_unique<ValueImpl>(ValueKindConstant<VK::Array>(), arr);
		return val;
	}
	Value Value::object(Object&& obj) noexcept
	{
		Value val;
		val.underlying_ = std::make_unique<ValueImpl>(ValueKindConstant<VK::Object>(), std::move(obj));
		return val;
	}
	Value Value::object(const Object& obj) noexcept
	{
		Value val;
		val.underlying_ = std::make_unique<ValueImpl>(ValueKindConstant<VK::Object>(), obj);
		return val;
	}

	bool operator==(const Value& lhs, const Value& rhs)
	{
		if (lhs.kind() != rhs.kind()) return false;

		switch (lhs.kind())
		{
			case ValueKind::Null: return true;
			case ValueKind::Boolean: return lhs.underlying_->boolean == rhs.underlying_->boolean;
			case ValueKind::Integer: return lhs.underlying_->integer == rhs.underlying_->integer;
			case ValueKind::Number: return lhs.underlying_->number == rhs.underlying_->number;
			case ValueKind::String: return lhs.underlying_->string == rhs.underlying_->string;
			case ValueKind::Array: return lhs.underlying_->string == rhs.underlying_->string;
			case ValueKind::Object: return lhs.underlying_->string == rhs.underlying_->string;
			default: std::abort();
		}
	}
	// } struct Value
	// struct Array {
	Value& Array::push_back(Value&& value)
	{
		underlying_.push_back(std::move(value));
		return underlying_.back();
	}
	Object& Array::push_back(Object&& obj) { return push_back(Value::object(std::move(obj))).object(); }
	Array& Array::push_back(Array&& arr) { return push_back(Value::array(std::move(arr))).array(); }
	Value& Array::insert_before(iterator it, Value&& value)
	{
		size_t index = it - underlying_.begin();
		underlying_.insert(it, std::move(value));
		return underlying_[index];
	}
	Object& Array::insert_before(iterator it, Object&& obj)
	{
		return insert_before(it, Value::object(std::move(obj))).object();
	}
	Array& Array::insert_before(iterator it, Array&& arr)
	{
		return insert_before(it, Value::array(std::move(arr))).array();
	}
	bool operator==(const Array& lhs, const Array& rhs) { return lhs.underlying_ == rhs.underlying_; }
	// } struct Array
	// struct Object {
	Value& Object::insert(std::string key, Value&& value)
	{
		if (contains(key)) std::abort();
		underlying_.push_back({std::move(key), std::move(value)});
		return underlying_.back().second;
	}
	Value& Object::insert(std::string key, const Value& value)
	{
		if (contains(key)) std::abort();
		underlying_.push_back({std::move(key), value});
		return underlying_.back().second;
	}
	Array& Object::insert(std::string key, Array&& value)
	{
		return insert(std::move(key), Value::array(std::move(value))).array();
	}
	Array& Object::insert(std::string key, const Array& value)
	{
		return insert(std::move(key), Value::array(value)).array();
	}
	Object& Object::insert(std::string key, Object&& value)
	{
		return insert(std::move(key), Value::object(std::move(value))).object();
	}
	Object& Object::insert(std::string key, const Object& value)
	{
		return insert(std::move(key), Value::object(value)).object();
	}

	Value& Object::insert_or_replace(std::string key, Value&& value)
	{
		auto v = get(key);
		if (v)
		{
			*v = std::move(value);
			return *v;
		}
		else
		{
			underlying_.push_back({std::move(key), std::move(value)});
			return underlying_.back().second;
		}
	}
	Value& Object::insert_or_replace(std::string key, const Value& value)
	{
		auto v = get(key);
		if (v)
		{
			*v = value;
			return *v;
		}
		else
		{
			underlying_.push_back({std::move(key), std::move(value)});
			return underlying_.back().second;
		}
	}
	Array& Object::insert_or_replace(std::string key, Array&& value)
	{
		return insert_or_replace(std::move(key), Value::array(std::move(value))).array();
	}
	Array& Object::insert_or_replace(std::string key, const Array& value)
	{
		return insert_or_replace(std::move(key), Value::array(value)).array();
	}
	Object& Object::insert_or_replace(std::string key, Object&& value)
	{
		return insert_or_replace(std::move(key), Value::object(std::move(value))).object();
	}
	Object& Object::insert_or_replace(std::string key, const Object& value)
	{
		return insert_or_replace(std::move(key), Value::object(value)).object();
	}

	auto Object::internal_find_key(std::string_view key) const noexcept -> underlying_t::const_iterator
	{
		return std::find_if(
			underlying_.begin(), underlying_.end(), [key](const auto& pair) { return pair.first == key; });
	}

	// returns whether the key existed
	bool Object::remove(std::string_view key) noexcept
	{
		auto it = internal_find_key(key);
		if (it == underlying_.end())
		{
			return false;
		}
		else
		{
			underlying_.erase(it);
			return true;
		}
	}

	Value* Object::get(std::string_view key) noexcept
	{
		auto it = internal_find_key(key);
		if (it == underlying_.end())
		{
			return nullptr;
		}
		else
		{
			return &underlying_[it - underlying_.begin()].second;
		}
	}
	const Value* Object::get(std::string_view key) const noexcept
	{
		auto it = internal_find_key(key);
		if (it == underlying_.end())
		{
			return nullptr;
		}
		else
		{
			return &it->second;
		}
	}

	bool operator==(const Object& lhs, const Object& rhs) { return lhs.underlying_ == rhs.underlying_; }
	// } struct Object

	// auto parse() {
	namespace
	{
		struct Parser
		{
			beicode::Utf8Decoder decoder;
			explicit Parser(std::string_view text) : decoder(text) { }

			char32_t next(std::error_code& ec) noexcept
			{
				decoder.next(ec);
				return cur();
			}
			char32_t cur() noexcept
			{
				return decoder.is_eof() ? beicode::end_of_file : *decoder;
			}

			void skip_whitespace(std::error_code& ec) noexcept
			{
				ec.clear();

				char32_t current = cur();
				while (is_whitespace(current))
				{
					current = next(ec);
					if (ec) return;
				}
			}

			static constexpr bool is_digit(char32_t code_point) noexcept
			{
				return code_point >= '0' && code_point <= '9';
			}
			static constexpr bool is_hex_digit(char32_t code_point) noexcept
			{
				return is_digit(code_point) || (code_point >= 'a' && code_point <= 'f') ||
					   (code_point >= 'A' && code_point <= 'F');
			}
			static bool is_number_start(char32_t code_point) noexcept
			{
				return code_point == '-' || is_digit(code_point);
			}

			static constexpr bool is_whitespace(char32_t code_point) noexcept
			{
				return code_point == ' ' || code_point == '\t' || code_point == '\n' || code_point == '\r';
			}

			static unsigned char from_hex_digit(char32_t code_point) noexcept
			{
				if (is_digit(code_point))
				{
					return static_cast<unsigned char>(code_point) - '0';
				}
				else if (code_point >= 'a' && code_point <= 'f')
				{
					return static_cast<unsigned char>(code_point) - 'a' + 10;
				}
				else if (code_point >= 'A' && code_point <= 'F')
				{
					return static_cast<unsigned char>(code_point) - 'A' + 10;
				}
				else
				{
					std::abort();
				}
			}

			// parses a _single_ code point of a string -- either a literal code point, or an escape sequence
			// returns end_of_file if it reaches an unescaped '"'.
			// _does not_ pair escaped surrogates -- returns the literal surrogate.
			// if ec is set, this function may return any value.
			char32_t parse_string_code_point(std::error_code& ec) noexcept
			{
				char32_t current = cur();
				if (current == '"')
				{
					next(ec);
					return beicode::end_of_file;
				}
				else if (current <= 0x001F)
				{
					next(ec);
					ec = json_parse_errc::ControlCharacterInString; // ignore ec
					return beicode::end_of_file;
				}
				else if (beicode::utf16_is_surrogate_code_point(current))
				{
					next(ec);
					ec = json_parse_errc::SurrogateCharacterInString; // ignore ec
					return beicode::end_of_file;
				}
				else if (current != '\\')
				{
					next(ec);
					return current;
				}

				// cur == '\\'
				if (decoder.is_eof())
				{
					ec = json_parse_errc::EofAfterEscape;
					return beicode::end_of_file;
				}
				current = next(ec);
				if (ec) return beicode::end_of_file;

				switch (current)
				{
					case '"': next(ec); return '"';
					case '\\': next(ec); return '\\';
					case '/': next(ec); return '/';
					case 'b': next(ec); return '\b';
					case 'f': next(ec); return '\f';
					case 'n': next(ec); return '\n';
					case 'r': next(ec); return '\r';
					case 't': next(ec); return '\t';
					case 'u':
					{
						char16_t code_unit = 0;
						for (int i = 0; i < 4; ++i)
						{
							current = next(ec);
							if (ec) return beicode::end_of_file;

							if (current == beicode::end_of_file)
							{
								ec = json_parse_errc::EofInUnicodeEscape;
								return beicode::end_of_file;
							}
							if (!is_hex_digit(current))
							{
								ec = json_parse_errc::InvalidCharacterInUnicodeEscape;
								return beicode::end_of_file;
							}
							code_unit *= 16;
							code_unit += from_hex_digit(current);
						}

						next(ec);
						return code_unit;
					}
					default:
						ec = json_parse_errc::InvalidCharacterAfterEscape;
						return beicode::end_of_file;
				}
			}

			std::string parse_string(std::error_code& ec) noexcept
			{
				if (cur() != '"') std::abort();

				next(ec);
				if (ec) return {};

				std::string res;
				char32_t previous_leading_surrogate = beicode::end_of_file;
				while (!decoder.is_eof())
				{
					auto code_point = parse_string_code_point(ec);

					if (previous_leading_surrogate != beicode::end_of_file)
					{
						if (beicode::utf16_is_trailing_surrogate_code_point(code_point))
						{
							const auto full_code_point =
								beicode::utf16_surrogates_to_code_point(previous_leading_surrogate, code_point);
							beicode::utf8_append_code_point(res, full_code_point);
							previous_leading_surrogate = beicode::end_of_file;
							continue;
						}

						beicode::utf8_append_code_point(res, previous_leading_surrogate);
						previous_leading_surrogate = beicode::end_of_file;
					}

					if (beicode::utf16_is_leading_surrogate_code_point(code_point))
					{
						previous_leading_surrogate = code_point;
					}
					else if (code_point == beicode::end_of_file)
					{
						return res;
					}
					else
					{
						beicode::utf8_append_code_point(res, code_point);
					}
				}

				ec = json_parse_errc::EofInString;
				return res;
			}

			Value parse_number(std::error_code& ec) noexcept
			{
				if (!is_number_start(cur())) std::abort();

				bool floating = false;
				bool negative = false; // negative & 0 -> floating, so keep track of it
				std::string number_to_parse;

				char32_t current = cur();
				if (cur() == '-')
				{
					number_to_parse.push_back('-');
					negative = true;
					current = next(ec);
					if (ec) return {};
					if (current == beicode::end_of_file)
					{
						ec = json_parse_errc::EofAfterMinusSign;
						return {};
					}
				}

				if (current == '0')
				{
					current = next(ec);
					if (ec) return {};

					if (current == '.')
					{
						current = next(ec);
						if (ec) return {};

						number_to_parse.append("0.");
						floating = true;
					}
					else if (is_digit(current))
					{
						ec = json_parse_errc::DigitsAfterLeadingZero;
						return {};
					}
					else
					{
						if (negative)
						{
							return Value::number(-0.0);
						}
						else
						{
							return Value::integer(0);
						}
					}
				}

				while (is_digit(current))
				{
					number_to_parse.push_back(static_cast<char>(current));
					current = next(ec);
					if (ec) return {};
				}
				if (!floating && current == '.')
				{
					floating = true;
					number_to_parse.push_back('.');
					current = next(ec);
					if (!is_digit(current))
					{
						ec = json_parse_errc::NoDigitsAfterDecimalPoint;
						return {};
					}
					while (is_digit(current))
					{
						number_to_parse.push_back(static_cast<char>(current));
						current = next(ec);
						if (ec) return {};
					}
				}

				if (floating)
				{
					double result = 0;
					int scanf_return = std::sscanf(number_to_parse.c_str(), "%lf", &result);
					if (scanf_return == 1)
					{
						if (std::abs(result) < INFINITY)
						{
							return Value::number(result);
						}
						else
						{
							ec = json_parse_errc::FloatingPointNumberTooBig;
							return {};
						}
					}
					else
					{
						std::abort(); // this should not be possible
					}
				}
				else
				{
					int64_t result = 0;
					auto from_chars_return = std::from_chars(number_to_parse.data(), number_to_parse.data() + number_to_parse.size(), result);
					if (from_chars_return.ec != std::errc{})
					{
						ec = std::make_error_code(from_chars_return.ec);
						return {};
					}
					if (from_chars_return.ptr != number_to_parse.data() + number_to_parse.size())
					{
						std::abort(); // again, this should not be possible
					}
					return Value::integer(result);
				}

				return Value{};
			}

			Value parse_keyword(std::error_code& ec) noexcept
			{
				char32_t current = cur();
				const char32_t* rest;
				Value val;
				switch (current)
				{
					case 't': // parse true
						rest = U"rue";
						val = Value::boolean(true);
						break;
					case 'f': // parse false
						rest = U"alse";
						val = Value::boolean(false);
						break;
					case 'n': // parse null
						rest = U"ull";
						val = Value::null(nullptr);
						break;
					default: std::abort();
				}

				for (const char32_t* rest_it = rest; *rest_it != '\0'; ++rest_it)
				{
					current = next(ec);
					if (ec) return {};

					if (current == beicode::end_of_file)
					{
						ec = json_parse_errc::EofInKeyword;
						return {};
					}
					if (current != *rest_it)
					{
						ec = json_parse_errc::InvalidCharacterInKeyword;
						return {};
					}
				}
				next(ec);

				return val;
			}

			Value parse_array(std::error_code& ec) noexcept
			{
				if (cur() != '[') std::abort();
				next(ec);
				if (ec) return {};

				Array arr;
				bool first = true;
				for (;;)
				{
					skip_whitespace(ec);
					if (ec) return {};

					char32_t current = cur();
					if (current == beicode::end_of_file)
					{
						ec = json_parse_errc::EofInArray;
						return {};
					}
					if (current == ']')
					{
						next(ec);
						return Value::array(std::move(arr));
					}

					if (first)
					{
						first = false;
					}
					else if (current == ',')
					{
						next(ec);
						if (ec) return {};

						skip_whitespace(ec);
						if (ec) return {};

						current = cur();
						if (current == beicode::end_of_file)
						{
							ec = json_parse_errc::EofInArray;
							return {};
						}
						if (current == ']')
						{
							ec = json_parse_errc::TrailingCommaInArray;
							return Value::array(std::move(arr));
						}
					}
					else
					{
						ec = json_parse_errc::ExpectedCommaOrArrayEnd;
						return {};
					}

					arr.push_back(parse_value(ec));
					if (ec) return {};
				}
			}

			std::pair<std::string, Value> parse_kv_pair(std::error_code& ec) noexcept
			{
				auto current = cur();

				std::pair<std::string, Value> res = {std::string(""), Value()};

				if (current == beicode::end_of_file)
				{
					ec = json_parse_errc::EofInObject;
					return res;
				}
				if (current != '"')
				{
					ec = json_parse_errc::ExpectedPropertyName;
					return res;
				}
				res.first = parse_string(ec);
				if (ec) return res;

				skip_whitespace(ec);
				if (ec) return res;

				current = cur();
				if (current == ':')
				{
					next(ec);
					if (ec) return res;
					skip_whitespace(ec);
					if (ec) return res;
				}
				else if (current == beicode::end_of_file)
				{
					ec = json_parse_errc::EofInObject;
					return res;
				}
				else
				{
					ec = json_parse_errc::ExpectedColon;
					return res;
				}

				res.second = parse_value(ec);

				return res;
			}

			Value parse_object(std::error_code& ec) noexcept
			{
				char32_t current = cur();

				if (current != '{') std::abort();
				next(ec);
				if (ec) return {};

				Object obj;
				bool first = true;
				for (;;)
				{
					skip_whitespace(ec);
					if (ec) return {};

					current = cur();
					if (current == beicode::end_of_file)
					{
						ec = json_parse_errc::EofInObject;
						return {};
					}
					else if (current == '}')
					{
						next(ec);
						return Value::object(std::move(obj));
					}

					if (first)
					{
						first = false;
					}
					else if (current == ',')
					{
						next(ec);
						if (ec) return {};
						skip_whitespace(ec);
						if (ec) return {};

						current = cur();
						if (current == beicode::end_of_file)
						{
							ec = json_parse_errc::EofInObject;
							return {};
						}
						else if (current == '}')
						{
							ec = json_parse_errc::TrailingCommaInObject;
							return {};
						}
					}
					else
					{
						ec = json_parse_errc::ExpectedCommaOrObjectEnd;
						return {};
					}

					auto val = parse_kv_pair(ec);
					if (ec) return {};

					obj.insert(std::move(val.first), std::move(val.second));
				}
			}

			Value parse_value(std::error_code& ec) noexcept
			{
				char32_t current = cur();
				if (current == beicode::end_of_file) std::abort();

				switch (current)
				{
					case '{': return parse_object(ec);
					case '[': return parse_array(ec);
					case '"': return Value::string(parse_string(ec));
					case 'n':
					case 't':
					case 'f': return parse_keyword(ec);
					default:
						if (is_number_start(current))
						{
							return parse_number(ec);
						}
						else
						{
							ec = json_parse_errc::InvalidCharacterForValueStart;
							return {};
						}
				}
			}

			Value parse(std::error_code& ec) noexcept
			{
				skip_whitespace(ec);
				if (ec) return {};

				auto val = parse_value(ec);
				if (ec) return val;

				skip_whitespace(ec);
				if (ec) return val;

				if (!decoder.is_eof())
				{
					ec = json_parse_errc::ExpectedEofAfterTopLevelValue;
				}
				return val;
			}
		};

		struct JsonParseCategory: std::error_category
		{
			const char* name() const noexcept override
			{
				return "json parsing";
			}

			std::string message(int err) const override
			{
				switch (static_cast<json_parse_errc>(err))
				{
					case json_parse_errc::NoError: return "no error";

					// string parse errors
					case json_parse_errc::EofInString: return "end-of-file reached in the middle of a string";
					case json_parse_errc::ControlCharacterInString: return "control character discovered in string";
					case json_parse_errc::SurrogateCharacterInString: return "surrogate character discovered in string";
					case json_parse_errc::EofAfterEscape: return "end-of-file reached after a \\";
					case json_parse_errc::InvalidCharacterAfterEscape: return "invalid character after a \\";
					case json_parse_errc::EofInUnicodeEscape: return "end-of-file reached during a unicode escape";
					case json_parse_errc::InvalidCharacterInUnicodeEscape: return "invalid character during a unicode escape";

					// number parse errors
					case json_parse_errc::EofAfterMinusSign: return "end-of-file after a minus sign";
					case json_parse_errc::DigitsAfterLeadingZero: return "digits after a leading zero";
					case json_parse_errc::NoDigitsAfterDecimalPoint: return "no digits after a decimal point";
					case json_parse_errc::FloatingPointNumberTooBig: return "floating point number was too big to fit in a double";
					case json_parse_errc::InvalidFloatingPointConstant: return "floating point constant was invalid";

					// keyword parse errors
					case json_parse_errc::EofInKeyword: return "end-of-file while parsing a keyword";
					case json_parse_errc::InvalidCharacterInKeyword: return "invalid character in a keyword";

					// array parse errors
					case json_parse_errc::EofInArray: return "end-of-file while parsing an array";
					case json_parse_errc::TrailingCommaInArray: return "array had a trailing comma";
					case json_parse_errc::ExpectedCommaOrArrayEnd: return "invalid character in array, expected comma or array end";

					// object parse errors
					case json_parse_errc::EofInObject: return "end-of-file while parsing an object";
					case json_parse_errc::TrailingCommaInObject: return "object had a trailing comma";
					case json_parse_errc::ExpectedCommaOrObjectEnd: return "invalid character in object, expected comma or object end";
					case json_parse_errc::ExpectedPropertyName: return "invalid character in object, expected property name";
					case json_parse_errc::ExpectedColon: return "no colon after a property name";

					// value parse errors
					case json_parse_errc::InvalidCharacterForValueStart: return "invalid character to start a value";

					// other errors
					case json_parse_errc::ExpectedEofAfterTopLevelValue: return "expected end-of-file after finishing parsing";
				}

				return "unknown error";
			}
		};
	}

	const std::error_category& json_parse_category() noexcept
	{
		static const JsonParseCategory t;
		return t;
	}

	Value parse(std::string_view text, std::error_code& ec) noexcept
	{
		return Parser{text}.parse(ec);
	}
	// } auto parse()

	namespace
	{
		struct Stringifier
		{
			std::string& buffer;

			void append_indent(int indent)
			{
				buffer.append(indent * 2, ' ');
			};

			void append_unicode_escape(char16_t code_unit)
			{
				buffer.append("\\u");

				// AFAIK, there's no standard way of doing this?
				constexpr const char hex_digit[16] = {
					'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

				buffer.push_back(hex_digit[(code_unit >> 12) & 0x0F]);
				buffer.push_back(hex_digit[(code_unit >> 8) & 0x0F]);
				buffer.push_back(hex_digit[(code_unit >> 4) & 0x0F]);
				buffer.push_back(hex_digit[(code_unit >> 0) & 0x0F]);
			}

			// taken from the ECMAScript 2020 standard, 24.5.2.2: Runtime Semantics: QuoteJSONString
			void append_quoted_json_string(std::string_view sv)
			{
				// Table 66: JSON Single Character Escape Sequences
				constexpr static std::pair<char32_t, const char*> escape_sequences[] = {
					{0x0008, R"(\b)"}, // BACKSPACE
					{0x0009, R"(\t)"}, // CHARACTER TABULATION
					{0x000A, R"(\n)"}, // LINE FEED (LF)
					{0x000C, R"(\f)"}, // FORM FEED (FF)
					{0x000D, R"(\r)"}, // CARRIAGE RETURN (CR)
					{0x0022, R"(\")"}, // QUOTATION MARK
					{0x005C, R"(\\)"},  // REVERSE SOLIDUS
				};
				using std::begin;
				using std::end;

				// 1. Let product be the String value consisting solely of the code unit 0x0022 (QUOTATION MARK).
				buffer.push_back('"');

				// 2. For each code point C in ! UTF16DecodeString(value), do
				// (note that we use utf8 instead of utf16)
				for (auto code_point : beicode::Utf8Decoder(sv))
				{
					// a. If C is listed in the "Code Point" column of Table 66, then
					const auto match = std::find_if(begin(escape_sequences),
													end(escape_sequences),
													[code_point](const std::pair<char32_t, const char*>& attempt) {
														return attempt.first == code_point;
													});
					// i. Set product to the string-concatenation of product and the escape sequence for C as
					// specified in the "Escape Sequence" column of the corresponding row.
					if (match != end(escape_sequences))
					{
						buffer.append(match->second);
						continue;
					}

					// b. Else if C has a numeric value less than 0x0020 (SPACE), or if C has the same numeric value as
					// a leading surrogate or trailing surrogate, then
					if (code_point < 0x0020 || beicode::utf16_is_surrogate_code_point(code_point))
					{
						// i. Let unit be the code unit whose numeric value is that of C.
						// ii. Set product to the string-concatenation of product and beicodeEscape(unit).
						append_unicode_escape(static_cast<char16_t>(code_point));
						break;
					}

					// c. Else,
					// i. Set product to the string-concatenation of product and the UTF16Encoding of C.
					// (again, we use utf-8 here instead)
					beicode::utf8_append_code_point(buffer, code_point);
				}

				// 3. Set product to the string-concatenation of product and the code unit 0x0022 (QUOTATION MARK).
				buffer.push_back('"');
			}

			void stringify_object(const Object& obj, int current_indent)
			{
				buffer.push_back('{');
				if (obj.size() != 0)
				{
					bool first = true;

					for (const auto& el : obj)
					{
						if (!first)
						{
							buffer.push_back(',');
						}
						first = false;

						buffer.push_back('\n');
						append_indent(current_indent + 1);

						append_quoted_json_string(el.first);
						buffer.append(": ");
						stringify(el.second, current_indent + 1);
					}
					buffer.push_back('\n');
					append_indent(current_indent);
				}
				buffer.push_back('}');
			}

			void stringify_array(const Array& arr, int current_indent)
			{
				buffer.push_back('[');
				if (arr.size() == 0)
				{
					buffer.push_back(']');
				}
				else
				{
					bool first = true;

					for (const auto& el : arr)
					{
						if (!first)
						{
							buffer.push_back(',');
						}
						first = false;

						buffer.push_back('\n');
						append_indent(current_indent + 1);

						stringify(el, current_indent + 1);
					}
					buffer.push_back('\n');
					append_indent(current_indent);
					buffer.push_back(']');
				}
			}

			void stringify(const Value& value, int current_indent)
			{
				switch (value.kind())
				{
					case VK::Null: buffer.append("null"); break;
					case VK::Boolean:
					{
						auto v = value.boolean();
						buffer.append(v ? "true" : "false");
						break;
					}
					// TODO: switch to `to_chars` once we are able to remove support for old compilers
					case VK::Integer: buffer.append(std::to_string(value.integer())); break;
					case VK::Number: buffer.append(std::to_string(value.number())); break;
					case VK::String:
					{
						append_quoted_json_string(value.string());
						break;
					}
					case VK::Array:
					{
						stringify_array(value.array(), current_indent);
						break;
					}
					case VK::Object:
					{
						stringify_object(value.object(), current_indent);
						break;
					}
				}
			}
		};
	}

	std::string stringify(const Value& value)
	{
		std::string res;
		Stringifier{res}.stringify(value, 0);
		res.push_back('\n');
		return res;
	}
	std::string stringify(const Object& obj)
	{
		std::string res;
		Stringifier{res}.stringify_object(obj, 0);
		res.push_back('\n');
		return res;
	}
	std::string stringify(const Array& arr)
	{
		std::string res;
		Stringifier{res}.stringify_array(arr, 0);
		res.push_back('\n');
		return res;
	}
	// } auto stringify()
}
