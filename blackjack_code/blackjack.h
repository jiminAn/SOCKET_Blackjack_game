#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// In Kr
// spades > diamonds > hearts > clubs
typedef struct card{
    int type;
    int num;
}Card;

Card p1_card[10];
Card p2_card[10];
int p1_cnt;
int p2_cnt;

bool check[4][13];

void show_card(Card c);
int givePoint(int i);
void cardRound(int players[], int size);
int randfor2();
bool checkDisqualify(int points);
bool checkWin(int points);
bool checkRepeat(Card c);
int printType(Card c);
bool checkAce(Card c[]);
void who_win();
