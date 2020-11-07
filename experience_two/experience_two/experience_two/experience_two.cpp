#include<iostream>
#include"windows.h"
using namespace std;
#define N 12
HANDLE m_S_Empty;//���⣬������Semaphore
HANDLE m_S_Full;//ͬ����������Semaphore
HANDLE m_M_Mutex;//����
int buf[N];//��Դ������

DWORD WINAPI Producer(LPVOID param);//�������߳�
DWORD WINAPI Consumer(LPVOID param);//�������߳�
void showBuf();//��ʾ��������Դ����

int main() {
    //��ʼ��
    m_S_Empty = CreateSemaphore(NULL, N, N, NULL);
    m_S_Full = CreateSemaphore(NULL, 0, N, NULL);
    m_M_Mutex = CreateSemaphore(NULL, 1, 1, NULL);
    //�½�N���������Լ��������߳�
    for (int n = 0; n < N; ++n) {
        CreateThread(NULL, 0, Consumer, NULL, 0, NULL);
        CreateThread(NULL, 0, Producer, NULL, 0, NULL);
    }
    //������˯��һ��ʱ��
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
                buf[i] = 0;//ȡ��
                cout << "Consumer " << GetCurrentThreadId() << "���ѳɹ�" << endl;
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
                buf[i] = 1;//����
                cout << "Producer " << GetCurrentThreadId() << "�����ɹ�" << endl;
                showBuf();
                break;
            }
        }
        ReleaseSemaphore(m_M_Mutex, 1, NULL);
        ReleaseSemaphore(m_S_Full, 1, NULL);
    }
}

void showBuf() {
    cout << "��Դ�����������";
    for (int j = 0; j < N; ++j) {
        cout << buf[j] << " ";
    }
    cout << endl;
}