#include "pch.h"
#include "Player.h"

int PlayerBase::PlayerCount;
std::vector<PlayerStats> PlayerBase::PlayerBaseStats;

Player::Player()
{
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
		std::cout << stats.name << " exists as PlayerID: " << stats.playerID << std::endl;
	}
	else
	{
		PlayerStats newStats;
		newStats.name = input;
		newStats.playerID = playerIndex;
		newStats.ThirteenWins = 0;
		PlayerBase::PlayerBaseStats.push_back(newStats);
		stats = PlayerBase::PlayerBaseStats[playerIndex];
		std::cout << stats.name << " added as new player PlayerID: " << stats.playerID << std::endl;
		PlayerBase::PlayerCount++;
	}
}

Player::~Player()
{
	std::cout << "Goodbye " << stats.name << "..." << std::endl;
	PlayerBase::PlayerCount--;
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
}

void Player::DisplayCard(int handIndex)
{
	switch (hand[handIndex].value)
	{
	case Ace: std::cout << "Ace of ";
			break;
	case Two: std::cout << "2 of ";
		break;
	case Three: std::cout << "3 of ";
		break;
	case Four: std::cout << "4 of ";
		break;
	case Five: std::cout << "5 of ";
		break;
	case Six: std::cout << "6 of ";
		break;
	case Seven: std::cout << "7 of ";
		break;
	case Eight: std::cout << "8 of ";
		break;
	case Nine: std::cout << "9 of ";
		break;
	case Ten: std::cout << "10 of ";
		break;
	case Jack: std::cout << "Jack of ";
		break;
	case Queen: std::cout << "Queen of ";
		break;
	case King: std::cout << "King of ";
		break;
	}
	switch (hand[handIndex].suit)
	{
	case Spades: std::cout << "Spades" << std::endl;
		break;
	case Clubs: std::cout << "Clubs" << std::endl;
		break;
	case Diamonds: std::cout << "Diamonds" << std::endl;
		break;
	case Hearts: std::cout << "Hearts" << std::endl;
		break;
	}
}


void Player::SortHand(int low, int high)
{
	if (low < high)
	{
		int partitionIndex = SortPartition(low, high);
		SortHand(low, partitionIndex - 1);
		SortHand(partitionIndex + 1, high);
	}
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

void Player::Play(std::vector<int> cards)
{
	std::cout << stats.name << " played:" << std::endl;
	for (unsigned int cardIndex = 0; cardIndex < cards.size(); cardIndex++)
	{
		DisplayCard(cards[cardIndex]);
		handValues[cards[cardIndex]] = 0;
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
				std::cout << "ERROR: Entered a non-numeric value when selecting cards to play...\n\nPlease input the number index of the card in your hand to play i.e. your first is card is '0'" << std::endl;
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