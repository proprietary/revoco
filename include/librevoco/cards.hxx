#pragma once

#include <algorithm>
#include <cstdint>
#include <ostream>
#include <string>
#include <vector>
// #include <concepts>

namespace cards
{
// template <typename CardType, typename AnswerType>
// concept quiz_item = requires(CardType card, AnswerType user_answer) {
// 	{ correct(card, user_answer) } -> std::convertible_to<bool>;
// };

template <typename derived_card_t, typename choice_t> class card
{
public:
	uint64_t card_id;

	bool correct(const choice_t& user_answer) const
	{
		return static_cast<derived_card_t*>(this)->correct(user_answer);
	}
};

template <typename choice_t>
class multiple_choice_card
    : public card<multiple_choice_card<choice_t>, choice_t>
{
public:
	std::string prompt;
	std::vector<choice_t> choices;

	bool correct(const choice_t& user_answer) const
	{
		std::any_of(choices.begin(), choices.end(),
			    [&user_answer](const choice_t& choice) -> bool {
				    return user_answer == choice;
			    });
	}
};

void print_choices(std::ostream, const multiple_choice_card<std::string>&);
} // namespace cards
