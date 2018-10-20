#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "LexicalAnalyzer.h"
#include "GrammarAnalyzer.h"
using namespace std;      

int main(int argc, char *argv[]) {
    // ���£���Դ�����ļ��Ĵ򿪹���
    string fname;           // Դ�����ļ�������
    if (argc > 1)           // �����в�����ʽ����Դ�����ļ���
        fname = argv[1];
    else                    // ����̨�䷽ʽ����Դ�����ļ���
        cin >> fname;
    ifstream fin(fname);    // ��Դ�����ļ�
    if (!fin.is_open()) {   // ����ļ��Ƿ�ɹ��򿪣���񣬱����˳�����
        cerr << "err: cannot open file: " << fname << endl;
        cerr << "Please check the path and file name." << endl;
        return 0;
    }
    // ���£��Խ������ļ��Ĵ򿪹���
    ofstream fout;
    if (argc > 2)           // �����в����������ļ���
        fout.open(argv[2]);
    else                    // Ĭ�Ͻ���ļ���
        fout.open(fname + "_result.txt");

    LexicalAnalyzer &lexicalAnalyzer = LexicalAnalyzer::initialLexicalAnalyzer(fin);
#ifdef STAGE_LEXICAL
    GrammarAnalyzer grammaAnalyzer(lexicalAnalyzer, fout);
#endif // STAGE_LEXICAL
    grammaAnalyzer.grammerAnalyze();
    fout.close();           // �رս���ļ�
    fin.close();            // �ر�Դ�����ļ�
    return 0;
}