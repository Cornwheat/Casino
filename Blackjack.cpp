#include "pch.h"
#include "Blackjack.h"


BlackjackPlayer::BlackjackPlayer()
{

}

BlackjackPlayer::~BlackjackPlayer()
{

}

Blackjack::Blackjack()
{
	seats = 1;
	players.push_back(dealer);
	while (!Round());
	std::cout << "No more players at the table. Thanks for coming..." << std::endl;
	return;
}


Blackjack::~Blackjack()
{

}

int Blackjack::ConvertCardToValue(Card card)
{
	int value = (int)card.value + 1;
	if (value == 1)
	{
		value = 11;
	}
	else if (value > 10)
	{
		value = 10;
	}
	return value;
}

void BlackjackPlayer::DrawCard(Deck& deck)
{
	Card draw = deck.Draw();
	handSize++;
	hand.push_back(draw);
	handValues.push_back(Blackjack::ConvertCardToValue(draw));
	return;
}

int BlackjackPlayer::Wager(int minimumBet, int maximumBet)
{
	if (minimumBet > maximumBet)
	{
		std::cout << "ERROR: Cannot bet higher than minimum bet..." << std::endl;
		return -1;
	}
	int wager = 0;
	do
	{
		wager = 0;
		std::string betPrompt = "Enter how much you'd like to bet or 'cancel': ";
		std::vector<std::string> betStrInput = Input(betPrompt);
		for (unsigned int strIndex = 0; strIndex < betStrInput[0].length(); strIndex++)
		{
			betStrInput[0][strIndex] = tolower(betStrInput[0][strIndex]);
		}
		if (betStrInput[0] == "cancel")
		{
			return -1;
		}
		else
		{
			std::vector<int> betInput = StringTokensToIntTokens(betStrInput);
			if (betInput.size() == 0)
			{
				continue;
			}
			wager = betInput[0];
			if (wager < minimumBet)
			{
				std::cout << "ERROR: Please bet a value higher than the minimum $" << minimumBet << " or 'cancel'..." << std::endl;
			}
			if (wager > maximumBet)
			{
				std::cout << "ERROR: Cannot bet more than $"<< maximumBet << "..." << std::endl;
				wager = -1;
			}
		}
	} while (wager < minimumBet);
	return wager;
}

void BlackjackPlayer::Payout(int winnings)
{
	bust = true;
	stats.money += winnings;
	PlayerBase::PlayerBaseStats[stats.playerID].money += winnings;
	return;
}

void BlackjackPlayer::Options(Deck& deck)
{
	DisplayHand();
	bool invalid = false;
	do
	{
		invalid = false;
		std::string prompt = "Actions:\n0. None (Play standard)\n1. Surrender\n2. Double Down\n3. Split\n\nChoose if you'd like to take any actions on the draw: ";
		std::vector<std::string> moves = Input(prompt);
		for (unsigned int strIndex = 0; strIndex < moves[0].length(); strIndex++)
		{
			moves[0][strIndex] = tolower(moves[0][strIndex]);
		}
		if (moves[0] == "none" || moves[0] == "0")
		{
			std::cout << "Press ENTER to continue...";
			std::string Cont;
			std::getline(std::cin, Cont);
			std::cout << std::string(30, '\n') << std::endl;
			return;
		}
		else if (moves[0] == "surrender" || moves[0] == "1")
		{
			if (Surrender())
			{
				return;
			}
			else
			{
				std::cout << std::endl;
				invalid = true;
			}
		}
		else if (moves[0] == "double" || moves[0] == "2")
		{
			if (DoubleDown(deck))
			{
				return;
			}
			else 
			{
				std::cout << std::endl;
				invalid = true;
			}
		}
		else if (moves[0] == "split" || moves[0] == "3")
		{
			// Split
			std::cout << "SPLIT: Press ENTER to continue...";
			std::string Cont;
			std::getline(std::cin, Cont);
			std::cout << std::string(30, '\n') << std::endl;
			return;
		}
		else
		{
			std::cout << "ERROR: Incorrect input, enter the number or the action you want to take...i.e. 'double' or '2' to double down" << std::endl;
			invalid = true;
		}
	} while (invalid);
	return;
}

