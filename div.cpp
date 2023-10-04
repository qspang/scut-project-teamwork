#include<string>
#include<iostream>
#include"scan_print.h"
#include"data.h"
#include"plus_sub.h"
#include"trans_comp.h"
#include"div.h"
using namespace std;

//除法运算流程函数
void divi(string& s1, string& s2, bool &minus, hpnum& a)
{
    strfixdiv(s1, s2, minus);
    if (s2 < "0000001") 
    { 
        cout << "ERROR" << endl;  
        return; 
    }    //除数过小，报错;

    Divi(s1, s2, a);

    sisewuru(a);
    printhp(a, minus);
}

//除法专用的字符串修饰函数
void strfixdiv(string& s1, string& s2, bool& minus)
{
    if (s1[0] == '-') { minus = (s2[0] != '-'); s1 = s1.substr(1); }
    else if ((minus = (s2[0] == '-')))     s2 = s2.substr(1);       //判断‘-’号输出与否，同时去除‘-’号

    unsigned i, j;

    if ((i = s2.find('.')) != s2.npos) 
    { 
        s2.erase(i, 1); 

        s1.append(s2.size() - i, '0');   // 被除数小数点右移(补零）
    }   

    if ((j = s1.find('.')) != s1.npos)
    {
        s1.erase(j, 1);

        if ((i = (s1.size() - j)) >= 11)    
            s1.erase(j + 11);
        else 
            s1.append(11 - i, '0');
    }
    else s1.append(11, '0');                                        //依次修饰除数与被除数 

    j = s1.size(), i = s2.size();                                    //delete ','
    while (j--) { if (s1[j] == ',') s1.erase(j, 1); }
    while (i--) { if (s2[i] == ',') s2.erase(i, 1); }
}

//除法运算核心模块
void Divi(string &s1, string &s2, hpnum& a)
{
    const string chushu = s2.substr(s2.find_first_not_of('0'));  //获取除数（去前导零）
    
    string temp;                                                 //新建空字符串
    NUM* headchushu = NULL;
    str2lian(chushu, headchushu);                                //建立除数链表

    a.len = s1.size();                                      //运算结果末位，与被除数对齐
    a.lenf = 11;                                            //课程设计要求保留10位小数，因而初步运算结果保留11位小数

    for (int i = a.len; i > 0; --i)
    {
        temp += s1[a.len - i];                               //内置的重载运算
        if (temp == "0")
        {
            temp.erase();                    //中间运算结果为零，须删去此‘0’，跳过该位上的运算
            continue;
        }

        while (!strcom(temp, chushu))
        {
            hpnum b;
            NUM* head1 = NULL;
            str2lian(temp, head1);
            Sub(head1, headchushu, b);
            a.c[i]++;
            hp2str(b, temp);

            if (temp == "0") { temp.erase(); break; }        //中间得数为零时，停止该位数上的运算
        }
    }
}

//四舍五入模块
void sisewuru(hpnum& a)                
{
    int i = 2;
    int j = (a.c[1] + 5) / 10;
    a.c[1] = 0;

    while (j)
    {
        a.c[i] += 1;
        j = a.c[i] / 10;
        a.c[i] %= 10;
        i++;
    }

    a.len++;      //四舍五入中，总数位增加1的情况
}