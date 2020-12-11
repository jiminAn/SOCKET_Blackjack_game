#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "blackjack.h"

void who_win(){
    bool p1_answer = true;
    bool p2_answer = true;
    int players[2] = {0,0}; //user vs. computer
    int size = 2;
	bool flag = true;
	int points = 0;
	srand(time(NULL));
	cardRound(players, size); // 1st turn : open the card
	cardRound(players, size); // 2nd turn : show only by oneself
	char choice;
	while(flag){
        for(int i = 0; i < size; i++){
            if(checkDisqualify(players[i])){
                if(i == (size - 1)){
                    if(checkDisqualify(players[i - 1])){
                        printf("\nNo one won the game!\n");
                        flag = false;
                        break;
                    }
                    else
                    {
                        printf("Player %d exceeded 21 points and got disqualified!\n", i + 1);
                        printf("Player %d won with %d points!\n", i, players[i - 1]);
                        flag = false;
                        break;
                    }
                    
                }
                else{
                    if(checkDisqualify(players[i + 1])){
                        printf("No one won the game!\n");
                        flag = false;
                        break;
                    }
                    else
                    {
                        printf("Player %d exceeded 21 points and got disqualified!\n", i + 1);
                        printf("Player %d won with %d points!\n", i + 2, players[i + 1]);
                        flag = false;
                        break;
                    }
                }
            }
    //        if(checkAce(p1_card)){
    //            if( players[0] + 10 <= 21)
    //                players[0] += 10;
    //        }
    //        if(checkAce(p2_card)){
    //            if( players[1] + 10 <= 21)
    //                players[1] += 10;
    //        }
            if(checkWin(players[i])){
                printf("blackjack! Player %d won with %d points!\n", i + 1, players[i]);
                flag = false;
                break;
            }
            if( !p1_answer && !p2_answer ){ // both says NO
                if(players[0] > players[1]){
                    printf("player 1 is winner!\n");
                    printf("player1 : %d | player2: %d ", players[0], players[1]);
                    flag = false;
                    break;
                }
                else if(players[0] < players[1]){
                    printf("player 2 is winner!\n");
                    printf("player1 : %d | player2: %d ", players[0], players[1]);
                    flag = false;
                    break;
                }
                else{
                    printf("No one won the game!\n");
                    printf("player1 : %d | player2: %d ", players[0], players[1]);
                    flag = false;
                    break;
                }
                printf("\n\n");
            }
            if( i == 0 && p1_answer){
                printf("====Player 1 cards====\n");
                for(int i = 0; i < p1_cnt; i++){
                    show_card(p1_card[i]);
                }
                printf("Do you want to hit, player 1? y or n\n");
            }
            else if( i == 1 && p2_answer){
                printf("====Player 2 cards====\n");
                for(int i = 0; i < p2_cnt; i++){
                    show_card(p2_card[i]);
                }
                printf("Do you want to hit, player 2? y or n\n");
            }
    //
    //        if( i == 0 && p1_answer)
    //            printf("Do you want to hit, player 1? y or n\n");
    //        else if( i == 1 && p2_answer)
    //            printf("Do you want to hit, player 2? y or n\n");
            scanf(" %c", &choice);
            if(choice == 'y' || choice == 'Y'){
                points = givePoint(i);
                players[i] += points;
            }
            else if(choice == 'n' || choice == 'N'){
                if( i == 0)
                    p1_answer = false;
                else
                    p2_answer = false;
                continue;
            }
            else{
                printf("Invalid input!\n");
                flag = false;
                break;
            }

        }
	}
}
///*
//	Gives random between 2 given number
//	1 - returns 1 points
//	2 - returns 11 points
//*/
//int randfor2(){ // 합리적이지 않은데 ㅋㅋ
//	int chosen = ((rand()) % (2 - 1 + 1) + 1);
//	if(chosen == 1)
//		return 1;
//	else if(chosen == 2)
//		return 11;
//}

// Checks so that cards are not repeated
bool checkRepeat(Card c_card){
    if ( check[c_card.type-1][c_card.num-1] == false){//non-repeat
        check[c_card.type-1][c_card.num-1] = true;
        return false;
    }
    else{//repeat
        return true;
    }

}

// Gets the card and returns the point
int givePoint(int i){
	int random_number = rand(); // generate random number
    
    // 1: spades, 2: diamonds, 3: hearts, 4: clubs
	int lower = 1, upper = 4;
	Card g_card;
    g_card.type = ((random_number) % (upper - lower + 1)) + lower;
    
    // 1: Ace ~ 11: J, 12: Q, 13: K
    lower = 1, upper = 13;
    g_card.num = ((random_number) % (upper - lower + 1)) + lower;
    
    
	if(checkRepeat(g_card)){ // reallocate the card
		givePoint(i);
	}
	else{
        if(i == 0){
            p1_card[p1_cnt].type = g_card.type;
            p1_card[p1_cnt++].num = g_card.num;
        }
        if(i == 1){
            p2_card[p2_cnt].type = g_card.type;
            p2_card[p2_cnt++].num = g_card.num;
        }
        show_card(g_card);
		if(g_card.num < 11){
//			if(g_card.num == 1){ // Ace
//				int chosen = randfor2();
//				return chosen;
//			}
			return g_card.num;
		}
		else
			return 10;
	}
	
}

// Checks if a player has more than 21 points
bool checkDisqualify(int points){
	if(points > 21)
		return true;
	else
		return false;
}

// Checks if a player has 21 points and won
bool checkWin(int points){
	if(points == 21)
		return true;
	else
		return false;
}

// To assign cards and points to players every round
void cardRound(int players[], int size){
	int points = 0;
	for(int i = 0; i < size; i++){
        printf("player %d : ",i+1);
		points = givePoint(i);
		players[i] += points;
	}
}

void show_card(Card c){
    int t = c.type;
    int n = c.num;
	char Picture;
	bool pic = true;
	if(n < 11 && n > 1){ // 2~10 are just number
		pic = false;
	}
	else if(n == 11)
		Picture = 'J';
	
	else if(n == 12)
		Picture = 'Q';
	
	else if(n == 13)
		Picture = 'Q';
	
	else
		Picture = 'A';
	
    printf("\n\n");
	switch(t){
		case 1: // spades
			if(pic){
				printf("%c of Spades", Picture);
			}
			else{
				printf("%d of Spades", n);
			}
			break;
		case 2: // diamonds
			if(pic){
				printf("%c of diamonds", Picture);
			}
			else{
				printf("%d of diamonds", n);
			}
			break;
		case 3: // hearts
			if(pic){
				printf("%c of hearts", Picture);
			}
			else{
				printf("%d of hearts", n);
			}
			break;
		case 4: //clubs
			if(pic){
				printf("%c of Clubs", Picture);
			}
			else{
				printf("%d of Clubs", n);
			}
			break;
		default:
			break;
	}
    printf("\n\n");
}

bool checkAce(Card c[]){
    for(int i = 0; i < 10; i++){
        if (c[i].num == 1)
            return true;
    }
    return false;
    
}
