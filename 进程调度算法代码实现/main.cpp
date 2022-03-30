#include"stdio.h"
#include"iostream"
#include"string.h"
#include"iomanip"
#include"math.h"
#include"stdlib.h"
#include<queue>
using namespace std;

#define INF 100000000
typedef struct PCB
{
    int ID;
    int arriving_time;//����ʱ��
    int leaving_time;//����ʱ��
    int starting_time;//��ʼʱ��
    int service_time;//��Ҫ����ʱ��
    int elapsed_time;//�Ѿ�����ʱ��
    int remaining_time;//ʣ��ʱ��
    int STARTBLOCK;//����������STARTBLOCK��ʱ��Ƭ�󽫽�������״̬
    int BLOCKTIME;//�������Ľ����ٵȴ�BLOCKTIME��ʱ��Ƭ��ת���ɾ���״̬
    float cycling_time;//��תʱ��
    float weighted_cycling_time;//��Ȩ��תʱ��
    int STATE;//����״̬
    bool isOver;//�Ƿ��������
    struct PCB* next;//ָ����һ��PCB
}PCBNode, *PCBList;

void Create_PCBlist(PCBList& L,int n)
{
    PCBList p, s;
    int i;
    L = (PCBList)malloc(sizeof(PCBNode));
    L->ID = 0;
    L->next = NULL;
    s = L;
    cout << "������ÿ��PCB�ĵ���ʱ��ͷ���ʱ��:" << endl;
    for (i = 1;i <= n;i++)
    {
        p = (PCBList)malloc(sizeof(PCBNode));
        p->ID = i;
        cin >> p->arriving_time >> p->service_time;
        p->starting_time = p->leaving_time = p->cycling_time = p->weighted_cycling_time = 0;
        p->next = NULL;
        s->next = p;
        s = p;
    }
}

void PrintExecutionResult(PCBList L)
{
    PCBList p = L->next;//ָ���һ��PCB
    cout << "ID" << " " << "����ʱ��" << " " << "����ʱ��" << " " <<
        "��ʼʱ��" << " " << "����ʱ��" << " " << "��תʱ��" << " " << "��Ȩ��תʱ��" << endl;
    cout << "------------------------------------------------------------" << endl;
    while (p != NULL)
    {
        cout << left << setw(5) << p->ID << setw(9) << p->arriving_time << setw(9) << p->service_time
            << setw(9) << p->starting_time << setw(9) << p->leaving_time << setw(9) <<
            p->cycling_time << setw(9) << setprecision(3) << p->weighted_cycling_time << endl;
        p = p->next;
    }
}

//�Ե���ʱ���������
PCBList SortArrivingtime(PCBList L)
{
    PCBList result = L;
    PCBNode *pre, *p, *tail, *temp;
    tail = NULL;
    pre = result;
    while ((result->next->next) != tail)//(head->next)!=tailͬ������ ����ִ�����һ�����Ƚ� 
    {
        p = result->next;
        pre = result;
        while (p->next != tail)
        {
            if ((p->arriving_time) > (p->next->arriving_time))
            {
                pre->next = p->next; //�����ڵ㷽���� 
                temp = p->next->next;
                p->next->next = p;
                p->next = temp;
                p = pre->next;  //p����һ���ڵ� 
            }
            p = p->next;  //p��ǰ��һ���ڵ� 
            pre = pre->next;
        }
        tail = p;
    }
    return result;
}

//FCFS
void FCFS(PCBList L, int n)
{
    PCBList sortL = SortArrivingtime(L);//������ʱ������
    PCBList p = sortL->next;//ָ�������ĵ�һ������
    int prefintime = 0;
    float sumcyt = 0, sumwcyt = 0;
    float avecyt = 0, avewcyt = 0;
    while (p != NULL)
    {
        p->starting_time = __max(prefintime, p->arriving_time);
        p->leaving_time = p->starting_time + p->service_time;
        //p->start_end.push_back(make_pair(p->starting_time, p->leaving_time));
        prefintime = p->leaving_time;
        p->cycling_time = p->leaving_time - p->arriving_time;
        sumcyt += p->cycling_time;
        p->weighted_cycling_time = p->cycling_time / p->service_time;
        sumwcyt += p->weighted_cycling_time;
        p = p->next;
    }
    avecyt = sumcyt / n;
    avewcyt = sumwcyt / n;
    cout << "FCFS�������ܱ�" << endl;
    cout << "------------------------------------------------------------" << endl;
    PrintExecutionResult(sortL);//�������ִ�н��������PPT�еı��
    cout << setw(41) << left << "ƽ��" << setw(9) << setprecision(3)
        << avecyt << setprecision(3) << avewcyt << endl;
    cout << "------------------------------------------------------------" << endl;
    //showInfos(sortL, n);
    cout << endl;
}

