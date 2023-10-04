#include<iostream>
#include<string>
#include"div.h"
#include"data.h"
#include"multi.h"
#include"plus_sub.h"
#include"scan_print.h"
using namespace std;

int main()
{
    //声明
    int T;          //运算组数
    char sign;      //运算符号
    string s1, s2;  //待运算两数

    scangroup(T);   //输入运算组数

    while (T--)
    {
        scandata(sign, s1, s2);     //输入各运算组

        bool minus;                 //输出'-'号与否
        hpnum a;

        if (sign == '*')            //乘法
            multi(minus, s1, s2, a);    
        else if (sign == '/')       //除法
            divi(s1, s2, minus, a);
        else
            plus_sub(minus, s1, s2, a, sign);       //加减法
    }

    return 0;
}