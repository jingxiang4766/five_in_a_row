#include <iostream>
#include <climits>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <set>
#include <math.h>
#include <chrono>
using namespace std;

int n = 11;
int o3 = 200;
int c3 = 50;
int five = 50000;
int o4 = 20000;
int c4 = 195;
int tt = 49;
int edge1 = 250; //99
int edge2 = 51;  //40
int edge3 = 11;  //11

int deduct = 0;
int o3_ = 0;
int c3_ = 0;
int five_ = 0;
int o4_ = 0;
int c4_ = 0;
int o2_ = 0;
int c2_ = 0;

long long int score(vector<vector<char> > board, char who, pair<int,int> upperleft, pair<int,int>bottomright);

int reduce(int i, int j){
    int ans = 0;
    if(i == 0 || i == n-1) ans += edge1;
    if(i == 1 || i == n-2) ans += edge2;
    if(i == 2 || i == n-3) ans += edge3;
    if(j == 0 || j == n-1) ans += edge1;
    if(j == 1 || j == n-2) ans += edge2;
    if(j == 2 || j == n-3) ans += edge3;
    return ans;
}

void count(int counter, bool block, bool gap){
    if(counter == 2 && block){
        c2_++;
        if(gap) deduct += 1;
    }
    if(counter == 2 && !block){
        o2_++;
        if(gap) deduct += 2;
    }
    if(counter == 3 && block){
        c3_++;
        if(gap) deduct += 20;
    }
    if(counter == 3 && !block){
        o3_++;
        if(gap) deduct += 40;
    }
    if(counter == 4 && block){
        c4_++;
        if(gap) deduct += 41;
    }
    if(counter == 4 && !block){
        o4_++;
        if(gap) deduct += 7600;
    }
    if(counter >= 5 && !gap) five_++;
    if(counter >= 5 && gap) c4_++;
}

void clean(){
    c2_ = 0;
    o2_ = 0;
    c3_ = 0;
    o3_ = 0;
    c4_ = 0;
    o4_ = 0;
    five_ = 0;
    deduct = 0;
}

pair<int,int> mtop(string s){
    int first = s[0] - 'a';
    int second = stoi(s.substr(1))-1;
    return make_pair(second, first);
}

string ptom(pair<int,int> p){
    string ans = "";
    ans += p.second + 'a';
    ans += to_string(p.first+1);
    return ans;
}

void move(vector<vector<char> >& board, pair<int,int> p, char who){
    board[p.first][p.second] = who;
}

pair<long long int, pair<int,int>> dfs(vector<vector<char> > board, pair<int,int> upperleft, pair<int,int>bottomright, int d){
    vector<pair<long long int, pair<int,int>>> ans;
    // base case
    if(d == 1){
        bool lastMove = true;
        for(int i = upperleft.first; i <= bottomright.first; i++){
            for(int j = upperleft.second; j <= bottomright.second; j++){
                if(board[i][j] == '0'){
                    lastMove = false;
                    board[i][j] = '2';
                    // int s = score(board, '1', upperleft, bottomright) - score(board, '2', upperleft, bottomright);
                    long long int s = -1 * score(board, '2', upperleft, bottomright);
                    ans.push_back(make_pair(s, make_pair(i,j)));
                    board[i][j] = '0';
                }
            }
        }
        if(lastMove) return make_pair(1, make_pair(1,1));
        int minimum = INT_MAX;
        int index = 0;
        for(int i = 0; i < ans.size(); i++){
            if(ans[i].first < minimum){
                index = i;
                minimum = ans[i].first;
            }
        }
        return ans[index];
    }

    // inductive case
    for(int i = upperleft.first; i <= bottomright.first; i++){
        for(int j = upperleft.second; j <= bottomright.second; j++){
            if(d % 2 == 0){
                if(board[i][j] == '0'){
                    board[i][j] = '1';
                    auto value = dfs(board, upperleft, bottomright, d-1);
                    value.first += d*score(board, '1', upperleft, bottomright);
		    value.first -= 2 * (abs(n/2-i) + abs(n/2-j));
                    value.first -= reduce(i, j);
		    value.second = make_pair(i,j);
                    ans.push_back(value);
                    board[i][j] = '0';
                }
            }
            else if(d % 2 == 1){
                if(board[i][j] == '0'){
                    board[i][j] = '2';
                    auto value = dfs(board, upperleft, bottomright, d-1);
                    value.first -= d*score(board, '2', upperleft, bottomright);
                    value.second = make_pair(i,j);
                    ans.push_back(value);
                    board[i][j] = '0';
                }
	    } 
            // cout << i << " " << j << " " << ans[ans.size()-1].first << endl;
        }
    }

    long long int minimum = ans[0].first;
    long long int maximum = ans[0].first;
    int index = 0;
    for(int i = 0; i < ans.size(); i++){
        if(d % 2 == 0 && ans[i].first > maximum){
            index = i;
            maximum = ans[i].first;
        }
        else if(d % 2 == 1 && ans[i].first < minimum){
            index = i;
            minimum = ans[i].first;
        }
    }
    // cout << ans[index].first << " " << ans[index].second.first << ans[index].second.second << endl;
    return ans[index];
}

