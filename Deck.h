#pragma once
#include <vector>

enum Suit
{
	Spades,
	Clubs,
	Diamonds,
	Hearts,
	Wild
};

enum Value
{
	Ace,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Jack,
	Queen,
	King,
	Joker
};

struct Card
{
	Suit suit;
	Value value;
};

class Deck
{
public:
	Deck();
	~Deck();
	std::vector<Card> deck;
	const int DefaultDeckSize = 52;

	void Display();
	static void DisplayCard(Card card);

	void Shuffle();
	Card Draw();
};

