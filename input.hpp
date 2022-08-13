#ifndef INPUT_HPP_INCLUDED
#define INPUT_HPP_INCLUDED

#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <type_traits>

// Keyboard keys. This is currently non-comprehensive.
enum struct KeyboardKey : unsigned
{
	left_arrow = XK_Left,
	right_arrow = XK_Right,
	escape = XK_Escape,
	q = XK_q,
};

// Provides window system context for reading keyboard state.
class KeyboardContext
{
private:
	Display* display_;

public:
	// Open the display from $DISPLAY.
	KeyboardContext() : display_(XOpenDisplay(nullptr)) {}
	~KeyboardContext() { XCloseDisplay(display_); }

	auto is_key_pressed(KeyboardKey key) -> bool
	{
		char key_states[32];
		XQueryKeymap(display_, key_states);

		// i is an index to a bit rather than a byte.
		auto const i = XKeysymToKeycode(display_, static_cast<unsigned>(key));
		return (key_states[i / 8] & (1 << (i % 8))) != 0;
	}
};

#endif
