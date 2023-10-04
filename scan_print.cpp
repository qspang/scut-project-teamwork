#include<iostream>
#include"scan_print.h"
using namespace std;

//录入运算组数
void scangroup(int& T)
{
    cout << "待计算的组数(可为任意非负整数): " << endl;
    cin >> T;
}

void scandata(char& sign, std::string& s1, std::string& s2)
{
    cout << "输入运算类型：（'+' , '-' , '*' 或 '/')" << endl;
    cin >> sign;

    cout << "依次输入需运算两数： （两数间可用空格隔开，亦可换行输入)" << endl;
    cin >> s1 >> s2;
}

//输出结果
void printhp(hpnum a, bool minus)
{
    cout << "得数为：" << endl;

    int i = a.len, j = 0;

    //前导零的处理
    while (!a.c[i] && i > 0) --i;
    if (i == 0)
    {
        cout << 0 << endl;  return;
    }     //数组全为零，得数为零
    if (i <= a.lenf)
    {
        i = a.lenf + 1;
    }             //整数全为零的情况，保留个位上的0

    //小数末尾零的处理
    while (!a.c[j] && j <= a.lenf) ++j;

    if (minus)
    {
        cout << '-';
    }   //是否输出‘-’号（得数为负数）

    for (; i >= j; i--)
    {
        if (i > a.lenf)
        {
            cout << a.c[i];

            if ((i - a.lenf) % 3 == 1 && (i - a.lenf) != 1)
            {
                cout << ',';
            }       //每三位整数，输出一个‘，’
        }
        else
        {
            if (i == a.lenf)
            {
                cout << '.';
            }       //输出小数点

            cout << a.c[i];
        }
    }

    cout << endl;
}