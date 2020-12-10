#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// 가위바위보 규칙을 설명해주는 함수
void Rule();

int main(int argc, char *argv[]) {
    int sd;
    char buf[256];
    struct sockaddr_in sin;

    // 가위바위보 규칙
    Rule();

    // 소켓 파일 기술자 생성
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // 주소 초기화 후 IP주소와 포트 지정
    memset((char *)&sin, '\0', sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(atoi(argv[2]));
    sin.sin_addr.s_addr = inet_addr(argv[1]);

    // 클라이언트가 서버에 접속 요청
    if (connect(sd, (struct sockaddr *)&sin, sizeof(sin))) {
        perror("connect");
        exit(1);
    }

    // 데이터 수신 (가위바위보 게임 시작 출력을 받아옴)
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
    printf(" ========가위바위보 게임=========\n");
    printf(" | 가위>보>주먹 주먹>가위>보      |\n");
    printf(" --------------------------------\n");

    
}
