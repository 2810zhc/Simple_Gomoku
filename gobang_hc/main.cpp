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
    // ��ʾ�ڿ����
    std::cout << "*****************************\n";
    std::cout << "*       ��������Ϸ         *\n";
    std::cout << "* 1. ��Ҷ�ս              *\n";
    std::cout << "* 2. �˻���ս              *\n";
    std::cout << "* 3. �����Ķ�              *\n";
    std::cout << "* 4. �˳�                  *\n";
    std::cout << "*****************************\n";
    std::cout << "��ѡ����Ϸģʽ(1-4): ";
}

int main() {
    int mode = 0;  // ���ڴ洢ѡ�����Ϸģʽ

    while (true) {
        // ��ʾ��Ϸģʽѡ��˵�
        showMainMenu();
        std::cin >> mode;

        std::cout << "��ѡ���� ";

        if (mode == 1) {
            std::cout << "��Ҷ�սģʽ��\n";
            Board board;
            Player player1("Player1", 'X'), player2("Player2", 'O');
            Player* currentPlayer = &player1;

            // ����ÿ��15���ʱ�����ƺ�30���ӵ��ܱ���ʱ��
            Timer timer(15, 30 * 60);
            timer.startGameTimer();

            int moveWarnings = 0;  // ÿһ����ʱ�޾������
            int countThree = 0;
            int countFour = 0;


            while (!board.isFull() && !timer.isGameTimeUp()) {
                board.print();
                //boardForbidden.print();
                std::cout << currentPlayer->getName() << " (" << currentPlayer->getStone() << ") ����: ";

                // ������ʱ��
                timer.startMoveTimer();

                // ʹ��һ������ֵ�����������״̬
                std::atomic<bool> inputReceived(false);
                int x, y;

                // ����һ���߳����������룬ģ��ʵʱ���
                std::thread inputThread([&]() {
                    std::cin >> x >> y;
                    inputReceived = true;
                    });

                // ���̼߳�ؼ�ʱ
                while (!inputReceived && !timer.isMoveTimeUp()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // �ȴ����벢��س�ʱ
                }

                // ������볬ʱ�����������
                if (timer.isMoveTimeUp()) {
                    std::cout << "\n";
                    std::cout << "��ʱ��δ����15����������롣\n";
                    moveWarnings++;
                    if (moveWarnings >= 3) {
                        std::cout << currentPlayer->getName() << " ����3��ʱ�޾��棬�и���\n";
                        break;
                    }
                }

                // ֹͣ��ʱ�����ȴ������߳̽���
                timer.stop();
                if (inputThread.joinable()) {
                    inputThread.join();
                }

                // ������������
                if (!board.placeStone(x, y, currentPlayer->getStone())) {
                    std::cout << "��Чλ�ã����������롣\n";
                    continue;
                }

                if (Rule::isForbiddenMove(board, x, y, currentPlayer->getStone(), countThree, countFour)) {
                    std::cout << currentPlayer->getName() << " ���֣��и���\n";
                    break;
                }


                if (Rule::checkWin(board, x, y, currentPlayer->getStone())) {
                    std::cout << currentPlayer->getName() << " ��ʤ��\n";
                    break;
                }

                // �����Ϸ��ʱ��
                if (timer.isGameTimeUp()) {
                    std::cout << "����ʱ���ѵ�����Ϸ������\n";
                    break;
                }

                // �л����
                currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;

                // ÿ���һ�����������Ӽ�ʱ��
                timer.resetMoveTimer();
            }

            if (!timer.isGameTimeUp()) {
                std::cout << "��Ϸʱ��: " << timer.getElapsedSeconds() << " ��\n";
            }
            else {
                // �ж��Ƿ��ʱ��
                if (moveWarnings < 3) {
                    std::cout << "����ʱ���ѽ������Ƿ������ʱ����(y/n): ";
                    char choice;
                    std::cin >> choice;

                    if (choice == 'y' || choice == 'Y') {
                        std::cout << "�������ʱ������(5��10): ";
                        int extraTime;
                        std::cin >> extraTime;
                        timer.resetGameTimer();
                        timer.startGameTimer(); // ���ò�������Ϸ��ʱ��

                        // ���������ʱ��
                        std::cout << "��ʱ����ʼ��\n";
                    }
                }
            }
        }
        else if (mode == 2) {
            std::cout << "�˻���սģʽ��\n";
            Board board;
            Player player1("Player1", 'X');
            Robot robot('O');  // ���������˶��󣬻�������'O'����
            Player* currentPlayer = &player1;

            // ����ÿ��15���ʱ�����ƺ�30���ӵ��ܱ���ʱ��
            Timer timer(15, 30 * 60);
            timer.startGameTimer();

            int moveWarnings = 0;  // ÿһ����ʱ�޾������
            int countThree = 0;
            int countFour = 0;
            int x, y;
            while (!board.isFull() && !timer.isGameTimeUp()) {
                std::cout << "\n";
                board.print();
                if (currentPlayer == &robot) {
                    //std::cout << "\n";
                    std::cout << currentPlayer->getName() << " (" << currentPlayer->getStone() << ") ����: ";
                }
                else {
                    std::cout << currentPlayer->getName() << " (" << currentPlayer->getStone() << ") ����: ";
                }

                // ������ʱ��
                timer.startMoveTimer();

                // ʹ��һ������ֵ�����������״̬
                std::atomic<bool> inputReceived(false);
                

                // �������
                if (currentPlayer == &player1) {
                    std::thread inputThread([&]() {
                        std::cin >> x >> y;
                        inputReceived = true;

                        });

                    // ���̼߳�ؼ�ʱ
                    while (!inputReceived && !timer.isMoveTimeUp()) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // �ȴ����벢��س�ʱ
                    }
                    
                    // ������볬ʱ�����������
                    if (timer.isMoveTimeUp()) {
                        std::cout << "\n";
                        std::cout << "��ʱ��δ����15����������롣\n";
                        moveWarnings++;
                        if (moveWarnings >= 3) {
                            std::cout << currentPlayer->getName() << " ����3��ʱ�޾��棬�и���\n";
                            break;
                        }
                    }

                    // ֹͣ��ʱ�����ȴ������߳̽���
                    timer.stop();
                    if (inputThread.joinable()) {
                        inputThread.join();
                    }

                    // ������������
                    if (!board.placeStone(x, y, currentPlayer->getStone())) {
                        std::cout << "��Чλ�ã����������롣\n";
                        continue;
                    }
                }
                // ����������
                else if (currentPlayer == &robot) {
                    std::tie(x, y)= robot.makeMove(board);  // �û�����ѡ������
                }

                // �жϽ���
                if (Rule::isForbiddenMove(board, x, y, currentPlayer->getStone(), countThree, countFour)) {
                    std::cout << currentPlayer->getName() << " ���֣��и���\n";
                    break;
                }

                // �ж��Ƿ�ʤ��
                if (Rule::checkWin(board, x, y, currentPlayer->getStone())) {
                    std::cout << currentPlayer->getName() << " ��ʤ��\n";
                    break;
                }

                // �����Ϸ��ʱ��
                if (timer.isGameTimeUp()) {
                    std::cout << "����ʱ���ѵ�����Ϸ������\n";
                    break;
                }

                // �л����
                currentPlayer = (currentPlayer == &player1) ? &robot : &player1;

                // ÿ���һ�����������Ӽ�ʱ��
                timer.resetMoveTimer();
            }

            if (!timer.isGameTimeUp()) {
                std::cout << "��Ϸʱ��: " << timer.getElapsedSeconds() << " ��\n";
            }
        }


        else if (mode == 3) {
            Readme readme;
            readme.displayRules();  // ��ʾ����
        }
        else if (mode == 4) {
            std::cout << "�˳���Ϸ��\n";
            break;  // �˳���Ϸ
        }
        else {
            std::cout << "��Чѡ��������ѡ��\n";
        }
    }

    return 0;
}
