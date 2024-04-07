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
    if(flag) return 1;
    flag=true;
    for(int i=7;i<13;i++) if(a[i]) {flag=false;break;}
    if(flag) return 2;
    return 0;
}
// int32_t EMSCRIPTEN_KEEPALIVE mancala_result(int32_t flag,int32_t* seq,int32_t size){
int32_t*  mancala_board(int32_t flag,int32_t* seq,int32_t size){
    int32_t* ans=(int32_t*)malloc(sizeof(int32_t)*20);
    int a[20]={};
    for(int i=0;i<6;i++) a[i]=a[i+7]=4;
    int p=0;
    bool wrong_state=false;
    while(p<size-1){
        int turn=seq[p]/10,sel=seq[p]%10;
        int pos=sel-1+(turn-1)*7;
        int x=a[pos];
        a[pos]=0;
        pos=(pos+1)%14;
        int opp_goal=(turn==1)?13:6;
        int my_goal=(turn==1)?6:13;
        while(x--){
            if(pos==opp_goal) {x++;pos=(pos+1)%14;continue;}
            a[pos]++;
            if(x==0){
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
    }//前size-1步不需要检查合法性
    //处理最后一步
    p=size-1;
    int turn=seq[p]/10,sel=seq[p]%10;
    int pos=sel-1+(turn-1)*7;
    if(turn!=flag){wrong_state=true;}
    else if(a[pos]==0){wrong_state=true;}
    else if(over(a)) {wrong_state=true;}
    else{
        int x=a[pos];
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
    }
    if(wrong_state){
        if(flag==1) a[14]=200+2*a[6]-48;
        else a[14]=200+48-2*a[13];
        for(int i=0;i<15;i++) ans[i]=a[i];
        return ans;
    }
    int t=over(a);
    if(t){
        int win_a=0,win_b=0;
        for(int i=0;i<7;i++) win_a+=a[i],win_b+=a[i+7];
        a[14]=200+win_a-win_b;
        for(int i=0;i<15;i++) ans[i]=a[i];
        return ans;
    }else{
        a[14]=flag;
        for(int i=0;i<15;i++) ans[i]=a[i];
        return ans;
    }
}
#ifdef __cplusplus
}
#endif
