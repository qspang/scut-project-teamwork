//程序中多处用到的转换函数，例如字符串转链表等
#include<string>
#include"data.h"
#ifndef TRANS_COMP_H
#define TRANS_COMP_H

void str2lian(std::string s, NUM*& head);
void hp2str(hpnum a, std::string& s);
bool strcom(std::string s1, std::string s2);
#endif