void BlackjackPlayer::Insurance()
{
	bool invalid = true;
	std::cout << stats.name << ": Bet $" << bet << std::endl;
	do
	{
		std::string insurancePrompt = "Dealer has revealed an Ace/Face card. Would you like to buy insurance (up to half your bet)?... (yes/no): ";
		bool buyInsurance = YesNoInput(insurancePrompt);
		if (buyInsurance)
		{
			insurance = Wager(0, (bet / 2));
			if (insurance > stats.money)
			{
				std::cout << "ERROR: Cannot bet more money than you currently own... $" << stats.money << std::endl;
				insurance = 0;
			}
			else if (insurance < 0)
			{
				return;
			}
			else
			{
				std::cout << "Insurance: $" << insurance << std::endl;
				stats.money -= insurance;
				PlayerBase::PlayerBaseStats[stats.playerID].money -= insurance;
				invalid = false;
			}
		}
		else
		{
			return;
		}
	} while (invalid);
	return;
}

void BlackjackPlayer::InsurancePayout()
{
	int winnings = insurance * 2;
	std::cout << std::endl << stats.name << " wins " << winnings << " from insurance.\n" << std::endl;
	Payout(winnings);
	return;
}

void BlackjackPlayer::BJ()
{
	int winnings = bet * (2.5);
	std::cout << "\nBlackjack! " << stats.name << " wins $" << winnings << "\n" << std::endl;
	Payout(winnings);
	return;
}

void BlackjackPlayer::Push()
{
	int winnings = bet;
	std::cout << "\nDealer and " << stats.name << " match. Push: $" << winnings << " bet returned\n" << std::endl;
	Payout(winnings);
	return;
}

bool BlackjackPlayer::Surrender()
{
	std::string prompt = "Would you like to surrender to receive half your bet back?... (yes/no): ";
	bool surrender = YesNoInput(prompt);
	if (surrender)
	{
		int winnings = bet/2;
		std::cout << stats.name << " surrenders. $" << winnings << " returned..." << std::endl;
		Payout(winnings);
		std::cout << "SURRENDER: Press ENTER to continue...";
		std::string Cont;
		std::getline(std::cin, Cont);
		std::cout << std::string(30, '\n') << std::endl;
	}
	return surrender;
}

bool BlackjackPlayer::DoubleDown(Deck& deck)
{
	std::string prompt = "Would you like to double down?... (yes/no): ";
	bool doubleDown = YesNoInput(prompt);
	if (doubleDown)
	{
		int addBet = Wager(0, bet);
		if (addBet < 0)
		{
			return false;
		}
		bet += addBet;
		if (!Hit(deck))
		{
			stand = true;
			std::cout << "Stand at " << sumValue << std::endl;
			std::cout << "STAND: Press ENTER to continue...";
			std::string Cont;
			std::getline(std::cin, Cont);
			std::cout << std::string(30, '\n') << std::endl;
		}
		else
		{
			std::cout << "BUST: Press ENTER to continue...";
			std::string Cont;
			std::getline(std::cin, Cont);
			std::cout << std::string(30, '\n') << std::endl;
		}
	}
	return doubleDown;
}

void BlackjackPlayer::Win()
{
	int winnings = (bet * 2);
	std::cout << stats.name << " wins $" << winnings << " with a hand value of " << sumValue << std::endl;
	Payout(winnings);
	return;
}

bool BlackjackPlayer::Turn(Deck& deck)
{
	DisplayHand();
	bool invalid = true;
	if (stand == true || bust == true)
	{
		return true;
	}

	do
	{
		invalid = false;
		std::string prompt = "Actions:\n0. Stand\n1. Hit\n\nWould you like to 'Stand' or 'Hit'?: ";
		std::vector<std::string> moves = Input(prompt);
		for (unsigned int strIndex = 0; strIndex < moves[0].length(); strIndex++)
		{
			moves[0][strIndex] = tolower(moves[0][strIndex]);
		}
		if (moves[0] == "stand" || moves[0] == "0")
		{
			stand = true;
			std::cout << "Stand at " << sumValue << std::endl;
			std::cout << "STAND: Press ENTER to continue...";
			std::string Cont;
			std::getline(std::cin, Cont);
			std::cout << std::string(30, '\n') << std::endl;
			return stand;
		}
		else if (moves[0] == "hit" || moves[0] == "1")
		{
			bust = Hit(deck);
			if (bust)
			{
				std::cout << "BUST: Press ENTER to continue...";
			}
			else
			{
				std::cout << "HIT: Press ENTER to continue...";
			}
			std::string Cont;
			std::getline(std::cin, Cont);
			std::cout << std::string(30, '\n') << std::endl;
			return bust;
		}
		else
		{
			std::cout << "ERROR: Incorrect input, enter the number or the action to stand or hit...i.e. 'Hit' or '1' to hit" << std::endl;
			invalid = true;
		}
	} while (invalid);
	return false;
}

