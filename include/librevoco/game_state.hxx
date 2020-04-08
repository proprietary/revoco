#include "cards.hxx"
#include <random>
#include <iostream>

class game_state_t
{
private:
	const cards::deck_t& deck_;
	/// Index of current card in `deck_`
	size_t current_card_idx_ = 0;
	/// Number of correct attempts per card; each index corresponds to that in `deck_.cards`
	std::vector<int> correct_scores_by_card_;
	/// Number of incorrect attempts per card; each index is an index into `deck_.cards`
	std::vector<int> incorrect_scores_by_card_;

public:
	explicit game_state_t(const cards::deck_t& deck) : deck_(deck) {
	}

	[[nodiscard]] const cards::deck_t& deck() const noexcept {
		return deck_;
	}

	void next() noexcept {
		// TODO apply spaced repetition here
		std::random_device rd;
		std::mt19937 mt {rd()};
		std::uniform_int_distribution<size_t> dist {0, deck_.cards.size() - 1};
		current_card_idx_ = dist(mt);
	}

	[[nodiscard]] const cards::card_t& current_card() const noexcept {
		return *deck_.cards[current_card_idx_];
	}

	float score(int card_index) const
	{
		return correct_scores_by_card_[card_index] /
		       std::max(1, total_attempts(card_index));
	}

	int total_attempts(int card_index) const
	{
		return correct_scores_by_card_[card_index] + incorrect_scores_by_card_[card_index];
	}

	/// Call when player gets the card wrong
	void lose();

	/// Call when player gets the card right
	void win();

private:
	void register_loss(size_t card_idx);
	void register_win(size_t card_idx);
};
