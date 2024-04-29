#pragma once

#define let const auto
#define var auto

namespace camel
{
namespace type
{
// Long names
typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned __int64 uint;
typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

// Short names
typedef unsigned __int8 u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;
typedef __int8 i8;
typedef __int16 i16;
typedef __int32 i32;
typedef __int64 i64;

// Special Types
typedef unsigned __int8 byte;

// Enum Types
enum Fallible
{
	Failiure = false,
	Success = true
};

// Other Types
class color
{
public:
	byte red;
	byte green;
	byte blue;
	color(byte r, byte g, byte b) : red(r), green(g), blue(b) {}
	color(uint32 arg) {
		red = (arg << 24) >> 24;
		green = (arg << 16) >> 24;
		blue = (arg << 8) >> 24;
	}
};

template<class T>
class Option
{
private:
	T data;
	bool defined;
public:
	bool null() { return defined; }
	const T& assume(const T& value) { return defined ? data : value; }
	void nullify() { defined = false; }

	const T& unwrap() {
		if (!defined) throw "NULLOPTION UNWRAPPED";
		return data;
	}

	Option<T>& operator=(T arg) {
		data = arg;
		defined = true;
		return *this;
	}
};
}

namespace event
{
// Enum Types
enum EventType
{
	None = false,
	Mouse_Click = 1,
	Key_Press = 2,
	Resize = 3,
	Close = 4
};

// Class Types
class EventData
{
public:
	camel::type::Option<int> x;
	camel::type::Option<int> y;
	camel::type::Option<wchar_t> key;
};

class Event
{
private:
	camel::event::EventType type;
	camel::event::EventData data;
public:
	Event() {
		type = None;
		data.key.nullify();
		data.x.nullify();
		data.y.nullify();
	}

	Event(EventType sort, camel::type::uint horizontal = 0, camel::type::uint vertical = 0, wchar_t keypress = ' ') {
		switch (sort) {
		case Mouse_Click:
			data.key = keypress;
			data.x.nullify();
			data.y.nullify();
		break;
		case Key_Press:
		case Resize:
			data.key = keypress;
			data.x = horizontal;
			data.y = vertical;
		break;
		case Close:
			data.key.nullify();
			data.x.nullify();
			data.y.nullify();
		break;
		}
	}

	auto x() const { return data.x; }
	auto y() const { return data.y; }
	auto width() const { return data.x; }
	auto height() const { return data.y; }
	auto key() const { return data.key; }

	auto typeof() const { return type; }
};

}

namespace window
{
class Window;
class WindowMode;
}
}
