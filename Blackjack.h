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

	void DrawCard(Deck& deck);
	int Wager(int minumumBet, int maximumBet);
	void Payout(int winnings);
	void Options(Deck& deck);
	void Insurance();
	void InsurancePayout();
	void BJ();
	void Push();
	bool Surrender();
	bool DoubleDown(Deck& deck);
	void Win();
	bool Turn(Deck& deck);
	bool Hit(Deck& deck);
	void ReduceAce();
	
	int SumHand();
};

class Blackjack
{
public:
	Blackjack();
	~Blackjack();

	const int maxNumberOfPlayers = 8;
	const int minimumBet = 10;

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

