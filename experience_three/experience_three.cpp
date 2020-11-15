#include<iostream>
using namespace std;

//������process_NUM�����̣�resource_NUM����Դ
#define process_NUM 5
#define resource_NUM 4

//����������ڴ洢��Ϣ
int Resource[resource_NUM];//ÿ����Դ����Ŀ
int Available[resource_NUM];//ÿ����Դ��ǰ��Ŀ
int Claim[process_NUM][resource_NUM];//Claim[i][j]��ʾ����i����Դj�����������
int Alloc[process_NUM][resource_NUM];//Alloc[i][j]��ʾ����i�Ѿ�����������Դj����Ŀ
int Need[process_NUM][resource_NUM];//��ʾ���̻���Ҫ��Ӧ��Դ��
int Finished[process_NUM];//��ʾ�����Ƿ����
int Path[process_NUM];//��ʾһ����ȫ����
int Request[resource_NUM];//��ʾ��ǰ�������������Դ����

//�ж��ڽ�����һ����Դ������Ƿ�ȫ
bool isSafe();
//�����ȫ����
void out_Path();
//�жϽ���i��������Դ��ǰ�Ƿ�ȫ
void P_request();

/*
������4����Դ��Ŀ��
3 14 12 12
��ֱ�����5����������Ҫ4����Դ��Ŀ��
Process0:0 0 1 2
Process1:1 7 5 0
Process2:2 3 5 6
Process3:0 6 5 2
Process4:0 6 5 6
����ÿ�������Ѿ�����ĸ�����Դ����
Process0:0 0 1 2
Process1:1 0 0 0
Process2:1 3 5 4
Process3:0 6 3 2
Process4:0 0 1 4

1

0 4 2 0


*/

int main() {
    //��ʼ��
    cout << "������" << resource_NUM << "����Դ��Ŀ��" << endl;
    for (int i = 0; i < resource_NUM; ++i) {
        cin>>Resource[i];
        Available[i] = Resource[i];
    }
    Claim[process_NUM][resource_NUM] = 0;
    Alloc[process_NUM][resource_NUM] = 0;
    Need[process_NUM][resource_NUM] = 0;
    Finished[process_NUM] = 0;
    Path[process_NUM] = 0;

    //������Ϣ
    cout << "��ֱ�����" << process_NUM << "����������Ҫ" << resource_NUM << "����Դ��Ŀ��" << endl;
    for (int i = 0; i < process_NUM; ++i) {
        cout << "Process" << i << ":";
        for (int j = 0; j < resource_NUM; ++j) {
            cin >> Claim[i][j];
        }
    }
    cout << "����ÿ�������Ѿ�����ĸ�����Դ����" << endl;
    for (int i = 0; i < process_NUM; ++i) {
        cout << "Process" << i << ":";
        for (int j = 0; j < resource_NUM; ++j) {
            cin >> Alloc[i][j];
            Need[i][j] = Claim[i][j] - Alloc[i][j];
            if (Need[i][j] < 0) {
                cout << "��ǰ����������Դʧ�ܣ����鲢������������Դ�����������" << endl;
                j--;
                continue;
            }
            Available[j] -= Alloc[i][j];
        }
    }

    /*
    cout << "������Դ������Ŀ��" << endl;
    for (int i = 0; i < resource_NUM; ++i) {
        cout << Available[i] << " ";
    }
    cout << endl;
    */
    if (isSafe()) {
        cout << "��ǰ���̿ɰ�ȫִ�У�" << endl;
        out_Path();
    }
    else {
        cout << "��ǰ���̲��ɰ�ȫִ�У�" << endl;
    }

    //������������
    while (1) {
        P_request();
    }
    
	return 0;
}

bool isSafe() {
    //L��־���н��̰�ȫִ��·��
	int i=0, j=0, L=0;
	int Temp[resource_NUM];//�����м��
   
    //���м������Ϊ������Դ��
	for (i = 0; i < resource_NUM; ++i) {
		Temp[i] = Available[i];
	}

    //�����н�����Ϊδ���״̬
	for (i = 0; i < process_NUM; ++i) {
		Finished[i] = 0;
	}

    for (i = 0; i < process_NUM; ++i) {
        if (Finished[i] == 1)
            continue;
        else {
            for (j = 0; j < resource_NUM; ++j) {
                //�����ǰ����������Դ����һ����Դ���ڿ�����Դ��������ǰ���̣�������һ����
                if (Need[i][j] > Temp[j])break;
            }
            //����ǰ����������������ԴС�ڿ�����Դ����j=resource_NUM���жϳ���
            if (j == resource_NUM) {
                Finished[i] = 1;
                //���յ�ǰ������Դ
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
    cout << "��ǰ���н���һ����ȫ�����ǣ�";
    cout << "Process" << Path[0] - 1;
    for (int i = 1; i < process_NUM; ++i) {
        cout << "->Process" << Path[i] - 1;
    }
    cout << endl;
}

void P_request() {
    //��������Դ����ʼ��Ϊ0
    memset(Request, 0, sizeof(Request));
    int p;
    cout << "�������������Ľ��̺�(0~4)��";
    cin >> p;
    cout << "�������������Դ����" << endl;
    for (int i = 0; i < resource_NUM; ++i) {
        cin >> Request[i];
    }
    for (int i = 0; i < resource_NUM; ++i) {
        if (Request[i] > Need[p][i]) {
            cout << "��ǰ����̰�ģ�" << endl;
            return;
        }
        if (Request[i] > Available[i]) {
            cout << "����ȫ������������" << endl;
            return;
        }
    }
    for (int i = 0; i < resource_NUM; ++i) {
        Available[i] -= Request[i];
        Alloc[p][i] += Request[i];
        Need[p][i] -= Request[i];
    }
    if (isSafe()) {
        cout << "���̿ɰ�ȫִ�У�����������Դ��" << endl;
        out_Path();
    }
    else {
        for (int i = 0; i < resource_NUM; ++i) {
            Available[i] += Request[i];
            Alloc[p][i] -= Request[i];
            Need[p][i] += Request[i];
        }
        cout << "�������̲��ɰ�ȫִ�У�������������Դ��" << endl;
    }
    return;
}
