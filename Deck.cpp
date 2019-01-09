#include "pch.h"
#include "Deck.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

Deck::Deck()
{
	for (unsigned int deckIndex = 0; deckIndex < 52; deckIndex++)
	{
		deck[deckIndex].suit = (Suit)(deckIndex % 4);
		deck[deckIndex].value = (Value)(deckIndex / 4);
	}
}

Deck::~Deck()
{
}

void Deck::Display()
{
	for (unsigned int deckIndex = 0; deckIndex < 52; deckIndex++)
	{
		switch (deck[deckIndex].value)
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
		switch (deck[deckIndex].suit)
		{
		case Spades: std::cout << "Spades\n";
			break;
		case Clubs: std::cout << "Clubs\n";
			break;
		case Diamonds: std::cout << "Diamonds\n";
			break;
		case Hearts: std::cout << "Hearts\n";
			break;
		}
	}
}

void Deck::Shuffle()
{
	std::srand(time(NULL));
	for (unsigned int deckIndex = 0; deckIndex < 52; deckIndex++)
	{
		unsigned int indexOne = rand() % 52;
		unsigned int indexTwo = rand() % 52;
		Card temp = deck[indexOne];
		deck[indexOne] = deck[indexTwo];
		deck[indexTwo] = temp;
	}
}