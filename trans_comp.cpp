#include<string>
#include"data.h"
using namespace std;

//结构体hpnum转换为字符串（除法专用）
void hp2str(hpnum a, string& s)
{
    
    a.len = s.size();
    int i = a.len; 
    string temp;

    while (!a.c[i] && i > 0) --i;
    
    for (int j = 1; j <= i; j++)
    {
        temp = char(a.c[j] + '0') + temp;
    }

    s = temp;
}

//字符串转换为链表
void str2lian(std::string s, NUM*& head)
{
    int len = s.size();
    head = new NUM;
    head->num = s[0] - '0';
    head->next = NULL;

    NUM* p = new NUM;
    for (int i = 1; i < len; i++)
    {
        p->next = head;
        p->num = s[i] - '0';
        head = p;

        p = new NUM;
    }

    delete p;           //删除空链点
}

//用字符串方法判断两数大小（数位已对齐）
bool strcom(string s1, string s2)
{
    if (s1.size() == s2.size())  
    {
        return s1 < s2;
    }
    else
    {
        return s1.size() < s2.size();
    }
}