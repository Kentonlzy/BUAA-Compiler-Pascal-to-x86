/*
    RegisterPool.h
    ��ʱ�Ĵ����� ����
*/

#ifndef REGISTER_POOL__H
#define REGISTER_POOL__H

#include "StackManager.h"
#include <vector>
#include <map>
#include <fstream>
#include <set>
using std::set;
using std::vector;
using std::map;
using std::ofstream;

// ��������Ϣ
struct Temporary {
    int offset;             // ������������ջ�ϵĻ�ַ��
    int baseoffset;         // ��������ջ�Ļ�ַ��
                            // ����ǵ�ǰ�㣬Ϊ 0���������㣬Ϊһ������
    string registerName;    // ���䵽�ļĴ�������û����Ϊ��
};

class RegisterPool {
public:

    // ���캯������һ������Ϊ��ʱ�Ĵ����ĸ���������Ϊ��ʱ�Ĵ���������
    RegisterPool(const vector<string> &registers, StackManager &stack, ofstream &out);
    // ÿ����һ���µĹ��̻�������Ҫ�������������
    void initialRegisterPool();
    // �� regist �Ĵ����ؼң��Ż�ջ�ϣ�
    void home(const string &registerName);
    // �����мĴ����ؼң��Ż�ջ�ϣ�
    void home();
    /* ����Ϊһ����ʱ���������������ڵļĴ�������
        ��������ʱ������ջ�ϣ����������һ���Ĵ���
        ������мĴ����ж�����Чֵ������ĳ����ʱ�����ؼң�Ȼ����ڳ����ļĴ�����Ҫ����ʱ����
        ����Ѹ���ʱ��������ջ�ռ䣬û������
        �����û�и������ʱ��������ջ�ռ䣬����������ռ䣬��ʱ��ı� pushCount ��ֵ */
    // Ҫ��Ҫ��ֵ�ڼĴ�����
    string getDestRegister(const string &temporary, bool wantValue = false);
    string getDestRegister(const string &variable, int offset,
                            int baseoffset = 0, bool wantValue = false);
    // ��Ҫ�������ֵ�ڼĴ����������ջ��
    string getDest(const string &temporary);
    string getDest(const string &variable, int offset, int baseoffset = 0);
    // ��ȡһ���յļĴ�����������������
    string getRegister();
    // ������������ļĴ���������Ĵ����صĵ���
    void lock(const string &registerName);
    void unlock(const string &registerName);
    // ���� lastOffset ��ֵ
    void setLastOffset(int);

private:
    // ��¼��ʱ��������ռ�֮ǰ��ƫ�ƣ�����������������ռ䣬
    // ����ʱ�١��١����䣬����Ҫ��¼���ǵ�ƫ��
    int lastOffset;
    map<string, string>::iterator registerit;
    StackManager &stack;    // ����ջ������
    ofstream &targetFile;   // Ŀ������ļ�
    map<string, Temporary> temporaries; // ��ʱ������Ϣ���������ƶ�Ӧ Temporary �ṹ
    map<string, string> registers;      // ��ʱ�Ĵ������Ĵ�������Ӧ������
    set<string> lockedRegisters;        // �����ļĴ�������Щ�Ĵ���������Ĵ����صĵ���
    // ȡһ���յļĴ���������������ܹ�ȷ���ڱ��У�����
    // ֻ����һ�룬δ�� temporaries ��
    string getRegister(const string &variable);
};

#endif // !REGISTER_POOL__H
