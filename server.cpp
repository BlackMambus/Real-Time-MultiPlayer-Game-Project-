#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <cstdlib>
#include <ctime>

using boost::asio::ip::tcp;

int main() {
    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1234));

    std::cout << "Server started. Waiting for 2 players...\n";

    tcp::socket player1(io_context);
    tcp::socket player2(io_context);
    acceptor.accept(player1);
    std::cout << "Player 1 connected.\n";
    acceptor.accept(player2);
    std::cout << "Player 2 connected.\n";

    srand(time(0));
    int secret = rand() % 100 + 1;
    std::cout << "Secret number: " << secret << "\n";

    tcp::socket* players[2] = { &player1, &player2 };
    int turn = 0;

    while (true) {
        boost::asio::write(*players[turn], boost::asio::buffer("Your turn\n"));

        char data[128];
        size_t len = players[turn]->read_some(boost::asio::buffer(data));
        int guess = std::stoi(std::string(data, len));

        std::string response;
        if (guess < secret) response = "Too low\n";
        else if (guess > secret) response = "Too high\n";
        else {
            response = "Correct! You win!\n";
            boost::asio::write(*players[turn], boost::asio::buffer(response));
            boost::asio::write(*players[1 - turn], boost::asio::buffer("You lose!\n"));
            break;
        }

        boost::asio::write(*players[turn], boost::asio::buffer(response));
        turn = 1 - turn;
    }

    return 0;
}



