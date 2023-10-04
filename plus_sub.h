//加，减法运算模块
#include<string>
#include"data.h"
#ifndef PLUS_SUB_H
#define PLUS_SUB_H

void plusorsub(char& sign, char s1, char s2);
void plus_sub(bool& minus, std::string& s1, std::string& s2, hpnum& a, char& sign);
void strfixplus_sub(std::string& s1, std::string& s2, hpnum& a);
char judge(std::string& s1, std::string& s2, bool& minus, char& sign, NUM*& head1, NUM*& head2);
void Plus(NUM* head1, NUM* head2, hpnum& a);
void Sub(NUM* head1, NUM* head2, hpnum& a);

#endif

