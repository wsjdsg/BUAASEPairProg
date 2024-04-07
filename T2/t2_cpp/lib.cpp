#include <iostream>
#include <cstdlib>
// #include <emscripten/emscripten.h>
using namespace std;
#ifdef __cplusplus
extern "C" {
#endif
int over(int*a){
    bool flag=true;
    for(int i=0;i<6;i++) if(a[i]) {flag=false;break;}
    if(flag){
        for(int i=7;i<13;i++) a[13]+=a[i],a[i]=0;
        return 1;
    }
    flag=true;
    for(int i=7;i<13;i++) if(a[i]) {flag=false;break;}
    if(flag){
        for(int i=0;i<6;i++) a[6]+=a[i],a[i]=0;
        return 2;
    }
    return 0;
}
// int32_t EMSCRIPTEN_KEEPALIVE mancala_result(int32_t flag,int32_t* seq,int32_t size){
int32_t  mancala_result(int32_t flag,int32_t* seq,int32_t size){
    int a[20]={}; //0~5 A 6 A计分  7~12 B 13 B计分
    for(int i=0;i<6;i++) a[i]=a[i+7]=4;
    int p=0;
    bool wrong_state=false;
    int init_flag=flag;
    while(p<size){
        int turn=seq[p]/10,sel=seq[p]%10;
        if(turn!=flag){
            wrong_state=true;break;
        }
        int pos=sel-1+(turn-1)*7;
        int x=a[pos];
        if(x==0){
            wrong_state=true;break;
        }
        a[pos]=0;
        pos=(pos+1)%14;
        int opp_goal=(turn==1)?13:6;
        int my_goal=(turn==1)?6:13;
        while(x--){
            if(pos==opp_goal) {x++;pos=(pos+1)%14;continue;}
            a[pos]++;
            if(x==0){
                if(pos!=my_goal) flag=(flag==1)?2:1;//最后一个子儿
                if(a[pos]==1 && pos!=my_goal && pos>=7*(turn-1) && pos<=7*(turn-1)+5){
                    if(a[12-pos]){
                        a[my_goal]+=a[pos]+a[12-pos];
                        a[pos]=a[12-pos]=0;
                    }//对面有棋子
                }
            }
            pos=(pos+1)%14;
        }
        p++;
        if(over(a) && p<size) {wrong_state=true;break;}
    }
    if(wrong_state) return 30000+p;
    int t=over(a);
    if(t){
        return a[6+(init_flag-1)*7]-a[6+(2-init_flag)*7]+15000;
    }else{
        return a[6+(init_flag-1)*7]+20000;
    }
}
#ifdef __cplusplus
}
#endif
int main(){
    FILE* in = fopen("in.txt", "r");
    FILE* out = fopen("out.txt", "w");

    for (int i = 0; i < 1000; i++) {
        int player;
        int size;
        fscanf(in, "%d %d", &player, &size);
        int32_t seq[size];
        for (int j = 0; j < size; j++) {
            int n;
            fscanf(in, "%d", &n);
            seq[j] = n;
        }
        fprintf(out, "%d\n", mancala_result(player, seq, size));
    }
    return 0;
}
