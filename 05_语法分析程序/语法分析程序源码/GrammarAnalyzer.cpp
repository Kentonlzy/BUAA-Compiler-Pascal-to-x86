/*
    GrammarAnalyzer.cpp
    �﷨��������ʵ��
*/
#include "GrammarAnalyzer.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

                           
// ���캯��
#ifdef STAGE_LEXICAL
GrammarAnalyzer::GrammarAnalyzer(LexicalAnalyzer &aLexicalAnalyzer, ofstream &fout): 
    lexicalAnalyzer(aLexicalAnalyzer), resultFile(fout), symbolCount(0) {}
#elif defined (STAGE_GRAMMAR)
GrammarAnalyzer::GrammarAnalyzer(LexicalAnalyzer &la, ErrorHandler &eh, ofstream &ofs):
    lexicalAnalyzer(la), errorHandler(eh), resultFile(ofs), symbolCount(0) {}
#endif
  
#ifdef STAGE_LEXICAL
using namespace name_ST;

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
void GrammarAnalyzer::grammarAnalyze() {
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
    case(TO);
    case(DOWNTO);
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
                                           
#elif defined(STAGE_GRAMMAR)

// �����̨���ļ�������﷨�ɷ���Ϣ
#define OUT(item) {                                     \
    cout << "This is a " << item << "!" << endl;        \
    resultFile << "This is a " << item << "!" << endl;  \
}

// ������
#define ERROR {                                         \
    cout << '\a' << "!!!!!!!" << endl;                  \
    resultFile << "!!!!!!!!!!!!!!!!!!!!!!" << endl;     \
}

// �﷨�������������������������ĺ��ĺ�����  
// ����ֻ�����﷨��û�����崦��
void GrammarAnalyzer::grammarAnalyze() {
    NEXTSYM;            // ��һ�����ű��ã�������������
    // �ֳ���
    blockAnalyze();
    // ���
    if (SYMBOL_TYPE != SymbolType::PERIOD) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }
    NEXTSYM;
    while (SYMBOL_TYPE != SymbolType::FINISH) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        NEXTSYM;
    }
    OUT("program");
}

#endif

// ���ڼ������﷨�ɷֵĴ�����
static ErrorEnd err;

// �� err ��Ϊ NORMAL����û�д���
#define CLEAR   err = ErrorEnd::NORMAL

// ���� err ������Ӧ�������������Ϊ�˴��븴��
#define TEST(item) {                    \
    if (err != ErrorEnd::NORMAL) {      \
        if (err == item)                \
            CLEAR;                      \
        return;                         \
    }                                   \
}   

// <�ֳ���> ����
void GrammarAnalyzer::blockAnalyze() {
    // ����˵������
    if (SYMBOL_TYPE == SymbolType::CONST) {
        constDeclareAnalyze();
        TEST(ErrorEnd::BLOCK);
    }
    // ����˵������
    if (SYMBOL_TYPE == SymbolType::VAR) {
        varDeclareAnalyze();
        TEST(ErrorEnd::BLOCK);
    }
    // ���̡�����˵������
    while (true) {
        switch (SYMBOL_TYPE) {
        case SymbolType::FUNCTION:
            functionDeclareAnalyze();
            TEST(ErrorEnd::BLOCK);
            break;
        case SymbolType::PROCEDURE:
            procedureDeclareAnalyze();
            TEST(ErrorEnd::BLOCK);
            break;
        default:
            goto END_DECLARE;
            break;
        }
    }
END_DECLARE:
    // ������䣬��ʶ�� BEGIN
    compoundStatementAnalyze();
    TEST(ErrorEnd::BLOCK);
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("block");
#endif // STAGE_GRAMMAR      
}

// <����˵������> ����
void GrammarAnalyzer::constDeclareAnalyze() {
    // const �ؼ���
    if (SYMBOL_TYPE != SymbolType::CONST) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ����Ҫ��һ�� <��������>
    constDefineAnalyze();
    TEST(ErrorEnd::CONST_DECLARE);
    // �Ժ��� <��������> �Ĵ���
    while (SYMBOL_TYPE == SymbolType::COMMA) {
        NEXTSYM;
        constDefineAnalyze();
        TEST(ErrorEnd::CONST_DECLARE);
    }
    // ʶ���β�ķֺ�
    if (SYMBOL_TYPE != SymbolType::SEMICOLON) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("const declaration section");
#endif // STAGE_GRAMMAR      
}

