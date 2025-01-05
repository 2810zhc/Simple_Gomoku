#include "Board.h"
#include "Player.h"
#include "Rule.h"
#include "Timer.h"
#include "Readme.h"
#include "Robot.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

void showMainMenu() {
    // 显示黑框界面
    std::cout << "*****************************\n";
    std::cout << "*       五子棋游戏         *\n";
    std::cout << "* 1. 玩家对战              *\n";
    std::cout << "* 2. 人机对战              *\n";
    std::cout << "* 3. 规则阅读              *\n";
    std::cout << "* 4. 退出                  *\n";
    std::cout << "*****************************\n";
    std::cout << "请选择游戏模式(1-4): ";
}

int main() {
    int mode = 0;  // 用于存储选择的游戏模式

    while (true) {
        // 显示游戏模式选择菜单
        showMainMenu();
        std::cin >> mode;

        std::cout << "您选择了 ";

        if (mode == 1) {
            std::cout << "玩家对战模式。\n";
            Board board;
            Player player1("Player1", 'X'), player2("Player2", 'O');
            Player* currentPlayer = &player1;

            // 设置每步15秒的时间限制和30分钟的总比赛时间
            Timer timer(15, 30 * 60);
            timer.startGameTimer();

            int moveWarnings = 0;  // 每一方的时限警告次数
            int countThree = 0;
            int countFour = 0;


            while (!board.isFull() && !timer.isGameTimeUp()) {
                board.print();
                //boardForbidden.print();
                std::cout << currentPlayer->getName() << " (" << currentPlayer->getStone() << ") 下棋: ";

                // 启动计时器
                timer.startMoveTimer();

                // 使用一个布尔值来控制输入的状态
                std::atomic<bool> inputReceived(false);
                int x, y;

                // 启动一个线程来进行输入，模拟实时监控
                std::thread inputThread([&]() {
                    std::cin >> x >> y;
                    inputReceived = true;
                    });

                // 主线程监控计时
                while (!inputReceived && !timer.isMoveTimeUp()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 等待输入并监控超时
                }

                // 如果输入超时，则结束输入
                if (timer.isMoveTimeUp()) {
                    std::cout << "\n";
                    std::cout << "超时！未能在15秒内完成输入。\n";
                    moveWarnings++;
                    if (moveWarnings >= 3) {
                        std::cout << currentPlayer->getName() << " 超过3次时限警告，判负！\n";
                        break;
                    }
                }

                // 停止计时器并等待输入线程结束
                timer.stop();
                if (inputThread.joinable()) {
                    inputThread.join();
                }

                // 继续进行落子
                if (!board.placeStone(x, y, currentPlayer->getStone())) {
                    std::cout << "无效位置，请重新输入。\n";
                    continue;
                }

                if (Rule::isForbiddenMove(board, x, y, currentPlayer->getStone(), countThree, countFour)) {
                    std::cout << currentPlayer->getName() << " 禁手，判负！\n";
                    break;
                }


                if (Rule::checkWin(board, x, y, currentPlayer->getStone())) {
                    std::cout << currentPlayer->getName() << " 获胜！\n";
                    break;
                }

                // 检查游戏总时限
                if (timer.isGameTimeUp()) {
                    std::cout << "比赛时间已到，游戏结束！\n";
                    break;
                }

                // 切换玩家
                currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;

                // 每完成一步后，重置落子计时器
                timer.resetMoveTimer();
            }

            if (!timer.isGameTimeUp()) {
                std::cout << "游戏时间: " << timer.getElapsedSeconds() << " 秒\n";
            }
            else {
                // 判断是否加时赛
                if (moveWarnings < 3) {
                    std::cout << "比赛时间已结束，是否申请加时赛？(y/n): ";
                    char choice;
                    std::cin >> choice;

                    if (choice == 'y' || choice == 'Y') {
                        std::cout << "请输入加时分钟数(5或10): ";
                        int extraTime;
                        std::cin >> extraTime;
                        timer.resetGameTimer();
                        timer.startGameTimer(); // 重置并启动游戏计时器

                        // 允许继续加时赛
                        std::cout << "加时赛开始！\n";
                    }
                }
            }
        }
        else if (mode == 2) {
            std::cout << "人机对战模式。\n";
            Board board;
            Player player1("Player1", 'X');
            Robot robot('O');  // 创建机器人对象，机器人用'O'棋子
            Player* currentPlayer = &player1;

            // 设置每步15秒的时间限制和30分钟的总比赛时间
            Timer timer(15, 30 * 60);
            timer.startGameTimer();

            int moveWarnings = 0;  // 每一方的时限警告次数
            int countThree = 0;
            int countFour = 0;
            int x, y;
            while (!board.isFull() && !timer.isGameTimeUp()) {
                std::cout << "\n";
                board.print();
                if (currentPlayer == &robot) {
                    //std::cout << "\n";
                    std::cout << currentPlayer->getName() << " (" << currentPlayer->getStone() << ") 下棋: ";
                }
                else {
                    std::cout << currentPlayer->getName() << " (" << currentPlayer->getStone() << ") 下棋: ";
                }

                // 启动计时器
                timer.startMoveTimer();

                // 使用一个布尔值来控制输入的状态
                std::atomic<bool> inputReceived(false);
                

                // 玩家输入
                if (currentPlayer == &player1) {
                    std::thread inputThread([&]() {
                        std::cin >> x >> y;
                        inputReceived = true;

                        });

                    // 主线程监控计时
                    while (!inputReceived && !timer.isMoveTimeUp()) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 等待输入并监控超时
                    }
                    
                    // 如果输入超时，则结束输入
                    if (timer.isMoveTimeUp()) {
                        std::cout << "\n";
                        std::cout << "超时！未能在15秒内完成输入。\n";
                        moveWarnings++;
                        if (moveWarnings >= 3) {
                            std::cout << currentPlayer->getName() << " 超过3次时限警告，判负！\n";
                            break;
                        }
                    }

                    // 停止计时器并等待输入线程结束
                    timer.stop();
                    if (inputThread.joinable()) {
                        inputThread.join();
                    }

                    // 继续进行落子
                    if (!board.placeStone(x, y, currentPlayer->getStone())) {
                        std::cout << "无效位置，请重新输入。\n";
                        continue;
                    }
                }
                // 机器人落子
                else if (currentPlayer == &robot) {
                    std::tie(x, y)= robot.makeMove(board);  // 让机器人选择落子
                }

                // 判断禁手
                if (Rule::isForbiddenMove(board, x, y, currentPlayer->getStone(), countThree, countFour)) {
                    std::cout << currentPlayer->getName() << " 禁手，判负！\n";
                    break;
                }

                // 判断是否胜利
                if (Rule::checkWin(board, x, y, currentPlayer->getStone())) {
                    std::cout << currentPlayer->getName() << " 获胜！\n";
                    break;
                }

                // 检查游戏总时限
                if (timer.isGameTimeUp()) {
                    std::cout << "比赛时间已到，游戏结束！\n";
                    break;
                }

                // 切换玩家
                currentPlayer = (currentPlayer == &player1) ? &robot : &player1;

                // 每完成一步后，重置落子计时器
                timer.resetMoveTimer();
            }

            if (!timer.isGameTimeUp()) {
                std::cout << "游戏时间: " << timer.getElapsedSeconds() << " 秒\n";
            }
        }


        else if (mode == 3) {
            Readme readme;
            readme.displayRules();  // 显示规则
        }
        else if (mode == 4) {
            std::cout << "退出游戏。\n";
            break;  // 退出游戏
        }
        else {
            std::cout << "无效选择，请重新选择。\n";
        }
    }

    return 0;
}
