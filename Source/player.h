typedef struct Player Player;

Player* PlayerCreate(void);
void PlayerFree(Player **player);
void PlayerSetHand(Player *player, int value);
void PlayerAddHand(Player *player, int value);
int PlayerGetHand(Player* player);