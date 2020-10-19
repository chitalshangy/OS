#include<iostream>
#include<windows.h>
using namespace std;
/*bool CreateProcess(
    LPCTSTR lpApplicationName,
    LPTSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,  
    LPSECURITY_ATTRIBUTES lpThreadAttributes,  
    BOOL bInheritHandles,  
    DWORD dwCreationFlags,
    LPVOID lpEnvironment,  
    LPCTSTR lpCurrentDirectory,   
    LPSTARTUPINFO lpStartupInfo, 
    LPPROCESS_INFORMATION lpProcessInformation
);

VOID ExitProcess( UINT uExitCode );
BOOL TerminateProcess( HANDLE hProcess, UINT uExitCode );

typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess; //Handle to the newly created process.
    HANDLE hThread; //Handle to the primary thread of the newly created process.
    DWORD dwProcessId; //Value that can be used to identify a process.
    DWORD dwThreadId; //Value that can be used to identify a thread. 
} 
*/
int main(){
    STARTUPINFO startInfo;
    PROCESS_INFORMATION proInfo;

    memset(&startInfo,0,sizeof(startInfo));
    startInfo.cb=sizeof(startInfo);

    bool newProcess=CreateProcess("C:\\Users\\Chital\\Desktop\\text.exe",NULL,NULL,NULL,false,NULL,NULL,NULL,&startInfo,&proInfo);

    if(newProcess){
        cout<<"create Process success!!!"<<endl;
        TerminateProcess(proInfo.hProcess,0);
        cout<<"new Process exit"<<endl;
    }
    else {
        cerr<<"create Process failed!!!"<<endl;
    }
    //TerminateProcess(proInfo.hProcess,0);
    //ExitProcess(0);
    return 0;
}