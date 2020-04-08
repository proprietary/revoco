#include "librevoco/game_state.hxx"

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

static int MAX_X = 0, MAX_Y = 0;

void run(game_state_t&);

int main(int argc, char* argv[], char* envp[])
{
	gflags::ParseCommandLineFlags(&argc, &argv, true);
	std::filesystem::path input_path { FLAGS_input };
	auto decks = cards::parse_from_yaml(input_path);
	game_state_t game { decks[1] };
	run(game);
	return 0;
}

static void draw_prompt(WINDOW* window, std::string prompt_text);
static void draw_answer(WINDOW* window, std::string answer_text);
static void draw_new_card(WINDOW* window, const cards::card_t& card);

void run(game_state_t& game) {
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


	// Draw static parts

	// draw border around window
	::box(win.get(), 0, 0);
	// exit sign
	::attron(A_REVERSE | A_BOLD);
	::mvprintw(MAX_Y - (int)(0.1 * (float)MAX_Y), 0, "Press ESC to exit");
	::attroff(A_REVERSE | A_BOLD);
	// usage
	::attron(A_BOLD);
	mvaddwstr(5, 10, L"\t↑ Correct\t↓ Incorrect\t→ Show answer\t");
	::attroff(A_BOLD);


	// Main game loop

	static bool exit_flag = false;

	int ch = 0;
	do {
		getmaxyx(::stdscr, MAX_Y, MAX_X);

		switch (ch) {
		case 27 /* ESC */:
			exit_flag = true;
			break;
		case 258 /* Arrow Down */:
		case 330 /* del */:
		case 263 /* backspace */: {
			// TODO mark card as incorrect
			// TODO move to next card
			game.next();
			const cards::card_t& card = game.current_card();
			draw_prompt(win.get(), card.challenge());
			draw_answer(win.get(), "");
			break;
		}
		case 259 /* Arrow Up */:
		case KEY_ENTER /* keypad Enter */:
		case 10 /* Enter */: {
			// Enter key means player got this card correct
			// TODO mark this card as correct
			// TODO move to next card
			game.next();
			const cards::card_t& card = game.current_card();
			draw_prompt(win.get(), card.challenge());
			draw_answer(win.get(), "");
			break;
		}
		case 261 /* Arrow Right */: {
			// Reveal answer
			game.next();
			const cards::card_t& card = game.current_card();
			draw_answer(win.get(), card.answer());
		}
		default:
			// ::wprintw(win.get(), "%d", ch);
			break;
		}

		// redraw
		::wrefresh(win.get());
		::refresh();
		ch = ::getch();
	} while (!exit_flag);

	::endwin(); // end curses
}

static void draw_prompt(WINDOW* window, std::string prompt_text) {
	int rows, cols;
	getmaxyx(window, rows, cols);
	mvwaddnstr(window, rows * 1/10, 0, prompt_text.c_str(), prompt_text.size());
}

static void draw_answer(WINDOW* window, std::string answer_text) {
	int rows, cols;
	getmaxyx(window, rows, cols);
	mvwaddnstr(window, rows * 6/10, 0, answer_text.c_str(), answer_text.size());
}

static void draw_new_card(WINDOW* window, const cards::card_t& card) {
	draw_prompt(window, card.challenge());
	draw_answer(window, card.answer());
}
