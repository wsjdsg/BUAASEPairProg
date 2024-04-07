#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector> 
#include <cstdio>
using namespace std;

int error_rate = 10;
int end_rate = 10;

int chessbord[3][8];

inline int get_rand(int min, int max) {
    return ( rand() % (max - min + 1) ) + min ;
}

inline bool is_end() {
    bool flag = true;
    for (int i = 1; i < 7; i++) {
        if (chessbord[1][i] != 0) {
            flag = false;
        }
    }
    if (flag) return true;
    flag = true;
    for (int i = 1; i < 7; i++) {
        if (chessbord[2][i] != 0) {
            flag = false;
        }
    }
    return flag;
}

int get_rand_place_right(int player) {
    if (is_end()) return 0;
    vector<int> places;
    for (int i = 1; i < 7; i++) {
        if (chessbord[player][i] > 0) places.push_back(i);
    }
    int place = get_rand(0, places.size() - 1);
    return player * 10 + places[place];
}

int get_rand_place_wrong(int player) {
    if (is_end()) return 0;
    vector<int> places;
    for (int i = 1; i < 7; i++) {
        if (chessbord[player][i] <= 0) places.push_back(i);
    }
    places.push_back(0);
    places.push_back(7);
    places.push_back(8);
    places.push_back(9);
    int place = get_rand(0, places.size() - 1);
    return player * 10 + places[place];
}

void init() {
    for (int i = 1; i < 8; i++) {
        chessbord[1][i] = 4;
        chessbord[2][i] = 4;
    }
    chessbord[1][7] = 0;
    chessbord[2][7] = 0;
    return ;
}

bool seed(int pos) {
    int player = pos / 10;
    int hole = pos % 10;
    int num = chessbord[player][hole];
    chessbord[player][hole] = 0;
    int seed_player = player;
    while (num--) {
        hole++;
        if (hole == 8) {
            hole = 0;
            num++;
            seed_player = 3 - seed_player;
        } else if (hole == 7) {
            if (seed_player == player) chessbord[seed_player][hole]++;
            else num++;
        } else {
            chessbord[seed_player][hole]++;
        }
    }
    if (seed_player == player && hole == 7) return true;
    if (seed_player == player && chessbord[seed_player][hole] == 1 && chessbord[3-seed_player][7-hole] != 0) {
        chessbord[seed_player][7] += chessbord[seed_player][hole];
        chessbord[seed_player][7] += chessbord[3-seed_player][7-hole];
        chessbord[seed_player][hole] = 0;
        chessbord[3-seed_player][7-hole] = 0;
    }
    return false;
}

void output(FILE* cases) {
	fprintf(cases, "%d  ", chessbord[1][7]);
	for (int i = 6; i >= 1; i--) {
		fprintf(cases, "%d  ", chessbord[1][i]);
	}
	fprintf(cases, "\n   ");
	for (int i = 1; i < 7; i++) {
		fprintf(cases, "%d  ", chessbord[2][i]);
	}
	fprintf(cases, "%d", chessbord[2][7]);
	fprintf(cases, "\n\n");
}

void total() {
	for (int i = 1; i < 7; i++) {
		chessbord[1][7] += chessbord[1][i];
		chessbord[1][i] = 0;
		chessbord[2][7] += chessbord[2][i];
		chessbord[2][i] = 0;
	}
}

int main()
{
	srand(time(0));
	FILE* in = fopen("in.txt", "w");
    FILE* ans = fopen("std.txt", "w");
	FILE* cases = fopen("cases.txt", "w");
	 
    int testcase_num = 10000;
    
	int now_player = 0;
	bool _continue = false;

	while (testcase_num--) {
        init();
        vector<int> seq;
        int pos = 0;
        int code = 0;
        now_player = get_rand(1, 2);
        int first_player = now_player;
        fprintf(cases, "\n\n<<<<<<<<<<<<<<<<<< %d >>>>>>>>>>>>>>>>>>\n\n", 100 - testcase_num);
        int round = 0;
        while (true) {
            if (get_rand(0, 1000) < error_rate) {
                if (_continue) {
                    if (get_rand(0, 1) == 0) {
                        pos = get_rand_place_wrong(3-now_player);
                    } else {
                        pos = get_rand_place_right(3-now_player);
                    }
                } else {
                    pos = get_rand_place_wrong(now_player);
                }
                seq.push_back(pos);
                code = 30000 + seq.size() - 1;
                break;
            } else if (get_rand(0, 1000) < end_rate) {
                code = 20000 + chessbord[first_player][7];
                break;
            } else {
                pos = get_rand_place_right(now_player);
                seq.push_back(pos);
            }
			
            _continue = seed(pos);
            fprintf(cases, "<%d>. %d\n", round, pos);
            output(cases);
            
            if (is_end()) {
                if (get_rand(0, 1000) < error_rate) {
                    if (get_rand(0, 1) == 0) seq.push_back(get_rand(11, 16));
                    else seq.push_back(get_rand(21, 26));
                    code = 30000 + seq.size() - 1;
                } else {
                    total();
                    code = 15000 + chessbord[first_player][7] - chessbord[3-first_player][7];
                }
                break;
            }
            if (!_continue) now_player = 3 - now_player;
            round++;
	    }
        fprintf(in, "%d %d ", first_player, seq.size());
        for (vector<int>::iterator i = seq.begin(); i != seq.end(); i++) {
            fprintf(in, "%d ", *i);
        }
        fprintf(in, "\n");
        fprintf(ans, "%d\n", code);
    }
	
	return 0;
} 
