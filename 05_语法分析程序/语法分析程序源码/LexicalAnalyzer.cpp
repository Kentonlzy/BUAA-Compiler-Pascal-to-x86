/*
    LexicalAnalyzer.cpp
    �ʷ���������ʵ�֡�
*/                    
#include "LexicalAnalyzer.h"
#include <cctype>
#include <map>
#include <string>
using namespace std;
using namespace name_ST;

static string strGot;   // �������ַ���
static char ch;         // ��һ�������������ģ���û�����������ַ�
                        //  �����������Ŀ�������Ч��

static map<string, SymbolType> reservedWords;   // �����ֱ�����ÿ����������������֮ǰ�Ĺ�ϵ

// ����ģʽ�������ṩ�ġ����캯����
LexicalAnalyzer &LexicalAnalyzer::initialLexicalAnalyzer(ifstream &file) {      
    static LexicalAnalyzer theLexicalAnalyzer(file);
    return theLexicalAnalyzer;
}
                
// �����Ĺ��캯��
LexicalAnalyzer::LexicalAnalyzer(ifstream &file): sourceFile(file) {
                                                // ���ļ���������Ϊ����
    // ��ʼ���к�Ϊ 1
    lineNumber = 1;
    // �������ļ��еĵ�һ���ַ�����
    ch = sourceFile.get();
    //��ʼ�������ֱ�                          
#define INSERT_RES(str, type)    \
        reservedWords.insert(pair<string, SymbolType>(str, type))   // ����꣬��������
    INSERT_RES("procedure", PROCEDURE);
    INSERT_RES("function", FUNCTION);
    INSERT_RES("const", CONST);
    INSERT_RES("var",  VAR);
    INSERT_RES("array", ARRAY);
    INSERT_RES("of", OF);
    INSERT_RES("integer", INTEGER);
    INSERT_RES("char", CHAR);
    INSERT_RES("begin", BEGIN);
    INSERT_RES("end", END);
    INSERT_RES("for", FOR);
    INSERT_RES("do", DO);
    INSERT_RES("while", WHILE);
    INSERT_RES("if", IF);
    INSERT_RES("then", THEN);
    INSERT_RES("else", ELSE);
    INSERT_RES("read", READ);
    INSERT_RES("write", WRITE);
    INSERT_RES("to", TO);
    INSERT_RES("downto", DOWNTO);
#undef  INSERT_RES                                                  // ȡ������ INSERT_RES ��
}

#define GET     ch = sourceFile.get()   // ����꣬�򻯴��ļ��ж�ȡһ���ַ��Ĳ���
#define PEEK    ch = sourceFile.peek()  // ����꣬�򻯲鿴�ļ�����һ���ַ��Ĳ���
#define ADD(c)  strGot.append(1, c)     // ����꣬�����ַ�����β����ַ��Ĳ���

