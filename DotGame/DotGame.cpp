#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <iomanip>
#include <map>
#include <random>
#include <stdio.h>
#include <conio.h>
#include <thread>
#include <vector>
#include <string>
#pragma warning(disable : 4996)
using namespace std;

const int MAP_WIDTH = 25;
const int MAP_HEIGHT = 25;
const int PMAP_WIDTH = 25;
const int PMAP_HEIGHT = 25;

DWORD startTime;
int score = 0; // 점수 변수 초기화

std::vector<int> sco;
std::vector<int> trapsX;
std::vector<int> trapsY;

enum ConsoleColor {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    Yellow = 14,
    White = 15
};

void SetConsoleColor(ConsoleColor textColor, ConsoleColor bgColor) {
    int colorCode = textColor + (bgColor * 16);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}

class Map {
private:
    char map[MAP_HEIGHT][MAP_WIDTH];

public:
    Map() {
        system("cls"); // 화면 지우기

        // 맵 초기화
        for (int i = 0; i < MAP_HEIGHT; ++i) {
            for (int j = 0; j < MAP_WIDTH; ++j) {
                map[i][j] = ' ';
            }
        }

        // 맵 가장자리에 장애물 배치
        for (int i = 0; i < MAP_HEIGHT; ++i) {
            map[i][0] = '|'; // 왼쪽 가장자리
            SetConsoleColor(LightBlue, Black);
            map[i][MAP_WIDTH - 1] = '|'; // 오른쪽 가장자리
        }
        for (int j = 0; j < MAP_WIDTH; ++j) {
            map[0][j] = '-'; // 위쪽 가장자리
            SetConsoleColor(LightBlue, Black);
            map[MAP_HEIGHT - 1][j] = '-'; // 아래쪽 가장자리
        }
    }

    void placeObstacle(int x, int y) {
        map[y][x] = 'X';
    }

    void clearPosition(int x, int y) {
        map[y][x] = ' ';
    }

    char getMapValue(int x, int y) {
        return map[y][x];
    }

    void printMap() {
        // 커서를 맨 위로 이동
        std::cout << "\033[1;1H";
        for (int i = 0; i < MAP_HEIGHT; ++i) {
            for (int j = 0; j < MAP_WIDTH; ++j) {
                // 각각의 맵 요소에 따라 색상 설정
                switch (map[i][j]) {
                case 'X':
                    SetConsoleColor(Red, Black); // 빨간색 배경 없음
                    break;
                case '@':
                    SetConsoleColor(LightBlue, Black); // 파란색 배경 없음
                    break;
                default:
                    SetConsoleColor(White, Black); // 흰색 배경 없음
                    break;
                }
                std::cout << map[i][j] << " ";
            }
            std::cout << std::endl;
        }
        SetConsoleColor(White, Black); // 출력 후 기본 색상으로 복원
    }

    bool canMove(int x, int y) {
        return map[y][x] != '|' && map[y][x] != '-';
    }

    void moveCharacter(int& x, int& y, char direction) {
        int newX = x;
        int newY = y;

        switch (direction) {
        case 'w': // 위쪽 이동
            newY--;
            break;
        case 's': // 아래쪽 이동
            newY++;
            break;
        case 'a': // 왼쪽 이동
            newX--;
            break;
        case 'd': // 오른쪽 이동
            newX++;
            break;
        }

        if (canMove(newX, newY)) {
            clearPosition(x, y);
            x = newX;
            y = newY;
            map[y][x] = '@';
        }
    }


};

class PvPMap {
private:
    char map[PMAP_HEIGHT][PMAP_WIDTH];

public:
    PvPMap() {
        system("cls"); // 화면 지우기

        // 맵 초기화
        for (int i = 0; i < PMAP_HEIGHT; ++i) {
            for (int j = 0; j < PMAP_WIDTH; ++j) {
                map[i][j] = ' ';
            }
        }

        // 맵 가장자리에 장애물 배치
        for (int i = 0; i < PMAP_HEIGHT; ++i) {
            map[i][0] = '|'; // 왼쪽 가장자리
            map[i][PMAP_WIDTH - 1] = '|'; // 오른쪽 가장자리
        }
        for (int j = 0; j < PMAP_WIDTH; ++j) {
            map[0][j] = '-'; // 위쪽 가장자리
            map[PMAP_HEIGHT - 1][j] = '-'; // 아래쪽 가장자리
        }
    }

