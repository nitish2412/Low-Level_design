#include<iostream>
#include<queue>
#include <memory>
#include <string>
#include <chrono>
using namespace std;

enum class SymbolType {X,O,A,B};

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

        virtual string getName() const = 0;
};

// Derived classes for specific symbols
class XSymbol : public PlayerSymbol {
    public:
        XSymbol() : PlayerSymbol(SymbolType::X) {}

        string getName() const override {
            return string(1, symbolToChar(getSymbolType()));
        }
};

class OSymbol : public PlayerSymbol {
    public:
        OSymbol() : PlayerSymbol(SymbolType::O) {}

        string getName() const override {
            return std::string(1, symbolToChar(getSymbolType()));
        }
};

class Board{
    private:
        int size;
        vector< vector<char>> board;
    public:
        Board(){}
        vector<vector<char>> getBoard(){
            return this->board;
        }
        void setSize(int size){
            this->size=size;
        }
        int getSize(){
            return this->size;
        }
        void boardInitialize(){
            for(int i=0;i<this->size;i++){
                vector<char> tmp;
                for(int j=0;j<this->size;j++){
                    tmp.push_back('_');
                }
                board.push_back(tmp);
            }
        }
        void printBoard(){
            for(int i=0;i<this->size;i++){
                for(int j=0;j<this->size;j++){
                    cout<<board[i][j]<<" ";
                }
                cout<<"\n";
            }
        }
        bool valid_move(int row, int col){
            if(this->board[row-1][col-1]=='_')
                return true;
            return false;
        }
        void write_move(int row, int col, char symbol){
            this->board[row-1][col-1]=symbol;
        }
        bool getFreecell(){
            for(int i=0;i<this->size;i++){
                for(int j=0;j<this->size;j++){
                   if(this->board[i][j]=='_'){
                        return true;
                   }
                }
            }

            return false;
        }

};


class Player {
    private:
        PlayerSymbol *symbol;
        string name;

    public:
        Player(PlayerSymbol *sym, const string& playerName)
            : symbol(sym), name(playerName) {}

        PlayerSymbol* getSymbol() const {
            return symbol;
        }

        string getName() const {
            return name;
        }
};
class Game {
    private:
        Board board;
        queue<Player> player_list;

    public:
        Game(){}
        void initialize(int size){
            cout<<"*************Game initialization*****************\n";
            board.setSize(size);
            board.boardInitialize();
            board.printBoard();   
        }
        void addPlayer(Player p){
            player_list.push(p);
        }
        bool valid_input(int row,int col){
            int size=board.getSize();
            if(row<=0 || row > size || col<=0 || col>size){
                return false;
            }
            return board.valid_move(row,col);

        }
        bool winner_check(int row, int col, Player p){
            int size=board.getSize();
            //char symbol = symbolToChar(p.getSymbol()->getSymbolType());
            char symbol = p.getSymbol()->getSymbolChar();
            vector<vector<char>> temp_board=board.getBoard();
            bool row_match=true;
            bool col_match=true;
            bool diagonal_match=true;
            bool anti_diagonal_match=true;
            for(int i=0;i<size;i++){
                    if(temp_board[row-1][i]!=symbol){
                        row_match=false;
                    }  
            }
            for(int i=0;i<size;i++){
                if(temp_board[i][col-1]!=symbol){
                    col_match=false;
                }
            }
            for(int i=0;i<size;i++){
                if(temp_board[i][i]!=symbol){
                    diagonal_match=false;
                }
            }
            for(int i=0;i<size;i++){
                if(temp_board[i][size-i-1]!=symbol){
                    anti_diagonal_match=false;
                }
            }
            return row_match || col_match || diagonal_match || anti_diagonal_match;

        }

        void start(){
            bool winner=false;
            int row,col;
            int i=0;
            while(!winner){
                cin>>row>>col;
                cout<<row<<" "<<col<<"\n";
                if(valid_input(row,col)==false){
                    cout<<"Please enter a valid move\n";
                    continue;
                }
                Player p=player_list.front();
                player_list.pop();
                player_list.push(p);
                char symbol=p.getSymbol()->getSymbolChar();
                cout<<p.getName()<<" "<<symbol<<"\n";
                board.write_move(row,col,symbol);
                board.printBoard();
                if(winner_check(row,col,p)){
                    winner=true;
                    cout<<p.getName()<<" Won the Game.\n";
                    break;
                }
                if(board.getFreecell()==false){
                    winner=true;
                    cout<<" Match Draw. Game over.\n";
                }

            }

        }

        
};

int main(){
    cout<<"Hello World\n";

    int boardSize;
    cout << "Enter the board size (n): ";
    cin >> boardSize;

    PlayerSymbol *xsymbol=new XSymbol();
    PlayerSymbol *osymbol=new OSymbol();
    Player p1(xsymbol,"Nitish");
    Player p2(osymbol,"Nikki");
    cout<<p1.getName()<<" "<<symbolToChar(p1.getSymbol()->getSymbolType())<<"\n";
    cout<<p2.getName()<<" "<<symbolToChar(p2.getSymbol()->getSymbolType())<<"\n";
    Game game;
    game.addPlayer(p1);
    game.addPlayer(p2);
    game.initialize(boardSize);
    game.start();

    return 0;
}