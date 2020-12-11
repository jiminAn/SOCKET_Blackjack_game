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
//블랙잭 게임

//컴퓨터의 가위바위보 값 결정
int ComRps(void);

// 컴퓨터와 사용자의 가위바위보 결과
int RpsResult(int user, int com);


//게임 기록을 파일에 입력하는 함수 (초기화 X, 직접 초기화)
void WriteLog(char str[]);

// 전적 기록을 파일에 입력하는 함수 (서버 재가동시 초기화)
void MyStat(int win, int lose, int draw, int count);

int players[2] = {0,0}; // user vs computer
int main(void) {

    char buf[256];
    struct sockaddr_in sin, cli;
    int sd, ns, clientlen = sizeof(cli);
    
    card_init();
    int com, user; // 컴퓨터 및 사용자의 블랙잭
    int win = 0; // 승리 횟수
    int lose = 0; // 패배 횟수
    int draw = 0; // 무승부 횟수
    int count = 0; // 게임 횟수
    int i; // 승리(1), 패배(2), 무승부(3)

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

    // 최대 5명
    if (listen(sd, 5) == -1) {
        perror("listen");
        exit(1);
    }

    // 반복 서버
    while (100) {
        // 클라이언트의 접속 허용
        if ((ns = accept(sd, (struct sockaddr *)&cli, &clientlen)) == -1) {
            perror("accept");
            exit(1);
        }

        // start black jack game
        sprintf(buf, "\n===============BLACKJACK  GAME===================\n\n");
        // 데이터 송신 (시작 화면 출력)
        if (send(ns, buf, strlen(buf) + 1, 0) == -1) {
            perror("send");
            exit(1);
        }

        // 데이터 수신 (buf 값(가위, 바위, 보)을 가져옴)
        if (recv(ns, buf, sizeof(buf), 0) == -1) {
            perror("recv");
            exit(1);
        }
        who_win();
        i = RpsResult(user, com);

        // 가위바위보 결과 출력
        sprintf(buf, "\n사용자 : %s vs 컴퓨터 : %s \n%s 하셨습니다!\n", RPS[user - 1], RPS[com - 1], RESULT[i - 1]);
        // 데이터 송신 (가위바위보 결과 값을 보냄)
        if (send(ns, buf, strlen(buf) + 1, 0) == -1) {
            perror("send");
            exit(1);
        }

        // 로그 기록 함수
        WriteLog(buf);

        // 전, 승, 무, 패 증가
        switch (i) {
        case 1: win++; break;
        case 2: lose++; break;
        case 3: draw++; break;
        }
        count++;

        // 전적 기록 함수
        MyStat(win, lose, draw, count);
    }

    // 소켓 파일 기술자 종료
    close(ns);
    close(sd);

    return 0;
}

void init_card(){
    int n = 1;
    for(int i = 0; i < 13; i++){// spades
        card[i].type = 's';
        if( n == 10 )
            card[i].num = n;
        else
            card[i].num = n++;
    }
    n = 0;
    for(int i = 0; i < 13; i++){// diamonds
        card[i].type = 'd';
        if( n == 10 )
            card[i].num = n;
        else
            card[i].num = n++;
    }
    n = 0;
    for(int i = 0; i < 13; i++){//hearts
        card[i].type = 'h';
        if( n == 10 )
            card[i].num = n;
        else
            card[i].num = n++;
    }
    n = 0;
    for(int i = 0; i < 13; i++){//clubs
        card[i].type = 'c';
        if( n == 10 )
            card[i].num = n;
        else
            card[i].num = n++;
    }
        
}

// 컴퓨터의 가위바위보 값 결정
int ComRps(void) {
    int com; // 컴퓨터의 가위바위보 값
    srand((unsigned)time(NULL));
    com = rand() % 3 + 1; // 1 ~ 3 까지의 랜덤 변수

    return com;
}

// 컴퓨터와 사용자의 가위바위보 결과
int RpsResult(int user, int com) {
    int i;

    if (user == com) // 무승부 (같을 경우)
        i = 3;

    else { // 무승부가 아닐 경우
        switch (user) { // 사용자의 값에 따라
        case 1: i = (com == 3 ? 1 : 2); break;
        case 2: i = (com == 1 ? 1 : 2); break;
        case 3: i = (com == 2 ? 1 : 2); break;
        }
    }

    // i = 1(승리), i = 2(패배), i = 3(무승부)
    return i;
}

// 게임 기록을 파일에 입력하는 함수
void WriteLog(char str[]) {
    int fd;
    int n;

    // log.txt 파일에 입력
    fd = open("log.txt", O_WRONLY | O_APPEND); // 쓰기 전용, 이어 쓰기
    if (fd == -1) {
        perror("Open");
        exit(1);
    }

    // buf값(가위바위보 결과 값)을 파일에 저장
    n = write(fd, str, strlen(str));
    if (n != strlen(str)) {
        perror("Write");
        exit(2);
    }
    close(fd);
}

// 전적 기록을 파일에 입력하는 함수
void MyStat(int win, int lose, int draw, int count) {
    int fd;
    int n;
    char str[256];

    // 전, 승, 무, 패, 승률 출력
    sprintf(str, "%d 전 \t %d 승 \t %d 무 \t %d 패 \n승률 : %1.f%%\n", count, win, draw, lose, (double)win / count * 100);

    // mystat.txt 파일에 입력
    fd = open("mystat.txt", O_WRONLY); // 쓰기 전용
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
