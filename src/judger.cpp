#include "judger.h"

#include <iostream>

#include "json.h"

Judger::MODE Judger::JUDGER_MODE = Judger::MODE::COMMAND_LINE;

Judger::~Judger() {
    if (m_pBoard != nullptr) delete m_pBoard;
    if (m_pCore != nullptr) delete m_pCore;
}

void Judger::initGameByJSON() {
    std::string str;
    getline(std::cin, str);
    Json::Reader reader;
    Json::Value input;
    reader.parse(str, input);

    Board::PIECE_COLOR coreColor = Board::PIECE_COLOR::WHITE;
    if (input["requests"][0u]["x"].asInt() == -1 &&
        input["requests"][0u]["y"].asInt() == -1) {
        coreColor = Board::PIECE_COLOR::BLACK;
    }
    Board::PIECE_COLOR opponentColor = static_cast<Board::PIECE_COLOR>(coreColor ^ 1);

    int turnID = input["responses"].size();

    if (coreColor == Board::PIECE_COLOR::WHITE) {
        for (int i = 0; i < turnID; i++) {
            m_pBoard->placeAt(input["requests"][i]["x"].asInt(),
                              input["requests"][i]["y"].asInt(), opponentColor);
            m_pBoard->placeAt(input["responses"][i]["x"].asInt(),
                              input["responses"][i]["y"].asInt(), coreColor);
        }
        m_pBoard->placeAt(input["requests"][turnID]["x"].asInt(),
                          input["requests"][turnID]["y"].asInt(), opponentColor);
    } else {
        m_pBoard->placeAt(input["responses"][0u]["x"].asInt(),
                          input["responses"][0u]["y"].asInt(), coreColor);
        for (int i = 1; i < turnID; i++) {
            m_pBoard->placeAt(input["requests"][i]["x"].asInt(),
                              input["requests"][i]["y"].asInt(), opponentColor);
            m_pBoard->placeAt(input["responses"][i]["x"].asInt(),
                              input["responses"][i]["y"].asInt(), coreColor);
        }
        m_pBoard->placeAt(input["requests"][turnID]["x"].asInt(),
                          input["requests"][turnID]["y"].asInt(), opponentColor);
    }

    m_pCore = new Core(m_pBoard, coreColor);
}

void Judger::printCoreMoveByJSON() {
    Json::Value ret;
    MoveGenerator::Move best = m_pCore->bestMove();
    ret["response"]["x"] = best.x;
    ret["response"]["y"] = best.y;
    Json::FastWriter writer;
    std::cout << writer.write(ret) << std::endl;
}

void Judger::startGame() {
    if (m_pBoard != nullptr) delete m_pBoard;
    m_pBoard = new Board();
    if (m_pCore != nullptr) delete m_pCore;

    if (Judger::JUDGER_MODE == MODE::COMMAND_LINE) {
        int color = -1;
        while (color != Board::PIECE_COLOR::WHITE && color != Board::PIECE_COLOR::BLACK) {
            std::cout << "Choose your color (white/black):\n";
            std::string input;
            std::cin >> input;
            if (input == "white") color = Board::PIECE_COLOR::WHITE;
            if (input == "black") color = Board::PIECE_COLOR::BLACK;
        }

        Board::PIECE_COLOR playerColor = static_cast<Board::PIECE_COLOR>(color);
        Board::PIECE_COLOR coreColor = static_cast<Board::PIECE_COLOR>(color ^ 1);

        m_pCore = new Core(m_pBoard, coreColor);

        m_pBoard->display();
        for (int i = 0, hand = Board::PIECE_COLOR::BLACK;
             i < Board::BOARD_SIZE * Board::BOARD_SIZE; i++, hand ^= 1) {
            if (hand == coreColor) {
                m_pCore->initTimer();
                int tm = m_pCore->run();
                std::cout << "Run time: " << tm << "ms\n";
                std::cout << "Best score: " << m_pCore->bestScore() << std::endl;
                MoveGenerator::Move best = m_pCore->bestMove();
                std::cout << "Core drop position: " << best.x << " " << best.y
                          << std::endl;
                if (m_pBoard->getState(best.x, best.y) == Board::BOARD_STATE::INVALID) {
                    std::cout << "Invalid move. Abort.\n";
                    return;
                }
                m_pCore->makeMove(best.x, best.y, coreColor);
                m_pBoard->display();
                if (checkFiveAt(best.x, best.y, coreColor)) {
                    std::cout << "Your loose.\n";
                    return;
                }
                continue;
            }

            if (hand == playerColor) {
                int x = -1, y = -1;
                std::cout << "Your drop position: ";
                std::cin >> x >> y;
                while (m_pBoard->getState(x, y) != Board::BOARD_STATE::UNPLACE) {
                    std::cout << "Invalid drop position.\n";
                    std::cout << "Your drop position: ";
                    std::cin >> x >> y;
                }
                m_pCore->makeMove(x, y, playerColor);
                m_pBoard->display();
                if (checkFiveAt(x, y, playerColor)) {
                    std::cout << "Your win.\n";
                    return;
                }
                continue;
            }
        }
        std::cout << "Draw.\n";
        return;
    }

    if (Judger::JUDGER_MODE == MODE::ONLINE_JUDGE) {
        initGameByJSON();
        m_pCore->run();
        printCoreMoveByJSON();
        return;
    }
}

bool Judger::checkFiveAt(int x, int y, Board::PIECE_COLOR color) {
    if (m_pBoard->getState(x, y) != color) return false;

    for (int k = 0; k < 4; k++) {
        int cnt = 0;
        int ti = x, tj = y;
        for (int s = 1; s <= 4; s++) {
            ti += Board::dr[k];
            tj += Board::dc[k];
            if (ti < 0 || ti >= Board::BOARD_SIZE || tj < 0 || tj >= Board::BOARD_SIZE)
                break;
            if (m_pBoard->getState(ti, tj) != color) break;
            cnt++;
        }
        if (cnt >= 4) return true;
        ti = x, tj = y;
        for (int s = 1; s <= 4; s++) {
            ti -= Board::dr[k];
            tj -= Board::dc[k];
            if (ti < 0 || ti >= Board::BOARD_SIZE || tj < 0 || tj >= Board::BOARD_SIZE)
                break;
            if (m_pBoard->getState(ti, tj) != color) break;
            cnt++;
            if (cnt >= 4) return true;
        }
    }
    return false;
}