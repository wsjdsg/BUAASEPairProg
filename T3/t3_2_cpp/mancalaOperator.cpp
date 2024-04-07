#include <iostream>
#include <cstdlib>
#include <vector> 
#include <cstdio>
#include <algorithm>
using namespace std;
extern "C"{
class Result {
public:
    int index;
    int score;
    int* status;

    Result(int index) {
        this->index = index;
        this->score = 0;
    }
};

int EPOCH = 2;
int BRANCH = 4;

bool cmp(const Result& r1, const Result& r2) {
    if (r1.score > r2.score) {
        return true;
    } else if (r1.score == r2.score && r1.index > r2.index) {
        return true;
    } else {
        return false;
    }
}

inline bool is_end(int* status) {
    bool flag = true;
    for (int i = 0; i <= 5; i++) {
        if (status[i] != 0) {
            flag = false;
        }
    }
    if (flag) return true;
    flag = true;
    for (int i = 7; i <= 12; i++) {
        if (status[i] != 0) {
            flag = false;
        }
    }
    return flag;
}

void output(int* status) {
	printf("\n");
	printf("%d  %d  %d  %d  %d  %d  %d  \n", status[6], status[5], status[4], status[3], status[2], status[1], status[0]);
	printf("   %d  %d  %d  %d  %d  %d  %d\n\n", status[7], status[8], status[9], status[10], status[11], status[12], status[13]);
}

inline int getindex(int player, int pos) {
	if (player == 1) return pos;
	else return pos + 7;
}

bool seed(int index, int* status) {
    int num = status[index];
    status[index] = 0;
    int skip = (index >= 0 && index <= 5) ? 13 : 6;
    int again = (index >= 0 && index <= 5) ? 6 : 13;
    int player = (index >= 0 && index <= 5) ? 1 : 2;
    while (num > 0) {
        index++;
        if (index == skip) continue;
        if (index == 14) index = 0;
        status[index]++;
        num--;
    }
    if (index == again) {
    	return true;
    }
    if (status[index] == 1 && status[12-index] != 0 && 
		((player == 1 && index <= 5 && index >= 0) || (player == 2 && index <= 12 && index >= 7))) {
        status[again] += status[index];
        status[again] += status[12-index];
        status[index] = 0;
        status[12-index] = 0;
    }

    return false;
}

int choose(int flag, int index, int* status, int** final_end_status) {
    int my_hole = (index >= 0 && index <= 5) ? 6 : 13;
    int start = status[my_hole];
    int score = 0;
    int* temp_status = new int[14];
    memcpy(temp_status, status, sizeof(int[14]));

    if (seed(index, temp_status) && !is_end(temp_status)) {
        int* max_status;
        int max = 0;
        for (int i = 0; i < 6; i++) {
            int next_index = getindex(flag, i);
            if (temp_status[next_index] != 0) {
                int next_score = choose(flag, next_index, temp_status, &max_status);
                if (next_score >= max) {
                    max = next_score;
                    *final_end_status = max_status;
                }
            }
        }
        score = temp_status[my_hole] - start + max;
    } else {
        score = temp_status[my_hole] - start;
        *final_end_status = temp_status;
    }

    return score;
}

bool equals(int* s) {
	return s[0] == 0 && s[1] == 0 && s[2] == 0 && s[3] == 0 && s[4] == 2 &&
		   s[5] == 0 && s[6] == 10 && s[7] == 1 && s[8] == 0 && s[9] == 1 &&
		   s[10] == 0 && s[11] == 0 && s[12] == 5 && s[13] == 29;
} 

int play(int flag, int* status, int epoch, int* final_choice) {
    vector<Result> results;
    for (int i = 0; i < 6; i++) {
        int index = getindex(flag, i);
        if (status[index] != 0) {
            Result* result = new Result(index);
            int score = choose(flag, index, status, &(result->status));
            //printf("???%d\n", result->status[0]);
            result->score = score;
            //printf("%d ", score);
            results.push_back(*result);
        }
    }
    sort(results.begin(), results.end(), cmp);
    int i = 0;
    int score = -10000;
    if (epoch < EPOCH) {
        while (i < min((int)results.size(), BRANCH)) {
            vector<Result>::iterator p_result = results.begin();
            p_result += i;
            int temp_score = 0;
            int temp_chioce = 0;
            if (!is_end(p_result->status)) {
            	if (i == 2 && epoch == 1) {
            		int n = 0;
            	}
            	if (equals(p_result->status)) {
            		int n = 0;
            	}
                temp_score = play(3-flag, p_result->status, epoch+1, &temp_chioce);
                if (p_result->score - temp_score > score) {
                	score = p_result->score - temp_score;
                	*final_choice = p_result->index;
            	}
            }
            else {
            	score = (results.begin())->score;
        		*final_choice = (results.begin())->index;
            }
            i++;
        }
    } else {
        score = (results.begin())->score;
        *final_choice = (results.begin())->index;
    }
    return score;
}

int mancala_operator(int flag, int* status) {
    int choice = 0;
    play(flag, status, 1, &choice);
    if (choice <= 5 && choice >= 0) return choice + 11;
    else return choice + 14;
}

int main()
{
	int seq[14] = {0,0,0,0,0,1,23,   0,0,0,0,0,1,23};
	printf("%d", mancala_operator(2, seq));
    return 0;
}
}