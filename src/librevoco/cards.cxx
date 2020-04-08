#include "librevoco/cards.hxx"
#include "flashcard_generated.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <cassert>
#include <stdexcept>

namespace cards
{
void print_choices(std::ostream output, const multiple_choice_card<std::string>& card)
{
	std::for_each(card.choices.begin(), card.choices.end(),
		      [&output](const auto& choice) -> void { output << choice << "\n"; });
}

std::vector<deck_t> parse_from_yaml(const std::filesystem::path& cards_source)
{
	if (!std::filesystem::exists(cards_source))
		throw std::runtime_error("No such file");
	std::vector<deck_t> out {};
	std::ifstream cards_file { cards_source };
	auto docs = YAML::LoadAll(cards_file);
	out.reserve(docs.size());
	assert(docs.size() == 2);
	for (auto it = docs.begin(); it != docs.end(); ++it) {
		const auto doc = *it;
		deck_t this_deck {};
		auto version = doc["version"];
		auto type = version.Type();
		if (version.as<int>() == 1) {
			if (doc["card_type"].as<std::string>() == "flashcards") {
				this_deck.deck_name = doc["deck_name"].as<std::string>();
				for (const auto& card_data : doc["cards"]) {
					std::unique_ptr<card_t> card { new flashcard_t { card_data["prompt"].as<std::string>(), card_data["answer"].as<std::string>() } };
					this_deck.cards.emplace_back(std::move(card));
				}
			}
		}
		out.emplace_back(std::move(this_deck));
	}
	return out;
}

bool consume_flashcard(const revoco::Flashcard::Flashcard& flashcard_) {
	// TODO
	auto x = flashcard_.answer();
	return false;
}


} // namespace cards
