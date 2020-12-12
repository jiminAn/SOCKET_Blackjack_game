#include "blackjack.h"


// 카드 중복 없이 할당
// n == 0 : player
// n == 1 : computer
int cardRound(int point, int n){
 
    int points = 0;
    if( n == 0)
        printf("_____player turn_____\n\n");
    else
        printf("_____computer turn_____\n\n");
    points = givePoint(n);
    point += points;
    return point;
    
}
// 카드 랜덤 선택(단, 중복 없이)
int givePoint(int i){
    
    int random_number = rand();
    
    // 1: spades, 2: diamonds, 3: hearts, 4: clubs
    int lower = 1, upper = 4;
    Card g_card;
    g_card.type = ((random_number) % (upper - lower + 1)) + lower;
    
    // 1: Ace ~ 11: J, 12: Q, 13: K
    lower = 1, upper = 13;
    g_card.num = ((random_number) % (upper - lower + 1)) + lower;
    
    
    if(checkRepeat(g_card)){ // 카드 중복일 경우 다시 뽑기
        givePoint(i);
    }
    else{
        if(i == 0){// 플레이어 카드 할당 & 저장
            p_card[p_cnt].type = g_card.type;
            p_card[p_cnt].num = g_card.num;
            show_card(p_card[p_cnt++]);
        }
        if(i == 1){ // 컴퓨터 카드 할당 & 저장
            com_card[com_cnt].type = g_card.type;
            com_card[com_cnt].num = g_card.num;
            show_card(com_card[p_cnt++]);
        }
        
        if(g_card.num < 11){
            if(g_card.num == 1){ // Ace
                int chosen = randfor2();
                return chosen;
            }
            return g_card.num;
        }
        else
            return 10;
    }
    
}
// 카드 중복 확인
bool checkRepeat(Card c_card){
    // 처음 뽑은 카드면 뽑은 카드로 체크
    if ( check[c_card.type-1][c_card.num-1] == false){
        check[c_card.type-1][c_card.num-1] = true;
        return false;
    }
    else{
        return true;
    }

}
// for Ace cards
// 1 - return 1 point
// 2 - return 11 points
int randfor2(){
    int chosen = ((rand()) % (2 - 1 + 1) + 1);
    if(chosen == 1)
        return 1;
    else if(chosen == 2)
        return 11;
}
// 카드 인터페이스 보여주기
void show_card(Card c){
    int t = c.type;
    int n = c.num;
    char pic[256];
    if( t == 0 )
        strcpy(pic,"♠");
    else if( t == 1 )
        strcpy(pic,"◇");
    else if( t == 2)
        strcpy(pic,"♡");
    else
        strcpy(pic,"♣");
    switch(n){
            case 1:
            {
                printf("*******\n");
                printf("*     *\n");
                printf("* %s   *\n", pic);
                printf("*   A *\n");
                printf("*     *\n");
                printf("*******\n");
                break;
            }
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            {
                printf("*******\n");
                printf("*     *\n");
                printf("* %s   *\n", pic);
                printf("*  %2d *\n", n);
                printf("*     *\n");
                printf("*******\n");
                break;
            }
            case 11:
            {
                printf("*******\n");
                printf("*     *\n");
                printf("* %s   *\n", pic);
                printf("*   J *\n");
                printf("*     *\n");
                printf("*******\n");
                break;
            }
            case 12:
            {
                printf("*******\n");
                printf("*     *\n");
                printf("* %s   *\n", pic);
                printf("*   Q *\n");
                printf("*     *\n");
                printf("*******\n");
                break;
            }
            case 13:
            {
                printf("*******\n");
                printf("*     *\n");
                printf("* %s   *\n", pic);
                printf("*   K *\n");
                printf("*     *\n");
                printf("*******\n");
                break;
            }
    }
}
