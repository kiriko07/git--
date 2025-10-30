#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define getpch(type) (type*)malloc(sizeof(type))

struct pcb {
    char name[10];    // 进程名
    char state;       // 状态：W(就绪)、R(运行)、F(完成)
    int super;        // 优先级（轮转法中不使用）
    int ntime;        // 剩余运行时间
    int rtime;        // 已运行时间
    struct pcb* link; // 链表指针
} *ready = NULL, * p;

typedef struct pcb PCB;

int m; // 全局变量：1=最高优先级算法，2=轮转法（用于区分调度类型）

// 函数声明
void inunization();
void sort();         // 按调度算法排序就绪队列
void input();
int space();
void check();        // 显示进程状态（根据m区分格式）
void destroy();
void running();      // 进程运行逻辑（根据m调整优先级）
void youxian();      // 最高优先级调度
void RoundRobin();   // 轮转法调度
void menu();

// 初始化界面
void inunization() {
    printf("\n\n\t\t********************************\t\t\n");
    printf("\t\t\t\t 实验一 进程调度\n");
    printf("\t\t********************************\t\t\n");
    printf("\n\n\n\t\t\t 计算机系 242 班\n");
    printf("\t\t\t 姓名:黄睿柏\n");
    printf("\t\t\t 学号:2442110183\n");
    printf("\t\t\t 完成时间:2025 年 10 月 16 日\n\n\n\n");
    printf("\t\t\t\t 请输入任意键进入演示过程\n");
    _getch();
}

// （5）修改的sort函数：根据算法类型排序
void sort() {
    PCB* first, * second;
    if (m == 1) { // 最高优先级算法：按优先级从高到低排序
        int insert = 0;
        if ((ready == NULL) || (p->super > ready->super)) {
            p->link = ready;
            ready = p;
        }
        else {
            first = ready;
            second = first->link;
            while (second != NULL) {
                if (p->super > second->super) {
                    p->link = second;
                    first->link = p;
                    second = NULL;
                    insert = 1;
                }
                else {
                    first = first->link;
                    second = second->link;
                }
            }
            if (insert == 0) {
                first->link = p;
            }
        }
    }
    if (m == 2) { // 轮转法：按FCFS顺序插入队尾
        if (ready == NULL) {
            p->link = ready;
            ready = p;
        }
        else {
            first = ready;
            second = first->link;
            while (second != NULL) {
                first = first->link;
                second = second->link;
            }
            first->link = p;
        }
    }
}

// （2）修改的input函数：根据算法类型决定是否输入优先级
void input() {
    int i;
    system("cls");
    printf("\n 请输入六个进程信息: \n");

    for (i = 0; i < 6; i++) {
        printf("\n 进程号 No: %d: \n", i + 1);
        p = getpch(PCB);

        printf(" 输入进程名: ");
        scanf("%s", p->name);

        // 最高优先级算法需要输入优先级，轮转法不需要（可省略或默认0）
        if (m == 1) {
            printf(" 输入进程优先数: ");
            scanf("%d", &p->super);
        }
        else {
            p->super = 0; // 轮转法优先级无效，赋默认值
        }

        printf(" 输入进程运行时间: ");
        scanf("%d", &p->ntime);

        p->rtime = 0;
        p->state = 'W';
        p->link = NULL;

        sort(); // 根据当前算法类型排序
    }
}

// 统计就绪队列进程数
int space() {
    int l = 0;
    PCB* pr = ready;
    while (pr != NULL) {
        l++;
        pr = pr->link;
    }
    return l;
}

