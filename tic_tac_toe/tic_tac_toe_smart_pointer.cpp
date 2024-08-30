#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <chrono>

// Enum class for symbol types
enum class SymbolType {
    X, O, A, B
};

// Function to convert SymbolType to character
inline char symbolToChar(SymbolType symbol) {
    switch (symbol) {
        case SymbolType::X: return 'X';
        case SymbolType::O: return 'O';
        case SymbolType::A: return 'A';
        case SymbolType::B: return 'B';
        default: return ' ';
    }
}

// Base class for player symbols
class PlayerSymbol {
protected:
    SymbolType type;

public:
    PlayerSymbol(SymbolType symbolType) : type(symbolType) {}

    virtual ~PlayerSymbol() = default;

    SymbolType getSymbolType() const {
        return type;
    }

    virtual char getSymbolChar() const {
        return symbolToChar(type);
    }

    virtual std::string getName() const = 0;
};

// Derived classes for specific symbols
class XSymbol : public PlayerSymbol {
public:
    XSymbol() : PlayerSymbol(SymbolType::X) {}

    std::string getName() const override {
        return std::string(1, symbolToChar(getSymbolType()));
    }
};

class OSymbol : public PlayerSymbol {
public:
    OSymbol() : PlayerSymbol(SymbolType::O) {}

    std::string getName() const override {
        return std::string(1, symbolToChar(getSymbolType()));
    }
};

// Additional symbols can be added similarly
class Board {
private:
    int size;
    std::vector<std::vector<char>> grid;

public:
    Board(int n) : size(n), grid(n, std::vector<char>(n, ' ')) {}

    void displayBoard() const;
    bool makeMove(int row, int col, const PlayerSymbol& symbol);
    char getCell(int row, int col) const { return grid[row][col]; }
    int getSize() const { return size; }
};

void Board::displayBoard() const {
    std::cout << "  ";
    for (int i = 0; i < size; ++i) {
        std::cout << i << "   ";
    }
    std::cout << "\n";

    for (int i = 0; i < size; ++i) {
        std::cout << i << " ";
        for (int j = 0; j < size; ++j) {
            std::cout << grid[i][j];
            if (j < size - 1) std::cout << " | ";
        }
        std::cout << "\n";
        if (i < size - 1) {
            std::cout << "  ";
            for (int k = 0; k < size; ++k) {
                std::cout << "---";
                if (k < size - 1) std::cout << "|";
            }
            std::cout << "\n";
        }
    }
}

bool Board::makeMove(int row, int col, const PlayerSymbol& symbol) {
    if (row >= 0 && row < size && col >= 0 && col < size && grid[row][col] == ' ') {
        grid[row][col] = symbol.getSymbolChar();
        return true;
    }
    return false;
}

class Player {
private:
    std::shared_ptr<PlayerSymbol> symbol;
    std::string name;

public:
    Player(std::shared_ptr<PlayerSymbol> sym, const std::string& playerName)
        : symbol(sym), name(playerName) {}

    std::shared_ptr<PlayerSymbol> getSymbol() const {
        return symbol;
    }

    std::string getName() const {
        return name;
    }
};
class Game {
private:
    Board board;
    std::vector<std::shared_ptr<Player>> players;
    int currentPlayerIndex;
    std::chrono::steady_clock::time_point startTime;

public:
    Game(const std::vector<std::shared_ptr<PlayerSymbol>>& symbols, const std::vector<std::string>& playerNames, int boardSize);

    void switchPlayer();
    void play();
    bool checkWin(const std::shared_ptr<PlayerSymbol>& symbol) const;
    bool checkDraw() const;
    bool isValidMove(int row, int col) const;
    void printWinningTime() const;
};

Game::Game(const std::vector<std::shared_ptr<PlayerSymbol>>& symbols, const std::vector<std::string>& playerNames, int boardSize)
    : board(boardSize), currentPlayerIndex(0), startTime(std::chrono::steady_clock::now()) {
    for (size_t i = 0; i < symbols.size(); ++i) {
        players.push_back(std::make_shared<Player>(symbols[i], playerNames[i]));
    }
}

void Game::switchPlayer() {
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}

bool Game::checkWin(const std::shared_ptr<PlayerSymbol>& symbol) const {
    char sym = symbol->getSymbolChar();
    int size = board.getSize();

    for (int i = 0; i < size; ++i) {
        bool rowWin = true;
        bool colWin = true;
        for (int j = 0; j < size; ++j) {
            if (board.getCell(i, j) != sym) rowWin = false;
            if (board.getCell(j, i) != sym) colWin = false;
        }
        if (rowWin || colWin) return true;
    }

    bool diag1Win = true;
    bool diag2Win = true;
    for (int i = 0; i < size; ++i) {
        if (board.getCell(i, i) != sym) diag1Win = false;
        if (board.getCell(i, size - 1 - i) != sym) diag2Win = false;
    }
    if (diag1Win || diag2Win) return true;

    return false;
}

bool Game::checkDraw() const {
    int size = board.getSize();

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (board.getCell(i, j) == ' ') return false;
        }
    }
    return true;
}

bool Game::isValidMove(int row, int col) const {
    return (row >= 0 && row < board.getSize() && col >= 0 && col < board.getSize() && board.getCell(row, col) == ' ');
}

void Game::printWinningTime() const {
    auto endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    std::cout << "Game Duration: " << duration.count() << " seconds\n";
}

void Game::play() {
    int row, col;
    while (true) {
        board.displayBoard();
        std::cout << "Player " << players[currentPlayerIndex]->getName()
                  << " (" << players[currentPlayerIndex]->getSymbol()->getName()
                  << "), enter your move (row and column): ";
        std::cin >> row >> col;

        if (isValidMove(row, col)) {
            board.makeMove(row, col, *players[currentPlayerIndex]->getSymbol());
            if (checkWin(players[currentPlayerIndex]->getSymbol())) {
                board.displayBoard();
                std::cout << "Player " << players[currentPlayerIndex]->getName()
                          << " (" << players[currentPlayerIndex]->getSymbol()->getName()
                          << ") wins!\n";
                printWinningTime();
                break;
            } else if (checkDraw()) {
                board.displayBoard();
                std::cout << "It's a draw!\n";
                printWinningTime();
                break;
            }
            switchPlayer();
        } else {
            std::cout << "Invalid move. Try again.\n";
        }
    }
}
int main() {
    int boardSize;
    std::cout << "Enter the board size (n): ";
    std::cin >> boardSize;

    // Initialize game with multiple players with different symbols and names
    std::vector<std::shared_ptr<PlayerSymbol>> symbols = {
        std::make_shared<XSymbol>(), 
        std::make_shared<OSymbol>(), 
        // Add more symbols if needed
    };

    std::vector<std::string> playerNames;
    for (size_t i = 0; i < symbols.size(); ++i) {
        std::string name;
        std::cout << "Enter name for player with symbol " << symbols[i]->getName() << ": ";
        std::cin >> name;
        playerNames.push_back(name);
    }
    Game game(symbols, playerNames, boardSize);
    game.play();

    return 0;
}