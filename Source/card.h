typedef enum Rank Rank;
typedef enum Suit Suit;

typedef struct Card Card;

#define DECK_SIZE 52

Card *DeckCreate();
void DeckShuffle(Card *deck);
void DeckFree(Card **deck);
void DeckPrintD(Card *deck);
Card *DeckGetCard(Card *deck, int index);

int CardGetRank(Card *card);