long long int score(vector<vector<char> > board, char who, pair<int,int> upperleft, pair<int,int>bottomright){
    clean();
    long long int ans = 0;
    for(int i = upperleft.first; i <= bottomright.first; i++){
        for(int j = upperleft.second; j <= bottomright.second; j++){
            if(board[i][j] != who) continue;
            // horizontal
            int counter = 1;
            bool gap = false;
            bool block = false;
            if(j == 0 || (j > 0 && board[i][j-1] != '0')) block = true;
            for(int k = j+1; k < j+6 && k < n; k++){
                if(j > 0 && board[i][j-1] == who) break;
                if(j > 1 && board[i][j-1] == '0' && board[i][j-2] == who) break;
                if(k == n-1 && board[i][k] == who && block == true && counter < 4) counter = 1;
                if(k == n-1 && board[i][k] == who) block = true;
                if(board[i][k] == who) counter++;
                else if(board[i][k] == '0' && !gap && k != j+5) gap = true;
                else if(board[i][k] == '0' && gap) {
                    if(board[i][k-1] == '0') gap = false;
                    break;
                }
                else{
                    if(block && counter != 5) counter = 1;
                    if(board[i][k-1] != '0') block = true;
                    break;
                }
            }
            // if(counter == 2 && block) counter--;
            count(counter, block, gap);
            // vertical
            counter = 1;
            gap = false;
            block = false;
            if(i == 0 || (i > 0 && board[i-1][j] != '0')) block = true;
            for(int k = i+1; k < i+6 && k < n; k++){
                if(i > 0 && board[i-1][j] == who) break;
                if(i > 1 && board[i-1][j] == '0' && board[i-2][j] == who) break;
                if(k == n-1 && board[k][j] == who && block == true && counter < 4) counter = 1;
                if(k == n-1 && board[k][j] == who) block = true;
                if(board[k][j] == who) counter++;
                else if (board[k][j] == '0' && !gap && k != i+5) gap = true;
                else if (board[k][j] == '0' && gap) {
                    if(board[k-1][j] == '0') gap = false;
                    break;
                }

                else{
                    if(block && counter != 5) counter = 1;
                    if(board[k-1][j] != '0') block = true;
                    break;
                }
            }
            // if(counter == 2 && block) counter--;
            count(counter, block, gap);
            // bleft
            counter = 1;
            gap = false;
            block = false;
            if(i == 0 || j == n-1 || (i > 0 && j < n-1 && board[i-1][j+1] != '0')) block = true;
            for(int k = 1; k < 6; k++){
                if(i+k >= n || j-k < 0) break;
                if((i > 0 && j < n-1 && board[i-1][j+1] == who)) break;
                if((i > 1 && j < n-2 && board[i-1][j+1] == '0' && board[i-2][j+2] == who)) break;
                if((i+k == n-1 || j-k == 0) && board[i+k][j-k] == who && block == true && counter < 4) counter = 1;
                if((i+k == n-1 || j-k == 0) && board[i+k][j-k] == who) block = true;
                if(board[i+k][j-k] == who) counter++;
                else if(board[i+k][j-k] == '0' && !gap && k != 5) gap = true;
                else if(board[i+k][j-k] == '0' && gap) {
                    if(board[i+(k-1)][j-(k-1)] == '0') gap = false;
                    break;
                }
                else{
                    if(block && counter != 5) counter = 1;
                    if(board[i+(k-1)][j-(k-1)] != '0') block = true;
                    break;
                }
            }
            // if(counter == 2 && block) counter--;
            count(counter, block, gap);
            // bright
            counter = 1;
            gap = false;
            block = false;
            if(i == 0 || j == 0 || (i > 0 && j > 0 && board[i-1][j-1] != '0')) block = true;
            for(int k = 1; k < 6; k++){
                if(i+k >= n || j+k >= n) break;
                if(i > 0 && j > 0 && board[i-1][j-1] == who) break;
                if(i > 1 && j > 1 && board[i-1][j-1] == '0' && board[i-2][j-2] == who) break;
                if((i+k == n-1 || j+k == n-1) && board[i+k][j+k] == who && block == true && counter < 4) counter = 1;
                if((i+k == n-1 || j+k == n-1) && board[i+k][j+k] == who) block = true;
                if(board[i+k][j+k] == who) counter++;
                else if(board[i+k][j+k] == '0' && !gap && k != 5) gap = true;
                else if(board[i+k][j+k] == '0' && gap) {
                    if(board[i+k-1][j+k-1] == '0') gap = false;
                    break;
                }
                else{
                    if(block && counter != 5) counter = 1;
                    if(board[i+k-1][j+k-1] != '0') block = true;
                    break;
                }
            }
            // if(counter == 2 && block) counter--;
            count(counter, block, gap);
        }
    }
    if(o3_!= 0 && c4_ != 0) deduct -= 8000;
    else if(c4_ > 1) deduct -= 9500;
    return c2_*2 + o2_*10 + o2_/2*tt + o3_*o3 + o3_/2*1500 + c3_*c3 + o4_*o4 + c4_*c4 + five_*five - deduct;
}