// （3）修改的check函数：根据算法类型调整输出格式
void check() {
    PCB* pr;
    printf("\n ***** 当前正在运行的进程是: %s ***** \n", p->name);

    // 最高优先级算法显示优先级，轮转法不显示
    if (m == 1) {
        printf(" 进程名 \t 状态 \t 优先级 \t 剩余时间 \t 已运行时间 \n");
        printf(" %s \t\t %c \t %d \t\t %d \t\t %d \n",
            p->name, p->state, p->super, p->ntime, p->rtime);
    }
    else {
        printf(" 进程名 \t 状态 \t 剩余时间 \t 已运行时间 \n");
        printf(" %s \t\t %c \t %d \t\t %d \n",
            p->name, p->state, p->ntime, p->rtime);
    }

    printf("\n ***** 当前就绪队列状态 ***** \n");
    if (m == 1) {
        printf(" 进程名 \t 状态 \t 优先级 \t 剩余时间 \t 已运行时间 \n");
    }
    else {
        printf(" 进程名 \t 状态 \t 剩余时间 \t 已运行时间 \n");
    }

    pr = ready;
    while (pr != NULL) {
        if (m == 1) {
            printf(" %s \t\t %c \t %d \t\t %d \t\t %d \n",
                pr->name, pr->state, pr->super, pr->ntime, pr->rtime);
        }
        else {
            printf(" %s \t\t %c \t %d \t\t %d \n",
                pr->name, pr->state, pr->ntime, pr->rtime);
        }
        pr = pr->link;
    }
}

// 销毁已完成的进程
void destroy() {
    printf("\n 进程 [%s] 已完成 \n", p->name);
    p->state = 'F';
    free(p);
}

// （4）修改的running函数：仅最高优先级算法降低优先级
void running() {
    p->rtime++;   // 已运行时间+1
    p->ntime--;   // 剩余时间-1（统一逻辑）

    if (m == 1) { // 仅最高优先级算法需要降低优先级
        p->super--;
    }

    if (p->ntime == 0) { // 剩余时间为0表示完成
        destroy();
    }
    else {
        p->state = 'W';
        sort(); // 根据当前算法重新插入队列
    }
}

// 最高优先级调度算法（设置m=1）
void youxian() {
    int len, h = 0;
    char ch;
    m = 1; // 标记当前为最高优先级算法
    system("cls");
    input();

    while ((len = space()) != 0 && ready != NULL) {
        h++;
        printf("\n ====================================== \n");
        printf(" 第 %d 次调度 \n", h);
        printf(" ====================================== \n");

        p = ready;
        ready = p->link;
        p->link = NULL;
        p->state = 'R';

        check();
        running();

        printf(" \n 按任一键继续下一次调度... ");
        _getch();
    }

    printf("\n\n 所有进程已完成调度！\n");
    printf(" 按任一键返回菜单... ");
    _getch();
}

// （6）实现的轮转法调度（设置m=2）
void RoundRobin() {
    int len, h = 0;
    char ch;
    m = 2; // 标记当前为轮转法
    system("cls");
    input();

    len = space();
    while ((len != 0) && (ready != NULL)) {
        ch = getchar(); // 吸收输入缓冲区残留字符
        h++;
        printf("\n The execute number: %d\n", h);

        p = ready;
        ready = p->link;
        p->link = NULL;
        p->state = 'R';

        check();
        running();

        printf(" \n 按任一键继续... ");
        _getch();
        len = space(); // 更新队列长度
    }

    printf("\n\n 进程已经完成. \n");
    ch = _getch();
}

// 主菜单
void menu() {
    system("cls");
    printf("\n\n\t\t********************************\t\t\n");
    printf("\t\t\t\t 进程调度演示 \n");
    printf("\t\t********************************\t\t\n");
    printf("\n\n\n\t\t\t 1. 演示最高优先数优先算法 \n");
    printf("\t\t\t 2. 演示轮转法算法 \n");
    printf("\t\t\t 0. 退出程序 \n");
    printf("\n\n\t\t\t 请选择功能（0-2）：");
    scanf("%d", &m); // m直接作为算法选择变量

    switch (m) {
    case 1:
        youxian();
        menu();
        break;
    case 2:
        RoundRobin();
        menu();
        break;
    case 0:
        system("cls");
        printf("\n 程序已退出！\n");
        break;
    default:
        system("cls");
        printf("\n 输入错误，请重新选择！\n");
        _getch();
        menu();
    }
}

void main() {
    inunization();
    menu();
}