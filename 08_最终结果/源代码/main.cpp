#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "GrammarAnalyzer.h"
#include "SemanticAnalyzer.h"
#include "TargetCodeGenerator.h"
using namespace std;      

#define WARNING     0X1
#define OPTIMIZE_1  0X2
#define OPTIMIZE_2  0x4

// �������в����йصģ�����һЩ����ѡ���������ļ�����
struct Options {
    int flags;
    ifstream sourceCode;
    ofstream intermediateCode;
    ofstream assemblyCode;
};

// ���������в���
bool handleOptions(int argc, char *argv[], Options &opt) {
    int p = 1;      // �����в���ָ��
    opt.flags = 0x7;
    if (argc < 4)
        return false;
    else if (argc >= 5 && argv[1][0] == '-') {
        p = 2;
        char *options = argv[1] + 1;
        while (*options != '\0') {
            switch (*options) {
            case 'd':
                options++;
                switch (*options) {
                case 'w':
                    opt.flags &= ~WARNING;
                    break;
                case 'o':
                    opt.flags &= ~OPTIMIZE_1;
                    break;
                case 'O':
                    opt.flags &= ~OPTIMIZE_2;
                    break;
                default:
                    return false;
                    break;
                }
                break;
            case 'w':
                opt.flags |= WARNING;
                break;
            case 'o':
                opt.flags |= OPTIMIZE_1;
                break;
            case 'O':
                opt.flags |= OPTIMIZE_2;
                break;
            default:
                return false;
                break;
            }
            options++;
        }
    }
    opt.sourceCode.open(argv[p++]);
    opt.intermediateCode.open(argv[p++]);
    opt.assemblyCode.open(argv[p]);
    return true;
}

// �ر��ļ�
void closeFiles(Options &opt) {
    opt.sourceCode.close();
    opt.intermediateCode.close();
    opt.assemblyCode.close();
}

// �÷�
void usage(char *order) {
    cerr << "usage: \n\t" << order << " ";
    cerr << "[- [[d]w] [[d]o] [[d]O] ] ";
    cerr << "<sourceFile> <intermediateFile> <assemblyFile>" << endl;
}

int main(int argc, char *argv[]) {
    // �������в����Ĵ������
    Options opt;
    if (!handleOptions(argc, argv, opt)) {
        usage(argv[0]);
        return 0;
    }

    // ��������ĳ�ʼ��
    // �ʷ�������
    LexicalAnalyzer &lexicalAnalyzer = LexicalAnalyzer::initialLexicalAnalyzer(opt.sourceCode);
    // ��������
    ErrorHandler &errorHandler = ErrorHandler::initialErrorHandler(
        lexicalAnalyzer, (opt.flags & WARNING)!=0);
    // ���ű������
    SymbolTableManager &symbolTableManager = SymbolTableManager::initialSymbolTableManager();
    // �ṹ������Ԫʽ
    Quadruples quadruples;
    // ���������
    SemanticAnalyzer &semanticAnalyzer = SemanticAnalyzer::initialSemanticAnalyzer(
        errorHandler, symbolTableManager, quadruples, lexicalAnalyzer);
    // �﷨������
    GrammarAnalyzer &grammarAnalyzer = GrammarAnalyzer::initialGrammarAnalyzer(
        lexicalAnalyzer, errorHandler, symbolTableManager, semanticAnalyzer);
    // Ŀ�����������
    TargetCodeGenerator targetCodeGenerator(opt.assemblyCode, quadruples, symbolTableManager);

    // ����
    grammarAnalyzer.grammarAnalyze();

    // ���������������������������ɴ���
    if (errorHandler.warningCount != 0)
        cerr << endl << errorHandler.warningCount << " WARNING in total" << endl;
    cerr << endl << errorHandler.errorCount << " ERROR in total" << endl;
    if (errorHandler.errorCount != 0)
        goto END;

    // �����Ԫʽ
    quadruples.output(opt.intermediateCode);

    // ����Ŀ�����
    targetCodeGenerator.generateCode();

END:
    closeFiles(opt);
    return 0;
}