#include<iostream>
#include"windows.h"
using namespace std;
#define N 12
HANDLE m_S_Empty;//互斥，生产者Semaphore
HANDLE m_S_Full;//同步，消费者Semaphore
HANDLE m_M_Mutex;//互斥
int buf[N];//资源共享区

DWORD WINAPI Producer(LPVOID param);//生产者线程
DWORD WINAPI Consumer(LPVOID param);//消费者线程
void showBuf();//显示共享区资源详情

int main() {
    //初始化
    m_S_Empty = CreateSemaphore(NULL, N, N, NULL);
    m_S_Full = CreateSemaphore(NULL, 0, N, NULL);
    m_M_Mutex = CreateSemaphore(NULL, 1, 1, NULL);
    //新建N个生产者以及消费者线程
    for (int n = 0; n < N; ++n) {
        CreateThread(NULL, 0, Consumer, NULL, 0, NULL);
        CreateThread(NULL, 0, Producer, NULL, 0, NULL);
    }
    //主进程睡眠一段时间
    Sleep(100);
    return 0;
}

DWORD WINAPI Consumer(LPVOID param) {
    while (true) {
        WaitForSingleObject(m_S_Full, INFINITE);
        WaitForSingleObject(m_M_Mutex, INFINITE);

        for (int i = 0; i < N; ++i)
        {
            if (buf[i] == 1)
            {
                buf[i] = 0;//取出
                cout << "Consumer " << GetCurrentThreadId() << "消费成功" << endl;
                showBuf();
                break;
            }
        }
        ReleaseSemaphore(m_M_Mutex, 1, NULL);
        ReleaseSemaphore(m_S_Empty, 1, NULL);
    }
}

DWORD WINAPI Producer(LPVOID param) {
    while (true) {
        WaitForSingleObject(m_S_Empty, INFINITE);
        WaitForSingleObject(m_M_Mutex, INFINITE);

        for (int i = 0; i < N; ++i)
        {
            if (buf[i] == 0)
            {
                buf[i] = 1;//放置
                cout << "Producer " << GetCurrentThreadId() << "生产成功" << endl;
                showBuf();
                break;
            }
        }
        ReleaseSemaphore(m_M_Mutex, 1, NULL);
        ReleaseSemaphore(m_S_Full, 1, NULL);
    }
}

void showBuf() {
    cout << "资源共享区情况：";
    for (int j = 0; j < N; ++j) {
        cout << buf[j] << " ";
    }
    cout << endl;
}