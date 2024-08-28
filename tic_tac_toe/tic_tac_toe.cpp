#include<iostream>
#include<queue>
using namespace std;

enum PieceType {X,O,T,U};

class PlayingPiece{
    private:
        PieceType piece;
    public:
        PlayingPiece(PieceType piece){
            this->piece=piece;
        }

};

class PlayingPieceX:public PlayingPiece{

    public:
        PlayingPieceX():PlayingPiece(PieceType::X){
        }
};

class PlayingPieceO:public PlayingPiece{

    public:
        PlayingPieceO():PlayingPiece(PieceType::O){
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
class Player{
    private:
        string name;
        char playingsymbol;
    public:
        Player(string name, char symbol){
            this->name=name;
            this->playingsymbol=symbol;
        }

        string getName(){
            return this->name;
        }

        void setName(string name){
            this->name=name;
        }

        char getPlayingSymbol(){
            return this->playingsymbol;
        }

        void setPlayingSymbol(char symbol){
            this->playingsymbol=symbol;
        }

};

class Game{
    private:
        Board board;
        queue<Player> player_list;
    public:
        Game(){
            
        }
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
            char symbol =p.getPlayingSymbol();
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
                cout<<p.getName()<<" "<<p.getPlayingSymbol()<<"\n";
                board.write_move(row,col,p.getPlayingSymbol());
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
    int s=3;
    cout<<"please choose size of the game:";
    cin>>s;

    Game game;
    

    string p1,p2;
    char s1, s2;
    cout<<"please Enter First player symbol and player name:";
    cin>>s1>>p1;
    cout<<"please Enter Second player symbol and player name:";
    cin>>s2>>p2;
    p1="nitish";
    p2="Gaurav";
    s1='X';
    s2='O';
    cout<<s1<<" "<<p1<<" "<<s2<<" "<<p2<<"\n";
    Player player1(p1,s1);
    Player player2(p2,s2);
    game.addPlayer(player1);
    game.addPlayer(player2);
    game.initialize(s);
    game.start();

    return 0;
}