// <��������> ����
void GrammarAnalyzer::constDefineAnalyze() {
    // <��ʶ��>
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else {
        NEXTSYM;
    }
    // �Ⱥ�
    if (SYMBOL_TYPE != SymbolType::EQUAL) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // <����>
    constAnalyze();
    TEST(ErrorEnd::CONST_DEFINE);
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("const defination");
#endif // STAGE_GRAMMAR
}

// <����> ����
void GrammarAnalyzer::constAnalyze() {
    switch (SYMBOL_TYPE) {
    // ������
    case SymbolType::PLUS: case SymbolType::MINUS:
        NEXTSYM;
        unsignedAnalyze();
        TEST(ErrorEnd::CONST);
        break;
    // �޷�������
    case SymbolType::NUMBER:
        unsignedAnalyze();
        TEST(ErrorEnd::CONST);
        break;
    // �ַ�����ʶ������
    case SymbolType::SINGLE_QUOTE:
        charAnalyze();
        TEST(ErrorEnd::CONST);
        break;
    // �﷨����
    default:
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        break;
    }
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("constant");
#endif // STAGE_GRAMMAR
}

// <�ַ�> ����
void GrammarAnalyzer::charAnalyze() {
    // ������
    if (SYMBOL_TYPE != SymbolType::SINGLE_QUOTE) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }
    // Ĭ�������е����ţ�����
    NEXTCH;
    NEXTSYM;
    // ʶ������
    if (SYMBOL_TYPE != SymbolType::SINGLE_QUOTE) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("charactor");
#endif // STAGE_GRAMMAR
}

// <�ַ���> ����
void GrammarAnalyzer::stringAnalyze() {
    // ��˫����
    if (SYMBOL_TYPE != SymbolType::DOUBLE_QUOTE) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }
    // Ĭ����˫���ţ�����
    NEXTSTR;
    NEXTSYM;
    // ʶ��˫����
    if (SYMBOL_TYPE != SymbolType::DOUBLE_QUOTE) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("string");
#endif // STAGE_GRAMMAR               
}

// <�޷�������> ����
void GrammarAnalyzer::unsignedAnalyze() {
    // ���ж��ǲ���������������������������������������������
    if (SYMBOL_TYPE != SymbolType::NUMBER) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else {
        // �ضϵȴ���      
        NEXTSYM;
    }
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("unsigned integer");
#endif // STAGE_GRAMMAR     
}

// <����˵������> ����
void GrammarAnalyzer::varDeclareAnalyze() {
    // var �ؼ���
    if (SYMBOL_TYPE != SymbolType::VAR) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ��һ�� <����˵�������壩> �Ĵ���
    varDefineAnalyze();
    TEST(ErrorEnd::VAR_DECLARE);
    // ʶ��ֺ�
    if (SYMBOL_TYPE != SymbolType::SEMICOLON) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // �������е� <����˵��> ���ֺŵĴ���
    while (SYMBOL_TYPE == SymbolType::IDENTIFIER) {
        varDefineAnalyze();
        TEST(ErrorEnd::VAR_DECLARE);
        if (SYMBOL_TYPE != SymbolType::SEMICOLON) {
            ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        } else
            NEXTSYM;
    }
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("variable declaration section");
#endif // STAGE_GRAMMAR      
}

// <����˵�������壩> ����
void GrammarAnalyzer::varDefineAnalyze() { 
    // �����һ����ʶ��
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // �����Զ��ŷָ��Ķ����ʶ��
    while (SYMBOL_TYPE == SymbolType::COMMA) {
        NEXTSYM;
        if (SYMBOL_TYPE == SymbolType::IDENTIFIER) {
            NEXTSYM;
        } else {
            ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        }
    }
    // ʶ��ð��
    if (SYMBOL_TYPE != SymbolType::COLON) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // Ĭ����ð�ţ�����
    typeAnalyze();
    TEST(ErrorEnd::VAR_DEFINE);
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("variable defination");
#endif // STAGE_GRAMMAR     
}

