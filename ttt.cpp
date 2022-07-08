#include <iostream>
#include <vector>
#include <map>
#include <random>
using namespace std;

int HUMAN=-1;
int AI=1;
vector<vector<int>> board(3,vector<int>(3,0));

bool wins(vector<vector<int>> state,int player){
    for(int i=0;i<3;i++){
        if(state[i][0]==state[i][1]&&state[i][1]==state[i][2]&&state[i][2]==player) return true;
        if(state[0][i]==state[1][i]&&state[1][i]==state[2][i]&&state[2][i]==player) return true;
    }
    if(state[0][0]==state[1][1]&&state[1][1]==state[2][2]&&state[2][2]==player) return true;
    if(state[2][0]==state[1][1]&&state[1][1]==state[0][2]&&state[0][2]==player) return true;
    return false;
}

int evaluate(vector<vector<int>> state){
    int score=0;
    if(wins(state,AI)) score=1;
    else if(wins(state,HUMAN)) score=-1;
    return score;
}

bool game_over(vector<vector<int>> state){
    return wins(state,AI) || wins(state,HUMAN);
}

vector<vector<int>> empty_cells(vector<vector<int>> &state){
    vector<vector<int>> cells;
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            if (state[i][j] == 0){
                vector<int> tp={i,j};
                cells.push_back(tp);
            }
        }
    }
    return cells;
}

bool valid_move(int x, int y){
    vector<vector<int>> empty=empty_cells(board);
    for(int i=0;i<empty.size();i++){
        if(empty[i][0]==x&&empty[i][1]==y) return true;
    }
    return false;
}

bool set_move(int x, int y, int player){
    if(valid_move(x,y)){
        board[x][y]=player;
        return true;
    }
    else return false;
}
    
vector<int> minimax(vector<vector<int>> state, int depth, int player){
    vector<int> best={-1,-1,INT_MAX};
    if(player==AI) best[2]=INT_MIN;
    if(depth==0||game_over(state)){
        best[2]=evaluate(state);
        return best;
    }
    vector<vector<int>> empty=empty_cells(state);
    vector<int> score;
    for(int i=0;i<empty.size();i++){
        state[empty[i][0]][empty[i][1]]=player;
        score=minimax(state,depth-1,-player);
        state[empty[i][0]][empty[i][1]]=0;
        score[0]=empty[i][0];
        score[1]=empty[i][1];
        if(player==AI){
            if(score[2]>best[2]) best=score;
        }
        else{
            if(score[2]<best[2]) best=score;
        }
    }
    return best;
}

void render(vector<vector<int>> state,char cchoice,char hchoice){
    map<int,char> m;
    m[-1]=hchoice;
    m[1]=cchoice;
    m[0]=' ';
    cout<<"---------------"<<endl;
    for(int i=0;i<state.size();i++){
        for(int j=0;j<state[i].size();j++){
            string tmp="| ";
            tmp+=m[state[i][j]];
            tmp.append(" |");
            cout<<tmp;
        }
        cout<<"\n---------------"<<endl;
    }
}

void ai_turn(char cchoice, char hchoice){
    int depth=empty_cells(board).size();
    if(depth==0||game_over(board)) return;

    cout<<"\n\nAI turn: "<<cchoice<<endl;
    render(board, cchoice, hchoice);
    int x,y;
    if(depth==9){
        random_device rd;
        mt19937 mt(rd());
        uniform_real_distribution<double> dist(0, 2);
        x=(int)dist(mt);
        y=(int)dist(mt);
        
    }else{
        vector<int> tm=minimax(board,depth,AI);
        x=tm[0];
        y=tm[1];
    }
    set_move(x,y,AI);
}

void human_turn(char cchoice, char hchoice){
    int depth=empty_cells(board).size();
    if(depth==0||game_over(board)) return;
    int move=-1;
    map<int,vector<int>> m;
    m={
        {1,{0, 0}}, {2,{0, 1}}, {3,{0, 2}},
        {4,{1, 0}}, {5,{1, 1}}, {6,{1, 2}},
        {7,{2, 0}}, {8,{2, 1}}, {9,{2, 2}},
    };
    cout<<"\n\nHuman turn: "<<hchoice<<endl;
    render(board, cchoice, hchoice);
    while(move<1||move>9){
        cout<<"Input 1-9"<<endl;
        cin>>move;
        if(!(move>0&&move<10)){
            move=-1;
            cout <<"Illegal input\n"<<endl;
            cin.clear();
            cin.ignore();
            continue;
        }
        bool flag=set_move(m[move][0],m[move][1],HUMAN);
        if(flag==false){
            cout<<"Invalid move\n"<<endl;
            move=-1;
        }
    }
}
    
    
int main(){
    char hchoice,cchoice,first;

    while(hchoice!='O'&&hchoice!='X'){
        cout<<"Select X or O"<<endl;
        cin>>hchoice;
    }
    if(hchoice=='X') cchoice='O';
    else cchoice='X';

    while(first!='Y'&&first!='N'){
        cout<<"First to start? [Y/N]"<<endl;
        cin>>first;   
    }
    while(empty_cells(board).size()>0&&game_over(board)==false){
        if(first=='N'){
            ai_turn(cchoice,hchoice);
            first=' ';
        }
        human_turn(cchoice,hchoice);
        ai_turn(cchoice,hchoice);
    }
    cout<<game_over(board)<<endl;
    if(wins(board,HUMAN)){
        cout<<"Human turn: "<<hchoice<<endl;
        render(board,cchoice,hchoice);
        cout<<"You win"<<endl;
    }else if(wins(board,AI)){
        cout<<"AI turn: "<<hchoice<<endl;
        render(board,cchoice,hchoice);
        cout<<"You lose"<<endl;
    }else{
        render(board,cchoice,hchoice);
        cout<<"Draw"<<endl;
        }
    do {
     cout << '\n' << "Press the e key to exit.";
    } while (cin.get() != 'e');
    return 0;
}
