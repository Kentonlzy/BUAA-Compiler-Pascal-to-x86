/*
    StackManager.h
    ����ջ������
    ���� esp ָ�룬�ṩһЩС���������Ĺ����ǣ�
        ʹ������Ҫ�� esp ���еĲ������Լ��� esp ��ص�Ѱַ�������������ܡ�
        ʹ����ֻ��Ҫ��ʹ��ǰ��ʼ����һ��Ȼ��ֻ��Ҫ�����ṩ�൱��ջ�׵�ƫ�ƣ�
            ���ܻ�ȡ��� esp �ĵ�ַ
    Ҫ���� StackManager ��������������ѹջ����ջ���Լ��� esp ָ��ļӼ�����
        ������ʹ�������ṩ�Ľӿڡ�
*/

#ifndef STACK_MANAGER__H
#define STACK_MANAGER__H

#include <string>
#include <fstream>
using std::string;
using std::ofstream;

/////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!
// ������Ǿ����ع��ģ�����
// ֮ǰ����ջֻ�� esp һ��ָ��ά�������������в�ͨ������
// �ع�֮�󣬸�Ϊ�� esp �� ebp ����ָ��ά��
// Ϊ����֮ǰ�İ汾���ݣ�������� baseoffset ���� offset Ϊ��������Զ��ټ�һ��
// ԭ�����£�
// �ڷ��ص�ַ�����棬����һ�� ebp����ʱ�� offset ��Ϊ 0��Ϊ��Ѱַ����
// �������ƫ��Ϊ��������Ϊ����һ�� ebp������Ӧ�ö��һ�� 4.
/////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!
class StackManager {
public:
    StackManager(ofstream &file);
    // ִ��������ָ����൱�� esp ָ�����ջ��ջ�ף�
    // �൱�ڽ�����һ���µ�ջ֡���Ժ�Ĳ�����������µ�ջ�ϣ�
    // �˺󣬸���ƫ��Ҳ����������ջ��ջ�׶��Եġ�
    void initialStack();    
    // ���� push ָ��, push value
    void push(const string &value);         
    // ���� pop ָ��, pop regist
    void pop(const string &registerName);   
    // ��������ָ� sub esp, n; ���� n = count * 4
    void allocN(int count);         
    // ��������ָ� add esp, n; ���� n = count * 4
    void freeN(int count);          
    // ���ص�ǰ esp �����ջ�׵�ƫ�ƣ�ע���� 4 ���ֽ�Ϊһ����λ��
    //  ���緵�� n�����൱�� esp == ebp + n * 4
    int getOffset();                
    // ����һ������ [ebp-n] ���ַ�����
    //  ���� n == offset * 4
    string getAddress(const string &dest, int offset, int baseoffset = 0);
    // ��ȡ����������ջ�ϵĵ�ַ������ַ���� dest �Ĵ���
    void getLocationOnStack(const string &dest, int offset, int baseoffset = 0);

private:
    ofstream &targetFile;
    int pushCount;
};

#endif // !STACK_MANAGER__H
