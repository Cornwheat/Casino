#pragma once
#include "Deck.h"
#include "Player.h"

enum MoveSet
{
	None,
	Single,
	Double,
	Triple,
	Quadruple,
	Straight
};

class ThirteenPlayer : public Player
{
public:
	bool pass;
	void Turn(int& playersInRound, int& high, MoveSet& set, int& expectedNumberOfCards, bool& start);
	MoveSet SetControl(int& expectedNumberOfCards);
};

class Thirteen
{
public:
	Thirteen();
	~Thirteen();

	ThirteenPlayer players[4];

	int startingPlayerIndex;

	void Deal();
	bool Round(bool& start);

	const int numberOfPlayers = 4;
	const int maxHandSize = 13;

	int ConvertCardToValue(Card card);
	static std::string ConvertValueToCard(int input);
	static std::string CurrentRound(MoveSet set, int high, int expectedNumberOfCards);
};

