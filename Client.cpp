#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
    boost::asio::io_context io_context;
    tcp::socket socket(io_context);
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1234));

    while (true) {
        char data[128];
        size_t len = socket.read_some(boost::asio::buffer(data));
        std::string message(data, len);
        std::cout << "Server: " << message;

        if (message.find("Your turn") != std::string::npos) {
            std::cout << "Enter your guess: ";
            std::string guess;
            std::getline(std::cin, guess);
            boost::asio::write(socket, boost::asio::buffer(guess));
        }

        if (message.find("win") != std::string::npos || message.find("lose") != std::string::npos)
            break;
    }

    return 0;
}
