/*
    type.h
    ���Ͷ��塣
*/               
#ifndef TYPE__H
#define TYPE__H

#include <string>
#include <vector>
using std::string;
using std::vector;

// �ʷ��������ص��·�������
namespace name_ST {
enum SymbolType { 
    ILLEGAL,            // �Ƿ��ַ�
    FINISH,             // �ļ���β

    PROCEDURE,          // procedure �ؼ���
    FUNCTION,           // function �ؼ���
    CONST,              // const �ؼ���   
    VAR,                // var �ؼ���
    ARRAY,              // array �ؼ���
    OF,                 // of �ؼ���
    INTEGER,            // integer �ؼ���
    CHAR,               // char �ؼ���
    BEGIN,              // begin �ؼ���
    END,                // end �ؼ���
    FOR,                // for �ؼ���
    DO,                 // do �ؼ���
    WHILE,              // while �ؼ���
    IF,                 // if �ؼ���
    THEN,               // then �ؼ���
    ELSE,               // else �ؼ���
    READ,               // read �ؼ���
    WRITE,              // write �ؼ���
    TO,                 // to �ؼ���
    DOWNTO,             // downto �ؼ���
    
    IDENTIFIER,         // ��ʶ��
    NUMBER,             // ����   
    STRING,             // �ַ���
    CHARATER,           // char ���͵��ַ�

    LEFT_BRACKET,       // ������
    RIGHT_BRACKET,      // ������
    LEFT_SQUARE,        // ������
    RIGHT_SQUARE,       // �ҷ�����
    SINGLE_QUOTE,       // ������                      
    DOUBLE_QUOTE,       // ˫����
    COMMA,              // ����
    SEMICOLON,          // �ֺ�
    PERIOD,             // ���
    PLUS,               // �Ӻ�
    MINUS,              // ����
    ASTERISK,           // �Ǻţ��˺�
    SLASH,              // б�ܣ�����
    COLON,              // ð��
    EQUAL,              // �Ⱥ�
    ASSIGN,             // ��ֵ��
    LESS,               // С�ں�
    LESS_EQUAL,         // С�ڵ��ں�
    GREATER,            // ���ں�
    GREATER_EQUAL,      // ���ڵ��ں�
    INEQUAL             // ���Ⱥ�
};
}

// ��ʶ��������
namespace name_IT {
enum IdentifierType{
    VAR,
    CONST,
    ARRAY,
    PROCEDURE,
    FUNCTION
};
}

// ��������
namespace name_BT {
enum BasicType {
    INTEGER,
    CHAR
};
}

// �˴���ǣ�������ʱ������һЩ����֮�󣬵���һ���﷨�ɷֽ���
// ���磺�ڴ�����ʽʱ����������д������� <���ʽ> �Ľ�β����������� EXPRESSION 
namespace name_EE{
enum ErrorEnd {
    NORMAL,                 // ������������ʾû�д��󣬲���Ҫ����
    BLOCK,                  // �ֳ���
    CONST_DECLARE,          // ����˵��
    CONST_DEFINE,           // ��������
    CONST,                  // ����
    CHAR,                   // �ַ�
    STRING,                 // �ַ���
    UNSIGNED,               // �޷�������
    VAR_DECLARE,            // ����˵��
    VAR_DEFINE,             // ��������
    TYPE,                   // ����
    BASIC_TYPE,             // ��������
    PROCEDURE_DECLARE,      // ����˵��
    FUNCTION_DECLARE,       // ����˵��
    PROCEDURE_HEAD,         // �����ײ�
    FUNCTION_HEAD,          // �����ײ�
    FORMAL_PARAMETER_LIST,  // ��ʽ������
    FORMAL_PARAMETER,       // ��ʽ����
    STATEMENT,              // ���
    ASSIGN_STATEMENT,       // ��ֵ���
    EXPRESSION,             // ���ʽ
    TERM,                   // ��
    FACTOR,                 // ����
    CALL_STATEMENT,         // �������
    REAL_PARAMETER_LIST,    // ʵ�ڲ�����
    CONDITION,              // ����
    RELATION_OPERATOR,      // ��ϵ�����
    CONDITION_STATEMENT,    // �������
    DO_WHILE_STATEMENT,     // do-while ���
    FOR_STATEMENT,          // for ���
    COMPOUND_STATEMENT,     // �������
    READ_STATEMENT,         // read ���
    WRITE_STATEMENT         // write ���
};
}

using name_ST::SymbolType;
using name_IT::IdentifierType;
using name_BT::BasicType;
using name_EE::ErrorEnd;

// ÿ����������Ϣ
struct Argument {
    string name;    // ������
    BasicType type; // ����
    bool vary;      // true ��ʾ����ַ��false ��ʾ��ֵ
};

struct STE {
    string name;            // ��ʶ������
    IdentifierType type;    // ��ʶ������

};

#endif // !TYPE__H