// ��ȡ��һ�����ţ����������ʹ��� symbolType������ķ��Ŵ��� symbol
void LexicalAnalyzer::nextSymbol() {
    while (isspace(ch))
        GET;
    if (isdigit(ch)) {          // �����֣����������һ��������     
        symbolType = NUMBER;                // �÷�������Ϊ�����֡�        
        token.number = ch - '0';            // �������ַ�ת��Ϊ����
        for (GET; isdigit(ch); GET) 
            token.number = token.number * 10 + ch - '0';  
                                            // �������ַ�ת��Ϊ���������� number ��ֵ
        // ���ѭ������ʱ��ch �Ƿ������ַ�
        //  ���� ch �ǻ�δ������ַ��������߼���ȷ
    } else if (isalpha(ch)) {   // ����ĸ�����������һ���Ǳ�ʶ��������
        token.symbol = &strGot;             // ��ʱ�����ķ�����һ��Ҫ���� token.symbol ��
        strGot.clear();                     // ������ַ���
        ADD(ch);                            // �Ƚ���һ���ַ������ַ������Ա������ٶ�
        for (GET; isalnum(ch); GET)
            ADD(ch);                                                           
        // ���ѭ������ʱ��ch ��һ���µ�δ������ַ����߼���ȷ

        // ���£��ж϶����ķ����Ǳ�ʶ�����Ǳ����֣�����������ʹ��� symbolType
        map<string, SymbolType>::iterator it = reservedWords.find(strGot);
        if (it == reservedWords.end())      // ���ڱ����ֱ��У�˵���Ǳ�ʶ��
            symbolType = IDENTIFIER;
        else                                // �ڱ����ֱ���
            symbolType = it->second;        // Ҫ�ѱ����ֵľ������ʹ��� symbol
    } else {                    // ���������ţ����ݷ��ű����жϷ��ŵ�����
                                //  ÿ�ַ���Ψһ��Ӧһ�����ͣ����Բ��ش��� token
#ifdef STAGE_LEXICAL
        token.symbol = &strGot;             // �� token.symbol ָ���ַ���
        strGot.clear();
        ADD(ch);                            // �ȰѴ��жϵ��ַ������ַ���
#endif // STAGE_LEXICAL
        switch (ch) {
        // ���£������ switch ���У�ÿһ�����ִ����֮�󣬶�ʹ�� ch �Ǵ�������ַ�
        // ����� switch �����棬ͳһ�ٶ�һ�����ַ�����
        case '(':
            symbolType = LEFT_BRACKET;
            break;
        case ')':
            symbolType = RIGHT_BRACKET;
            break;
        case '[':
            symbolType = LEFT_SQUARE;
            break;
        case ']':
            symbolType = RIGHT_SQUARE;
            break;
        case '\'':
            symbolType = SINGLE_QUOTE;
            break;
        case '\"':
            symbolType = DOUBLE_QUOTE;
            break;
        case ',':
            symbolType = COMMA;
            break;
        case ';':
            symbolType = SEMICOLON;
            break;
        case '.':
            symbolType = PERIOD;
            break;
        case '+':
            symbolType = PLUS;
            break;
        case '-':
            symbolType = MINUS;
            break;
        case '*':
            symbolType = ASTERISK;
            break;
        case '/':
            symbolType = SLASH;
            break;
        case ':':
            PEEK;                           // ���ﲻ���� get��
                                            //  �������ͨ��ð�ţ��� get ������ putback��
                                            // ������ ch ���������Ŀ�ľ��ǲ��ض��������˻�ȥ��
                                            //  ������� ch��ȫ��ʹ�� peek��Ч�ʲ��� ch ��
                                            // ����������� ch ���������ұ����� putback!
            if (ch == '=') {
#ifdef STAGE_LEXICAL
                ADD(ch);
#endif // STAGE_LEXICAL
                GET;                        // ����Ǹ�ֵ�����ٰѵȺ�ȡ����
                symbolType = ASSIGN;
            } else
                symbolType = COLON;         // ����ͨ��ð�ţ������ٶ���һ���ַ�
            break;
        case '=':
            symbolType = EQUAL;
            break;
        case '<':
            PEEK;                           // ͬ�ϣ���������� peek ������ get��
            switch (ch) {
            case '=':  
#ifdef STAGE_LEXICAL
                ADD(ch);
#endif // STAGE_LEXICAL
                GET;                        // ע����� get
                symbolType = LESS_EQUAL;
                break;
            case '>':  
#ifdef STAGE_LEXICAL
                ADD(ch);
#endif // STAGE_LEXICAL
                GET;                        // ע����� get
                symbolType = INEQUAL;
                break;
            default:
                symbolType = LESS;          // ע������û�� get
                break;
            }
            break;
        case '>':
            PEEK;                           // ͬ�ϣ����ﲻ���� get!
            if (ch == '=') {
#ifdef STAGE_LEXICAL
                ADD(ch);
#endif // STAGE_LEXICAL
                GET;                        // get!
                symbolType = GREATER_EQUAL;
            } else
                symbolType = GREATER;       // û�� get!
            break;
        case EOF:                           // �����ļ���β
            symbolType = FINISH;
            break;
        default:                            // ���������������ַ����ַ���
                                            //  ����������������ַ���һ���ǷǷ��ַ�
            symbolType = ILLEGAL;
            break;
        }
        // ���ϣ�switch �����֮��ch Ϊ��������ַ�
        GET;  // �˴���Ҫ�� get һ���µ��ַ�����
    }                                
}

// ��ȡһ���ַ���������˫���ţ������Ƿ��ַ���˫���Ž�ֹ��
//  ����ֹ֮ǰ�Ĳ��ִ��� token.symbol������Ϊ�մ�
//  ��symbolType ��Ϊ STRING
void LexicalAnalyzer::nextString() {
    token.symbol = &strGot; // �� token.symbol ָ���¶����ַ���
    strGot.clear();         // ����ַ����Ա���       
    while (ch == 32 || ch == 33 || (ch >= 35 && ch <= 126)) {
        ADD(ch);
        GET;
    }
    // ���ѭ������ʱ��ch ��һ���µ�δ������ַ����߼���ȷ
#ifdef STAGE_LEXICAL
    symbolType = STRING;
#else // !STAGE_LEXCIAL
    if (ch != '\"') {           // �����Ƿ��ַ�
        symbolType = ILLEGAL;   // ���� ILLEGAL ����
        strGot.clear();
        ADD(ch);                // ���Ƿ����ַ����� token �У�
                                //  ��ʱ token ��ֻ����һ���Ƿ��ַ�
        GET;                    // ��Ҫ�����ٶ�һ�����ã�����
    } else
        symbolType = STRING;    // ���������� STRING
#endif // STAGE_LEXICAL
}

// ��ȡ��һ�� char ���͵��ַ���
// ����ǺϷ��� char���� symbolType ��Ϊ CHARATER��
//  ������ ASCII ֵ���� token.number
// ������ǺϷ��� char���� symbolType ��Ϊ ILLEGAL
void LexicalAnalyzer::nextChar() {
    while (isspace(ch))
        GET;                    // �����հ׷�
#ifdef STAGE_LEXICAL
    token.symbol = &strGot;
    strGot.clear();
    ADD(ch);
#else // !STAGE_LEXICAL
    token.number = ch;          // ���ַ��� ASCII ֵ���� token.number
#endif // STAGE_LEXCIAL
    if (isalnum(ch))            // �ǺϷ��ַ�������ĸ�������ַ�
        symbolType = CHARATER;
    else                        // �Ƿ��ַ�
        symbolType = ILLEGAL;
    GET;                        // get һ���µ��ַ����ã���
}