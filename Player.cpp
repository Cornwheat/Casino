#include "pch.h"
#include "Player.h"

int PlayerBase::PlayerCount;
std::vector<PlayerStats> PlayerBase::PlayerBaseStats;

Player::Player()
{
	handSize = 0;
	std::cout << "Enter the name for the player: ";
	std::string input;
	std::getline(std::cin,input);
	while (input.length() <= 0)
	{
		std::cout << "ERROR: name length must be greater than 0..." << std::endl;
		std::cout << "Enter the name for the player: ";
		std::getline(std::cin, input);
	}
	bool exists = false;
	int playerIndex = PlayerBase::PlayerCount;
	for (unsigned int playerBaseIndex = 0; playerBaseIndex < PlayerBase::PlayerCount; playerBaseIndex++)
	{
		if (input == PlayerBase::PlayerBaseStats[playerBaseIndex].name)
		{
			exists = true;
			playerIndex = playerBaseIndex;
			break;
		}
	}
	if (exists)
	{
		stats = PlayerBase::PlayerBaseStats[playerIndex];
		std::cout << "Welcome back " << stats.name << ". This is your PlayerID: " << stats.playerID << std::endl;
	}
	else
	{
		PlayerStats newStats;
		newStats.name = input;
		newStats.playerID = playerIndex;
		newStats.money = 1000;
		newStats.ThirteenWins = 0;
		PlayerBase::PlayerBaseStats.push_back(newStats);
		stats = PlayerBase::PlayerBaseStats[playerIndex];
		std::cout << "Hello "<< stats.name << ". You've been added as a new player with PlayerID: " << stats.playerID << std::endl;
		PlayerBase::PlayerCount++;
	}
	return;
}

Player::Player(bool dealer)
{
	stats.name = "Dealer";
	stats.money = 1000000000;
	dealer = true;
	return;
}

Player::~Player()
{
	if (!dealer)
	{
		std::cout << "Goodbye " << stats.name << "..." << std::endl;
		PlayerBase::PlayerCount--;
		return;
	}
}

void Player::DisplayHand()
{
	std::cout << std::endl << "Player: " << stats.name << std::endl;
	for (unsigned int handIndex = 0; handIndex < handSize; handIndex++)
	{
		std::cout << handIndex << ": ";
		DisplayCard(handIndex);
	}
	std::cout << std::endl;
	return;
}

void Player::DisplayCard(int handIndex)
{
	Deck::DisplayCard(hand[handIndex]);
	return;
}

void Player::SortHand(int low, int high)
{
	if (low < high)
	{
		int partitionIndex = SortPartition(low, high);
		SortHand(low, partitionIndex - 1);
		SortHand(partitionIndex + 1, high);
	}
	return;
}

int Player::SortPartition(int low, int high)
{
	int pivot = handValues[high];
	int smallIndex = (low - 1);
	for (int index = low; index <= (high - 1); index++)
	{
		if (handValues[index] <= pivot)
		{
			smallIndex++;
			int temp = handValues[smallIndex];
			Card tempCard = hand[smallIndex];
			handValues[smallIndex] = handValues[index];
			hand[smallIndex] = hand[index];
			handValues[index] = temp;
			hand[index] = tempCard;
		}
	}
	int temp = handValues[smallIndex + 1];
	Card tempCard = hand[smallIndex + 1];
	handValues[smallIndex + 1] = handValues[high];
	hand[smallIndex + 1] = hand[high];
	handValues[high] = temp;
	hand[high] = tempCard;
	return (smallIndex + 1);
}

std::vector<std::string> Player::Input(std::string prompt)
{
	std::string input;
	std::cout << prompt;
	std::getline(std::cin, input);
	while (input.length() <= 0)
	{
		std::cout << "ERROR: Please enter an input and then press ENTER..." << std::endl;
		std::cout << prompt;
		std::getline(std::cin, input);
	}
	std::vector<std::string> moves;
	std::string move;
	const std::string delimiter = " ";
	for (unsigned inputIndex = 0; inputIndex < input.length(); inputIndex++)
	{
		if (input[inputIndex] != ' ')
		{
			move += input[inputIndex];
		}
		else
		{
			if (!move.empty())
			{
				moves.push_back(move);
				move = "";
			}
		}
	}
	if (!move.empty())
	{
		moves.push_back(move);
	}
	return moves;
}

bool Player::YesNoInput(std::string prompt)
{
	while (1)
	{
		std::vector<std::string> reply = Player::Input(prompt);

		for (unsigned int strIndex = 0; strIndex < reply[0].length(); strIndex++)
		{
			reply[0][strIndex] = tolower(reply[0][strIndex]);
		}
		if (reply[0] == "yes" || reply[0] == "y")
		{
			return true;
		}
		else if (reply[0] == "no" || reply[0] == "n")
		{
			return false;
		}
		else
		{
			std::cout << "ERROR: Please enter 'yes' or 'no'...\n" << std::endl;
		}
	}
}

void Player::Play(std::vector<int> cards)
{
	std::cout << stats.name << " played:" << std::endl;
	for (unsigned int cardIndex = 0; cardIndex < cards.size(); cardIndex++)
	{
		DisplayCard(cards[cardIndex]);
		handValues[cards[cardIndex]] = -1;
	}
	std::vector<int> newHandValues;
	std::vector<Card> newHand;
	for (unsigned int handIndex = 0; handIndex < handSize; handIndex++)
	{
		if (handValues[handIndex] > 0)
		{
			newHandValues.push_back(handValues[handIndex]);
			newHand.push_back(hand[handIndex]);
		}
	}
	handValues = newHandValues;
	hand = newHand;
	handSize -= cards.size();
	return;
}

void Player::DrawCard(Deck& deck)
{
	Card draw = deck.Draw();
	handSize++;
	hand.push_back(draw);
	handValues.push_back(ConvertCardToValue(draw));
	return;
}

int Player::ConvertCardToValue(Card card)
{
	return (int)card.value;
}

std::vector<int> Player::StringTokensToIntTokens(std::vector<std::string> input)
{
	std::vector<int> output;
	for (unsigned int inputIndex = 0; inputIndex < input.size(); inputIndex++)
	{
		for (unsigned int strIndex = 0; strIndex < input[inputIndex].length(); strIndex++)
		{
			if (input[inputIndex][strIndex] < '0' || input[inputIndex][strIndex] > '9')
			{
				std::cout << "ERROR: Entered a non-numeric value..." << std::endl;
				return {};
			}
		}
		int token = std::stoi(input[inputIndex]);
		output.push_back(token);
	}
	return output;
}

void Player::QuickSort(std::vector<int>& input, int low, int high)
{
	{
		if (low < high)
		{
			int partitionIndex = SortPartition(input, low, high);
			QuickSort(input, low, partitionIndex - 1);
			QuickSort(input, partitionIndex + 1, high);
		}
	}
	return;
}

int Player::SortPartition(std::vector<int>& input, int low, int high)
{
	int pivot = input[high];
	int smallIndex = (low - 1);
	for (int index = low; index <= (high - 1); index++)
	{
		if (input[index] <= pivot)
		{
			smallIndex++;
			int temp = input[smallIndex];
			input[smallIndex] = input[index];
			input[index] = temp;
		}
	}
	int temp = input[smallIndex + 1];
	input[smallIndex + 1] = input[high];
	input[high] = temp;
	return (smallIndex + 1);
}