// <����> ����
void GrammarAnalyzer::typeAnalyze() {
    if (SYMBOL_TYPE == SymbolType::ARRAY) {     // ����
        NEXTSYM;
        // ʶ��������
        if (SYMBOL_TYPE != SymbolType::LEFT_SQUARE) {
            ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        } else
            NEXTSYM;
        // Ĭ���������ţ�����
        unsignedAnalyze();   
        TEST(ErrorEnd::TYPE);
        // ʶ��������
        if (SYMBOL_TYPE != SymbolType::RIGHT_SQUARE) {
            ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        } else
            NEXTSYM;
        // Ĭ���������ţ�����
        // ʶ�� of
        if (SYMBOL_TYPE != SymbolType::OF) {
            ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        } else
            NEXTSYM;
        // Ĭ���� of������
        basicTypeAnalyze();
        TEST(ErrorEnd::TYPE);
    } else {                                    // ��������
        basicTypeAnalyze();
        TEST(ErrorEnd::TYPE);
    }
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("type");
#endif // STAGE_GRAMMAR
}

// <��������> ����
void GrammarAnalyzer::basicTypeAnalyze() {
    switch (SYMBOL_TYPE) {
    case SymbolType::INTEGER:
        NEXTSYM;
        break;
    case SymbolType::CHAR:
        NEXTSYM;
        break;
    default:
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        break;
    }
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("basic type");
#endif // STAGE_GRAMMAR

}

// <����˵������>
void GrammarAnalyzer::procedureDeclareAnalyze() {
    // �����ײ�
    procedureHeadAnalyze();
    TEST(ErrorEnd::PROCEDURE_DECLARE);
    // �ֳ���
    blockAnalyze();
    TEST(ErrorEnd::PROCEDURE_DECLARE);
    // �ֺ�
    if (SYMBOL_TYPE != SymbolType::SEMICOLON) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("procedure declaration section");
#endif // STAGE_GRAMMAR          
}

// <����˵������> ����
void GrammarAnalyzer::functionDeclareAnalyze() {
    // �����ײ�
    functionHeadAnalyze();
    TEST(ErrorEnd::FUNCTION_DECLARE);
    // �ֳ���
    blockAnalyze();
    TEST(ErrorEnd::FUNCTION_DECLARE);
    // �ֺ�
    if (SYMBOL_TYPE != SymbolType::SEMICOLON) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;

#ifdef STAGE_GRAMMAR
    OUT("function declaration section");
#endif // STAGE_GRAMMAR      
}

// <�����ײ�> ����
void GrammarAnalyzer::procedureHeadAnalyze() {
    // function �ؼ���
    if (SYMBOL_TYPE != SymbolType::PROCEDURE) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ��ʶ��
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else {
        NEXTSYM;
    }
    // ��ʽ������
    if (SYMBOL_TYPE == SymbolType::LEFT_BRACKET) {
        formalParameterListAnalyze();
        TEST(ErrorEnd::PROCEDURE_HEAD);
    }
    // �ֺ�
    if (SYMBOL_TYPE != SymbolType::SEMICOLON) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("procedure head");
#endif // STAGE_GRAMMMAR
}

// <�����ײ�> ����
void GrammarAnalyzer::functionHeadAnalyze() {
    // function �ؼ���
    if (SYMBOL_TYPE != SymbolType::FUNCTION) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ��ʶ��
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else {
        NEXTSYM;
    }
    // ��ʽ������
    if (SYMBOL_TYPE == SymbolType::LEFT_BRACKET) {
        formalParameterListAnalyze();
        TEST(ErrorEnd::FUNCTION_HEAD);
    }
    // ð��
    if (SYMBOL_TYPE != SymbolType::COLON) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ��������
    basicTypeAnalyze();
    TEST(ErrorEnd::FUNCTION_HEAD);
    // �ֺ�
    if (SYMBOL_TYPE != SymbolType::SEMICOLON) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("function head");
#endif // STAGE_GRAMMAR
}

// ��ʽ������
void GrammarAnalyzer::formalParameterListAnalyze() {
    // ������
    if (SYMBOL_TYPE != SymbolType::LEFT_BRACKET) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ��һ����ʽ������
    formalParameterAnalyze();
    TEST(ErrorEnd::FORMAL_PARAMETER_LIST);
    // ��������ʽ������
    while (SYMBOL_TYPE == SymbolType::SEMICOLON) {
        NEXTSYM;
        formalParameterAnalyze();
        TEST(ErrorEnd::FORMAL_PARAMETER_LIST);
    }
    // ������
    if (SYMBOL_TYPE != SymbolType::RIGHT_BRACKET) { 
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("formal parameter list");
#endif // STAGE_GRAMMAR          
}

// ��ʽ������
void GrammarAnalyzer::formalParameterAnalyze() {
    // var �ؼ���
    if (SYMBOL_TYPE == SymbolType::VAR) {
        NEXTSYM;
    }
    // ��һ����ʶ��
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else {
        NEXTSYM;
    }
    // ������ʶ��
    while (SYMBOL_TYPE == SymbolType::COMMA) {
        NEXTSYM;
        if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
            ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~
        } else {
            NEXTSYM;
        }
    }
    // ð��
    if (SYMBOL_TYPE != SymbolType::COLON) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ��������
    basicTypeAnalyze();
    TEST(ErrorEnd::FORMAL_PARAMETER);
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("formal parameter section");
#endif // STAGE_GRAMMAR          
}

