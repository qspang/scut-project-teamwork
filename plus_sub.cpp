#include<string>
#include"data.h"
#include"trans_comp.h"
#include"plus_sub.h"
#include"scan_print.h"
using namespace std;

//加减法流程函数
void plus_sub(bool& minus, string& s1, string& s2, hpnum& a, char& sign)
{
    NUM* head1 = NULL, * head2 = NULL;        //声明链表

    strfixplus_sub(s1, s2, a);
    if ((judge(s1, s2, minus, sign, head1, head2)) == '+')
        Plus(head1, head2, a);
    else
        Sub(head1, head2, a);

    printhp(a, minus);
}

//判断运算类型（是加或减），返回‘-’或‘+’，顺带完成字符串到链表的转换，以及正负号的判断
char judge(string& s1, string& s2, bool& minus, char& sign, NUM*& head1 , NUM*& head2 )
{
    char s1sign, s2sign, op;  //前两者记录数字正负，末者为运算符号
    
    if (s1[0] == '-')                       
    { 
        s1sign = '-';
        s1 = s1.substr(1); 
    }
    else s1sign = '+';

    if (s2[0] == '-') 
    { 
        s2sign = '-';
        s2 = s2.substr(1); 
    }
    else s2sign = '+';

    op = (sign ^ s1sign ^ s2sign);   //位运算的运用
    
    if (strcom(s1, s2))             //前数小于后数
    {
        str2lian(s2, head1);
        str2lian(s1, head2);
        minus = (sign^s2sign);          //同+异-
    }
    else
    {
        str2lian(s1, head1);
        str2lian(s2, head2);
        minus = (s1sign == '-');        //与前数正负一致
    }

    return op;
}

//加减法字符串初步修饰
void strfixplus_sub(string& s1, string& s2, hpnum& a)
{
    unsigned lentemp, len1f, len2f;

    if ((lentemp = s1.find('.')) == s1.npos) { len1f = 0; }
    else { len1f = s1.size() - lentemp - 1; s1.erase(lentemp, 1); }
    if ((lentemp = s2.find('.')) == s2.npos) { len2f = 0; }
    else { len2f = s2.size() - lentemp - 1; s2.erase(lentemp, 1); }

    if (len1f < len2f)
    {
        s1.append(len2f - len1f, '0');
        a.lenf = len2f;
    }
    else
    {
        s2.append(len1f - len2f, '0');
        a.lenf = len1f;
    }

    unsigned j = s1.size(), i = s2.size();                                    //delete ','
    while (j--) { if (s1[j] == ',') s1.erase(j, 1); }
    while (i--) { if (s2[i] == ',') s2.erase(i, 1); }
}

//加法核心运算模块
void Plus(NUM* head1, NUM* head2, hpnum& a)
{
    int i = 1;
    for (; head1; head1 = head1->next, ++i)
    {
        if (head2)
        {
            a.c[i] += head1->num + head2->num;
            a.c[i + 1] += a.c[i] / 10;   //s2存在的时候
            a.c[i] %= 10;
            head2 = head2->next;
        }
        else                     //s2已遍历。
        {
            a.c[i] += head1->num;
            a.c[i + 1] += a.c[i] / 10;
            a.c[i] %= 10;
        }
    }

    a.len = i;
}

//减法核心运算模块
void Sub(NUM* head1, NUM* head2, hpnum& a)
{
    int i = 1;
    for (; head1; head1 = head1->next, ++i)
    {
        if (head2)
        {
            a.c[i] += head1->num - head2->num;
            if (a.c[i] < 0)
            {
                a.c[i + 1] -= 1;
                a.c[i] += 10;
            }
            head2 = head2->next;
        }

        else                     //s2已遍历。
        {
            a.c[i] += head1->num;
            a.c[i + 1] += a.c[i] / 10;
            a.c[i] %= 10;
            if (a.c[i] < 0)
            {
                a.c[i + 1] -= 1;
                a.c[i] += 10;
            }
        }
    }

    a.len = i;
}