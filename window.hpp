#pragma once

#include "camel.hpp"

/**
* To create a window, use the class WindowMode. WindowMode can
	* be constructed with optional paramters name, width, & height
	* create a window with .create()
	* be passed into the window constructor
* To use a window, use the class Window. Window can
	* be constructed with WindowMode
	* draw pixels with .pixel(x, y, color)
		* x/y are uint
		* colth .open() (Fallible)
	* close wor is a type::byte[3] (r, g, b)
	* open wiith .close() (Fallible)
*/

extern class camel::window::WindowMode;
extern class camel::window::Window;

#if defined(_WIN32)
#define Windows 1
#else
#define Windows 0
#endif

class camel::window::WindowMode
{
public:
	camel::type::uint width;
	camel::type::uint height;
	const wchar_t* name;

	WindowMode(const wchar_t* arg = L"Window", camel::type::uint x = 512, camel::type::uint y = 512)
	{
		width = x;
		height = y;
		name = arg;
	}
};

#if Windows

#include <windows.h>

class camel::window::Window
{
protected:
	HWND window;				// hWindow
	HDC context;				// hDeviceContext
	WNDCLASS wclass = { 0 };	// WindowClass
	const wchar_t* name;
	camel::type::uint width = 0, height = 0;
	bool is_open;
	mutable camel::event::Event event;
public:
	// Getter Methods
	camel::type::uint get_width() const { return this->width; }
	camel::type::uint get_height() const { return this->height; }
	bool const window_open() const { return this->is_open; }

	// Construction/Destruction
	Window(WindowMode mode)
	{
		wclass.lpfnWndProc = DefWindowProc;
		wclass.hInstance = GetModuleHandle(NULL);
		wclass.hbrBackground = HBRUSH(1);
		wclass.lpszClassName = L"WClass";
		RegisterClass(&wclass);

		width = mode.width;
		height = mode.height;
		name = mode.name;
		is_open = false;
	}

	~Window() { close(); }

	// Creating/Destroying
	camel::type::Fallible open()
	{
		if (is_open) return camel::type::Fallible::Failiure;

		window = CreateWindow
		(
			L"WClass", name,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			width, height, 0, 0,
			GetModuleHandle(0), 0
		);

		if (!window) return camel::type::Fallible::Failiure;

		context = GetDC(window);
		ShowWindow(window, 1);
		UpdateWindow(window);

		is_open = true;

		return camel::type::Fallible::Success;
	}

	camel::type::Fallible close()
	{
		if (!is_open) return camel::type::Fallible::Failiure;
		ReleaseDC(window, context);
		DestroyWindow(window);
		context = 0;
		window = 0;

		is_open = false;

		return camel::type::Fallible::Success;
	}

	// Modifying
	camel::type::Fallible refresh() {
		if (close() == camel::type::Fallible::Failiure) return camel::type::Fallible::Failiure;
		if (open() == camel::type::Fallible::Failiure) return camel::type::Fallible::Failiure;
	}

	camel::type::Fallible resize(camel::type::uint new_width, camel::type::uint new_height) {
		width = new_width;
		height = new_height;
		return refresh();
	}

	// Drawing
	camel::type::Fallible pixel(camel::type::uint x, camel::type::uint y, camel::type::color input) const
	{
		if (!is_open) return camel::type::Fallible::Failiure;
		COLORREF wcolor =
			(static_cast<COLORREF>(input.red) << 16) |
			(static_cast<COLORREF>(input.green) << 8) |
			static_cast<COLORREF>(input.blue);
		SetPixel(context, x, y, wcolor);
		return camel::type::Fallible::Success;
	}

	// Event Handling
	camel::type::Fallible poll_event() const {
		if (!is_open) return camel::type::Fallible::Failiure;
		MSG msg = { 0 };
		PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		switch (msg.message) {
		case WM_QUIT:
			event = camel::event::Event
			(
				camel::event::EventType::Close
			);
			return camel::type::Fallible::Success;
		break;
		case WM_SIZE:
			event = camel::event::Event
			(
				camel::event::EventType::Resize,
				LOWORD(msg.lParam),
				HIWORD(msg.lParam)
			);
			return camel::type::Fallible::Success;
		break;
		case WM_LBUTTONDOWN:
			event = camel::event::Event
			(
				camel::event::EventType::Mouse_Click,
				LOWORD(msg.lParam),
				HIWORD(msg.lParam)
			);
			return camel::type::Fallible::Success;
		break;
		case WM_KEYDOWN:
			event = camel::event::Event
			(
				camel::event::EventType::Key_Press,
				static_cast<wchar_t>(msg.wParam)
			);
			return camel::type::Fallible::Success;
		break;
		default:
			return camel::type::Fallible::Failiure;
		}
	}

	const camel::event::Event& get_event() {
		return event;
	}
};

#endif
