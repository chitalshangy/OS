#include<iostream>
using namespace std;

//假设有process_NUM个进程，resource_NUM种资源
#define process_NUM 5
#define resource_NUM 4

//定义对象，用于存储信息
int Resource[resource_NUM];//每种资源总数目
int Available[resource_NUM];//每种资源当前数目
int Claim[process_NUM][resource_NUM];//Claim[i][j]表示进程i对资源j的最大需求量
int Alloc[process_NUM][resource_NUM];//Alloc[i][j]表示进程i已经被分配了资源j的数目
int Need[process_NUM][resource_NUM];//表示进程还需要对应资源数
int Finished[process_NUM];//表示进程是否结束
int Path[process_NUM];//表示一个安全序列
int Request[resource_NUM];//表示当前进程新提出的资源请求

//判断在接受下一个资源请求后是否安全
bool isSafe();
//输出安全序列
void out_Path();
//判断进程i请求新资源后当前是否安全
void P_request();

/*
请输入4类资源数目：
3 14 12 12
请分别输入5个进程所需要4类资源数目：
Process0:0 0 1 2
Process1:1 7 5 0
Process2:2 3 5 6
Process3:0 6 5 2
Process4:0 6 5 6
输入每个进程已经分配的各类资源数：
Process0:0 0 1 2
Process1:1 0 0 0
Process2:1 3 5 4
Process3:0 6 3 2
Process4:0 0 1 4

1

0 4 2 0


*/

int main() {
    //初始化
    cout << "请输入" << resource_NUM << "类资源数目：" << endl;
    for (int i = 0; i < resource_NUM; ++i) {
        cin>>Resource[i];
        Available[i] = Resource[i];
    }
    Claim[process_NUM][resource_NUM] = 0;
    Alloc[process_NUM][resource_NUM] = 0;
    Need[process_NUM][resource_NUM] = 0;
    Finished[process_NUM] = 0;
    Path[process_NUM] = 0;

    //输入信息
    cout << "请分别输入" << process_NUM << "个进程所需要" << resource_NUM << "类资源数目：" << endl;
    for (int i = 0; i < process_NUM; ++i) {
        cout << "Process" << i << ":";
        for (int j = 0; j < resource_NUM; ++j) {
            cin >> Claim[i][j];
        }
    }
    cout << "输入每个进程已经分配的各类资源数：" << endl;
    for (int i = 0; i < process_NUM; ++i) {
        cout << "Process" << i << ":";
        for (int j = 0; j < resource_NUM; ++j) {
            cin >> Alloc[i][j];
            Need[i][j] = Claim[i][j] - Alloc[i][j];
            if (Need[i][j] < 0) {
                cout << "当前进程请求资源失败，请检查并且重新输入资源数或结束程序！" << endl;
                j--;
                continue;
            }
            Available[j] -= Alloc[i][j];
        }
    }

    /*
    cout << "各个资源空闲数目：" << endl;
    for (int i = 0; i < resource_NUM; ++i) {
        cout << Available[i] << " ";
    }
    cout << endl;
    */
    if (isSafe()) {
        cout << "当前进程可安全执行！" << endl;
        out_Path();
    }
    else {
        cout << "当前进程不可安全执行！" << endl;
    }

    //进行新请求处理
    while (1) {
        P_request();
    }
    
	return 0;
}

bool isSafe() {
    //L标志并行进程安全执行路径
	int i=0, j=0, L=0;
	int Temp[resource_NUM];//工作中间表
   
    //将中间表设置为空余资源数
	for (i = 0; i < resource_NUM; ++i) {
		Temp[i] = Available[i];
	}

    //将所有进程置为未完成状态
	for (i = 0; i < process_NUM; ++i) {
		Finished[i] = 0;
	}

    for (i = 0; i < process_NUM; ++i) {
        if (Finished[i] == 1)
            continue;
        else {
            for (j = 0; j < resource_NUM; ++j) {
                //如果当前进程所需资源中有一类资源大于空余资源，跳出当前进程，进入下一进程
                if (Need[i][j] > Temp[j])break;
            }
            //当当前进程所有类所需资源小于空余资源，则j=resource_NUM，判断成立
            if (j == resource_NUM) {
                Finished[i] = 1;
                //回收当前进程资源
                for (int k = 0; k < resource_NUM; ++k) {
                    Temp[k] += Alloc[i][k];
                }
                Path[L++] = i + 1;
                i = -1;
            }
            else continue;
        }
        if (L == process_NUM) {
            return true;
        }
    }
    return false;
}

void out_Path() {
    cout << "当前并行进程一个安全序列是：";
    cout << "Process" << Path[0] - 1;
    for (int i = 1; i < process_NUM; ++i) {
        cout << "->Process" << Path[i] - 1;
    }
    cout << endl;
}

void P_request() {
    //将请求资源数初始化为0
    memset(Request, 0, sizeof(Request));
    int p;
    cout << "请输入进行请求的进程号(0~4)：";
    cin >> p;
    cout << "请输入请求的资源数：" << endl;
    for (int i = 0; i < resource_NUM; ++i) {
        cin >> Request[i];
    }
    for (int i = 0; i < resource_NUM; ++i) {
        if (Request[i] > Need[p][i]) {
            cout << "当前进程贪心！" << endl;
            return;
        }
        if (Request[i] > Available[i]) {
            cout << "不安全，不允许请求！" << endl;
            return;
        }
    }
    for (int i = 0; i < resource_NUM; ++i) {
        Available[i] -= Request[i];
        Alloc[p][i] += Request[i];
        Need[p][i] -= Request[i];
    }
    if (isSafe()) {
        cout << "进程可安全执行，允许请求资源！" << endl;
        out_Path();
    }
    else {
        for (int i = 0; i < resource_NUM; ++i) {
            Available[i] += Request[i];
            Alloc[p][i] -= Request[i];
            Need[p][i] += Request[i];
        }
        cout << "请求后进程不可安全执行，不允许请求资源！" << endl;
    }
    return;
}
