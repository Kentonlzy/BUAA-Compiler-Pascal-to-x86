/*
    GrammarAnalyzer.cpp
    �﷨��������ʵ��
*/
#include "GrammarAnalyzer.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;


#ifdef STAGE_LEXICAL
// ���캯��
GrammarAnalyzer::GrammarAnalyzer(LexicalAnalyzer &aLexicalAnalyzer, ofstream &fout): 
    lexicalAnalyzer(aLexicalAnalyzer), resultFile(fout), symbolCount(0) {}
#endif // STAGE_LEXICAL
  
#ifdef STAGE_LEXICAL

#define OUT_BASE(width, content, tail) {    \
    cout << setw(width) << content << tail;         \
    resultFile << setw(width) << content << tail;   \
} 
#define OUT(content) {                      \
    OUT_BASE(3, symbolCount, " ");          \
    OUT_BASE(15, SYMBOL_TYPE, "    ");      \
    OUT_BASE(0, content, endl);             \
}                                           // ����꣬�򻯴˴���������������Ĳ���

// ִ�������ʱ��Ҫ��֤��ǰ�����Ѿ�������ϣ���û���µ�
// �������ִ����Ϻ�Ŀ������Ѿ�������ϣ���û���µ�
#define SCAN_TO(type) {                     \
    do {           /* ��������������һ���ַ� */ \
        NEXTSYM;                            \
        if (SYMBOL_TYPE == NUMBER) {        \
            OUT(TOKEN_NUMBER);              \
        } else {                            \
            OUT(TOKEN_SYMBOL);              \
        }                                   \
    } while (SYMBOL_TYPE != type);          \
    /* ��ʱĿ������Ѿ�������� */              \
}
        

// �﷨�������������������������ĺ��ĺ�����  
// ������ʷ������Ľ��������ݣ���Ļ��һ�ݣ��ļ���һ��
void GrammarAnalyzer::grammerAnalyze() {
    NEXTSYM;                                        // �ȶ���һ������
    while (SYMBOL_TYPE != FINISH) {  // ���ļ�������Ϊ���������־
        switch (SYMBOL_TYPE) {                      // �жϷ��ŵ����ͣ��������ʲô
        case NUMBER:
            OUT(TOKEN_NUMBER);                      // ��������ʽ���� token
            NEXTSYM;                                // ��Ҫ�����ٶ�һ���µı��ã�
            break;                                              
        default:            
            OUT(TOKEN_SYMBOL);                      // ���ַ�����ʽ���� token     
                                                    // �Ȱ� token ��������ж�����ʲô
            // �˴����ż��ٶ�һ�����ַ���Ҫ���ݶ����ķ��ž���Ҫ��ʲô
            switch (SYMBOL_TYPE) {
            case SINGLE_QUOTE:
                NEXTCH;                             // �������ţ����ͳ��ַ���Ҫ�ٶ�һ���ַ�
                OUT(TOKEN_SYMBOL);                  // �Ѷ���������ַ����
                SCAN_TO(SINGLE_QUOTE);              // ɨ�裬ֱ��������һ��������
                NEXTSYM;                            // �ٶ�һ���·��ű���
                break;
            case DOUBLE_QUOTE:
                NEXTSTR;                            // ��˫���ţ�Ҫ������Ĳ��ֵ����ַ�������
                OUT(TOKEN_SYMBOL);                  // �Ѷ���������ַ������
                SCAN_TO(DOUBLE_QUOTE);              // ɨ�裬ֱ��������һ��˫����
                NEXTSYM;                            // �ٶ�һ���·��ű���
                break;
            default:                                      
                NEXTSYM;                            // �ٶ�һ���µı��ã�ǧ������������
                break;
            }
        }
    }  
}   

// ���� SymbolType �� << �����
ostream &operator<< (ostream &ofs, SymbolType st) {
    switch (st) {   

#define case(type) case type:       \
    ofs << #type;                   \
    break                           // ����꣬�� case ��������
                   
    case(ILLEGAL);
    case(FINISH);
    case(PROCEDURE);
    case(FUNCTION);
    case(CONST);
    case(VAR);
    case(ARRAY);
    case(OF);
    case(INTEGER);
    case(CHAR);
    case(BEGIN);
    case(END);
    case(FOR);
    case(DO);
    case(WHILE);
    case(IF);
    case(THEN);
    case(ELSE);
    case(READ);
    case(WRITE);
    case(IDENTIFIER);
    case(NUMBER);
    case(STRING);
    case(CHARATER);
    case(LEFT_BRACKET);
    case(RIGHT_BRACKET);
    case(LEFT_SQUARE);
    case(RIGHT_SQUARE);
    case(SINGLE_QUOTE);
    case(DOUBLE_QUOTE);
    case(COMMA);
    case(SEMICOLON);
    case(PERIOD);
    case(PLUS);
    case(MINUS);
    case(ASTERISK);
    case(SLASH);
    case(COLON);
    case(EQUAL);
    case(ASSIGN);
    case(LESS);
    case(LESS_EQUAL);
    case(GREATER);
    case(GREATER_EQUAL);
    case(INEQUAL);                
#undef case                       // ȡ������ case���ó��������
    default:
        break;
    }
    return ofs;
}
                                           
#else  // !STAGE_LEXICAL
                                                 
// �﷨�������������������������ĺ��ĺ�����  
void GrammarAnalyzer::grammarAnalyze() {}

#endif // STAGE_LEXICAL