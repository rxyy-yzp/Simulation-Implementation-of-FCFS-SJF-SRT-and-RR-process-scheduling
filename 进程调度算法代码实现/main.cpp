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
    int arriving_time;//到达时间
    int leaving_time;//结束时间
    int starting_time;//开始时间
    int service_time;//需要服务时长
    int elapsed_time;//已经运行时间
    int remaining_time;//剩余时间
    int STARTBLOCK;//进程再运行STARTBLOCK个时间片后将进入阻塞状态
    int BLOCKTIME;//已阻塞的进程再等待BLOCKTIME个时间片后将转换成就绪状态
    float cycling_time;//周转时间
    float weighted_cycling_time;//带权周转时间
    int STATE;//运行状态
    bool isOver;//是否运行完成
    struct PCB* next;//指向下一个PCB
}PCBNode, *PCBList;

void Create_PCBlist(PCBList& L,int n)
{
    PCBList p, s;
    int i;
    L = (PCBList)malloc(sizeof(PCBNode));
    L->ID = 0;
    L->next = NULL;
    s = L;
    cout << "请输入每个PCB的到达时间和服务时间:" << endl;
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
    PCBList p = L->next;//指向第一个PCB
    cout << "ID" << " " << "到达时间" << " " << "服务时间" << " " <<
        "开始时间" << " " << "结束时间" << " " << "周转时间" << " " << "带权周转时间" << endl;
    cout << "------------------------------------------------------------" << endl;
    while (p != NULL)
    {
        cout << left << setw(5) << p->ID << setw(9) << p->arriving_time << setw(9) << p->service_time
            << setw(9) << p->starting_time << setw(9) << p->leaving_time << setw(9) <<
            p->cycling_time << setw(9) << setprecision(3) << p->weighted_cycling_time << endl;
        p = p->next;
    }
}

//对到达时间进行排序
PCBList SortArrivingtime(PCBList L)
{
    PCBList result = L;
    PCBNode *pre, *p, *tail, *temp;
    tail = NULL;
    pre = result;
    while ((result->next->next) != tail)//(head->next)!=tail同样适用 ，多执行最后一个步比较 
    {
        p = result->next;
        pre = result;
        while (p->next != tail)
        {
            if ((p->arriving_time) > (p->next->arriving_time))
            {
                pre->next = p->next; //交换节点方法二 
                temp = p->next->next;
                p->next->next = p;
                p->next = temp;
                p = pre->next;  //p回退一个节点 
            }
            p = p->next;  //p再前进一个节点 
            pre = pre->next;
        }
        tail = p;
    }
    return result;
}

//FCFS
void FCFS(PCBList L, int n)
{
    PCBList sortL = SortArrivingtime(L);//按到达时间排序
    PCBList p = sortL->next;//指向排序后的第一个进程
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
    cout << "FCFS调度性能表" << endl;
    cout << "------------------------------------------------------------" << endl;
    PrintExecutionResult(sortL);//输出最终执行结果（类似PPT中的表格）
    cout << setw(41) << left << "平均" << setw(9) << setprecision(3)
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
    PCBList sortL = SortArrivingtime(L);//按到达时间排序
    int t = sortL->next->arriving_time;//t为当前的时间，初始化为进程的最早到达时间
    for (PCBList p = sortL->next;p != NULL;p = p->next)//设置所有PCB未完成
    {
        p->isOver = false;
        p->starting_time = p->leaving_time = p->cycling_time = p->weighted_cycling_time = 0;
    }
    for (int i = 1;i <= n;i++)
    {
        PCBList cur = sortL;
        int min = INF;
        for (PCBList q = sortL->next;q != NULL;q = q->next)
        {//该循环找出到达时间小于t并且服务时间最短的那个进程用cur记录。
            if ((q->isOver == false) && (q->arriving_time <= t) && (min > q->service_time))
            {
                cur = q;
                min = q->service_time;
            }
        }
        if (min != INF)//min不是初始值INF,找到了符合条件的进程,在t时刻开始运行。
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
        else //没有找到符合条件的进程，t时没有进程到达
        {
            t = INF;//将t赋值为INF
            for (PCBList r = sortL->next;r != NULL;r = r->next)
            {
                if (!(r->isOver))
                {//找到剩余进程的最早到达时间，并将值赋给t
                    t = __min(t, r->arriving_time);
                }
            }
            i--;
        }
    }
    avecyt = sumcyt / n;
    avewcyt = sumwcyt / n;
    cout << "SJF调度性能表" << endl;
    cout << "------------------------------------------------------------" << endl;
    PrintExecutionResult(sortL);//输出最终执行结果（类似PPT中的表格）
    cout << setw(41) << left << "平均" << setw(9) << setprecision(3)
        << avecyt << setprecision(3) << avewcyt << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << endl;
}

