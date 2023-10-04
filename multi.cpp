#include<string>
#include"data.h"
#include"trans_comp.h"
#include"multi.h"
#include"scan_print.h"
using namespace std;

//乘法处理流程
void multi(bool& minus, string& s1, string& s2, hpnum& a)
{
    NUM* head1 = NULL, * head2 = NULL;

    strfixmul(minus, s1, s2, a);
    str2lian(s1, head1);
    str2lian(s2, head2);
    Multi(head1, head2, a);

    printhp(a, minus);
}

//乘法字符串修饰
void strfixmul(bool& minus, string& s1, string& s2, hpnum& a)
{
     
    if (s1[0] == '-') 
    { 
        minus = (s2[0] != '-'); 
        s1 = s1.substr(1); 
    }
    else    if ((minus = (s2[0] == '-')))
    {
        s2 = s2.substr(1);
    }

    unsigned j, i;
    if ((j = s1.find('.')) != s1.npos) 
    { 
        a.lenf += (s1.size() - j - 1);
        s1.erase(j, 1); 
    }
    if ((j = s2.find('.')) != s2.npos) { a.lenf += (s2.size() - j - 1);  s2.erase(j, 1); }

    j = s1.size(), i = s2.size();                                    //delete ','
    while (j--) { if (s1[j] == ',') s1.erase(j, 1); }
    while (i--) { if (s2[i] == ',') s2.erase(i, 1); }
}

//乘法核心模块
void Multi(NUM* head1, NUM* head2, hpnum& a)
{
    int i = 1, j;

    for (; head2; head2 = head2->next, ++i)
    {
        if (!head2->num) continue;

        NUM* p = head1;

        for (j = i; p; ++j)
        {
            a.c[j] += head2->num * p->num;
            p = p->next;
        }
    }

    for (i = 1; i <= j; i++)
    {
        a.c[i + 1] += a.c[i] / 10;
        a.c[i] %= 10;
    }

    a.len = j;
}