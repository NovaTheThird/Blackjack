//-----------------------------------------------------------------------------
//
// Name: blackjack.c
// Author: Nicholas Brennan
// Purpose: Blackjack logic.
//
// Date Created: 6/26/23
// Last Updated: 6/28/23
//
// Hi. I don't entirely like what lies below. It could look nicer and be
// formatted much more intuitavely, but considering I have less than a year
// of expierence with coding, I think a FSM (I hope that's the right term)
// is more than good enough.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Include Files:
//-----------------------------------------------------------------------------

#include "card.h"
#include "conio.h" // _getch();
#include "player.h"
#include "stdio.h"
#include "string.h" 

//-----------------------------------------------------------------------------
// Forward References:
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Constants:
//-----------------------------------------------------------------------------

#define FALSE 0
#define TRUE  1

typedef enum States
{
  ERROR = -1, DEFAULT, HELP, EXIT, PLAY, RULES, UPDATE
}States;

typedef enum GameStates
{
  ERRORGAME = -1, BEGIN, DECIDE, HIT, STAND, DEALER, WIN, LOSE, DRAW, PLAYAGAIN
}GameStates;

//-----------------------------------------------------------------------------
// Public Structures:
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Variables:
//-----------------------------------------------------------------------------

int isRunning = FALSE;
int inGame = FALSE;

States menuState = ERROR;
GameStates gameState = ERROR;

//-----------------------------------------------------------------------------
// Private Function Declarations:
//-----------------------------------------------------------------------------

static void MenuInit(void);
static void MenuUpdate(void);
static void Play(void);

//-----------------------------------------------------------------------------
// Public Functions:
//-----------------------------------------------------------------------------

// 
// \param void
// \return N/A
// \brief The main game logic for blackjack
//
void Blackjack(void)
{
  MenuInit();
  MenuUpdate();
  return;
}

//-----------------------------------------------------------------------------
// Private Functions:
//-----------------------------------------------------------------------------

// 
// \param void
// \return N/A
// \brief Prints the header of the game
//
static void MenuInit(void)
{
  printf("\nBLACKJACK V0.3.1 | BY NICHOLAS BRENNAN\n");
  printf("--------------------------------------\n");

  isRunning = TRUE;
  menuState = DEFAULT;

  return;
}

static void MenuUpdate(void)
{
  const char input[50];

  while (isRunning)
  {
    switch (menuState)
    {
      case DEFAULT:
        printf("\nWelcome to Blackjack\n");
        printf("What would you like to do: ");
        scanf_s("%6s", input);

        if (strncmp(input, "exit", sizeof("exit")) == 0)
          menuState = EXIT;
        else if (strncmp(input, "play", sizeof("play")) == 0)
          menuState = PLAY;
        else if (strncmp(input, "help", sizeof("help")) == 0)
          menuState = HELP;
        else if (strncmp(input, "rules", sizeof("rules")) == 0)
          menuState = RULES;
        else if (strncmp(input, "update", sizeof("update")) == 0)
          menuState = UPDATE;
        else
          menuState = ERROR;

        break;

      case EXIT:
        return;

      case HELP:
        printf("\n              List of Commands              \n");
        printf("----------------------------------------------\n");
        printf("play   - starts blackjack  \n");
        printf("exit   - exits the program \n");
        printf("help   - shows all commands <------\n");
        printf("rules  - shows the rules of the game\n");
        printf("update - shows what's coming in the next update\n");
        menuState = DEFAULT;
        break;

      case PLAY:
        Play();
        menuState = DEFAULT;
        break;

      case RULES:
        printf("\nWelcome to Blackjack. Things are a little different in this version.\n");
        printf("  You play the dealer for the first to 21. If either of you go over,\n");
        printf("  the other automatically wins.\n");
        printf("  Aces are *only* worth one. \n  Face cards continue past ten (jacks = 11, etc.)\n\n");

        menuState = DEFAULT;
        break;

      case UPDATE:
        printf("\nCOMING NEXT UPDATE:\n");
        printf("  - Changing face cards to 10\n\n");

        menuState = DEFAULT;
        break;

      case ERROR:
        printf("Invalid Input\n");
        menuState = DEFAULT;
        break;
    }
  }
}

