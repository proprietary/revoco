#include "librevoco/librevoco.hxx"

#include <csignal>
#include <iostream>
#include <locale>
#include <memory>
#include <ncursesw/ncurses.h>
#include <ncursesw/panel.h>
#include <gflags/gflags.h>


// Command Line Flags

static bool validate_string_not_empty(const char* flag_name, const std::string& flag_value) {
	return !flag_value.empty();
}

DEFINE_string(input, "", "path to YAML file specifying flashcard decks to load");
DEFINE_validator(input, &validate_string_not_empty);

namespace
{
static int MAX_X = 0, MAX_Y = 0;
} // namespace

void run();

int main(int argc, char* argv[], char* envp[])
{
	gflags::ParseCommandLineFlags(&argc, &argv, true);
	run();
	return 0;
}

void run() {
	std::locale::global(std::locale(""));
	::initscr();
	::raw();
	::cbreak();
	::noecho();
	::keypad(::stdscr, TRUE); // enable use of F1..12 and arrow keys
	if (!::has_colors()) {
		std::cerr << "Terminal does not support colors" << std::endl;
	}
	getmaxyx(stdscr, MAX_X, MAX_Y);
	auto delwin_fn = [](WINDOW* x) { ::delwin(x); };
	int win_x = (int)(0.05 * (float)MAX_Y);
	int win_y = (int)(0.1 * (float)MAX_X);
	std::unique_ptr<WINDOW, decltype(delwin_fn)> win{
	    ::newwin(40, 80, (int)(0.05 * (float)MAX_Y), (int)(0.1 * (float)MAX_X))};

	// handle dynamically resizing windows--terminal emulators
	std::signal(SIGWINCH, [](int sig) -> void {
		getmaxyx(stdscr, MAX_X, MAX_Y);
		resize_term(MAX_Y, MAX_X);
	});

	// draw border around window
	::box(win.get(), 0, 0);

	static bool exit_flag = false;

	int ch = 0;
	while (!exit_flag) {
		ch = ::getch();
		switch (ch) {
		case 27 /* ESC */:
		case KEY_F(1) /* F1 */:
			exit_flag = true;
			break;
		case 'n': {
			// draw new card
			int x = getmaxx(win.get());
			int y = getmaxy(win.get());
			mvprintw(y - 2, 0, "hlelaslk");
			break;
		}
		case 330 /* del */:
		case 263 /* backspace */: {
			// TODO mark card as incorrect
			// TODO move to next card
			// mvwprintw(win.get(), MAX_X
			break;
		}
		default:
			::wprintw(win.get(), "%d", ch);
		}
		// exit sign
		::attron(A_REVERSE | A_BOLD);
		::mvprintw((int)(0.5 * (float)MAX_Y), 0, "Press ESC to exit");
		::attroff(A_REVERSE | A_BOLD);
		// redraw
		::wrefresh(win.get());
		::refresh();
	}

	::endwin(); // end curses
}
