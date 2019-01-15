#include "pch.h"
#include "Thirteen.h"

void ThirteenPlayer::Turn(int& playersInRound, int& high, MoveSet& set, int& expectedNumberOfCards, bool& start)
{
	if (pass)
	{
		return;
	}

	bool controller = false;
	bool invalid = false;
	do
	{
		if (invalid && controller)
		{
			set = None;
		}
		invalid = false;
		DisplayHand();
		if (set == None)
		{
			controller = true;
			set = SetControl(expectedNumberOfCards);
		}
		if (!controller)
		{
			std::cout << Thirteen::CurrentRound(set,high,expectedNumberOfCards) << std::endl;
		}
		std::string prompt = "Enter the cards you want to play (by index in hand) or 'pass': ";
		std::vector<std::string> moves = Input(prompt);
		std::vector<int> cards;

		for (unsigned int strIndex = 0; strIndex < moves[0].length(); strIndex++)
		{
			moves[0][strIndex] = tolower(moves[0][strIndex]);
		}

		if (moves[0] == "pass")
		{
			if (controller)
			{
				std::cout << "ERROR: Can not pass if setting the round..." << std::endl;
				set = None;
				invalid = true;
				continue;
			}
			playersInRound -= 1;
			pass = true;
			std::cout << std::string(30, '\n') << std::endl;
			std::cout << stats.name << " passes" << std::endl;
			return;
		}
		else if (moves[0] == "cancel" && controller)
		{
			invalid = true;
			continue;
		}
		else
		{
			cards = StringTokensToIntTokens(moves);
			if (cards.size() == 0)
			{
				invalid = true;
				continue;
			}
		}
		if (cards.size() != expectedNumberOfCards)
		{
			if (!controller || set != Straight)
			{
				std::cout << "ERROR: Must play number of cards equal to round set i.e. 2 cards for doubles or 3 cards for 3-card straight...\n" << std::endl;
				invalid = true;
				continue;
			}
		}
		QuickSort(cards, 0, cards.size() - 1);
		for (unsigned int cardIndex = 0; cardIndex < (cards.size()-1); cardIndex++)
		{
			if (cards[cardIndex] == cards[cardIndex + 1])
			{
				std::cout << "ERROR: Duplicate input value...\n" << std::endl;
				invalid = true;
				break;
			}
		}
		if (invalid)
		{
			continue;
		}
		if (cards[0] < 0 || cards[cards.size() - 1] >= handSize)
		{
			std::cout << "ERROR: Input indices not in hand...\n" << std::endl;
			invalid = true;
			continue;
		}
		if (start)
		{
			if (handValues[cards[0]] != 1)
			{
				std::cout << "ERROR: The starting play must contain the 3 of spades...\n" << std::endl;
				invalid = true;
				continue;
			}
		}
		int maxValue = handValues[cards[cards.size() - 1]];
		if (maxValue <= high)
		{
			std::cout << "ERROR: Must play card of higher value than current round high card...\n" << std::endl;
			invalid = true;
			continue;
		}
		if (set == Double || set == Triple || set == Quadruple)
		{
			int face = (handValues[cards[0]]-1) / 4;
			for (unsigned int cardIndex = 1; cardIndex < cards.size(); cardIndex++)
			{
				if ((handValues[cards[cardIndex]]-1) / 4 != face)
				{
					std::cout << "ERROR: Must play cards with the same face values i.e. two 6 for doubles or three 7 for triple...\n" << std::endl;
					invalid = true;
					break;
				}
			}
			if (invalid)
			{
				continue;
			}
		}
		if (set == Straight)
		{
			if (cards.size() < 3)
			{
				std::cout << "ERROR: Straight must be 3 or more cards...\n" << std::endl;
				invalid = true;
				continue;
			}
			int face = (handValues[cards[0]] - 1) / 4;
			for (unsigned int cardIndex = 1; cardIndex < cards.size(); cardIndex++)
			{
				if ((handValues[cards[cardIndex]] - 1) / 4 != face + 1)
				{
					std::cout << "ERROR: Straight must be a sequence of cards i.e. 3,4,5 for 3-card straight...\n" << std::endl;
					invalid = true;
					break;
				}
				if (((handValues[cards[cardIndex]] - 1) / 4) >= 12)
				{
					std::cout << "ERROR: Straights cannot contain a 2...\n" << std::endl;
					invalid = true;
					break;
				}
				face++;
			}
			if (invalid)
			{
				continue;
			}
			if (controller)
			{
				expectedNumberOfCards = cards.size();
			}
		}
		if (!invalid)
		{
			if (controller && set == Straight)
			{
				expectedNumberOfCards = cards.size();
			}
			if (start)
			{
				start = false;
			}
			high = maxValue;
			std::cout << std::string(30,'\n') << std::endl;
			Play(cards);
		}
	} while (invalid);
}

