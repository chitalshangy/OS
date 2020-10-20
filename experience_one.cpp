#include<iostream>
#include<windows.h>
using namespace std;
int main(){
    STARTUPINFO startInfo;
    PROCESS_INFORMATION proInfo;

    memset(&startInfo,0,sizeof(startInfo));
    startInfo.cb=sizeof(startInfo);
    //唤醒子进程
    bool newProcess=CreateProcess("text.exe",
        NULL,NULL,NULL,false,NULL,NULL,NULL,&startInfo,&proInfo);
        
    //中断子进程
    //TerminateProcess(proInfo.hProcess,0);
    if(newProcess){
        cout<<"create Process success!!!"<<endl;
        //system("pause");
    }
    else {
        cerr<<"create Process failed!!!"<<endl;
    }
    //cout<<"terminate childprocess"<<endl;
    Sleep(1000);
    cout<<"main process end"<<endl;
    return 0;
}