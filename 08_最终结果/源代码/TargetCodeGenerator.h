/*
    TargetCodeGenerator.h
    Ŀ�����������
*/
#ifndef TARGET_CODE_GENERATOR__H
#define TARGET_CODE_GENERATOR__H

#include "SymbolTableManager.h"
#include "Quadruples.h"
#include "RegisterPool.h"
#include <fstream>
#include <string>
using std::ofstream;
using std::string;

class TargetCodeGenerator {
public:
    TargetCodeGenerator(ofstream &, Quadruples &, SymbolTableManager &);
    void generateCode();

private:
    ofstream &targetFile;
    StackManager stack;
    RegisterPool *registerPool;
    Quadruples &quadruples;
    SymbolTableManager &table;
    Quadruples::iterator qit;
    int level;
    int displayOffset;
    vector<Argument> *args;
    string procname;
    vector<string> parameters;

    /* һЩ�ڲ��õ�С���� */
    // ����ÿ���ļ��Ŀ�ͷ����
    void printAssembleHead();
    // ������Ԫʽ�е�һ������������ ��������������ʱ����������Ҫ���ǼĴ���
    void getOper(const string &oldformat, string &newformat);
    // ������Ԫʽ�е�Ŀ�Ĳ�������Ҫ���ǼĴ���
    void getDest(const string &oldformat, string &newformat, bool wantValue = false);
    // ����ָ��
    void divInstruction();
    // ��ָ����Ԫʽ����
    void threeOperatorInstruction(void (*func)(ofstream &, 
                                            const string &, const string &));
    // NEG ָ��
    void negInstruction();
    // ASS ָ��
    void assInstruction();
    // GA ָ��
    void gaInstructioin();
    // ASFA ָ��
    void asfaInstruction();
    // ASTA ָ��
    void astaInstruction();
    // FAS ָ��
    void fasInstruction();
    // CUT ָ��
    void cutInstruction();
    // LAB ָ��
    void labInstruction();
    // �������/����ͷ�� �����ǩ�����üĴ����غͶ�ջ��������
    // ���ű�����������
    // ���� level, args, displayOffset, procname.
    void handleHead();
    // ��ʼ�� �ֲ����� �� ��ʱ����
    void initialVariables();
    // CMP ָ��
    void cmpInstruction();
    // ��תָ��
    void jumpInstruction(void (*func)(ofstream &, const string &));
    // ���� display ��
    void buildDisplay(int destLevel);
    // ���ݲ���
    void passParameters();
    // CALP ָ��
    void calpInstruction();
    // CALF ָ��
    void calfInstruction();
    // RTN ָ����������ջ�Ĵ���
    void rtnInstruction();
    // ��дָ��
    void readOrWrite(const string &functionname,
        const string &formatString, const string &content);
    void readOrWriteInstruction(const string &function, const string &formatString);
    // NL ָ��
    void nlInstruction();
};

#endif // !TARGET_CODE_GENERATOR__H