MoveSet ThirteenPlayer::SetControl(int& expectedNumberOfCards)
{
	bool invalid = false;
	do
	{
		invalid = false;
		std::string prompt = "Set control for the round\n1. Single\n2. Double\n3. Triple\n4. Quadruple\n5. Straight\n\nPlease enter the name or number of the pattern to set for the round: ";
		std::vector<std::string> moves = Input(prompt);
		for (unsigned int strIndex = 0; strIndex < moves[0].length(); strIndex++)
		{
			moves[0][strIndex] = tolower(moves[0][strIndex]);
		}
		if (moves[0] == "single" || moves[0] == "1")
		{
			expectedNumberOfCards = 1;
			return Single;
		}
		else if (moves[0] == "double" || moves[0] == "2")
		{
			expectedNumberOfCards = 2;
			return Double;
		}
		else if (moves[0] == "triple" || moves[0] == "3")
		{
			expectedNumberOfCards = 3;
			return Triple;
		}
		else if (moves[0] == "quadruple" || moves[0] == "4")
		{
			expectedNumberOfCards = 4;
			return Quadruple;
		}
		else if (moves[0] == "straight" || moves[0] == "5")
		{
			return Straight;
		}
		else
		{
			std::cout << "ERROR: Incorrect input, enter the number or the pattern you want to set the round as...i.e. 'Single' or '1'" << std::endl;
			invalid = true;
		}
	} while (invalid);


	return None;
}

Thirteen::Thirteen()
{
	bool play = true;
	while (play)
	{
		bool start = true;
		std::cout << std::endl;
		for (unsigned int playerIndex = 0; playerIndex < numberOfPlayers; playerIndex++)
		{
			std::cout << players[playerIndex].stats.name << ": " << players[playerIndex].stats.ThirteenWins << " wins" << std::endl;
		}
		std::cout << std::endl;
		Deal();
		while (!Round(start));
		std::string prompt = "Would you like to play again?... (yes/no): ";
		play = Player::YesNoInput(prompt);
		std::cout << std::string(30, '\n') << std::endl;
	}
	return;
}

Thirteen::~Thirteen()
{
	std::cout << "Thanks for playing..." << std::endl;
}

int Thirteen::ConvertCardToValue(Card card)
{
	int value = (int)card.value * 4;
	value += (int)card.suit;
	if (value < 8)
	{
		value += 52;
	}
	value -= 7;
	return value;
}

std::string Thirteen::ConvertValueToCard(int input)
{
	std::string card = "";
	int value = input + 7;
	if (value >= 52)
	{
		value -= 52;
	}
	Value face = (Value)(value / 4);
	Suit suit = (Suit)(value % 4);

	switch (face)
	{
	case Ace: card += "Ace of ";
		break;
	case Two: card += "2 of ";
		break;
	case Three: card += "3 of ";
		break;
	case Four: card += "4 of ";
		break;
	case Five: card += "5 of ";
		break;
	case Six: card += "6 of ";
		break;
	case Seven: card += "7 of ";
		break;
	case Eight: card += "8 of ";
		break;
	case Nine: card += "9 of ";
		break;
	case Ten: card += "10 of ";
		break;
	case Jack: card += "Jack of ";
		break;
	case Queen: card += "Queen of ";
		break;
	case King: card += "King of ";
		break;
	}
	switch (suit)
	{
	case Spades: card += "Spades";
		break;
	case Clubs: card += "Clubs";
		break;
	case Diamonds: card += "Diamonds";
		break;
	case Hearts: card += "Hearts";
		break;
	}
	return card;
}

