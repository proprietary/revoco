#include <yaml-cpp/yaml.h>
#include <gtest/gtest.h>
#include "librevoco/cards.hxx"
#include <fstream>
#include <filesystem>
#include <algorithm>

TEST(YAMLParser, ParseYAMLSmokeTest) {
	YAML::Node node = YAML::Load("[1, 2, 3]");
	ASSERT_TRUE(node.Type() == YAML::NodeType::Sequence);
	ASSERT_TRUE(node.IsSequence());
	YAML::Node flashcards_file = YAML::LoadFile("test/data/flashcards.yaml");
}

TEST(YAMLParser, ParseYAMLDocuments) {
	std::ifstream flashcards_file {"test/data/flashcards.yaml"};
	auto docs = YAML::LoadAll(flashcards_file);
	EXPECT_EQ(docs.size(), 2);
	EXPECT_TRUE(docs[0]["version"].as<int>() == 1);
	EXPECT_TRUE(docs[1]["version"].as<int>() == 1);
	EXPECT_STREQ(docs[0]["card_type"].as<std::string>().c_str(), "flashcards");
}

TEST(YAMLParser, ParseMultipleDecks) {
	std::filesystem::path flashcards_path {"test/data/flashcards.yaml"};
	auto decks = cards::parse_from_yaml(flashcards_path);
	EXPECT_EQ(decks[0].cards.size(), 3);
	EXPECT_STREQ(decks[0].deck_name.c_str(), "Trivia");
	EXPECT_STREQ(decks[0].cards[0]->challenge().c_str(), "What year is it?\n");
	EXPECT_EQ(decks[1].cards.size(), 7);
	std::for_each(decks[1].cards.begin(), decks[1].cards.end(), [](const std::unique_ptr<cards::card_t>& card) -> void {
		EXPECT_TRUE(card->challenge().size() > 0);
	});
}
