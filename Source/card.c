//-----------------------------------------------------------------------------
//
// Name: card.c
// Author: Nicholas Brennan
// Purpose: Handles everything to do with a card.
//
// Date Created: 6/26/23
// Last Updated: 6/28/23
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Include Files:
//-----------------------------------------------------------------------------

#include "stdlib.h"
#include "stdio.h"
#include "time.h"

//-----------------------------------------------------------------------------
// Forward References:
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Constants:
//-----------------------------------------------------------------------------

#define DECK_SIZE 52

typedef enum Suit
{
  HEARTS, DIAMONDS, SPADES, CLUBS
}Suit;

typedef enum Rank
{
  ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING
}Rank;

//-----------------------------------------------------------------------------
// Public Structures:
//-----------------------------------------------------------------------------

typedef struct Card
{
  Suit suit;
  Rank rank;
}Card;

//-----------------------------------------------------------------------------
// Public Variables:
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private Function Declarations:
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Functions:
//-----------------------------------------------------------------------------

Card *DeckCreate()
{
  Card *deck = calloc(52, sizeof(Card));

  for (int i = 0; i < DECK_SIZE; i++)
  {
    deck[i].rank = i % KING + 1;
    deck[i].suit = i / 13 + 1;
  }

  return deck;
}

void DeckFree(Card **deck)
{
  free(*deck);
  *deck = NULL;
}

void DeckShuffle(Card *deck)
{
  srand( time(NULL) );

  for (int i = 0; i < DECK_SIZE; i++)
  {
    int swap_index = rand() % DECK_SIZE;
    Card temp = deck[i];
    deck[i] = deck[swap_index];
    deck[swap_index] = temp;
  }
}

Card *DeckGetCard(Card *deck, int index)
{
  return &deck[index];
}

// Debug function for testing card shuffling
void DeckPrintD(Card *deck)
{
  for (int i = 0; i < DECK_SIZE; i++)
    printf("SUIT: %d, RANK: %d\n", deck[i].suit, deck[i].rank);
}

int CardGetRank(Card *card)
{
  return (int)card->rank;
}

//-----------------------------------------------------------------------------
// Private Functions:
//-----------------------------------------------------------------------------