//SJF
void SJF(PCBList L, int n)
{
    float sumcyt = 0, sumwcyt = 0;
    float avecyt = 0, avewcyt = 0;
    PCBList sortL = SortArrivingtime(L);//������ʱ������
    int t = sortL->next->arriving_time;//tΪ��ǰ��ʱ�䣬��ʼ��Ϊ���̵����絽��ʱ��
    for (PCBList p = sortL->next;p != NULL;p = p->next)//��������PCBδ���
    {
        p->isOver = false;
        p->starting_time = p->leaving_time = p->cycling_time = p->weighted_cycling_time = 0;
    }
    for (int i = 1;i <= n;i++)
    {
        PCBList cur = sortL;
        int min = INF;
        for (PCBList q = sortL->next;q != NULL;q = q->next)
        {//��ѭ���ҳ�����ʱ��С��t���ҷ���ʱ����̵��Ǹ�������cur��¼��
            if ((q->isOver == false) && (q->arriving_time <= t) && (min > q->service_time))
            {
                cur = q;
                min = q->service_time;
            }
        }
        if (min != INF)//min���ǳ�ʼֵINF,�ҵ��˷��������Ľ���,��tʱ�̿�ʼ���С�
        {
            cur->starting_time = t;
            cur->leaving_time = t + cur->service_time;
            //cur->start_end.push_back(make_pair(t, t + cur->service_time));
            cur->cycling_time = cur->leaving_time - cur->arriving_time;
            cur->weighted_cycling_time = cur->cycling_time / cur->service_time;
            sumcyt += cur->cycling_time;
            sumwcyt += cur->weighted_cycling_time;
            t += cur->service_time;
            cur->isOver = true;
        }
        else //û���ҵ����������Ľ��̣�tʱû�н��̵���
        {
            t = INF;//��t��ֵΪINF
            for (PCBList r = sortL->next;r != NULL;r = r->next)
            {
                if (!(r->isOver))
                {//�ҵ�ʣ����̵����絽��ʱ�䣬����ֵ����t
                    t = __min(t, r->arriving_time);
                }
            }
            i--;
        }
    }
    avecyt = sumcyt / n;
    avewcyt = sumwcyt / n;
    cout << "SJF�������ܱ�" << endl;
    cout << "------------------------------------------------------------" << endl;
    PrintExecutionResult(sortL);//�������ִ�н��������PPT�еı��
    cout << setw(41) << left << "ƽ��" << setw(9) << setprecision(3)
        << avecyt << setprecision(3) << avewcyt << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << endl;
}

//SRT
void SRT(PCBList L, int n)
{
    float sumcyt = 0, sumwcyt = 0;
    float avecyt = 0, avewcyt = 0;
    PCBList sortL = SortArrivingtime(L);//������ʱ������
    int t = sortL->next->arriving_time;//tΪ��ǰ��ʱ�䣬��ʼ��Ϊ���̵����絽��ʱ��
    for (PCBList p = sortL->next;p != NULL;p = p->next)
    {
        p->remaining_time = p->service_time;//��ʼ��ʣ��ִ��ʱ��
        p->starting_time = p->leaving_time = p->cycling_time = p->weighted_cycling_time = 0;
    }
    int cnt = 0;//cntΪ�����н������̵�����
    while (cnt < n)
    {
        PCBList cur = sortL;
        int min = INF;
        for (PCBList p = sortL->next;p != NULL;p = p->next)
        {//��ѭ���ҳ�����ʱ��С��t����ʣ��ʱ����̵��Ǹ�������cur��¼��
            if ((p->arriving_time <= t) && (p->remaining_time > 0) && (p->remaining_time < min))
            {
                min = p->remaining_time;
                cur = p;
            }
        }
        if (min == INF)
        {//û���ҵ����������Ľ���,tʱû�н��̵���
            t = INF;
            for (PCBList r = sortL->next;r != NULL;r = r->next)
            {
                if (r->remaining_time > 0)
                {
                    t = __min(t, r->arriving_time);
                }
            }
        }
        else//�ҵ��˷��������Ľ���
        {
            int nextArriving = INF;//nextArrivingΪ��t֮���һ���µ���Ľ��̵ĵ���ʱ�䣬��ʼ��Ϊ������
            for (PCBList q = cur->next;q != NULL;q = q->next)//��ѭ���������ҵ��������ʱ�䣬������nextArriving
            {
                if (q->remaining_time == q->service_time && q->arriving_time > t)//���ҵ��Ļ�û���й�
                {
                    nextArriving = q->arriving_time;
                    break;
                }
            }
            if (nextArriving - t >= cur->remaining_time)
            {//�����һ�����̵ĵ���ʱ���ȥ��ǰ��ʱ��>=����������̵�ʣ��ʱ��,������̿���ֱ��������������     
                if (cur->remaining_time == cur->service_time)//δִ�й���Ҫ�ҳ�ʼʱ��
                {
                    cur->starting_time = t;//����ÿ�����̿�ʼʱ��
                }
                t += cur->remaining_time;
                cur->leaving_time = t;//����ÿ�����̽���ʱ��
                cur->cycling_time = cur->leaving_time - cur->arriving_time;
                cur->weighted_cycling_time = cur->cycling_time / cur->service_time;
                sumcyt += cur->cycling_time;
                sumwcyt += cur->weighted_cycling_time;
                cur->remaining_time = 0;
                cnt++;
            }
            else//�����������ֻ�����е���һ�����̵�����������ѵ�����̵�ʣ��ʱ���ж�Ҫִ����һ�����̡�
            {
                if (cur->remaining_time == cur->service_time)//δִ�й���Ҫ�ҳ�ʼʱ��
                {
                    cur->starting_time = t;
                }
                cur->remaining_time -= nextArriving - t;
                t = nextArriving;
            }
        }
    }
    avecyt = sumcyt / n;
    avewcyt = sumwcyt / n;
    cout << "SRT�������ܱ�" << endl;
    cout << "------------------------------------------------------------" << endl;
    PrintExecutionResult(sortL);//�������ִ�н��������PPT�еı��
    cout << setw(41) << left << "ƽ��" << setw(9) << setprecision(3)
        << avecyt << setprecision(3) << avewcyt << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << endl;
}

