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

//int players[2] = {0,0}; //user vs. computer


void WriteLog(int result, int user_p, int com_p);
void MyStat(int win, int lose, int draw, int cnt);

int main(void) {
    srand(time(NULL));
    int result;
    int win, lose, draw, cnt;
    char buf[256];
    char buf2[256];
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
    
    // client최대 5
    if (listen(sd, 5) == -1) {
        perror("listen");
        exit(1);
    }
    while(1){
        com_point = 0;
        com_cnt = 0;
        // 클라이언트의 접속 허용
        if ((ns = accept(sd, (struct sockaddr *)&cli, &clientlen)) == -1) {
            perror("accept");
            exit(1);
        }

        // 블랙잭 게임 시작 출력
        sprintf(buf, "=====블랙잭 게임에 오신걸 환영합니다=====\n\n\nAnytime you Ctrl+C를 누를 경우 종료 가능합니다.\n\n\n=====================================\n\n");
        // 데이터 송신 (시작 화면 출력)
        if (send(ns, buf, strlen(buf) + 1, 0) == -1) {
            perror("send");
            exit(1);
        }
        
    //    // 데이터 수신 (buf 값(사용자의 블랙잭 값 가져옴))
    //    if (recv(ns, buf, sizeof(buf), 0) == -1) {
    //        perror("recv");
    //        exit(1);
    //    }

        //user_point = atoi(buf);//사용자 점수 저장
        
        // 컴퓨터 블랙잭 카드 두장 뽑기 & 컴퓨터 점수
        com_point = cardRound(com_point,1);
        com_point = cardRound(com_point,1);
        printf("computer point : %d \n", com_point);
        
        while(com_point < 17){
            com_point = cardRound(com_point,1);
            printf("computer point : %d \n", com_point);
        }
        if( com_point == 21)
            //sprintf(buf, "컴퓨터의 BLOCKJACK!! 당신의 패배입니다");
            sprintf(buf, "%d", s1);
        else if (com_point > 21)
            //sprintf(buf, "컴퓨터의 %d로 초과했습니다! 당신의 승리입니다", com_point);
            sprintf(buf, "%d", s2);
        else
            sprintf(buf, "%d", com_point);
        // 데이터 송신 (컴퓨터 블랙잭 값 보내기)
        if (send(ns, buf, strlen(buf) + 1, 0) == -1) {
            perror("send");
            exit(1);
        }
        //데이터 수신 (buf 값(사용자 결과))
        if (recv(ns, buf, sizeof(buf), 0) == -1) {
           perror("recv");
           exit(1);
        }
        user_point = atoi(buf);
        
//         //데이터 수신 (buf 값(승패 결과))
//        if (recv(ns, buf2, sizeof(buf), 0) == -1) {
//            perror("recv2");
//            exit(1);
//        }
//        printf("result_buf2 : %s \n", buf2);
//        result = atoi(buf2);
        if( user_point > com_point)
            result = 1;
        else if( user_point < com_point)
            result = 2;
        else
            result = 0;
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
// 게임 기록을 파일에 입력하는 함수
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
//    printf("log %s \n",str);
    // buf값(가위바위보 결과 값)을 파일에 저장
    n = write(fd, str, strlen(str));
    if (n != strlen(str)) {
        perror("Write");
        exit(2);
    }
    close(fd);
}

// 전적 기록을 파일에 입력하는 함수
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