    void placeObstacle(int x, int y) {
        map[y][x] = 'X';
    }

    void clearPosition(int x, int y) {
        map[y][x] = ' ';
    }

    char getMapValue(int x, int y) {
        return map[y][x];
    }

    void printMap() {
        // 커서를 맨 위로 이동
        std::cout << "\033[1;1H";
        for (int i = 0; i < PMAP_HEIGHT; ++i) {
            for (int j = 0; j < PMAP_WIDTH; ++j) {
                // 각 요소의 색상 설정
                switch (map[i][j]) {
                case 'X':
                    SetConsoleColor(Red, Black); // 빨간색 배경 없음
                    break;
                case '@':
                    SetConsoleColor(LightBlue, Black); // 파란색 배경 없음
                    break;
                case '#':
                    SetConsoleColor(Yellow, Black); // 노란색 배경 없음
                    break;
                default:
                    SetConsoleColor(White, Black); // 흰색 배경 없음
                    break;
                }
                std::cout << map[i][j] << " ";
            }
            std::cout << std::endl;
        }
        SetConsoleColor(White, Black); // 출력 후 기본 색상으로 복원
    }

    bool canMove(int x, int y) {
        return map[y][x] != '|' && map[y][x] != '-';
    }

    bool userCanMove(int x, int y) {
        return map[y][x] != '|' && map[y][x] != '-' && map[y][x] != '@' && map[y][x] != '#';
    }

    void p1moveCharacter(int& x, int& y, char direction) {
        int newX = x;
        int newY = y;

        // 이동 방향에 따라 새로운 좌표 계산
        switch (direction) {
        case 'w': // 위쪽 이동
            newY--;
            break;
        case 's': // 아래쪽 이동
            newY++;
            break;
        case 'a': // 왼쪽 이동
            newX--;
            break;
        case 'd': // 오른쪽 이동
            newX++;
            break;
        }

        // 새로운 위치로 이동 가능한지 확인 후 이동
        if (userCanMove(newX, newY)) {
            // 이동 전 위치에는 도트로 표시
            clearPosition(x, y);
            x = newX;
            y = newY;
            // 이동 후 위치에는 캐릭터 표시
            map[y][x] = '@';
        }
    }

    void p2moveCharacter(int& x, int& y, char direction) {
        int newX = x;
        int newY = y;

        // 이동 방향에 따라 새로운 좌표 계산
        switch (direction) {
        case 'w': // 위쪽 이동
            newY--;
            break;
        case 's': // 아래쪽 이동
            newY++;
            break;
        case 'a': // 왼쪽 이동
            newX--;
            break;
        case 'd': // 오른쪽 이동
            newX++;
            break;
        }

        // 새로운 위치로 이동 가능한지 확인 후 이동
        if (userCanMove(newX, newY)) {
            // 이동 전 위치에는 도트로 표시
            clearPosition(x, y);
            x = newX;
            y = newY;
            // 이동 후 위치에는 캐릭터 표시
            map[y][x] = '#';
        }
    }
};

void drawMainMenu() {
    system("cls"); // 화면 지우기

    // ANSI 이스케이프 시퀀스를 사용하여 색상 및 스타일 변경
    cout << "\033[1;33m"; // 노란색 및 굵은 텍스트

    // 메뉴 헤더
    cout << "\033[1;36m"; // 청록색 및 굵은 텍스트
    cout << "┌───────────────────────────────┐" << endl;
    cout << "│           * JODOT *           │" << endl;
    cout << "└───────────────────────────────┘" << endl;

    // 텍스트 색상 및 스타일 변경
    cout << "\033[0m"; // 원래 색상 및 스타일

    // 메뉴 옵션
    cout << "\033[1;36m"; // 청록색 및 굵은 텍스트
    cout << "┌───────────────────────────────┐" << endl;
    cout << "│         1. 솔로 모드          │" << endl;
    cout << "│         2. pvp  모드          │" << endl;
    cout << "│         3. 랭킹 보기          │" << endl;
    cout << "│         4. 게임 종료          │" << endl;
    cout << "└───────────────────────────────┘" << endl;

    // 텍스트 색상 및 스타일 변경
    cout << "\033[0m"; // 원래 색상 및 스타일

    cout << "메뉴를 선택하세요: ";
}

