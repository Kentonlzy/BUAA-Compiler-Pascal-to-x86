/*
    Quadruples.h
    �ṹ������Ԫʽ
    ������ʽ��֯
*/
#ifndef QUADRUPLES__H
#define QUADRUPLES__H

#include "type.h"
#include <fstream>

struct Quadruple {
    Operator op;
    string oper1;
    string oper2;
    string dest;
    Quadruple(Operator ope, const string &o1, const string &o2, const string &d): 
        op(ope), oper1(o1), oper2(o2), dest(d), prenode(NULL), sucnode(NULL) {}
private:
    // ������ṹ��ص�ָ����
    Quadruple *prenode; // ǰ���ڵ�
    Quadruple *sucnode; // ��̽ڵ�
    // ��Ԫ�࣬���Բ�������ṹ
    friend class Quadruples;
};

class Quadruples {
public:
    void addTail(Quadruple *);      // ����β��һ����Ԫʽ
    void output(std::ofstream &);   // �����Ԫʽ���ļ�
    Quadruples();                   // ���캯��
    ~Quadruples();                  // ��������
private:
    Quadruple *head;    // ��Ԫʽ�����ͷ�ڵ�
    Quadruple *tail;    // ��Ԫʽ�����β�ڵ�
    // ���ÿ�������
    Quadruples(const Quadruples &);
};

#endif // !QUADRUPLES__H
