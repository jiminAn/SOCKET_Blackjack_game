# UNIX Programming Project 2

## 1. 주제 설명
### 주제
블랙잭 게임
### 주제 설명
소켓 프로그래밍을 활용한 컴퓨터와 플레이어 간의 1대1 블랙잭 게임으로 클라이언트 프로그램에서는 플레이어의 카드를, 서버 프로그램에서는 컴퓨터의 카드를 확인할 수 있음. 기존의 블랙잭 룰 중 가장 보편적인 룰을 따르며 과제 마감 기한을 맞추고 프로그램의 복잡도를 낮추기 위해 아래와 같은 간단한 룰로 구현

### 해당 프로그램에서 사용한 블랙잭 룰
1. 카드이 총 합이 21일 경우 BLACKJACK 
2. 총 합이 <21>을 초과하기 전까지 플레이어는 카드 뽑기를 go/stop 할 수 있음
3. 컴퓨터는 카드의 총합이 17미만일 경우에 카드 뽑기 go를 택함
4. Ace는 50%의 확률로 1 혹은 11로 계산됨
5. 카드 뽑기를 go 했을 때 21을 초과할 경우 상대방의 카드와 상관없이 짐(FAIL)
*카드를 문양 별로(스페이드 > 다이아 > 하트 > 클로버) 뽑기는 하나 문양 순위에 상관 없이 컴퓨터와 플레이어가 같은 점수일 경우 비기는 것으로 처리

## 2. 시스템 설계도
## 2.1 파일 설명
### <blackjack code>
1. blackjack.c/h: 블랙잭 게임 관련 함수 및 헤더파일
2. blackjack.o: gcc -Wall -c blackjack.c 결과 object 파일
### <client code>
1. client.c: 블랙잭 게임 클라이언트 코드
2. client.o: gcc -Wall -c client.c 결과 object 파일
### <server code>
1. server.c: 블랙잭 게임 서버 코드
2. server.o: gcc -Wall -c server.c 결과 object 파일
### <게임 및 전적 기록 코드와 파일>
1. reset.c: log.txt 내용 초기화
2. log.txt/mystat.txt : 게임 기록 및 전적 기록 txt 파일
### <그 외 프로그램>
1. test_c : gcc -o test_c blackjack.o client.o 결과 블랙잭 클라이언트 프로그램
2. test_s : gcc -o test_c blackjack.o server.o 결과 블랙잭 서버 프로그램

## 2.2 컴파일 순서 
### 1. 각 코드의 오브젝트 파일 생성
- gcc -Wall -c blackjack.c -> blackjack.o
- gcc -Wall -c client.c -> client.o
- gcc -Wall -c server.c ->  server.o
### 2. 각 오브젝트 파일을 링크하여 최종 실행 파일 생성
- gcc -o test_c blackjack.o client.o -> test_c
- gcc -o test_c blackjack.o server.o -> test_c
### 3. 서버/클라이언트 프로그램 실행 전 log.txt 파일 초기화 
- gcc -c reset rest.c
- ./rest


## 2.3 소켓 함수의 호출 순서
![image-20200930195006846](https://github.com/jiminAn/UNIX_final/blob/master/img/1.png)

## 3. 사용된 기술
- 소켓 프로그래밍을 이용한 서버/클라이언트 프로그램
- 파일 입출력을 통한 게임 기록 및 전적 기록
- blackjack.c 코드에서 rand(), cardRound()를 통한 카드 중복 없이 뽑기 가능
- blackjack.c 코드에서 show_card()를 통해 카드 모양으로 인터페이스 출력

## 4. 사용 매뉴얼
1. 게임을 시작하기전 reset 프로그램을 통해 게임 기록 파일인 log.txt 초기화 : ./reset
2.  터미널 두개를 사용 : 터미널1, 터미널2
3. 터미널1에서 서버 프로그램인 test_s를 실행 : ./test_s
4.  터미널2에서 클라이언트 프로그램인 test_c를 실행 : ./test_c
5. 카드 합을 보면서 더 뽑을지(go) 멈출지(stop) 플레이어가 결정 
6. stop할 경우 서버에서 받아온 컴퓨터의 블랙잭 결과값과 비교하여 승패 출력
 7. log.txt에 게임 기록/ mystat.txt에 전적 기록
 
 ## 5. 사용 예
 ![image-20200930195006846](https://github.com/jiminAn/UNIX_final/blob/master/img/2.png)



