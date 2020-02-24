#include "librevoco/librevoco.hxx"

#include <iostream>

int main(void) {
	cards::card my_card { 42 };
	std::cout << my_card.card_id << " is my card id\n";
	return 0;
}
