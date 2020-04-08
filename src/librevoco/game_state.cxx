#include "librevoco/game_state.hxx"

void game_state_t::register_loss(size_t card_idx) {
	// TODO
}

void game_state_t::register_win(size_t card_idx) {
	// TODO
}

void game_state_t::lose() { register_loss(current_card_idx_); }

void game_state_t::win() { register_win(current_card_idx_); }
