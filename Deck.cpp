#include "pch.h"
#include "Deck.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

Deck::Deck()
{
	for (unsigned int deckIndex = 0; deckIndex < DefaultDeckSize; deckIndex++)
	{
		Card newCard;
		newCard.suit = (Suit)(deckIndex % 4);
		newCard.value = (Value)(deckIndex / 4);
		deck.push_back(newCard);
	}
}

Deck::~Deck()
{
}

void Deck::Display()
{
	for (unsigned int deckIndex = 0; deckIndex < deck.size(); deckIndex++)
	{
		DisplayCard(deck[deckIndex]);
	}
}

void Deck::DisplayCard(Card card)
{
	switch (card.value)
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
	default:
		std::cout << "Joker of ";
	}
	switch (card.suit)
	{
	case Spades: std::cout << "Spades" << std::endl;
		break;
	case Clubs: std::cout << "Clubs" << std::endl;
		break;
	case Diamonds: std::cout << "Diamonds" << std::endl;
		break;
	case Hearts: std::cout << "Hearts" << std::endl;
		break;
	default:
		std::cout << "Wild" << std::endl;
	}
	return;
}

void Deck::Shuffle()
{
	std::srand(time(NULL));
	for (unsigned int deckIndex = 0; deckIndex < deck.size(); deckIndex++)
	{
		unsigned int indexOne = rand() % deck.size();
		unsigned int indexTwo = rand() % deck.size();
		Card temp = deck[indexOne];
		deck[indexOne] = deck[indexTwo];
		deck[indexTwo] = temp;
	}
}

Card Deck::Draw()
{
	if (deck.size() <= 0)
	{
		std::cout << "Deck is empty...Reshuffling deck" << std::endl;
		Deck newDeck;
		deck = newDeck.deck;
		Shuffle();
	}
	Card draw = deck.back();
	deck.pop_back();
	return draw;
}