void end() {
    system("cls"); // 화면 지우기

    // ANSI 이스케이프 시퀀스를 사용하여 색상 및 스타일 변경
    cout << "\033[1;33m"; // 노란색 및 굵은 텍스트

    // 메뉴 헤더
    cout << "\033[1;36m"; // 청록색 및 굵은 텍스트
    cout << "5초 후 자동 종료" << endl;
    cout << "┌───────────────────────────────┐" << endl;
    cout << "│         * GAME OVER *         │" << endl;
    cout << "└───────────────────────────────┘" << endl;

    // 텍스트 색상 및 스타일 변경
    cout << "\033[0m"; // 원래 색상 및 스타일

    // 메뉴 옵션
    cout << "\033[1;36m"; // 청록색 및 굵은 텍스트
    cout << "┌───────────────────────────────┐" << endl;
    cout << "          최종점수:" <<   score  << endl;
    cout << "└───────────────────────────────┘" << endl;
    // 텍스트 색상 및 스타일 변경
    cout << "\033[0m"; // 원래 색상 및 스타일

    sco.push_back(score);
}

void pvp1PEnd() {
    system("cls"); // 화면 지우기

    // ANSI 이스케이프 시퀀스를 사용하여 색상 및 스타일 변경
    cout << "\033[1;33m"; // 노란색 및 굵은 텍스트

    // 메뉴 헤더
    cout << "\033[1;36m"; // 청록색 및 굵은 텍스트
    cout << "5초 후 자동 종료" << endl;
    cout << "┌───────────────────────────────┐" << endl;
    cout << "│         * GAME OVER *         │" << endl;
    cout << "└───────────────────────────────┘" << endl;

    // 텍스트 색상 및 스타일 변경
    cout << "\033[0m"; // 원래 색상 및 스타일

    // 메뉴 옵션
    cout << "\033[1;36m"; // 청록색 및 굵은 텍스트
    cout << "┌───────────────────────────────┐" << endl;
    cout << "           승자 : 유저1 "<< endl;
    cout << "└───────────────────────────────┘" << endl;
    // 텍스트 색상 및 스타일 변경
    cout << "\033[0m"; // 원래 색상 및 스타일

}

void pvp2PEnd() {
    system("cls"); // 화면 지우기

    // ANSI 이스케이프 시퀀스를 사용하여 색상 및 스타일 변경
    cout << "\033[1;33m"; // 노란색 및 굵은 텍스트

    // 메뉴 헤더
    cout << "\033[1;36m"; // 청록색 및 굵은 텍스트
    cout << "5초 후 자동 종료" << endl;
    cout << "┌───────────────────────────────┐" << endl;
    cout << "│         * GAME OVER *         │" << endl;
    cout << "└───────────────────────────────┘" << endl;

    // 텍스트 색상 및 스타일 변경
    cout << "\033[0m"; // 원래 색상 및 스타일

    // 메뉴 옵션
    cout << "\033[1;36m"; // 청록색 및 굵은 텍스트
    cout << "┌───────────────────────────────┐" << endl;
    cout << "           승자 : 유저2 " << endl;
    cout << "└───────────────────────────────┘" << endl;
    // 텍스트 색상 및 스타일 변경
    cout << "\033[0m"; // 원래 색상 및 스타일

}

// 내림차순으로 정렬하는 비교 함수
bool descendingOrder(int a, int b) {
    return a > b; // a가 b보다 크면 true를 반환하여 내림차순으로 정렬
}

void sort() {
    std::sort(sco.begin(), sco.end(), descendingOrder);
    for (int i = 0; i < sco.size(); i++) {
        cout << "  " << i + 1 << "등│ 점수 │ " << sco[i] << endl;
    }
}

