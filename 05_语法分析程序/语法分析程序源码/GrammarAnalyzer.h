/*
    GrammarAnalyzer.h
    �ṩ�﷨��������� calss ����
*/
#ifndef GRAMMA_ANALYZER__H
#define GRAMMA_ANALYZER__H

// ��ֹ�� ���﷨������ ��ҵ��Ҫʵ�ֵĶ���������Ժ󻹿��ܻ�ġ�
#define STAGE_GRAMMAR
////////////////////////////////////////////////////

#include <fstream>
#include "LexicalAnalyzer.h"
#include "ErrorHandler.h"
using std::ofstream;

// �﷨������
class GrammarAnalyzer {

public:                   

#ifdef STAGE_LEXICAL
    GrammarAnalyzer(LexicalAnalyzer &aLexicalAnalyzer, ofstream &fout);    
#elif defined(STAGE_GRAMMAR)
    GrammarAnalyzer(LexicalAnalyzer &, ErrorHandler &, ofstream &);
#endif // STAGE_LEXICAL

    void grammarAnalyze();

private:                                          
    int symbolCount;                    // ���ż���
    LexicalAnalyzer &lexicalAnalyzer;   // �ʷ����������﷨����ʹ��
#ifndef STAGE_LEXICAL
    ErrorHandler &errorHandler;         // ��������
#endif // !STAGE_LEXICAL

#if defined(STAGE_LEXICAL) || defined(STAGE_GRAMMAR)
    ofstream &resultFile;                               
#endif // STAGE_LEXICAL || STAGE_GRAMMAR

    // ���£����﷨�ɷֵķ���
    void blockAnalyze();
    void constDeclareAnalyze();
    void constDefineAnalyze();
    void constAnalyze();
    void charAnalyze();
    void stringAnalyze();
    void unsignedAnalyze();
    void varDeclareAnalyze();
    void varDefineAnalyze();
    void typeAnalyze();
    void basicTypeAnalyze();
    void procedureDeclareAnalyze();
    void functionDeclareAnalyze();
    void procedureHeadAnalyze();
    void functionHeadAnalyze();
    void formalParameterListAnalyze();
    void formalParameterAnalyze();
    void statementAnalyze();
    void assignStatementAnalyze();
    void expressionAnalyze();
    void termAnalyze();
    void factorAnalyze();
    void callStatementAnalyze();
    void realParameterListAnalyze();
    void conditionAnalyze();
    void relationOperatorAnalyze();
    void conditionStatementAnalyze();
    void doWhileStatementAnalyze();
    void forStatementAnalyze();
    void compoundStatementAnalyze();
    void readStatementAnalyze();
    void writeStatementAnalyze();

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