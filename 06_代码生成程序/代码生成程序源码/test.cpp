/*
    test.cpp
    �����������кܶ���� C++ ���Ե�ϸ����Ҫ�ڳ�����ѧϰ��
    ��Щ�����ڱ��ļ�������
    ����ʽ�汾�У�����������ļ���
*/
#include "type.h"
#include <cstdio>
#include <iostream>
#include <fstream>
using namespace std;

int main() {
    vector<Argument> args;
    Argument a = {"12345t", BasicType::INTEGER, false};
    ProcedureProperty pp = {args};
    args.push_back(a);
    cout << pp.arguments.size() << endl;
    a.name = "str";
    a.type = BasicType::CHAR;
    a.vary = false;
    cout << args[0].name << endl;

    return 0;
}