void ranked() {
    system("cls"); // 화면 지우기

    // ANSI 이스케이프 시퀀스를 사용하여 색상 및 스타일 변경
    cout << "\033[1;33m"; // 노란색 및 굵은 텍스트

    // 메뉴 헤더
    cout << "\033[1;36m"; // 청록색 및 굵은 텍스트
    cout << "10초 후 자동 종료 됩니다." << endl;
    cout << "┌───────────────────────────────┐" << endl;
    cout << "│           * RANK *            │" << endl;
    cout << "└───────────────────────────────┘" << endl;

    // 텍스트 색상 및 스타일 변경
    cout << "\033[0m"; // 원래 색상 및 스타일

    // 메뉴 옵션
    cout << "\033[1;36m"; // 청록색 및 굵은 텍스트
    cout << "┌───────────────────────────────┐" << endl;
    sort();
    cout << "└───────────────────────────────┘" << endl;
    Sleep(10000);
}

void trap() {
    for (int i = 0; i < 10; i++) {
    Map map;
    int trapX = rand() % (MAP_WIDTH - 2) + 1; // 가장자리를 피해서 랜덤 좌표 생성
    int trapY = rand() % (MAP_HEIGHT - 2) + 1;
    trapsX.push_back(trapX);
    trapsY.push_back(trapY);
    map.placeObstacle(trapX, trapY);
}
}

