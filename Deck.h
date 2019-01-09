#pragma once
#include <vector>

enum Suit
{
	Spades,
	Clubs,
	Diamonds,
	Hearts
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
	King
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
	Card deck[52];
	void Display();
	void Shuffle();
};

