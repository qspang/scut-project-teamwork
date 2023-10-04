//存储结构
#ifndef DATE_H
#define DATE_H

//结构体，用于辅助处理数据
//本程序中主要用于暂时存储运算结果
struct hpnum
{
    int len;                //初略记录数据‘长度’，没有处理前导零与后导零
    int lenf = 0;           //记录小数部分长度
    int c[10000] = { 0 };     //用于记录数据的数组
};

//链表，用于存储数据
struct NUM
{
    int num;                //每个链表节点的数据，只有一位整数
    NUM* next;
};
#endif
