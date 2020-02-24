#include "librevoco/cards.hxx"

namespace cards
{
void print_choices(std::ostream output,
		   const multiple_choice_card<std::string>& card)
{
	std::for_each(card.choices.begin(), card.choices.end(),
		      [&output](const auto& choice) -> void {
			      output << choice << "\n";
		      });
}
} // namespace cards