void RR(PCBList L, int n)
{
    int timeslice = 1;//�˴�ʱ��Ƭ����Ϊ1
    float sumcyt = 0, sumwcyt = 0;
    float avecyt = 0, avewcyt = 0;
    PCBList sortL = SortArrivingtime(L);//������ʱ������
    queue <PCBList> que;
    PCBList p = L->next;
    for (PCBList p = sortL->next;p != NULL;p = p->next)
    {
        p->remaining_time = p->service_time;//��ʼ��ʣ��ִ��ʱ��
        p->starting_time = p->leaving_time = p->cycling_time = p->weighted_cycling_time = 0;
        p->STATE = 0;
    }
    double time = __max(0.0, p->arriving_time);
    que.push(p);
    p->STATE = 1;
    for (int i = 1;i <= n;i++)
    {
        while (!que.empty())
        {
            PCBList temp = que.front();   
            que.pop();
            if (temp->remaining_time == temp->service_time)//δִ�й���Ҫ�ҳ�ʼʱ��
            {
                temp->starting_time = time;//����ÿ�����̿�ʼʱ��
            }
            temp->elapsed_time = temp->remaining_time >= timeslice ? time + timeslice : time + temp->remaining_time;
            for (PCBList q = sortL->next;q != NULL;q = q->next)
            {
                if (q->STATE == 0 && q->arriving_time <= temp->elapsed_time)
                {
                    q->STATE = 1;
                    que.push(q);
                }
            }
            if (temp->remaining_time > timeslice)
            {
                temp->remaining_time -= timeslice;
                que.push(temp);
                time += timeslice;
            }
            else
            {
                time += temp->remaining_time;
                temp->remaining_time = 0;
                temp->leaving_time = time;//�������ʱ��
                temp->cycling_time = temp->leaving_time - temp->arriving_time;
                temp->weighted_cycling_time = temp->cycling_time / temp->service_time;
                sumcyt += temp->cycling_time;
                sumwcyt += temp->weighted_cycling_time;
            }         
        }
        bool flag = false; //�ж��Ƿ����еĽ��̶��Ѿ����
        for (PCBList r = sortL->next;r != NULL;r = r->next)
        {
            if (r->STATE == 0)
            {
                que.push(r);//��һ��ʱ���ǰ����ӵ�������
                time = r->arriving_time;   //����Ҫ����time
                flag = true;
                break;
            }
        } 
        if (!flag)
            break;
    }
    avecyt = sumcyt / n;
    avewcyt = sumwcyt / n;
    cout << "RR�������ܱ�(ʱ��Ƭ����Ϊ1����λʱ��)" << endl;
    cout << "------------------------------------------------------------" << endl;
    PrintExecutionResult(sortL);//�������ִ�н��������PPT�еı��
    cout << setw(41) << left << "ƽ��" << setw(9) << setprecision(3)
        << avecyt << setprecision(3) << avewcyt << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << endl;
}

//�������ݣ�0 3 2 6 4 4 6 5 8 2
int main()
{
    int PCBnum;//PCB�ܸ���
    cout << "������PCB����:";
    cin >> PCBnum;
    PCBList L;
    Create_PCBlist(L, PCBnum);//������ʼPCB����
    cout << "************************************************************" << endl;
    FCFS(L, PCBnum);//FCFS�㷨
    SJF(L, PCBnum); //SJF�㷨
    SRT(L, PCBnum); //SRT�㷨
    RR(L, PCBnum);  //RR�㷨
    system("pause");
	return 0;
}