// ���
void GrammarAnalyzer::statementAnalyze() {
    switch (SYMBOL_TYPE) { 
    // ��ֵ��� �� ���̵������
    case SymbolType::IDENTIFIER:
#ifdef STAGE_GRAMMAR
        NEXTSYM;
        switch (SYMBOL_TYPE) {
        // ��ֵ���
        case SymbolType::ASSIGN:
        case SymbolType::LEFT_SQUARE:
            assignStatementAnalyze();
            TEST(ErrorEnd::STATEMENT);
            break;
        // �������
        default:
            callStatementAnalyze();
            TEST(ErrorEnd::STATEMENT);
            break;
        }
#else
#endif // STAGE_GRAMMAR
        break;
    // �������
    case SymbolType::IF:
        conditionStatementAnalyze();
        TEST(ErrorEnd::STATEMENT);
        break;
    // do-while ���
    case SymbolType::DO:
        doWhileStatementAnalyze();
        TEST(ErrorEnd::STATEMENT);
        break;
    // �������
    case SymbolType::BEGIN:
        compoundStatementAnalyze();
        TEST(ErrorEnd::STATEMENT);
        break;
    // �����
    case SymbolType::READ:
        readStatementAnalyze();
        TEST(ErrorEnd::STATEMENT);
        break;
    // д���
    case SymbolType::WRITE:
        writeStatementAnalyze();
        TEST(ErrorEnd::STATEMENT);
        break;
    // for ���
    case SymbolType::FOR:
        forStatementAnalyze();
        TEST(ErrorEnd::STATEMENT);
        break;
    // �����
    default:
        break;
    }
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("statement");
#endif // STAGE_GRAMMAR

}

// ��ֵ���
void GrammarAnalyzer::assignStatementAnalyze() {
#ifndef STAGE_GRAMMAR
    // ��ʶ��
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~
    } else {
        NEXTSYM;
    }
#endif // !STAGE_GRAMMAR
    // �����ţ������鸳ֵ
    if (SYMBOL_TYPE == SymbolType::LEFT_SQUARE) {
        NEXTSYM;
        // ���ʽ
        expressionAnalyze();
        TEST(ErrorEnd::ASSIGN_STATEMENT);
        // �ҷ�����
        if (SYMBOL_TYPE != SymbolType::RIGHT_SQUARE) {
            ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~
        } else
            NEXTSYM;
    }
    // ��ֵ��
    if (SYMBOL_TYPE != SymbolType::ASSIGN) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ���ʽ
    expressionAnalyze();
    TEST(ErrorEnd::ASSIGN_STATEMENT);
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("assign statement");
#endif // STAGE_GRAMMAR
}

// ���ʽ
void GrammarAnalyzer::expressionAnalyze() {
    // ������
    switch (SYMBOL_TYPE) {
    case SymbolType::PLUS:
        NEXTSYM;
        break;
    case SymbolType::MINUS:
        NEXTSYM;
        break;
    default:
        break;
    }
    // ��
    termAnalyze();
    TEST(ErrorEnd::EXPRESSION);
    while (true) {
        switch (SYMBOL_TYPE) {
        // �Ӻ�
        case SymbolType::PLUS:
            NEXTSYM;
            break;
        // ����
        case SymbolType::MINUS:
            NEXTSYM;
            break;
        default:
            goto END_EXPRESSION;
            break;
        }
        // ��
        termAnalyze();
        TEST(ErrorEnd::EXPRESSION);
    }
END_EXPRESSION:
    CLEAR;
#ifdef STAGE_GRAMMAR
    OUT("expression");
#endif // STAGE_GRAMMAR
}

// ��
void GrammarAnalyzer::termAnalyze() {
    // ����
    factorAnalyze();
    TEST(ErrorEnd::TERM);
    while (true) {
        switch (SYMBOL_TYPE) {
        // �˺�
        case SymbolType::ASTERISK:
            NEXTSYM;
            break;
        case SymbolType::SLASH:
            NEXTSYM;
            break;
        default:
            goto END_TERM;
        }
        // ����
        factorAnalyze();
        TEST(ErrorEnd::TERM);
    }
END_TERM:
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("term");
#endif // STAGE_GRAMMAR
}

