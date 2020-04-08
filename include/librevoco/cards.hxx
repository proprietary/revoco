#pragma once

#include <algorithm>
#include <any>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <ostream>
#include <string>
#include <tuple>
#include <vector>
#include <utility>
// #include <concepts>

namespace cards
{
// template <typename CardType, typename AnswerType>
// concept quiz_item = requires(CardType card, AnswerType user_answer) {
// 	{ correct(card, user_answer) } -> std::convertible_to<bool>;
// };

class card_t
{
protected:
	uint64_t card_id_;
	std::string prompt_;

public:
	explicit card_t(std::string prompt) : prompt_(std::move(prompt)) {}
	virtual ~card_t() noexcept = default;
	virtual bool correct(const std::any choice) const = 0;
	virtual std::string answer() const = 0;
	virtual std::string challenge() const { return prompt_; }
	uint64_t id() const { return card_id_; }
};

template <typename choice_t = std::string> class multiple_choice_card : public card_t
{
public:
	std::vector<choice_t> choices;
	typename decltype(choices)::size_type correct_choice_idx;

	bool correct(const std::any user_answer) const override
	{
		const choice_t& choice = std::any_cast<choice_t>(user_answer);
		std::any_of(choices.begin(), choices.end(),
			    [&user_answer](const choice_t& choice) -> bool {
				    return user_answer == choice;
			    });
	}

	std::string answer() const override { return choices.get(correct_choice_idx); }
};

void print_choices(std::ostream, const multiple_choice_card<std::string>&);

class flashcard_t : public card_t
{
private:
	std::string other_side_;

public:
	explicit flashcard_t(std::string prompt, std::string answer)
	    : other_side_(std::move(answer)), card_t(std::move(prompt))
	{
	}

	bool correct(const std::any _) const override
	{
		// this should never be called, really
		return true;
	}
	std::string answer() const override { return other_side_; }
};

class deck_t
{
public:
	std::string deck_name;
	std::vector<std::unique_ptr<card_t>> cards;
};

std::vector<deck_t> parse_from_yaml(const std::filesystem::path& cards_source);
std::vector<deck_t> parse_from_yaml(const char* cards_source);
} // namespace cards
