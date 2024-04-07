#include <iostream>
#include <cstdlib>
#include <emscripten/emscripten.h>
using namespace std;
#define EXTERN extern "C"

EXTERN EMSCRIPTEN_KEEPALIVE int32_t bocchi_shut_up(int32_t flag,int32_t* seq,int32_t size){
    int a[10]={};
    if(flag==1){
        for(int i=0;i<size;i++) if((seq[i]/10)==1) a[seq[i]%10]++;
        int max_freq=0x80808080,val;
        for(int i=1;i<=6;i++){
            if(a[i]>max_freq) max_freq=a[i],val=i+10;
        } 
        int cnt=0;
        for(int i=1;i<=6;i++) if(a[i]==max_freq) cnt++;
        if(cnt>=2) return 10;
        else return val;
    }else if(flag==2){ 
        for(int i=0;i<size;i++) if((seq[i]/10)==2) a[seq[i]%10]++;
        int max_freq=0x80808080,val;
        for(int i=1;i<=6;i++){
            if(a[i]>max_freq) max_freq=a[i],val=i+20;
        } 
        int cnt=0;
        for(int i=1;i<=6;i++) if(a[i]==max_freq) cnt++;
        if(cnt>=2) return 10;
        else return val;
    }
    return -1;
}