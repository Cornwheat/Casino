#pragma once
#include <string>
#include <iostream>
#include "Deck.h"


struct PlayerStats
{
	std::string name;
	int playerID;
	int money;
	int ThirteenWins;
};

class PlayerBase
{
public:
	static int PlayerCount;
	static std::vector<PlayerStats> PlayerBaseStats;
};

class Player
{
public:
	Player();
	Player(bool dealer);
	~Player();


	PlayerStats stats;

	bool dealer;
	int handSize;
	std::vector<int> handValues;
	std::vector<Card> hand;

	void DisplayHand();
	void DisplayCard(int handIndex);
	void SortHand(int low, int high);
	void Play(std::vector<int>cards);
	virtual void DrawCard(Deck& deck);
	virtual int ConvertCardToValue(Card card);

	static std::vector<std::string> Input(std::string prompt);
	static bool YesNoInput(std::string prompt);
	static std::vector<int> StringTokensToIntTokens(std::vector<std::string> input);
	void QuickSort(std::vector<int>& input, int low, int high);

private:
	int SortPartition(int low, int high);
	int SortPartition(std::vector<int>& input, int low, int high);
};