vector<vector<char>> test { {'0','0','0','0','0','0','0','0','0','0'},
                            {'0','0','0','0','2','0','0','0','0','0'},
                            {'0','0','0','0','0','1','0','0','0','0'},
                            {'0','0','0','0','2','1','1','0','0','0'},
                            {'0','0','0','2','2','1','1','1','0','0'},
                            {'0','2','0','0','1','1','1','0','2','0'},
                            {'0','0','0','2','0','0','2','0','0','0'},
                            {'0','0','0','0','0','0','0','0','0','0'},
                            {'0','0','0','0','0','0','0','0','0','0'},
                            {'0','0','0','0','0','1','0','1','0','0'} };
void test_dfs(){
    n = 10;
    auto ul = make_pair(0,0);
    auto br = make_pair(9,9);

    auto next = dfs(test, ul, br, 2);
    cout << ptom(next.second) << endl;
}

void test_score(){
    n = 10;
    auto ul = make_pair(0,0);
    auto br = make_pair(9,9);

    cout << score(test, '1', ul, br) << " " << score(test, '2', ul, br) << endl;
    cout << score(test, '1', ul, br) - score(test, '2', ul, br) << endl;
}



int main(int argc,  char** argv){
    //speedup
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    chrono::steady_clock::time_point time_1 = std::chrono::steady_clock::now();
    chrono::steady_clock::time_point time_2 = std::chrono::steady_clock::now();
    // cout << chrono::duration_cast<std::chrono::seconds>(time_2 - time_1).count() << endl;

    bool myTurn = false;
    if(argc > 1 && argv[1][1] == 'n'){
        n = stoi(argv[2]);
    }
    else if(argc > 2 && argv[2][1] == 'n'){
        n = stoi(argv[3]);
    }
    if(argc > 1 && argv[1][1] == 'l'){
        myTurn = true;
    }
    else if(argc > 3 && argv[3][1] == 'l'){
        myTurn = true;
    }

    pair<int,int> upperleft, bottomright;
    vector<vector<char> > board(n, vector<char>(n, '0'));
    if(myTurn){
        upperleft = make_pair((n+1)/2, (n+1)/2);
        bottomright = upperleft;
        cout << "Move played: " << ptom(upperleft) << endl;
        move(board, upperleft, '1');
        myTurn = !myTurn;
    }
    else{
        string s = "";
        cin >> s;
        upperleft = mtop(s);
        while(upperleft.first < 0 || upperleft.first > n-1 || upperleft.second < 0 || upperleft.second > n-1){
            cout << "Invalid move" << endl;
            cin >> s;
            upperleft = mtop(s);
        }
        cout << "Move played: " << s << endl;
        bottomright = upperleft;
        move(board, upperleft, '2');
        myTurn = !myTurn;
    }
    upperleft.first = max(0, upperleft.first-1);
    upperleft.second = max(0, upperleft.second-1);
    bottomright.first = min(n-1, bottomright.first+1);
    bottomright.second = min(n-1, bottomright.second+1);

    // while(abs(score(board, '1', upperleft, bottomright)-score(board, '2', upperleft, bottomright)) < 50000){
    //     pair<int,int> next_move;
    //     if(myTurn){
    //         pair<long long int, pair<int,int>> pp = dfs(board, upperleft, bottomright, 2);
    //         cout << "Move played: " << ptom(pp.second) << endl;
    //         board[pp.second.first][pp.second.second] = '1';
    //         next_move = pp.second;
    //         myTurn = !myTurn;
    //     }
    //     else{
    //         string s = "";
    //         cin >> s;
    //         pair<int,int> p = mtop(s);
    //         while(board[p.first][p.second] != '0'){
    //             cout << "Invalid move" << endl;
    //             cin >> s;
    //             p = mtop(s);
    //         }
    //         cout << "Move played: " << s << endl;
    //         board[p.first][p.second] = '2';
    //         next_move = p;
    //         myTurn = !myTurn;
    //     }
    //     upperleft.first = max(0, min(upperleft.first, next_move.first-2));
    //     upperleft.second = max(0, min(upperleft.second, next_move.second-2));
    //     bottomright.first = min(n-1, max(bottomright.first, next_move.first+2));
    //     bottomright.second = min(n-1,max(bottomright.second, next_move.second+2));
    // }
    






    // test_dfs();
    test_score();
    return 0;
}
