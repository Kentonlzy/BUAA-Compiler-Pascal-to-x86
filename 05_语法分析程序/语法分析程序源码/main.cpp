#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "GrammarAnalyzer.h"
using namespace std;      

// �������в����йصģ�����һЩ����ѡ���������ļ�����
struct Options {
    ifstream sourceCode;
    ofstream intermediateCode;
    ofstream assemblyCode;
};

// ���������в���
bool handleOptions(int argc, char *argv[], Options &opt) {
    if (argc <= 1)
        return false;
    opt.sourceCode.open(argv[1]);
    opt.intermediateCode.open("result.txt");
    return true;
}

// �ر��ļ�
void closeFiles(Options &opt) {
    opt.sourceCode.close();
    opt.intermediateCode.close();
    opt.assemblyCode.close();
}

int main(int argc, char *argv[]) {
    // �������в����Ĵ������
    Options opt;
    handleOptions(argc, argv, opt);

    LexicalAnalyzer &lexicalAnalyzer = LexicalAnalyzer::initialLexicalAnalyzer(opt.sourceCode);
    ErrorHandler &errorHandler = ErrorHandler();
#ifdef STAGE_LEXICAL
    GrammarAnalyzer grammarAnalyzer(lexicalAnalyzer, opt.intermediateCode);
#elif defined(STAGE_GRAMMAR) 
    GrammarAnalyzer grammarAnalyzer(lexicalAnalyzer, errorHandler, opt.intermediateCode);
#endif 
    grammarAnalyzer.grammarAnalyze();

    closeFiles(opt);
    return 0;
}