int main(){
    Map map;
    PvPMap pmap;
    trap();

    int playerX = 12; // 캐릭터 초기 위치
    int playerY = 12;
    int playerX1 = 13; // 캐릭터 초기 위치
    int playerY1 = 13;
    int playerX2 = 12; // 캐릭터 초기 위치
    int playerY2 = 12;
    score = 0;

    bool isGameRunning = false; // 게임 진행 여부 플래그
    bool isPvPRunning = false;
    while (true) {
        drawMainMenu(); // 메인 메뉴 그리기

        char choice;
        cin >> choice;

        switch (choice) {
        case '1': // 게임 시작
            isGameRunning = true;
            system("cls"); // 화면 지우기
            break;
        case '2':
            isPvPRunning = true;
            system("cls");
            break;
        case '3':
            ranked();
            break;
        case '4': // 게임 종료
            return 0;
        default:
            cout << "잘못된 입력입니다. 다시 입력하세요." << endl;
            continue;
        }


        if (isPvPRunning) {
            while (isPvPRunning) {
                cout << "유저1 = wasd = @, 유저2 = 방향키 = #" << endl;
                pmap.printMap();

                for (int i = 0; i < trapsX.size(); i++) {
                    pmap.clearPosition(trapsX[i], trapsY[i]); // 이동하기 전 위치 지움
                    int direction = rand() % 4; // 무작위 이동 방향 결정
                    switch (direction) {
                    case 0: // 위쪽
                        if (pmap.canMove(trapsX[i], trapsY[i] - 1))
                            trapsY[i]--;
                        break;
                    case 1: // 아래쪽
                        if (pmap.canMove(trapsX[i], trapsY[i] + 1))
                            trapsY[i]++;
                        break;
                    case 2: // 왼쪽
                        if (pmap.canMove(trapsX[i] - 1, trapsY[i]))
                            trapsX[i]--;
                        break;
                    case 3: // 오른쪽
                        if (pmap.canMove(trapsX[i] + 1, trapsY[i]))
                            trapsX[i]++;
                        break;
                    }
                    pmap.placeObstacle(trapsX[i], trapsY[i]);

                    // 캐릭터와 장애물 충돌 감지
                    if (playerX1 == trapsX[i] && playerY1 == trapsY[i]) {
                        isPvPRunning = false; // 게임 종료 플래그 설정
                        system("cls"); // 화면 지우기
                        pmap.clearPosition(trapsX[i], trapsY[i]);
                        playerX1 = 13;
                        playerY1 = 13;
                        playerX2 = 12;
                        playerY2 = 12;
                        pvp2PEnd();
                        Sleep(5000);
                    }

                    else if (playerX2 == trapsX[i] && playerY2 == trapsY[i]) {
                        isPvPRunning = false; // 게임 종료 플래그 설정
                        system("cls"); // 화면 지우기
                        pmap.clearPosition(trapsX[i], trapsY[i]);
                        playerX1 = 13;
                        playerY1 = 13;
                        playerX2 = 12;
                        playerY2 = 12;
                        pvp1PEnd();
                        Sleep(5000);
                    }
                }
                // 키 입력 처리
                if (GetAsyncKeyState('W') & 0x8000) {
                    pmap.p1moveCharacter(playerX1, playerY1, 'w');
                }
                if (GetAsyncKeyState('S') & 0x8000) {
                    pmap.p1moveCharacter(playerX1, playerY1, 's');
                }
                if (GetAsyncKeyState('A') & 0x8000) {
                    pmap.p1moveCharacter(playerX1, playerY1, 'a');
                }
                if (GetAsyncKeyState('D') & 0x8000) {
                    pmap.p1moveCharacter(playerX1, playerY1, 'd');
                }

                // 키 입력 처리
                if (GetAsyncKeyState(VK_UP) & 0x8000) {
                    pmap.p2moveCharacter(playerX2, playerY2, 'w');
                }
                if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
                    pmap.p2moveCharacter(playerX2, playerY2, 's');
                }
                if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
                    pmap.p2moveCharacter(playerX2, playerY2, 'a');
                }
                if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
                    pmap.p2moveCharacter(playerX2, playerY2, 'd');
                }
                Sleep(20);
            }
        }


        if (isGameRunning) {
            // 게임 시작 시간 기록
            DWORD startTime = GetTickCount();
            playerX = 12;
            playerY = 12;

            while (isGameRunning) {
                map.printMap();
                // 맵 출력
                for(int i = 0; i < trapsX.size(); i++){
                map.clearPosition(trapsX[i], trapsY[i]); // 이동하기 전 위치 지움
                int direction = rand() % 4; // 무작위 이동 방향 결정
                switch (direction) {
                case 0: // 위쪽
                    if (map.canMove(trapsX[i], trapsY[i] - 1))
                        trapsY[i]--;
                    break;
                case 1: // 아래쪽
                    if (map.canMove(trapsX[i], trapsY[i] + 1))
                        trapsY[i]++;
                    break;
                case 2: // 왼쪽
                    if (map.canMove(trapsX[i] - 1, trapsY[i]))
                        trapsX[i]--;
                    break;
                case 3: // 오른쪽
                    if (map.canMove(trapsX[i] + 1, trapsY[i]))
                        trapsX[i]++;
                    break; 
                }
                map.placeObstacle(trapsX[i], trapsY[i]);

                        // 캐릭터와 장애물 충돌 감지
        if (playerX == trapsX[i] && playerY == trapsY[i]) {
            isGameRunning = false; // 게임 종료 플래그 설정
            system("cls"); // 화면 지우기
            playerX = 12;
            playerY = 12;
            map.clearPosition(trapsX[i], trapsY[i]);
            end();
            Sleep(5000);
        }
      }
                // 키 입력 처리
                if (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState('W') & 0x8000) {
                    map.moveCharacter(playerX, playerY, 'w');
                }
                if (GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState('S') & 0x8000) {
                    map.moveCharacter(playerX, playerY, 's');
                }
                if (GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('A') & 0x8000) {
                    map.moveCharacter(playerX, playerY, 'a');
                }
                if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000) {
                    map.moveCharacter(playerX, playerY, 'd');
                }

                //// 게임 진행 시간 계산 (초 단위)
                DWORD elapsedTime = (GetTickCount() - startTime) / 1000;

                //// 일정 주기마다 점수를 증가시킴 (예: 1초에 1000점씩)
                score = elapsedTime * 1000;

                //cout << "세로" << yy << "가로" << xx << "점수" << score << endl;
                //cout << "가로" << xx << endl;
                //cout << "점수" << score << endl;

                //// 점수, 좌표 출력
                printf("세로: %d \n가로: %d \n점수: %d  ", playerY, playerX, score);

                //// 짧은 시간 동안 대기
                Sleep(20);
            }

            // trap 스레드 종료 대기
            //invoke.join();
            // 게임 종료 후 점수 계산
            score = (GetTickCount() - startTime) / 1000 * 1000;
            cout << "Game Over! Score: " << score << endl;
        }
    }

    return 0;
}
