/*
    SymbolTableManager.h
    ���ű�������Ľӿںͱ�������
*/
#ifndef SYMBOL_TABLE_MANAGER__H
#define SYMBOL_TABLE_MANAGER__H

#include "type.h"

class SymbolTableManager {
public:
    STE *ste;           // ����ָ�룬���ڷ��ű����������罻����Ϣ��
                        // ���ʱ�����鵽�ı���������
                        // ���ʱ��������ı���������

    static SymbolTableManager &initialSymbolTableManager();     // ����ģʽ
    void goIn(const string &);                                  // �� position �����ƽ�һ��
    void goOut();                                               // �� position �����˻�һ��
    bool isHere(const string &);                                // �жϱ��е�ǰ���Ƿ����������
    bool find(const string &);                                  // ����鵽���� true��
                                                                // ���鵽���� ste ָ����
    void insert();                                              // ���
    void defineArguments(STE *te, vector<Argument> &args);// ����������������ٸ���һ����Ϊ�ֲ�����
    string &encodeKey();                                        // �� ste ת��ΪΨһ��ʶ
    string &encodePosition();                                   // �� position ת��ΪΨһ��ʶ
    void decodeKey(const string &);                             // ��Ψһ��ʶת��Ϊ ste
    void addString(const string &name, const string &str);      // �� str ����Ϊ name ���������
    map<string, string> &getStringMap();                        // ���� stringMap ����������ʹ��

private:
    STE *position;      // ��ǰ����λ��
    SymbolTableManager();                                       // ���캯��
    SymbolTableManager(const SymbolTableManager &);             // ��������
    SymbolTableManager &operator= (const SymbolTableManager &); // = �����
};

#endif // !SYMBOL_TABLE_MANAGER__H
