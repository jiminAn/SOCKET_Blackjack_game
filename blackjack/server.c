#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include "blackjack.h"

#define PORTNUM 9000

void WriteLog(int result, int user_p, int com_p);// 게임 기록을 파일에 입력하는 함수
void MyStat(int win, int lose, int draw, int cnt);// 전적 기록을 파일에 입력하는 함수

int main(void) {
    srand(time(NULL));
    int result;
    int win, lose, draw, cnt;
    char buf[256];
    int com_point = 0;
    int user_point = 0;
    struct sockaddr_in sin, cli;
    int sd, ns, clientlen = sizeof(cli);
    int s1, s2; // s1: 컴 이김 , s2 : 컴 짐
    s1 = 1;
    s2 = 2;
    win = lose = draw = cnt = 0;

    // 소켓 파일 기술자 생성
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    
    // 주소 초기화 후 IP 주소와 포트 지정
    memset((char *)&sin, '\0', sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORTNUM); // 포트 넘버 연결
    sin.sin_addr.s_addr = inet_addr("127.0.0.1"); // ip 주소 연결
    
    
    // 소켓과 서버 주소를 바인딩
    if (bind(sd, (struct sockaddr *)&sin, sizeof(sin))) {
        perror("bind");
        exit(1);
    }
    
    // client 최대 5
    if (listen(sd, 5) == -1) {
        perror("listen");
        exit(1);
    }
    while(1){// 반복 서버
        com_point = 0;
        com_cnt = 0;
        check_init();
        // 클라이언트의 접속 허용
        if ((ns = accept(sd, (struct sockaddr *)&cli, &clientlen)) == -1) {
            perror("accept");
            exit(1);
        }

        // 블랙잭 게임 시작 출력
        sprintf(buf, "================WELCOME TO SIMPLE BLACKJACK================\n\n\n블랙잭 게임에 오신걸 환영합니다.\n\n\n======================================================\n\n");
        // 데이터 송신 (시작 화면 출력)
        if (send(ns, buf, strlen(buf) + 1, 0) == -1) {
            perror("send");
            exit(1);
        }
        // 컴 : 처음에 두 장 뽑기
        com_point = cardRound(com_point,1);
        com_point = cardRound(com_point,1);
        printf("computer point : %d \n", com_point);
        
        
        while(com_point < 17){// 카드 합이 17이하일 경우 뽑기
            com_point = cardRound(com_point,1);
            printf("computer point : %d \n", com_point);
        }
        // 컴퓨터 블랙잭 여부 보내기/ 블랙잭 아닐 경우 총 합 보내기
        if( com_point == 21)
            sprintf(buf, "%d", s1);
        else if (com_point > 21)
            sprintf(buf, "%d", s2);
        else
            sprintf(buf, "%d", com_point);
        
        // 데이터 송신
        if (send(ns, buf, strlen(buf) + 1, 0) == -1) {
            perror("send");
            exit(1);
        }
        //데이터 수신 (buf 값(사용자 결과 받기))
        if (recv(ns, buf, sizeof(buf), 0) == -1) {
           perror("recv");
           exit(1);
        }
        user_point = atoi(buf);
        
        // 승패 저장
        // 1: 사용자 이김 2: 사용자 짐 3: 비김
        if( user_point > 21 && com_point > 21){
            result = 2;
        }
        else if (user_point > 21 || com_point > 21){
            if( user_point > 21)
                result = 2;
            else
                result = 1;
        }
        else{
            if( user_point > com_point)
                result = 1;
            else if( user_point < com_point)
                result = 2;
            else
                result = 0;
        }
        // 로그 기록 함수
        WriteLog(result,user_point,com_point);

        // 전, 승, 무, 패 증가
        switch (result) {
            case 0: draw++;
                    break;
            case 1: win++;
                    break;
            case 2: lose++;
                    break;
        }
        cnt++;

        // 전적 기록 함수
        MyStat(win, lose, draw, cnt);
        
    }
    
    
    // 소켓 파일 기술자 종료
    close(ns);
    close(sd);

    return 0;
}
void WriteLog(int result, int user_p, int com_p) {
    int fd;
    int n;
    char str[256];
    // log.txt 파일에 입력
    fd = open("log.txt", O_WRONLY | O_APPEND); // 쓰기 전용, 이어 쓰기
    if (fd == -1) {
        perror("Open");
        exit(1);
    }
    printf("result : %d \n", result);
    switch(result){
        case 0:
                sprintf(str,"PLAYER : %dpoints | COMPUTER : %dpoints !DRAW!  \n",user_p, com_p);
                break;
        case 1:
            sprintf(str,"PLAYER : %dpoints | COMPUTER : %dpoints !PLAYER WIN! \n",user_p, com_p);
                break;
        case 2:
            sprintf(str,"PLAYER : %dpoints | COMPUTER : %dpoints !PLAYER LOSE! \n",user_p, com_p);
                break;
    }
    n = write(fd, str, strlen(str));
    if (n != strlen(str)) {
        perror("Write");
        exit(2);
    }
    close(fd);
}
void MyStat(int win, int lose, int draw, int cnt) {
    int fd;
    int n;
    char str[256];

    // 전, 승, 무, 패, 승률 출력
    sprintf(str, "%d 전 \t %d 승 \t %d 무 \t %d 패 \n승률 : %1.f%%\n", cnt, win, draw, lose, (double)win / cnt * 100);

    // mystat.txt 파일에 입력
    fd = open("mystat.txt", O_WRONLY); // 쓰기 전용, 이어 쓰기
    if (fd == -1) {
        perror("Open");
        exit(1);
    }

    // 전적을 파일에 저장
    n = write(fd, str, strlen(str));
    if (n != strlen(str)) {
        perror("Write");
        exit(2);
    }
    close(fd);
}
