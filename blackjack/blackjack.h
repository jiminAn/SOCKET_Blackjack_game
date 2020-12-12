#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

// In Kr
// spades > diamonds > hearts > clubs
typedef struct card{
    int type; // 카드 문양 종류
    int num;  // 카드 숫자 Ace,2~10,J,Q,K
}Card;

Card p_card[10]; // 플레이어 카드 기억
Card com_card[10]; // 컴퓨터 카드 기억
int p_cnt;
int com_cnt;

bool check[4][13]; // 카드 중복 체크용
void show_card(Card c);// 카드 인터페이스 보여주기
int givePoint(int i);//카드 랜덤 선택(단, 중복없이)
int cardRound(int point, int n); //카드 중복 없이 할당
int randfor2();//for Ace cards
bool checkRepeat(Card c);// 카드 중복 확인