bool BlackjackPlayer::Hit(Deck& deck)
{
	DrawCard(deck);
	std::cout << stats.name << " draws the ";
	DisplayCard(handSize - 1);
	sumValue = SumHand();
	if (sumValue > 21)
	{
		ReduceAce();
	}
	if (sumValue > 21)
	{
		std::cout << stats.name << " busts at " << sumValue << std::endl;
		return true;
	}
	return false;
}

void BlackjackPlayer::ReduceAce()
{
	for (unsigned int cardIndex = 0; cardIndex < handSize; cardIndex++)
	{
		if (handValues[cardIndex] == 11)
		{
			handValues[cardIndex] = 1;
			sumValue = SumHand();
			return;
		}
	}
	return;
}

int BlackjackPlayer::SumHand()
{
	int sum = 0;
	for (unsigned int cardIndex = 0; cardIndex < handSize; cardIndex++)
	{
		sum += handValues[cardIndex];
	}
	return sum;
}

void Blackjack::DisplayDealer()
{
	std::cout << "\nDealer: " << std::endl;
	std::cout << "0. Hidden Card" << std::endl;
	std::cout << "1. ";
	players[0].DisplayCard(1);
	std::cout << std::endl;
	return;
}

void Blackjack::DisplayTable()
{
	DisplayDealer();
	for (unsigned int playerIndex = 1; playerIndex < seats; playerIndex++)
	{
		players[playerIndex].DisplayHand();
	}
	std::cout << "Press ENTER to continue...";
	std::string Cont;
	std::getline(std::cin, Cont);
	std::cout << std::string(30, '\n') << std::endl;
}


bool Blackjack::Round()
{
	Deck deck;
	deck.Shuffle();

	bool addingPlayers = true;
	while (addingPlayers && seats <= maxNumberOfPlayers)
	{
		std::string prompt = "Would a new player like to join the table?... (yes/no): ";
		addingPlayers = Player::YesNoInput(prompt);
		if (addingPlayers)
		{
			players.push_back(BlackjackPlayer());
			std::cout << std::endl;
			seats++;
		}
	}
	if (seats <= 1)
	{
		return true;
	}

	std::cout << std::string(30, '\n') << std::endl;

	Reset();

	Deal(deck);
	for (unsigned int playerIndex = 1; playerIndex < seats; playerIndex++)
	{
		std::cout << std::string(30, '\n') << std::endl;
		do 
		{
			if (players[playerIndex].stand == true || players[playerIndex].bust == true)
			{
				break;
			}
			DisplayDealer();
		}
		while(!players[playerIndex].Turn(deck));
	}

	std::cout << std::string(30, '\n') << std::endl;

	for (unsigned int playerIndex = 1; playerIndex < seats; playerIndex++)
	{
		players[playerIndex].DisplayHand();
	}

	players[0].DisplayHand();
	while (players[0].sumValue < 17)
	{
		std::cout << "Dealer must draw. Press ENTER to draw card...";
		std::string Cont;
		std::getline(std::cin, Cont);
		std::cout << std::endl;

		players[0].bust = players[0].Hit(deck);
		players[0].DisplayHand();
		if (players[0].bust)
		{
			std::cout << "Dealer busts!!!" << std::endl;
			players[0].sumValue = 0;
			break;
		}
	}

	if (!players[0].bust)
	{
		std::cout << "Dealer Hand Value: " << players[0].sumValue << std::endl;
	}

	for (unsigned int playerIndex = 1; playerIndex < seats; playerIndex++)
	{
		if (!players[playerIndex].bust)
		{
			if (players[playerIndex].sumValue > players[0].sumValue)
			{
				players[playerIndex].Win();
			}
			else if (players[playerIndex].sumValue == players[0].sumValue)
			{
				players[playerIndex].Push();
			}
			else
			{
				std::cout << players[playerIndex].stats.name << " lost with a hand value of " << players[playerIndex].sumValue << std::endl;
			}
		}
	}
	std::cout << "Press ENTER to continue...";
	std::string Cont;
	std::getline(std::cin, Cont);
	std::cout << std::string(30, '\n') << std::endl;
	return false;
}

