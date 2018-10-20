/*
    GrammarAnalyzer.h
    �ṩ�﷨��������� calss ����
*/
#ifndef GRAMMA_ANALYZER__H
#define GRAMMA_ANALYZER__H

#include <fstream>
#include "LexicalAnalyzer.h"
using std::ofstream;

// �﷨������
class GrammarAnalyzer {

public:                   

#ifdef STAGE_LEXICAL  
    GrammarAnalyzer(LexicalAnalyzer &aLexicalAnalyzer, ofstream &fout);    
#endif // STAGE_LEXICAL

    void grammerAnalyze();

private:                                          
    int symbolCount;                    // ���ż���
    LexicalAnalyzer &lexicalAnalyzer;   // �ʷ����������﷨����ʹ��      

#ifdef STAGE_LEXICAL
    ofstream &resultFile;
#endif // STAGE_LEXICAL

};                
                                                    
#define TOKEN_SYMBOL    (*lexicalAnalyzer.token.symbol)     // string ���͵� token
#define TOKEN_NUMBER    (lexicalAnalyzer.token.number)      // int ���͵� token
#define SYMBOL_TYPE     (lexicalAnalyzer.symbolType)        // ���ŵ� SymbotType ����ֵ
                                            // ����꣬�򻯶Է��ź����ͻ�ȡ    

#define NEXTSYM {                           \
    lexicalAnalyzer.nextSymbol();           \
    symbolCount++;                          \
}
#define NEXTSTR {                           \
    lexicalAnalyzer.nextString();           \
    symbolCount++;                          \
}
#define NEXTCH {                            \
    lexicalAnalyzer.nextChar();             \
    symbolCount++;                          \
}                                           // ����꣬�򻯻�ȡ��һ�����ŵĲ���

#endif // !GRAMMA_ANALYZER__H