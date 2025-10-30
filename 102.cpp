#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define getpch(type) (type*)malloc(sizeof(type))

struct pcb {
    char name[10];    // ������
    char state;       // ״̬��W(����)��R(����)��F(���)
    int super;        // ���ȼ�����ת���в�ʹ�ã�
    int ntime;        // ʣ������ʱ��
    int rtime;        // ������ʱ��
    struct pcb* link; // ����ָ��
} *ready = NULL, * p;

typedef struct pcb PCB;

int m; // ȫ�ֱ�����1=������ȼ��㷨��2=��ת�����������ֵ������ͣ�

// ��������
void inunization();
void sort();         // �������㷨�����������
void input();
int space();
void check();        // ��ʾ����״̬������m���ָ�ʽ��
void destroy();
void running();      // ���������߼�������m�������ȼ���
void youxian();      // ������ȼ�����
void RoundRobin();   // ��ת������
void menu();

// ��ʼ������
void inunization() {
    printf("\n\n\t\t********************************\t\t\n");
    printf("\t\t\t\t ʵ��һ ���̵���\n");
    printf("\t\t********************************\t\t\n");
    printf("\n\n\n\t\t\t �����ϵ 242 ��\n");
    printf("\t\t\t ����:���\n");
    printf("\t\t\t ѧ��:2442110183\n");
    printf("\t\t\t ���ʱ��:2025 �� 10 �� 16 ��\n\n\n\n");
    printf("\t\t\t\t �����������������ʾ����\n");
    _getch();
}

// ��5���޸ĵ�sort�����������㷨��������
void sort() {
    PCB* first, * second;
    if (m == 1) { // ������ȼ��㷨�������ȼ��Ӹߵ�������
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
    if (m == 2) { // ��ת������FCFS˳������β
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

// ��2���޸ĵ�input�����������㷨���;����Ƿ��������ȼ�
void input() {
    int i;
    system("cls");
    printf("\n ����������������Ϣ: \n");

    for (i = 0; i < 6; i++) {
        printf("\n ���̺� No: %d: \n", i + 1);
        p = getpch(PCB);

        printf(" ���������: ");
        scanf("%s", p->name);

        // ������ȼ��㷨��Ҫ�������ȼ�����ת������Ҫ����ʡ�Ի�Ĭ��0��
        if (m == 1) {
            printf(" �������������: ");
            scanf("%d", &p->super);
        }
        else {
            p->super = 0; // ��ת�����ȼ���Ч����Ĭ��ֵ
        }

        printf(" �����������ʱ��: ");
        scanf("%d", &p->ntime);

        p->rtime = 0;
        p->state = 'W';
        p->link = NULL;

        sort(); // ���ݵ�ǰ�㷨��������
    }
}

// ͳ�ƾ������н�����
int space() {
    int l = 0;
    PCB* pr = ready;
    while (pr != NULL) {
        l++;
        pr = pr->link;
    }
    return l;
}

// ��3���޸ĵ�check�����������㷨���͵��������ʽ
void check() {
    PCB* pr;
    printf("\n ***** ��ǰ�������еĽ�����: %s ***** \n", p->name);

    // ������ȼ��㷨��ʾ���ȼ�����ת������ʾ
    if (m == 1) {
        printf(" ������ \t ״̬ \t ���ȼ� \t ʣ��ʱ�� \t ������ʱ�� \n");
        printf(" %s \t\t %c \t %d \t\t %d \t\t %d \n",
            p->name, p->state, p->super, p->ntime, p->rtime);
    }
    else {
        printf(" ������ \t ״̬ \t ʣ��ʱ�� \t ������ʱ�� \n");
        printf(" %s \t\t %c \t %d \t\t %d \n",
            p->name, p->state, p->ntime, p->rtime);
    }

    printf("\n ***** ��ǰ��������״̬ ***** \n");
    if (m == 1) {
        printf(" ������ \t ״̬ \t ���ȼ� \t ʣ��ʱ�� \t ������ʱ�� \n");
    }
    else {
        printf(" ������ \t ״̬ \t ʣ��ʱ�� \t ������ʱ�� \n");
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

// ��������ɵĽ���
void destroy() {
    printf("\n ���� [%s] ����� \n", p->name);
    p->state = 'F';
    free(p);
}

// ��4���޸ĵ�running��������������ȼ��㷨�������ȼ�
void running() {
    p->rtime++;   // ������ʱ��+1
    p->ntime--;   // ʣ��ʱ��-1��ͳһ�߼���

    if (m == 1) { // ��������ȼ��㷨��Ҫ�������ȼ�
        p->super--;
    }

    if (p->ntime == 0) { // ʣ��ʱ��Ϊ0��ʾ���
        destroy();
    }
    else {
        p->state = 'W';
        sort(); // ���ݵ�ǰ�㷨���²������
    }
}

// ������ȼ������㷨������m=1��
void youxian() {
    int len, h = 0;
    char ch;
    m = 1; // ��ǵ�ǰΪ������ȼ��㷨
    system("cls");
    input();

    while ((len = space()) != 0 && ready != NULL) {
        h++;
        printf("\n ====================================== \n");
        printf(" �� %d �ε��� \n", h);
        printf(" ====================================== \n");

        p = ready;
        ready = p->link;
        p->link = NULL;
        p->state = 'R';

        check();
        running();

        printf(" \n ����һ��������һ�ε���... ");
        _getch();
    }

    printf("\n\n ���н�������ɵ��ȣ�\n");
    printf(" ����һ�����ز˵�... ");
    _getch();
}

// ��6��ʵ�ֵ���ת�����ȣ�����m=2��
void RoundRobin() {
    int len, h = 0;
    char ch;
    m = 2; // ��ǵ�ǰΪ��ת��
    system("cls");
    input();

    len = space();
    while ((len != 0) && (ready != NULL)) {
        ch = getchar(); // �������뻺���������ַ�
        h++;
        printf("\n The execute number: %d\n", h);

        p = ready;
        ready = p->link;
        p->link = NULL;
        p->state = 'R';

        check();
        running();

        printf(" \n ����һ������... ");
        _getch();
        len = space(); // ���¶��г���
    }

    printf("\n\n �����Ѿ����. \n");
    ch = _getch();
}

// ���˵�
void menu() {
    system("cls");
    printf("\n\n\t\t********************************\t\t\n");
    printf("\t\t\t\t ���̵�����ʾ \n");
    printf("\t\t********************************\t\t\n");
    printf("\n\n\n\t\t\t 1. ��ʾ��������������㷨 \n");
    printf("\t\t\t 2. ��ʾ��ת���㷨 \n");
    printf("\t\t\t 0. �˳����� \n");
    printf("\n\n\t\t\t ��ѡ���ܣ�0-2����");
    scanf("%d", &m); // mֱ����Ϊ�㷨ѡ�����

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
        printf("\n �������˳���\n");
        break;
    default:
        system("cls");
        printf("\n �������������ѡ��\n");
        _getch();
        menu();
    }
}

void main() {
    inunization();
    menu();
}