void Blackjack::Deal(Deck& deck)
{
	std::vector<BlackjackPlayer> currentPlayers;
	players[0].DrawCard(deck);
	players[0].DrawCard(deck);
	currentPlayers.push_back(players[0]);
	int currentSeats = 1;

	for (unsigned int playerIndex = 1; playerIndex < seats; playerIndex++)
	{
		std::cout << std::endl << players[playerIndex].stats.name << "'s Turn: $" << players[playerIndex].stats.money << std::endl;
		std::cout << "Table minimum: $" << minimumBet << std::endl;
		bool invalid = true;
		do
		{
			std::string prompt = "Enter if you want to 'bet' or 'leave' the table: ";
			std::vector<std::string> move = players[playerIndex].Input(prompt);
			for (unsigned int strIndex = 0; strIndex < move[0].length(); strIndex++)
			{
				move[0][strIndex] = tolower(move[0][strIndex]);
			}
			if (move[0] == "leave")
			{
				std::cout << "Goodbye " << players[playerIndex].stats.name << "..." << std::endl;
				invalid = false;
			}
			else if (move[0] == "bet")
			{
				players[playerIndex].bet = players[playerIndex].Wager(minimumBet, players[playerIndex].stats.money);
				if (players[playerIndex].bet < 0)
				{
					continue;
				}
				players[playerIndex].stats.money -= players[playerIndex].bet;
				PlayerBase::PlayerBaseStats[players[playerIndex].stats.playerID].money = players[playerIndex].stats.money;
				invalid = false;
				players[playerIndex].DrawCard(deck);
				players[playerIndex].DrawCard(deck);
				currentPlayers.push_back(players[playerIndex]);
				currentSeats++;
			}
			else
			{
				std::cout << "ERROR: Please enter 'bet' or 'leave'..." << std::endl;
			}
		} while (invalid);
	}
	players = currentPlayers;
	seats = currentSeats;
	if (seats <= 1)
	{
		return;
	}

	for (unsigned int playerIndex = 0; playerIndex < seats; playerIndex++)
	{
		players[playerIndex].sumValue = players[playerIndex].SumHand();
		if (players[playerIndex].sumValue > 21)
		{
			players[playerIndex].ReduceAce();
		}
	}

	std::cout << std::string(30, '\n') << std::endl;
	DisplayTable();

	if (players[0].handValues[1] == 11 || players[0].handValues[1] == 10)
	{
		DisplayDealer();
		for (unsigned int playerIndex = 1; playerIndex < seats; playerIndex++)
		{
			players[playerIndex].Insurance();
			std::cout << std::endl;
		}

		if (players[0].sumValue == 21)
		{
			players[0].DisplayHand();
			std::cout << "Dealer Blackjack!...\n" << std::endl;
		}
		else
		{
			std::cout << "Dealer does not have Blackjack...\n" << std::endl;
		}
		std::cout << "Press ENTER to continue...";
		std::string Cont;
		std::getline(std::cin, Cont);
		std::cout << std::string(30, '\n') << std::endl;
	}

	for (unsigned int playerIndex = 1; playerIndex < seats; playerIndex++)
	{
		if (players[playerIndex].sumValue == 21 && players[0].sumValue != 21)
		{
			players[playerIndex].BJ();
			std::cout << "Congratulations! Press ENTER to continue...";
			std::string Cont;
			std::getline(std::cin, Cont);
			std::cout << std::string(30, '\n') << std::endl;
		}
		else if (players[playerIndex].sumValue == 21 && players[0].sumValue == 21)
		{
			players[playerIndex].Push();
			std::cout << "Press ENTER to continue...";
			std::string Cont;
			std::getline(std::cin, Cont);
			std::cout << std::string(30, '\n') << std::endl;
		}
		else if (players[0].sumValue == 21)
		{
			if (players[playerIndex].insurance > 0)
			{
				players[playerIndex].InsurancePayout();
				std::cout << "Press ENTER to continue...";
				std::string Cont;
				std::getline(std::cin, Cont);
				std::cout << std::string(30, '\n') << std::endl;
			}
			players[playerIndex].bust = true;
		}
		else 
		{
			if (players[playerIndex].insurance > 0)
			{
				std::cout << players[playerIndex].stats.name << " lost $" << players[playerIndex].insurance << " insurance...(No dealer blackjack)" << std::endl;
			}

			DisplayDealer();
			players[playerIndex].Options(deck);
		}

	}

}

void Blackjack::Reset()
{
	for (unsigned int playerIndex = 0; playerIndex < seats; playerIndex++)
	{
		players[playerIndex].handSize = 0;
		players[playerIndex].hand.clear();
		players[playerIndex].handValues.clear();
		players[playerIndex].sumValue = 0;
		players[playerIndex].bust = false;
		players[playerIndex].stand = false;
		players[playerIndex].bet = 0;
		players[playerIndex].insurance = 0;

	}
	return;
}