static void Play(void)
{
  inGame = TRUE;

  char input[50];
  int deckIndex = 0;

  gameState = BEGIN;

  int dealerCanPlay = TRUE;
  int playerCanPlay = TRUE;

  Player *player = PlayerCreate();
  Player *dealer = PlayerCreate();

  PlayerSetHand(player, 0);
  PlayerSetHand(dealer, 0);

  Card *deck = DeckCreate();
  DeckShuffle(deck);

  while (inGame)
  {
    switch (gameState)
    {
      case BEGIN:

        Card *card = DeckGetCard(deck, deckIndex);
        PlayerAddHand(player, CardGetRank(card));
        deckIndex++;

        card = DeckGetCard(deck, deckIndex);
        PlayerAddHand(dealer, CardGetRank(card));
        deckIndex++;

        card = DeckGetCard(deck, deckIndex);
        PlayerAddHand(player, CardGetRank(card));
        deckIndex++;

        card = DeckGetCard(deck, deckIndex);
        PlayerAddHand(dealer, CardGetRank(card));
        deckIndex++;

        printf("You drew two cards. Your new total hand is %d.\n", PlayerGetHand(player));
        printf("The dealer drew two cards. Their new total hand is %d.\n", PlayerGetHand(dealer));

        if (PlayerGetHand(player) > 21)
        {
          gameState = LOSE;
          break;
        }

        if (PlayerGetHand(player) == 21)
        {
          
          gameState = WIN;
          break;
        }

        if (PlayerGetHand(dealer) > 21)
        {
          gameState = WIN;
          break;
        }

        if (PlayerGetHand(dealer) == 21)
        {
          
          gameState = LOSE;
          break;
        }

        gameState = DECIDE;

        break;
      case DECIDE:
        if (playerCanPlay)
        {
          printf("\nHit or stand: ");
          scanf("%s", input);

          if (strncmp(input, "hit", sizeof("hit")) == 0)
            gameState = HIT;
          else if (strncmp(input, "stand", sizeof("stand")) == 0)
            gameState = STAND;
          else
            gameState = ERROR;

          break;
        }

        // Conditions for if both players stand
        if (!playerCanPlay && !dealerCanPlay)
        {
          if (PlayerGetHand(player) == PlayerGetHand(dealer))
          {
            printf("You and the dealer have the same total hand.\n");
            gameState = DRAW;
          }

          printf("Your hand is %d. The dealer's hand is %d.\n", PlayerGetHand(player), PlayerGetHand(dealer));

          if (PlayerGetHand(player) > PlayerGetHand(dealer))
            gameState = WIN;
          else
            gameState = LOSE;

          break;
        }
        
        printf("You cannot play this round. Press any key to continue.\n\n");
        _getch();
        gameState = DEALER;
        break;

      case HIT:
        card = DeckGetCard(deck, deckIndex);
        PlayerAddHand(player, CardGetRank(card));
        printf("You drew a %d. Your new total hand is %d.\n", CardGetRank(card), PlayerGetHand(player));
        deckIndex++;
        
        if (PlayerGetHand(player) > 21)
        {
          gameState = LOSE;
          break;
        }

        if (PlayerGetHand(player) == 21)
        {
          
          gameState = WIN;
          break;
        }

        gameState = DEALER;

        break;

      case STAND:
        printf("You stand. Your total hand is %d.\n", PlayerGetHand(player));

        if (PlayerGetHand(player) < PlayerGetHand(dealer))
        {
          printf("Your hand is %d. The dealer's hand is %d.\n", PlayerGetHand(player), PlayerGetHand(dealer));
          gameState = LOSE;
          break;
        }

        playerCanPlay = FALSE;
        gameState = DEALER;
        break;

      case DEALER:
        if (dealerCanPlay)
        {
          if (PlayerGetHand(dealer) < 19)
          {
            Card *dealerCard = DeckGetCard(deck, deckIndex);
            PlayerAddHand(dealer, CardGetRank(dealerCard));
            deckIndex++;

            printf("The dealer draws a %d. The dealer's hand is now %d.\n\n", CardGetRank(dealerCard), PlayerGetHand(dealer));

            if (PlayerGetHand(dealer) > 21)
            {
              gameState = WIN;
              break;
            }

            if (PlayerGetHand(dealer) == 21)
            {
              printf("Blackjack!\n");
              gameState = LOSE;
              break;
            }

            gameState = DECIDE;
            break;
          }
        }
        
        printf("The dealer stands. Their total hand is %d.\n\n", PlayerGetHand(dealer));
        dealerCanPlay = FALSE;
        gameState = DECIDE;
        break;

      case WIN:
        printf("\nYou win!\n");
        gameState = PLAYAGAIN;
        break;

      case LOSE:
        printf("\nYou lose!\n");
        gameState = PLAYAGAIN;
        break;

      case DRAW:
        printf("\nDraw!\n");
        gameState = PLAYAGAIN;
        break;

      case PLAYAGAIN:
        printf("Would you like to play again? (yes / no) ");
        scanf("%s", input);

        if (strncmp(input, "yes", sizeof("yes")) == 0)
        {
          DeckShuffle(deck);
          PlayerSetHand(player, 0);
          PlayerSetHand(dealer, 0);
          playerCanPlay = TRUE;
          dealerCanPlay = TRUE;
          gameState = BEGIN;
        }
        else if (strncmp(input, "no", sizeof("no")) == 0)
        {
          inGame = FALSE;
        }
        else
        {
          printf("Invalid Input\n");
        }

        break;

      case ERRORGAME:
        printf("Invalid Input\n");
        gameState = DECIDE;
        break;
    }
  }

  PlayerFree(&player);
  PlayerFree(&dealer);
  DeckFree(&deck);
}