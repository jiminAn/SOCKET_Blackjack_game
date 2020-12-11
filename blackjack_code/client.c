#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PORTNUM 9000
// rule of blackjack
void Rule();

int main(void) {
    int sd;
    char buf[256];
    struct sockaddr_in sin;

    Rule();// explain rule of blackjack

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

    // receive data : <buf : start black jack game>
    if (recv(sd, buf, sizeof(buf), 0) == -1) {
        perror("recv");
        exit(1);
    }
    printf("%s\n", buf);

    // buf에 입력
    printf("입력 : ");
    scanf("%s", &buf);

    // 데이터 송신 (buf 값을 보냄)
    if (send(sd, buf, strlen(buf) + 1, 0) == -1) {
        perror("send");
        exit(1);
    }

    // 데이터 수신 (가위바위보 결과 값을 받아옴)
    if (recv(sd, buf, sizeof(buf), 0) == -1) {
        perror("recv");
        exit(1);
    }
    printf("%s\n", buf);

    // 소켓 파일 기술자 종료
    close(sd);

    return 0;
}

void Rule() {

    printf("\n");
    printf(" ========RULE OF BLACK JACK GAME=========\n");
    printf(" Players are each dealt two cards, one up (exposed) and one down (hidden). The value of cards two through ten is their pip value (2 through 10). Face cards (Jack, Queen, and King) are all worth ten. Aces can be worth one or eleven. A hand's value is the sum of the card values. Players are allowed to draw additional cards to improve their hands.If the player is dealt an Ace and a ten-value card (called a blackjack or natural. If the player exceeds a sum of 21 busts, the player loses.");
    printf("\n\n\n");

    
}
