#pragma once
#include "Deck.h"
#include "Player.h"

class BlackjackPlayer : public Player
{
public:
	BlackjackPlayer();
	~BlackjackPlayer();
	BlackjackPlayer(bool Dealer) : Player(Dealer)
	{
		dealer = Dealer;
	};

	int sumValue;
	int bet;
	int insurance;
	bool bust;
	bool stand;
	int splitValue;
	int splitBet;

	void DrawCard(Deck& deck);
	bool Turn(Deck& deck);
	void Options(Deck& deck);

	void BJ();
	void Insurance();
	void InsurancePayout();
	void Push();
	void Push(int split);
	bool Surrender();
	bool DoubleDown(Deck& deck);
	bool Split(Deck& deck);

	void Win();
	void Win(int split);

	bool Hit(Deck& deck);
	void ReduceAce();
	
	int SumHand();
};

class Blackjack
{
public:
	Blackjack();
	~Blackjack();

	static const int maxNumberOfPlayers = 8;
	static const int minimumBet = 10;

	BlackjackPlayer dealer = BlackjackPlayer(true);
	unsigned int seats;
	std::vector<BlackjackPlayer> players;

	static int ConvertCardToValue(Card card);
	void DisplayDealer();
	void DisplayTable();

	bool Round();
	void Deal(Deck& deck);
	void Reset();
};