//SRT
void SRT(PCBList L, int n)
{
    float sumcyt = 0, sumwcyt = 0;
    float avecyt = 0, avewcyt = 0;
    PCBList sortL = SortArrivingtime(L);//按到达时间排序
    int t = sortL->next->arriving_time;//t为当前的时间，初始化为进程的最早到达时间
    for (PCBList p = sortL->next;p != NULL;p = p->next)
    {
        p->remaining_time = p->service_time;//初始化剩余执行时间
        p->starting_time = p->leaving_time = p->cycling_time = p->weighted_cycling_time = 0;
    }
    int cnt = 0;//cnt为已运行结束进程的数量
    while (cnt < n)
    {
        PCBList cur = sortL;
        int min = INF;
        for (PCBList p = sortL->next;p != NULL;p = p->next)
        {//该循环找出到达时间小于t并且剩余时间最短的那个进程用cur记录。
            if ((p->arriving_time <= t) && (p->remaining_time > 0) && (p->remaining_time < min))
            {
                min = p->remaining_time;
                cur = p;
            }
        }
        if (min == INF)
        {//没有找到符合条件的进程,t时没有进程到达
            t = INF;
            for (PCBList r = sortL->next;r != NULL;r = r->next)
            {
                if (r->remaining_time > 0)
                {
                    t = __min(t, r->arriving_time);
                }
            }
        }
        else//找到了符合条件的进程
        {
            int nextArriving = INF;//nextArriving为在t之后第一个新到达的进程的到达时间，初始化为正无穷
            for (PCBList q = cur->next;q != NULL;q = q->next)//该循环作用是找到这个到达时间，并赋给nextArriving
            {
                if (q->remaining_time == q->service_time && q->arriving_time > t)//若找到的还没运行过
                {
                    nextArriving = q->arriving_time;
                    break;
                }
            }
            if (nextArriving - t >= cur->remaining_time)
            {//如果下一个进程的到达时间减去当前的时间>=现在这个进程的剩余时间,这个进程可以直接运行至结束。     
                if (cur->remaining_time == cur->service_time)//未执行过，要找初始时间
                {
                    cur->starting_time = t;//计算每个进程开始时间
                }
                t += cur->remaining_time;
                cur->leaving_time = t;//计算每个进程结束时间
                cur->cycling_time = cur->leaving_time - cur->arriving_time;
                cur->weighted_cycling_time = cur->cycling_time / cur->service_time;
                sumcyt += cur->cycling_time;
                sumwcyt += cur->weighted_cycling_time;
                cur->remaining_time = 0;
                cnt++;
            }
            else//否则这个进程只能运行到下一个进程到达，根据所有已到达进程的剩余时长判断要执行哪一个进程。
            {
                if (cur->remaining_time == cur->service_time)//未执行过，要找初始时间
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
    cout << "SRT调度性能表" << endl;
    cout << "------------------------------------------------------------" << endl;
    PrintExecutionResult(sortL);//输出最终执行结果（类似PPT中的表格）
    cout << setw(41) << left << "平均" << setw(9) << setprecision(3)
        << avecyt << setprecision(3) << avewcyt << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << endl;
}

void RR(PCBList L, int n)
{
    int timeslice = 1;//此处时间片长度为1
    float sumcyt = 0, sumwcyt = 0;
    float avecyt = 0, avewcyt = 0;
    PCBList sortL = SortArrivingtime(L);//按到达时间排序
    queue <PCBList> que;
    PCBList p = L->next;
    for (PCBList p = sortL->next;p != NULL;p = p->next)
    {
        p->remaining_time = p->service_time;//初始化剩余执行时间
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
            if (temp->remaining_time == temp->service_time)//未执行过，要找初始时间
            {
                temp->starting_time = time;//计算每个进程开始时间
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
                temp->leaving_time = time;//输出结束时间
                temp->cycling_time = temp->leaving_time - temp->arriving_time;
                temp->weighted_cycling_time = temp->cycling_time / temp->service_time;
                sumcyt += temp->cycling_time;
                sumwcyt += temp->weighted_cycling_time;
            }         
        }
        bool flag = false; //判断是否所有的进程都已经完成
        for (PCBList r = sortL->next;r != NULL;r = r->next)
        {
            if (r->STATE == 0)
            {
                que.push(r);//将一个时间最靠前的添加到队列中
                time = r->arriving_time;   //这里要更新time
                flag = true;
                break;
            }
        } 
        if (!flag)
            break;
    }
    avecyt = sumcyt / n;
    avewcyt = sumwcyt / n;
    cout << "RR调度性能表(时间片长度为1个单位时间)" << endl;
    cout << "------------------------------------------------------------" << endl;
    PrintExecutionResult(sortL);//输出最终执行结果（类似PPT中的表格）
    cout << setw(41) << left << "平均" << setw(9) << setprecision(3)
        << avecyt << setprecision(3) << avewcyt << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << endl;
}

//测试数据：0 3 2 6 4 4 6 5 8 2
int main()
{
    int PCBnum;//PCB总个数
    cout << "请输入PCB总数:";
    cin >> PCBnum;
    PCBList L;
    Create_PCBlist(L, PCBnum);//创建初始PCB队列
    cout << "************************************************************" << endl;
    FCFS(L, PCBnum);//FCFS算法
    SJF(L, PCBnum); //SJF算法
    SRT(L, PCBnum); //SRT算法
    RR(L, PCBnum);  //RR算法
    system("pause");
	return 0;
}


