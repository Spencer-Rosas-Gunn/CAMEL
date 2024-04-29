//	CAMEL (C++ Abstract Multimedia Electronic-Software Library)

#include "camel.hpp"
#include "window.hpp"

#include <iostream>

camel::type::color shader(camel::type::uint x, camel::type::uint y) {
	srand(x * x + y * y);
	return rand();
}

int main(void)
{
	camel::window::WindowMode mode(L"TEST", 1024, 576);
	camel::window::Window window(mode);
	window.open();

	for (int y = 0; y < 576; y++) {
		for (int x = 0; x < 1024; x++) {
			window.pixel(x, y, shader(x, y));
		}
	}

	while (true) {
		window.poll_event();

		if (window.get_event().typeof() == camel::event::EventType::Key_Press)
			std::cout << window.get_event().key().unwrap();

		if (window.get_event().typeof() == camel::event::EventType::Close)
			break;
	}
}
