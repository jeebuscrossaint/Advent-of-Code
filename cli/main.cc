#include "aocli.hh"

int main() {
    AoCCLI cli;
    cli.setSession("your_session_token_here");

    try {
        // Get input for current day
        cli.getInput();

        // Or specify day and year
        cli.getInput(1, 2023);
    } catch (const AoCCLI::AoCException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