// ����
void GrammarAnalyzer::factorAnalyze() {
    switch (SYMBOL_TYPE) {
    // ��ʶ��������Ԫ�ء������������
    case SymbolType::IDENTIFIER:
#ifdef STAGE_GRAMMAR
        NEXTSYM;
        switch (SYMBOL_TYPE) {
        // ����Ԫ��
        case SymbolType::LEFT_SQUARE:
            NEXTSYM;
            expressionAnalyze();
            TEST(ErrorEnd::FACTOR);
            // �ҷ�����
            if (SYMBOL_TYPE != SymbolType::RIGHT_SQUARE) {
                ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~��
            } else
                NEXTSYM;
            break;
        case SymbolType::LEFT_BRACKET:
            callStatementAnalyze();
            TEST(ErrorEnd::FACTOR);
            break;
        default:
            break;
        }
#endif // STAGE_GRAMMAR
        break;
    case SymbolType::LEFT_BRACKET:
        NEXTSYM;
        // ���ʽ
        expressionAnalyze();
        TEST(ErrorEnd::FACTOR);
        if (SYMBOL_TYPE != SymbolType::RIGHT_BRACKET) {
            ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~
        } else
            NEXTSYM;
        break;
    case SymbolType::NUMBER:
        // �޷�������
        unsignedAnalyze();
        TEST(ErrorEnd::FACTOR);
        break;
    default:
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~
        break;
    }
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("factor");
#endif // STAGE_GRAMMAR
}

// �������
void GrammarAnalyzer::callStatementAnalyze() {
#ifndef STAGE_GRAMMAR
    // ��ʶ��
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else {
        NEXTSYM;
    }
#endif // !STAGE_GRAMMER
    // ʵ�ڲ�����
    if (SYMBOL_TYPE == SymbolType::LEFT_BRACKET) {
        realParameterListAnalyze();
        TEST(ErrorEnd::CALL_STATEMENT);
    }
    CLEAR;
    
#ifdef STAGE_GRAMMAR
    OUT("call statement");
#endif // STAGE_GRAMMAR
}

// ʵ�ڲ�����
void GrammarAnalyzer::realParameterListAnalyze() {
    // ������
    if (SYMBOL_TYPE != SymbolType::LEFT_BRACKET) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ��һ��ʵ�ڲ���
    expressionAnalyze();
    TEST(ErrorEnd::REAL_PARAMETER_LIST);
    while (SYMBOL_TYPE == SymbolType::COMMA) {
        NEXTSYM;
        // ʵ�ڲ���
        expressionAnalyze();
        TEST(ErrorEnd::REAL_PARAMETER_LIST);
    }
    // ������
    if (SYMBOL_TYPE != SymbolType::RIGHT_BRACKET) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("real parameter list");
#endif // STAGE_GRAMMAR
}

// ����
void GrammarAnalyzer::conditionAnalyze() {
    // ���ʽ
    expressionAnalyze();
    TEST(ErrorEnd::CONDITION);
    // ��ϵ�����
    relationOperatorAnalyze();
    TEST(ErrorEnd::CONDITION);
    // ���ʽ
    expressionAnalyze();
    TEST(ErrorEnd::CONDITION);
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("condition");
#endif // STAGE_GRAMMAR
}

// ��ϵ�����
void GrammarAnalyzer::relationOperatorAnalyze() {
    switch (SYMBOL_TYPE) {
    case SymbolType::LESS:
        NEXTSYM;
        break;
    case SymbolType::LESS_EQUAL:
        NEXTSYM;
        break;
    case SymbolType::GREATER:
        NEXTSYM;
        break;
    case SymbolType::GREATER_EQUAL:
        NEXTSYM;
        break;
    case SymbolType::EQUAL:
        NEXTSYM;
        break;
    case SymbolType::INEQUAL:
        NEXTSYM;
        break;
    default:
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~
        break;
    }
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("relation operator");
#endif // STAGE_GRAMMAR
}

