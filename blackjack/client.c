#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "blackjack.h"

#define PORTNUM 9000
// 블랙잭 규칙을 설명해주는 함수
void Rule();
int main(void) {
    srand(time(NULL));
    char choice;
    int user_point = 0;
    int com_point = 0;
    int sd;
    char buf[256];
    struct sockaddr_in sin;
    int result; // 0:draw 1:win 2:fail(player)
    
    Rule();

    // 소켓 파일 기술자 생성
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // 주소 초기화 후 IP주소와 포트 지정
    memset((char *)&sin, '\0', sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORTNUM);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 클라이언트가 서버에 접속 요청
    if (connect(sd, (struct sockaddr *)&sin, sizeof(sin))) {
        perror("connect");
        exit(1);
    }

    // 데이터 수신 (블랙잭 게임 시작 출력을 받아옴)
    if (recv(sd, buf, sizeof(buf), 0) == -1) {
        perror("recv");
        exit(1);
    }
    printf("%s\n", buf);

    //사용자 블랙잭 카드 두 장 뽑기
    user_point = cardRound(user_point, 0);
    user_point = cardRound(user_point, 0);
    printf("player point : %d \n", user_point);
    //점수 : 정수->char *저장
   // sprintf(buf, "%d", user_point);
    
    
//    // 데이터 송신 (buf 값을 보냄)
//    if (send(sd, buf, strlen(buf) + 1, 0) == -1) {
//        perror("send");
//        exit(1);
//    }
    // 한 장 더 뽑을지 물어보기
    do{
        printf("Do you want to hit, player? y or n\n");
        scanf(" %c", &choice);
        getchar();
        if(choice == 'y' || choice == 'Y'){
            user_point = cardRound(user_point, 0);
            printf("player point : %d \n", user_point);
        }
        if( user_point > 21){
            result = 2;
            printf("플레이어 EXCEEDED 21!! 당신의 패배입니다");
            // 소켓 파일 기술자 종료
            close(sd);
            return 0;
        }
        if(choice == 'n' || choice == 'N'){
            break;
        }
    }while(choice == 'y' || choice == 'Y');
    
    // 데이터 수신 (블랙잭 컴퓨터 상태를 받아옴)
    if (recv(sd, buf, sizeof(buf), 0) == -1) {
        perror("recv");
        exit(1);
    }
    com_point = atoi(buf);
   // printf("compoint : %d \n", com_point);
    if( com_point == 1 && user_point != 21 ){
        printf("컴퓨터의 BLACKJACK!! 당신의 패배입니다");
        result = 2;
    }
    else if(com_point == 2){
        printf("컴퓨터의 EXCEEDED 21!! 당신의 승리입니다");
        result = 1;
    }
    else{
        if( user_point > com_point ){
            printf("player %d points | computer %d points\n",user_point, com_point);
            result = 1;
            printf("PLAYER WIN!");
        }
        else if( user_point < com_point ){
            printf("player %d points | computer %d points\n",user_point, com_point);
            result = 2;
            printf("PLAYER LOSE!");
        }
        else{
            printf("player %d points | computer %d points\n",user_point, com_point);
            result = 0;
            printf("PLAYER AND COMPUTER DRAW!");
        }
            
    }
    sprintf(buf, "%d", user_point);
    // 데이터 송신 (buf 값을 보냄)
    if (send(sd, buf, strlen(buf) + 1, 0) == -1) {
        perror("send");
        exit(1);
    }
    
//    printf("result : %d \n", result);
//    sprintf(buf, "%d", result);
//    printf("result_buf : %s \n", buf);
//    // 데이터 송신 (buf 값을 보냄)
//    if (send(sd, buf, strlen(buf) + 1, 0) == -1) {
//        perror("send");
//        exit(1);
//    }
    // 소켓 파일 기술자 종료
    close(sd);

    return 0;
}
// 블랙잭 게임 설명
void Rule(){

  
    printf("\n");
    printf("===================블랙잭 게임 설명===================\n");
    printf(" | 합이 21에 가까운 사람이 이기는 게임입니다       |\n");
    printf(" | 첫 번째 카드는 사용자와 컴퓨터 모두 오픈합니다    |\n");
    printf(" | 자신이 가지고 있는 카드의 총 합을 턴 마다 계산해서 |\n");
    printf(" | 21이 넘지 않는다면 게임을 계속할 수 있습니다     |\n");
    printf(" | 사용자는 go/stop을 할 수 있습니다            |\n");
    printf(" | 21이 초과할 경우 상대 패와 상관없이 패배하게됩니다 |\n");
    printf(" | Ctrl+c할 경우 게임을 종료할 수 있습니다.       |\n");
    printf(" --------------------------------\n");

    
}


   
