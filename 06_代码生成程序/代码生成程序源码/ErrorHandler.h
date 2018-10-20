/*
    ErrorHandler.h
    ���� ������������ �༰����صĺ�
*/

#ifndef ERROR_HANDLER__H
#define ERROR_HANDLER__H

#include "LexicalAnalyzer.h"
#include <map>
using std::map;

class ErrorHandler {
public:
    // �������
    int errorCount;
    int warningCount;
    // skip �����ɽ���ʱ�� SymbolType ������ֵ���ձ�
    map<SymbolType, ErrorEnd> endmap;
    void addEndmap(SymbolType, ErrorEnd);   // �� endmap ���һ�ԣ�������ظ�������
    void removeEndmap(SymbolType);          // �� endmap ���Ƴ� SymbolType�����û�У��޲���
    // ����ģʽ
    static ErrorHandler &initialErrorHandler(LexicalAnalyzer &, bool warning);
    // ��������
    void printError(ErrorType);
    void printError(ErrorType, const string &);
    ErrorEnd skip();
    // ���溯��
    void ErrorHandler::printWarning(ErrorType err);
    void ErrorHandler::printWarning(ErrorType err, const string &msg);

private:
    // �ʷ�������
    LexicalAnalyzer &lexicalAnalyzer;
    // ���濪��
    bool warning;
    // ����ģʽ
    ErrorHandler(LexicalAnalyzer &, bool warning);
    ErrorHandler(const ErrorHandler &);
    ErrorHandler &operator= (const ErrorHandler &);
};

#endif // !ERROR_HANDLER__H