// �������
void GrammarAnalyzer::conditionStatementAnalyze() {
    // if �ؼ���
    if (SYMBOL_TYPE != SymbolType::IF) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~
    } else {
        NEXTSYM;
    }
    // ����
    conditionAnalyze();
    TEST(ErrorEnd::CONDITION);
    // then �ؼ���
    if (SYMBOL_TYPE != SymbolType::THEN) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ���
    statementAnalyze();
    TEST(ErrorEnd::CONDITION_STATEMENT);
    // else �ؼ���
    if (SYMBOL_TYPE == SymbolType::ELSE) {
        NEXTSYM;
        statementAnalyze();
        TEST(ErrorEnd::CONDITION_STATEMENT);
    }
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("condition statement");
#endif // STAGE_GRAMMAR
}

// do-while ���
void GrammarAnalyzer::doWhileStatementAnalyze() {
    // do �ؼ���
    if (SYMBOL_TYPE != SymbolType::DO) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ���
    statementAnalyze();
    TEST(ErrorEnd::DO_WHILE_STATEMENT);
    // whlie �ؼ���
    if (SYMBOL_TYPE != SymbolType::WHILE) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ����
    conditionAnalyze();
    TEST(ErrorEnd::DO_WHILE_STATEMENT);
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("do-while statement");
#endif // STAGE_GRAMMAR
}

// for ���
void GrammarAnalyzer::forStatementAnalyze() {
    // for �ؼ���
    if (SYMBOL_TYPE != SymbolType::FOR) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ��ʶ��
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ��ֵ��
    if (SYMBOL_TYPE != SymbolType::ASSIGN) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ���ʽ
    expressionAnalyze();
    TEST(ErrorEnd::FOR_STATEMENT);
    // to | downto �ؼ���
    switch (SYMBOL_TYPE) {
    case SymbolType::TO:
        NEXTSYM;
        break;
    case SymbolType::DOWNTO:
        NEXTSYM;
        break;
    default:
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~
        break;
    }
    // ���ʽ
    expressionAnalyze();
    TEST(ErrorEnd::FOR_STATEMENT);
    // do �ؼ���
    if (SYMBOL_TYPE != SymbolType::DO) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ���
    statementAnalyze();
    TEST(ErrorEnd::FOR_STATEMENT);
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("for statement");
#endif // STAGE_GRAMMAR
}

// �������
void GrammarAnalyzer::compoundStatementAnalyze() {
    // begin �ؼ���
    if (SYMBOL_TYPE != SymbolType::BEGIN) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ���
    statementAnalyze();
    TEST(ErrorEnd::COMPOUND_STATEMENT);
    // ���ţ�������
    while (SYMBOL_TYPE == SymbolType::SEMICOLON) {
        NEXTSYM;
        statementAnalyze();
        TEST(ErrorEnd::COMPOUND_STATEMENT);
    }
    // end �ؼ���
    if (SYMBOL_TYPE != SymbolType::END) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("compound statement");
#endif // STAGE_GRAMMAR
}

// �����
void GrammarAnalyzer::readStatementAnalyze() {
    // read �ؼ���
    if (SYMBOL_TYPE != SymbolType::READ) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ������
    if (SYMBOL_TYPE != SymbolType::LEFT_BRACKET) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ��ʶ��
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    while (SYMBOL_TYPE == SymbolType::COMMA) {
        NEXTSYM;
        if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
            ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~
        } else
            NEXTSYM;
    }
    // ������
    if (SYMBOL_TYPE != SymbolType::RIGHT_BRACKET) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("read statement");
#endif // STAGE_GRAMMAR
}

// д���
void GrammarAnalyzer::writeStatementAnalyze() {
    // write �ؼ���
    if (SYMBOL_TYPE != SymbolType::WRITE) {
        ERROR;// ����~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ������
    if (SYMBOL_TYPE != SymbolType::LEFT_BRACKET) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    // ����
    if (SYMBOL_TYPE == SymbolType::DOUBLE_QUOTE) {
        // �ַ���
        stringAnalyze();
        TEST(ErrorEnd::WRITE_STATEMENT);
        if (SYMBOL_TYPE == SymbolType::COMMA) {
            NEXTSYM;
            // ���ʽ
            expressionAnalyze();
            TEST(ErrorEnd::WRITE_STATEMENT);
        }
    } else {                                      
        // ���ʽ
        expressionAnalyze();
        TEST(ErrorEnd::WRITE_STATEMENT);
    }
    // ������
    if (SYMBOL_TYPE != SymbolType::RIGHT_BRACKET) {
        ERROR;// ����~~~~~~~~~~~~~~~~~~~~~~~~
    } else
        NEXTSYM;
    CLEAR;

#ifdef STAGE_GRAMMAR
    OUT("write statement");
#endif // STAGE_GRAMMAR

}