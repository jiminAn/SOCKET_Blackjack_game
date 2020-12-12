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

int main(void) {
    srand(time(NULL));
    int user_point = 0;
    int sd;
    char buf[256];
    struct sockaddr_in sin;

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

    //사용자 블랙잭 카드 첫장 뽑기
    user_point = cardRound(user_point, 0);
    //점수 : 정수->char *저장
    sprintf(buf, "%d", user_point);
    
    // 데이터 송신 (buf 값을 보냄)
    if (send(sd, buf, strlen(buf) + 1, 0) == -1) {
        perror("send");
        exit(1);
    }
    
    
    // 소켓 파일 기술자 종료
    close(sd);

    return 0;
}



   