std::string Thirteen::CurrentRound(MoveSet set, int high, int expectedNumberOfCards)
{
	std::string state = "";
	switch (set)
	{
	case Single: state = "Single: ";
		break;
	case Double: state = "Double: ";
		break;
	case Triple: state = "Triple: ";
		break;
	case Quadruple: state = "Quadruple: ";
		break;
	case Straight: state = (std::to_string(expectedNumberOfCards) + "-Card Straight: ");
		break;
	}
	state += ConvertValueToCard(high);
	state += " high";
	return state;
}

void Thirteen::Deal()
{
	for (unsigned int playerIndex = 0; playerIndex < numberOfPlayers; playerIndex++)
	{
		players[playerIndex].handSize = maxHandSize;
		players[playerIndex].hand.resize(maxHandSize);
		players[playerIndex].handValues.resize(maxHandSize);
	}
	Deck deck;
	deck.Shuffle();
	for (unsigned int deckIndex = 0; deckIndex < deck.deck.size(); deckIndex++)
	{
		players[deckIndex % numberOfPlayers].hand[deckIndex / numberOfPlayers] = deck.deck[deckIndex];
		players[deckIndex % numberOfPlayers].handValues[deckIndex / numberOfPlayers] = ConvertCardToValue(deck.deck[deckIndex]);
		if (players[deckIndex % numberOfPlayers].handValues[deckIndex / numberOfPlayers] == 1)
		{
			startingPlayerIndex = deckIndex % numberOfPlayers;
		}
	}
	for (unsigned int playerIndex = 0; playerIndex < numberOfPlayers; playerIndex++)
	{
		players[playerIndex].SortHand(0, maxHandSize - 1);
	}
}

bool Thirteen::Round(bool& start)
{
	int high = -1;
	int playersInRound = 4;

	MoveSet set = None;
	int expectedNumberOfCards = 0;
	for (unsigned int playerIndex = 0; playerIndex < numberOfPlayers; playerIndex++)
	{
		players[playerIndex].pass = false;
	}

	for (unsigned int playerIndex = startingPlayerIndex; playersInRound > 1; playerIndex++)
	{
		if (playerIndex > 3)
		{
			playerIndex -= 4;
		}
		if (!players[playerIndex].pass)
		{
			for (unsigned int checkIndex = 0; checkIndex < numberOfPlayers; checkIndex++)
			{
				std::cout << players[checkIndex].stats.name << ": " << std::to_string(players[checkIndex].handSize) << " cards. ";
			}
			std::cout << "\n" << std::endl;
			std::cout << "Press ENTER for " << players[playerIndex].stats.name << "'s turn...";
			std::string ready;
			std::getline(std::cin, ready);
			std::cout << std::string(30, '\n') << std::endl;
		}
		players[playerIndex].Turn(playersInRound,high,set,expectedNumberOfCards,start);
		if (players[playerIndex].handSize <= 0)
		{
			PlayerBase::PlayerBaseStats[players[playerIndex].stats.playerID].ThirteenWins++;
			players[playerIndex].stats.ThirteenWins++;
			std::cout << std::endl << players[playerIndex].stats.name << " wins!!!\n" << std::endl;
			return true;
		}
	}

	for (unsigned int playerIndex = 0; playerIndex < numberOfPlayers; playerIndex++)
	{
		if (players[playerIndex].pass == false)
		{
			std::cout << std::endl << players[playerIndex].stats.name << " wins the round and takes control...\n" << std::endl;
			startingPlayerIndex = playerIndex;
			break;
		}
	}

	return false;
}