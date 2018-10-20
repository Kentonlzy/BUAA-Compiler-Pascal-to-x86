/*
    LexicalAnalyzer.h
    �ṩ�ʷ���������� class ������
*/
#ifndef LEXICAL_ANALYSIS__H
#define LEXICAL_ANALYSIS__H

#include <fstream>
using std::string;
using std::ifstream;

// �·�������
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

// �ʷ�������
class LexicalAnalyzer {
public:
    static LexicalAnalyzer &initialLexicalAnalyzer(ifstream &file); 
                                    // ����ģʽ��ԭ���ǣ�
                                    //  token.symbol ������̬���� strGot
                                    //  ��� LexicalAnalyzer.cpp
    void nextSymbol();      // ��ȡ��һ�����ţ����������ʹ��� symbolType��
                            //  ����ķ��Ŵ��� symbol
    void nextChar();        // ��ȡ��һ�� char ���͵��ַ���
                            //  ����ǺϷ��� char���� symbolType 
                            //  ��Ϊ CHARATER�������� ASCII ֵ���� token.number
                            //  ������ǺϷ��� char���� symbolType ��Ϊ ILLEGAL
    void nextString();      // ��ȡ��һ���ַ���������˫���ţ������Ƿ��ַ���˫���Ž�ֹ��
                            //  ����ֹ֮ǰ�Ĳ��ִ��� token.symbol������Ϊ�մ�
                            //  ��symbolType ��Ϊ STRING

private:
    ifstream &sourceFile;           // Դ�����ļ�������
    SymbolType symbolType;          // �·��ŵ�����
    union Token {                   // �·��ŵ����ݣ�ʹ��������������Ϊ��ͬ���ŵ����Ͳ�ͬ
        int number;         // �����͵�����ֵ
        string *symbol;     // �ַ������͵��ַ���ָ��
    } token;                 
    friend class GrammarAnalyzer;   // ��Ԫ�ࣺ�﷨������
                                    //  �﷨������Ҫֱ�ӻ�ȡ�·��ŵ����ͺ�����
                                        
    LexicalAnalyzer(ifstream &file);            // ˽�л����캯��������ģʽ
                                                //  ����ΪԴ�ļ��� ifstream ����
                                                //  ��������������Ч��
    LexicalAnalyzer(const LexicalAnalyzer &);   // ˽�л��������죬����ģʽ����ֹ����
    LexicalAnalyzer & operator = (const LexicalAnalyzer &);
                                                // ˽�л����ص� = ���������ֹ��ֵ
};

// ���£�Ϊ�ˡ��ʷ��������׶���ҵ��������ÿ�������һ�����뿪�� STAGE_LEXICAL
//  ���յı������У���ȡ���� STAGE_LEXICAL �Ķ���
#define STAGE_LEXICAL

#ifdef STAGE_LEXICAL
using std::ostream;
ostream &operator<< (ostream &ofs, SymbolType st);      // ���� SymbolType �� << �����
                                                        //  ʹ�������ö��������������
#endif // STAGE_LEXICAL


#endif  // !LEXICAL_NANLYSIS__H