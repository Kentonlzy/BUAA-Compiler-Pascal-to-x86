/*
    GrammarAnalyzer.cpp
    �﷨��������ʵ��
*/
#include "GrammarAnalyzer.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

#define TOKEN_SYMBOL    (*lexicalAnalyzer.token.symbol)     // string ���͵� token
#define TOKEN_NUMBER    (lexicalAnalyzer.token.number)      // int ���͵� token
#define SYMBOL_TYPE     (lexicalAnalyzer.symbolType)        // ���ŵ� SymbotType ����ֵ
                                            // ����꣬�򻯶Է��ź����ͻ�ȡ    

#define NEXTSYM lexicalAnalyzer.nextSymbol()

#define NEXTSTR lexicalAnalyzer.nextString()

#define NEXTCH lexicalAnalyzer.nextChar()
                                            // ����꣬�򻯻�ȡ��һ�����ŵĲ���

// ���ڼ������﷨�ɷֵĴ�����
static ErrorEnd err;

// ������Ϣ���
#define ERROUT  errorHandler.printError
#define WARNOUT errorHandler.printWarning

// ����������һЩ�ַ�
#define SKIP    err = errorHandler.skip()

// �� err ��Ϊ NORMAL����û�д���
#define CLEAR   err = ErrorEnd::NORMAL

// ���� err ������Ӧ�������������Ϊ�˴��븴��
#define TEST(item) {                    \
    if (err != ErrorEnd::NORMAL) {      \
        if (err == item)                \
            CLEAR;                      \
        goto END;                         \
    }                                   \
}
                           
// ���캯��
// ����ģʽ
GrammarAnalyzer::GrammarAnalyzer(LexicalAnalyzer &la, ErrorHandler &eh, 
                                 SymbolTableManager &stm, SemanticAnalyzer &sa):
    lexicalAnalyzer(la), errorHandler(eh), table(stm), semanticAnalyzer(sa) {}
GrammarAnalyzer &GrammarAnalyzer::initialGrammarAnalyzer(LexicalAnalyzer &la,
                                                         ErrorHandler &eh,
                                                         SymbolTableManager &stm,
                                                         SemanticAnalyzer &sa) {
    static GrammarAnalyzer theGrammarAnalyzer(la, eh, stm, sa);
    return theGrammarAnalyzer;
}

// ���� ����
// ���������������̵����
void GrammarAnalyzer::grammarAnalyze() {
    NEXTSYM;            // ��һ�����ű��ã�������������
    semanticAnalyzer.programEndmapInit();
    // �ֳ���
    blockAnalyze();
    TEST(ErrorEnd::PROGRAM);
    // ���
    if (SYMBOL_TYPE != SymbolType::PERIOD) {
        ERROUT(ErrorType::NO_PERIOD);
    }
    NEXTSYM;
    if (SYMBOL_TYPE != SymbolType::FINISH)
        ERROUT(ErrorType::NO_FINISH);
END:
    ;
}

// <�ֳ���> ����
void GrammarAnalyzer::blockAnalyze() {
    semanticAnalyzer.blockEndmapInit();
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
    // �������
    semanticAnalyzer.funcprocStart();   // ����/���̿�ʼ
    compoundStatementAnalyze();
    semanticAnalyzer.funcprocReturn();  // ����/���̷���
    TEST(ErrorEnd::BLOCK);
    CLEAR;
END:
    semanticAnalyzer.blockEndmapClear();
}

// <����˵������> ����
void GrammarAnalyzer::constDeclareAnalyze() {
    semanticAnalyzer.constDeclareEndmapInit();
    // const �ؼ���
    if (SYMBOL_TYPE != SymbolType::CONST) {
        ERROUT(ErrorType::NO_CONST);
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
        ERROUT(ErrorType::NO_SEMICOLON, "The former statement missed a semicolon");
        SKIP;
        semanticAnalyzer.constDeclareSkip();
        TEST(ErrorEnd::CONST_DECLARE);
    } else
        NEXTSYM;
    CLEAR;
END:
    semanticAnalyzer.constDeclareEndmapClear();
}

// <��������> ����
void GrammarAnalyzer::constDefineAnalyze() {
    semanticAnalyzer.constDeclareEndmapInit();
    // <��ʶ��>
    string id;      // ��ʶ��
    BasicType type; // ���ͣ����� or �ַ�
    int value = 0;  // ������ֵ��Ĭ��Ϊ 0
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROUT(ErrorType::NO_IDENTIFIER);
        SKIP;
        TEST(ErrorEnd::CONST_DEFINE);
    } else if (table.isHere(TOKEN_SYMBOL)) {
        ERROUT(ErrorType::REDEFINE, TOKEN_SYMBOL);
        SKIP;
        TEST(ErrorEnd::CONST_DEFINE);
    } else {
        id = TOKEN_SYMBOL;
        NEXTSYM;
    }
    // �Ⱥ�
    if (SYMBOL_TYPE != SymbolType::EQUAL) {
        ERROUT(ErrorType::NO_EQUAL);
    } else
        NEXTSYM;
    // Ĭ�ϴ˴��еȺţ�����
    // <����>
    constAnalyze(value, type);
    TEST(ErrorEnd::CONST_DEFINE);
    // û�д������
    semanticAnalyzer.constDefine(id, type, value);
    CLEAR;
END:
    semanticAnalyzer.constDeclareEndmapClear();
}

// <����> ����
void GrammarAnalyzer::constAnalyze(int &value, BasicType &type) {
    switch (SYMBOL_TYPE) {
    // ������
    case SymbolType::PLUS: case SymbolType::MINUS:
        type = BasicType::INTEGER;
        NEXTSYM;
        unsignedAnalyze(value);
        TEST(ErrorEnd::CONST);
        break;
    // �޷�������
    case SymbolType::NUMBER:
        type = BasicType::INTEGER;
        unsignedAnalyze(value);
        TEST(ErrorEnd::CONST);
        break;
    // �ַ�����ʶ������
    case SymbolType::SINGLE_QUOTE:
        type = BasicType::CHAR;
        charAnalyze(value);
        TEST(ErrorEnd::CONST);
        break;
    // �﷨����
    default:
        ERROUT(ErrorType::NO_CHNUM);
        break;
    }
    CLEAR;
END:
    ;
}

// <�ַ�> ����
void GrammarAnalyzer::charAnalyze(int &value) {
    semanticAnalyzer.charEndmapInit();
    // ������
    if (SYMBOL_TYPE != SymbolType::SINGLE_QUOTE) {
        ERROUT(ErrorType::NO_SINGLEQUOTE);
    }
    // Ĭ�������е����ţ�����
    NEXTCH;
    if (SYMBOL_TYPE != SymbolType::CHARATER) {
        ERROUT(ErrorType::ILLEGAL_CHAR, TOKEN_SYMBOL);
        SKIP;
        semanticAnalyzer.charSkip();
        TEST(ErrorEnd::CHAR);
    } else {
        value = TOKEN_NUMBER;
        NEXTSYM;
    }
    // ʶ������
    if (SYMBOL_TYPE != SymbolType::SINGLE_QUOTE) {
        ERROUT(ErrorType::NO_SINGLEQUOTE);
    } else
        NEXTSYM;
    CLEAR;
END:
    semanticAnalyzer.charEndmapClear();
}

// <�ַ���> ����
void GrammarAnalyzer::stringAnalyze(string &strname) {
    semanticAnalyzer.stringEndmapInit();
    // ��˫����
    if (SYMBOL_TYPE != SymbolType::DOUBLE_QUOTE) {
        ERROUT(ErrorType::NO_DOUBLEQUOTE);
    }
    // Ĭ����˫���ţ�����
    NEXTSTR;
    if (SYMBOL_TYPE == SymbolType::STRING) {
        strname = semanticAnalyzer.stringDefine(TOKEN_SYMBOL);
        NEXTSYM;
    } else {
        ERROUT(ErrorType::ILLEGAL_STRING, TOKEN_SYMBOL);
        SKIP;
        semanticAnalyzer.stringSkip();
        TEST(ErrorEnd::STRING);
    }
    // ʶ��˫����
    if (SYMBOL_TYPE != SymbolType::DOUBLE_QUOTE) {
        ERROUT(ErrorType::NO_DOUBLEQUOTE);
    } else
        NEXTSYM;
    CLEAR;
END:
    semanticAnalyzer.stringEndmapClear();
}

// <�޷�������> ����
void GrammarAnalyzer::unsignedAnalyze(int &value) {
    // ���ж��ǲ���������������������������������������������
    if (SYMBOL_TYPE != SymbolType::NUMBER) {
        ERROUT(ErrorType::NO_NUMBER);
    } else {
        value = TOKEN_NUMBER;
        if (lexicalAnalyzer.overflow) {
            WARNOUT(ErrorType::INT_OVERFLOW, stringPlusInt("New value is ", value));
        }
        NEXTSYM;
    }
    CLEAR;    
}

// <����˵������> ����
void GrammarAnalyzer::varDeclareAnalyze() {
    semanticAnalyzer.varDeclareEndmapInit();
    // var �ؼ���
    if (SYMBOL_TYPE != SymbolType::VAR) {
        ERROUT(ErrorType::NO_VAR);
    } else
        NEXTSYM;
    // ��һ�� <����˵�������壩> �Ĵ���
    varDefineAnalyze();
    TEST(ErrorEnd::VAR_DECLARE);
    // ʶ��ֺ�
    if (SYMBOL_TYPE != SymbolType::SEMICOLON) {
        ERROUT(ErrorType::NO_SEMICOLON, "former var declaration missed a semicolon");
    } else
        NEXTSYM;
    // �������е� <����˵��> ���ֺŵĴ���
    while (SYMBOL_TYPE == SymbolType::IDENTIFIER) {
        varDefineAnalyze();
        TEST(ErrorEnd::VAR_DECLARE);
        if (SYMBOL_TYPE != SymbolType::SEMICOLON) {
            ERROUT(ErrorType::NO_SEMICOLON, "former var declaration missed a semicolon");
        } else
            NEXTSYM;
    }
    CLEAR;
END:
    semanticAnalyzer.varDeclareEndmapClear();
}

// <����˵�������壩> ����
void GrammarAnalyzer::varDefineAnalyze() { 
    semanticAnalyzer.varDefineEndmapInit();
    vector<string> names;                           // ��ʶ�����֣�˵���п����ж����ʶ����
                                                    //  Ҫ�ȴ�������һ�����
    IdentifierType idType = IdentifierType::VAR;    // ��ʶ�����ͣ�����/����
    BasicType bType;                                // �������ͣ�����/�ַ���
    int limit;                                      //  ����������
    // �����һ����ʶ��
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROUT(ErrorType::NO_IDENTIFIER);
        SKIP;
        TEST(ErrorEnd::VAR_DEFINE);
    } else {
        // ����Ƿ��ض���
        if (table.isHere(TOKEN_SYMBOL)) {
            ERROUT(ErrorType::REDEFINE, TOKEN_SYMBOL);
        } else
            names.push_back(TOKEN_SYMBOL);
        NEXTSYM;
    }
    // �����Զ��ŷָ��Ķ����ʶ��
    while (SYMBOL_TYPE == SymbolType::COMMA) {
        NEXTSYM;
        if (SYMBOL_TYPE == SymbolType::IDENTIFIER) {
            // ����Ƿ��ض���
            if (table.isHere(TOKEN_SYMBOL))
                ERROUT(ErrorType::REDEFINE, TOKEN_SYMBOL);
            else
                names.push_back(TOKEN_SYMBOL);
            NEXTSYM;
        } else {
            ERROUT(ErrorType::NO_IDENTIFIER);
            SKIP;
            TEST(ErrorEnd::VAR_DEFINE);
        }
    }
    // ʶ��ð��
    if (SYMBOL_TYPE != SymbolType::COLON) {
        ERROUT(ErrorType::NO_COLON);
    } else
        NEXTSYM;
    // Ĭ����ð�ţ�����
    typeAnalyze(idType, bType, limit);
    TEST(ErrorEnd::VAR_DEFINE);
    // ����ű�
    if (idType == IdentifierType::VAR)
        semanticAnalyzer.varDefine(names, bType);
    else
        semanticAnalyzer.arrayDefine(names, bType, limit);
    CLEAR;
END:
    semanticAnalyzer.varDefineEndmapClear();
}

// <����> ����
void GrammarAnalyzer::typeAnalyze(IdentifierType &idType, BasicType &bType, int &lim) {
    if (SYMBOL_TYPE == SymbolType::ARRAY) {     // ����
        idType = IdentifierType::ARRAY;
        NEXTSYM;
        // ʶ��������
        if (SYMBOL_TYPE != SymbolType::LEFT_SQUARE) {
            ERROUT(ErrorType::NO_LEFTSQUARE);
        } else
            NEXTSYM;
        // Ĭ���������ţ�����
        unsignedAnalyze(lim);   
        TEST(ErrorEnd::TYPE);
        // ʶ��������
        if (SYMBOL_TYPE != SymbolType::RIGHT_SQUARE) {
            ERROUT(ErrorType::NO_RIGHTSQUARE);
        } else
            NEXTSYM;
        // Ĭ���������ţ�����
        // ʶ�� of
        if (SYMBOL_TYPE != SymbolType::OF) {
            ERROUT(ErrorType::NO_OF);
        } else
            NEXTSYM;
        // Ĭ���� of������
        basicTypeAnalyze(bType);
        TEST(ErrorEnd::TYPE);
    } else {                                    // ��������
        basicTypeAnalyze(bType);
        TEST(ErrorEnd::TYPE);
    }
    CLEAR;
END:
    ;
}

// <��������> ����
void GrammarAnalyzer::basicTypeAnalyze(BasicType &bType) {
    switch (SYMBOL_TYPE) {
    case SymbolType::INTEGER:
        NEXTSYM;
        break;
    case SymbolType::CHAR:
        NEXTSYM;
        break;
    default:
        ERROUT(ErrorType::NO_BASICTYPE);
        SKIP;
        return;
        break;
    }
    CLEAR;
}

// <����˵������>
void GrammarAnalyzer::procedureDeclareAnalyze() {
    semanticAnalyzer.procedureDeclareEndmapInit();
    string procname;
    // �����ײ�
    procedureHeadAnalyze(procname);
    TEST(ErrorEnd::PROCEDURE_DECLARE);
    table.goIn(procname);   // ���ű���������������һ��
    // �ֳ���
    blockAnalyze();
    table.goOut();          // ���ű�����������һ��
    TEST(ErrorEnd::PROCEDURE_DECLARE);
    // �ֺ�
    if (SYMBOL_TYPE != SymbolType::SEMICOLON) {
        ERROUT(ErrorType::NO_SEMICOLON);
    } else
        NEXTSYM;
    CLEAR;
END:
    semanticAnalyzer.procedureDeclareEndmapClear();
}

// <����˵������> ����
void GrammarAnalyzer::functionDeclareAnalyze() {
    semanticAnalyzer.functionDeclareEndmapInit();
    string funcname;
    // �����ײ�
    functionHeadAnalyze(funcname);
    TEST(ErrorEnd::FUNCTION_DECLARE);
    // �ֳ���
    table.goIn(funcname);           // ��������һ��
    blockAnalyze();
    table.goOut();                  // �������һ��
    TEST(ErrorEnd::FUNCTION_DECLARE);
    // �ֺ�
    if (SYMBOL_TYPE != SymbolType::SEMICOLON) {
        ERROUT(ErrorType::NO_SEMICOLON);
    } else
        NEXTSYM;
END:
    semanticAnalyzer.functionDeclareEndmapClear();
}

// <�����ײ�> ����
void GrammarAnalyzer::procedureHeadAnalyze(string &name) {
    semanticAnalyzer.procedureHeadEndmapInit();
    vector<Argument> args;
    // procedure �ؼ���
    if (SYMBOL_TYPE != SymbolType::PROCEDURE) {
        ERROUT(ErrorType::NO_PROCEDURE);
    } else
        NEXTSYM;
    // ��ʶ��
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROUT(ErrorType::NO_IDENTIFIER, "The procedure should have a name");
        SKIP;
        TEST(ErrorEnd::PROCEDURE_HEAD);
    } else {
        if (table.isHere(TOKEN_SYMBOL)) {
            ERROUT(ErrorType::REDEFINE, TOKEN_SYMBOL);
            SKIP;
            TEST(ErrorEnd::PROCEDURE_HEAD);
        } else {
            name = TOKEN_SYMBOL;
            NEXTSYM;
        }
    }
    // ��ʽ������
    if (SYMBOL_TYPE == SymbolType::LEFT_BRACKET) {
        formalParameterListAnalyze(args);
        TEST(ErrorEnd::PROCEDURE_HEAD);
    }
    // ���
    semanticAnalyzer.procedureDefine(name, args);
    // �ֺ�
    if (SYMBOL_TYPE != SymbolType::SEMICOLON) {
        ERROUT(ErrorType::NO_SEMICOLON);
    } else
        NEXTSYM;
    CLEAR;
END:
    semanticAnalyzer.procedureHeadEndmapClear();
}

// <�����ײ�> ����
void GrammarAnalyzer::functionHeadAnalyze(string &name) {
    semanticAnalyzer.functionHeadEndmapInit();
    vector<Argument> args;
    BasicType returnType;
    // function �ؼ���
    if (SYMBOL_TYPE != SymbolType::FUNCTION) {
        ERROUT(ErrorType::NO_FUNCTION);
    } else
        NEXTSYM;
    // ��ʶ��
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROUT(ErrorType::NO_IDENTIFIER, "There should be a name of function");
        SKIP;
        TEST(ErrorEnd::FUNCTION_HEAD);
    } else {
        // ����ض���
        if (table.isHere(TOKEN_SYMBOL)) {
            ERROUT(ErrorType::REDEFINE, TOKEN_SYMBOL);
            SKIP;
            TEST(ErrorEnd::FUNCTION_HEAD);
        } else {
            name = TOKEN_SYMBOL;
            NEXTSYM;
        }
    }
    // ��ʽ������
    if (SYMBOL_TYPE == SymbolType::LEFT_BRACKET) {
        formalParameterListAnalyze(args);
        TEST(ErrorEnd::FUNCTION_HEAD);
    }
    // ð��
    if (SYMBOL_TYPE != SymbolType::COLON) {
        ERROUT(ErrorType::NO_COLON);
    } else
        NEXTSYM;
    // Ĭ����ð�ţ�����
    // ��������
    basicTypeAnalyze(returnType);
    TEST(ErrorEnd::FUNCTION_HEAD);
    // ���
    semanticAnalyzer.functionDefine(name, args, returnType);
    // �ֺ�
    if (SYMBOL_TYPE != SymbolType::SEMICOLON) {
        ERROUT(ErrorType::NO_SEMICOLON);
    } else
        NEXTSYM;
    CLEAR;
END:
    semanticAnalyzer.functionHeadEndmapClear();
}

// ��ʽ������
void GrammarAnalyzer::formalParameterListAnalyze(vector<Argument> &args) {
    // ������
    if (SYMBOL_TYPE != SymbolType::LEFT_BRACKET) {
        ERROUT(ErrorType::NO_LEFTBRACKET);
    } else
        NEXTSYM;
    // Ĭ���������ţ�����
    // ��һ����ʽ������
    formalParameterAnalyze(args);
    TEST(ErrorEnd::FORMAL_PARAMETER_LIST);
    // ��������ʽ������
    while (SYMBOL_TYPE == SymbolType::SEMICOLON) {
        NEXTSYM;
        formalParameterAnalyze(args);
        TEST(ErrorEnd::FORMAL_PARAMETER_LIST);
    }
    // ������
    if (SYMBOL_TYPE != SymbolType::RIGHT_BRACKET) {
        ERROUT(ErrorType::NO_RIGHTBRACKET);
    } else
        NEXTSYM;
    CLEAR;
END:
    ;
}

// ��ʽ������
void GrammarAnalyzer::formalParameterAnalyze(vector<Argument> &args) {
    semanticAnalyzer.formalParameterEndmapInit();
    bool vary = false;
    BasicType type;
    vector<string> names;
    // var �ؼ���
    if (SYMBOL_TYPE == SymbolType::VAR) {
        vary = true;
        NEXTSYM;
    }
    // ��һ����ʶ��
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROUT(ErrorType::NO_IDENTIFIER);
        SKIP;
        TEST(ErrorEnd::FORMAL_PARAMETER);
    } else {
        names.push_back(TOKEN_SYMBOL);
        NEXTSYM;
    }
    // ������ʶ��
    while (SYMBOL_TYPE == SymbolType::COMMA) {
        NEXTSYM;
        if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
            ERROUT(ErrorType::NO_IDENTIFIER);
            SKIP;
            TEST(ErrorEnd::FORMAL_PARAMETER);
        } else {
            names.push_back(TOKEN_SYMBOL);
            NEXTSYM;
        }
    }
    // ð��
    if (SYMBOL_TYPE != SymbolType::COLON) {
        ERROUT(ErrorType::NO_COLON);
    } else
        NEXTSYM;
    // Ĭ����ð�ţ�����
    // ��������
    basicTypeAnalyze(type);
    TEST(ErrorEnd::FORMAL_PARAMETER);
    // �Ѷ����Ľ�����������
    semanticAnalyzer.formalParameterAdd(names, type, vary, args);
    CLEAR;
END:
    semanticAnalyzer.formalParameterEndmapClear();
}

// ���
void GrammarAnalyzer::statementAnalyze() {
    semanticAnalyzer.statementEndmapInit();
    string value;
    BasicType type;
    switch (SYMBOL_TYPE) { 
    // ��ֵ��� �� ���̵������
    case SymbolType::IDENTIFIER:
        table.find(TOKEN_SYMBOL);
        if (table.ste == NULL) {
            ERROUT(ErrorType::UNDEFINED, TOKEN_SYMBOL);
            SKIP;
            TEST(ErrorEnd::STATEMENT);
        }
        switch (table.ste->type) {
        case IdentifierType::ARRAY:
        case IdentifierType::VAR:
            assignStatementAnalyze();
            TEST(ErrorEnd::STATEMENT);
            break;
        case IdentifierType::FUNCTION:  // ����������ֵ��ֵ�����
        case IdentifierType::PROCEDURE:
            callStatementAnalyze(value, type);
            TEST(ErrorEnd::STATEMENT);
            break;
        default:
            ERROUT(ErrorType::CONSTANT, TOKEN_SYMBOL);
            break;
        }
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
END:
    semanticAnalyzer.statementEndmapClear();
}

// ��ֵ���
void GrammarAnalyzer::assignStatementAnalyze() {
    // ��ʶ��
    string name;        // ��ʶ��
    string offset;      // ��������飬��¼�±�
    string value;       // Ҫ����ֵ
    BasicType nameType; // ����ֵ�ı���������
    BasicType valueType;// Ҫ����ֵ������
    bool variable;      // ���ʽ�Ƿ�Ϊ�������ʽ
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROUT(ErrorType::NO_IDENTIFIER);
    } else {
        table.find(TOKEN_SYMBOL);
        if (table.ste == NULL) {
            ERROUT(ErrorType::UNDEFINED, TOKEN_SYMBOL);
            SKIP;
            TEST(ErrorEnd::ASSIGN_STATEMENT);
        }
        name = table.encodeKey();   // Ψһ��ʶ
        nameType = table.ste->valueType;
        switch (table.ste->type) {
        // �� ����Ԫ�� ��ֵ
        case IdentifierType::ARRAY:
            NEXTSYM;
            // ������
            if (SYMBOL_TYPE != SymbolType::LEFT_SQUARE) {
                ERROUT(ErrorType::NO_LEFTSQUARE);
            } else
                NEXTSYM;
            // Ĭ���������ţ�����
            // ���ʽ���±�
            expressionAnalyze(offset, valueType, variable);
            TEST(ErrorEnd::ASSIGN_STATEMENT);
            // �ҷ�����
            if (SYMBOL_TYPE != SymbolType::RIGHT_SQUARE) {
                ERROUT(ErrorType::NO_RIGHTSQUARE);
            } else
                NEXTSYM;
            // Ĭ�����ҷ����ţ�����
            // ��ֵ��
            if (SYMBOL_TYPE != SymbolType::ASSIGN) {
                ERROUT(ErrorType::NO_ASSIGN);
                SKIP;
                TEST(ErrorEnd::ASSIGN_STATEMENT);
            } else
                NEXTSYM;
            // ���ʽ��Ҫ����ֵ
            expressionAnalyze(value, valueType, variable);
            TEST(ErrorEnd::ASSIGN_STATEMENT);
            // ����ָ��
            semanticAnalyzer.assignToArray(name, offset, value, nameType, valueType);
            break;
        // ����ͨ������ֵ
        default:
            NEXTSYM;
            // ��ֵ��
            if (SYMBOL_TYPE != SymbolType::ASSIGN) {
                ERROUT(ErrorType::NO_ASSIGN);
                SKIP;
                TEST(ErrorEnd::ASSIGN_STATEMENT);
            } else
                NEXTSYM;
            // ���ʽ��Ҫ����ֵ
            expressionAnalyze(value, valueType, variable);
            TEST(ErrorEnd::ASSIGN_STATEMENT);
            // ����ָ��
            semanticAnalyzer.assign(name, value, nameType, valueType);
            break;
        }
    }
    CLEAR;
END:
    ;
}

// ������������ֵ
void GrammarAnalyzer::functionAssignStatementAnalyze(const string &name) {
    table.decodeKey(name);
    BasicType functype = table.ste->valueType;
    BasicType valuetype;
    bool variable;
    string value;
    // ��ֵ��
    if (SYMBOL_TYPE != SymbolType::ASSIGN) {
        ERROUT(ErrorType::NO_ASSIGN);
        SKIP;
        TEST(ErrorEnd::FUNCTION_ASSIGN);
    } else
        NEXTSYM;
    // ���ʽ
    expressionAnalyze(value, valuetype, variable);
    TEST(ErrorEnd::FUNCTION_ASSIGN);
    semanticAnalyzer.functionAssign(name, value, functype, valuetype);
    CLEAR;
END:
    ;
}

// ���ʽ
void GrammarAnalyzer::expressionAnalyze(string &value, BasicType &type, bool &variable) {
    // ������
    bool neg = false;           // ����ķ���
    string oper1, oper2, dest;  // ��������Ĳ����������
    char op;                    // �����
    BasicType oper2type;
    bool oper2variable;
    switch (SYMBOL_TYPE) {
    case SymbolType::PLUS:
        NEXTSYM;
        break;
    case SymbolType::MINUS:
        neg = true;
        NEXTSYM;
        break;
    default:
        break;
    }
    // ��
    termAnalyze(dest, type, variable);
    TEST(ErrorEnd::EXPRESSION);
    if (neg) {
        oper1 = dest;
        semanticAnalyzer.negative(oper1, dest);
    }
    while (true) {
        switch (SYMBOL_TYPE) {
        // �Ӻ�
        case SymbolType::PLUS:
            op = '+';
            NEXTSYM;
            break;
        // ����
        case SymbolType::MINUS:
            op = '-';
            NEXTSYM;
            break;
        default:
            goto END_EXPRESSION;
            break;
        }
        // ��
        variable = false;
        oper1 = dest;
        termAnalyze(oper2, oper2type, oper2variable);
        TEST(ErrorEnd::EXPRESSION);
        type = BasicType::INTEGER;
        switch (op) {
        case '+':
            semanticAnalyzer.addition(oper1, oper2, dest);
            break;
        case '-':
            semanticAnalyzer.subtraction(oper1, oper2, dest);
            break;
        default:
            goto END_EXPRESSION;
            break;
        }
    }
END_EXPRESSION:
    CLEAR;
END:
    value = dest;
}

// ��
void GrammarAnalyzer::termAnalyze(string &value, BasicType &type, bool &variable) {
    semanticAnalyzer.termEndmapInit();
    string oper1, oper2, dest;
    char op;
    BasicType oper2type;
    bool oper2variable;
    // ����
    factorAnalyze(dest, type, variable);
    TEST(ErrorEnd::TERM);
    while (true) {
        switch (SYMBOL_TYPE) {
        // �˺�
        case SymbolType::ASTERISK:
            op = '*';
            NEXTSYM;
            break;
        case SymbolType::SLASH:
            op = '/';
            NEXTSYM;
            break;
        default:
            goto END_TERM;
        }
        // ����
        variable = false;
        oper1 = dest;
        factorAnalyze(oper2, oper2type, oper2variable);
        TEST(ErrorEnd::TERM);
        type = BasicType::INTEGER;
        switch (op) {
        case '*':
            semanticAnalyzer.multiplicatioin(oper1, oper2, dest);
            break;
        case '/':
            semanticAnalyzer.division(oper1, oper2, dest);
            break;
        default:
            goto END_TERM;
            break;
        }
    }
END_TERM:
    CLEAR;
END:
    value = dest;
    semanticAnalyzer.termEndmapClear();
}

// ����
void GrammarAnalyzer::factorAnalyze(string &value, BasicType &type, bool &variable) {
    semanticAnalyzer.factorEndmapInit();
    string name;
    string offset;
    int n;
    BasicType t;
    bool v;
    switch (SYMBOL_TYPE) {
    // ��ʶ��������Ԫ�ء������������
    case SymbolType::IDENTIFIER:
        table.find(TOKEN_SYMBOL);
        if (table.ste == NULL) {
            ERROUT(ErrorType::UNDEFINED, TOKEN_SYMBOL);
            SKIP;
            TEST(ErrorEnd::FACTOR);
        }
        switch (table.ste->type) {
        // ����
        case IdentifierType::VAR:
            value = table.encodeKey();
            type = table.ste->valueType;
            variable = true;
            NEXTSYM;
            break;
        // ����
        case IdentifierType::CONST:
            semanticAnalyzer.intToString(table.ste->multiInt, value);
            type = table.ste->valueType;
            variable = false;
            NEXTSYM;
            break;
        // ����Ԫ��
        case IdentifierType::ARRAY:
            name = table.encodeKey();
            type = table.ste->valueType;
            variable = true;
            NEXTSYM;
            // ������
            if (SYMBOL_TYPE != SymbolType::LEFT_SQUARE) {
                ERROUT(ErrorType::NO_LEFTSQUARE);
            } else
                NEXTSYM;
            // Ĭ���������ţ���ʼ������ʽ
            expressionAnalyze(offset, t, v);
            TEST(ErrorEnd::FACTOR);
            // �ҷ�����
            if (SYMBOL_TYPE != SymbolType::RIGHT_SQUARE) {
                ERROUT(ErrorType::NO_RIGHTSQUARE);
            } else
                NEXTSYM;
            semanticAnalyzer.assignFromArray(name, offset, value);
            break;
        // �����������
        case IdentifierType::FUNCTION:
            variable = false;
            callStatementAnalyze(value, type);
            TEST(ErrorEnd::FACTOR);
            break;
        default:
            ERROUT(ErrorType::NO_VALUE, TOKEN_SYMBOL + " is not a valid value");
            SKIP;
            break;
        }
        break;
    case SymbolType::LEFT_BRACKET:
        NEXTSYM;
        // ���ʽ
        expressionAnalyze(value, type, variable);
        TEST(ErrorEnd::FACTOR);
        if (SYMBOL_TYPE != SymbolType::RIGHT_BRACKET) {
            ERROUT(ErrorType::NO_RIGHTBRACKET);
        } else
            NEXTSYM;
        break;
    case SymbolType::NUMBER:
        // �޷�������
        unsignedAnalyze(n);
        TEST(ErrorEnd::FACTOR);
        semanticAnalyzer.intToString(n, value);
        type = BasicType::INTEGER;
        variable = false;
        break;
    default:
        ERROUT(ErrorType::NO_VALUE);
        break;
    }
    CLEAR;
END:
    semanticAnalyzer.factorEndmapClear();
}

// �������
void GrammarAnalyzer::callStatementAnalyze(string &value, BasicType &type) {
    // ��ʶ��
    bool hasValue;
    string name;
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROUT(ErrorType::NO_IDENTIFIER);
        SKIP;
        TEST(ErrorEnd::CALL_STATEMENT);
    } else {
        table.find(TOKEN_SYMBOL);
        if (table.ste == NULL) {
            ERROUT(ErrorType::UNDEFINED, TOKEN_SYMBOL);
            SKIP;
            TEST(ErrorEnd::CALL_STATEMENT);
        }
        name = table.encodeKey();
        switch (table.ste->type) {
        case IdentifierType::FUNCTION:
            NEXTSYM;
            if (SYMBOL_TYPE == SymbolType::ASSIGN) {
            // ��������ֵ��ֵ
                functionAssignStatementAnalyze(name);
                TEST(ErrorEnd::CALL_STATEMENT);
                goto END;
            } else {
                hasValue = true;
                type = table.ste->valueType;
            }
            break;
        case IdentifierType::PROCEDURE:
            hasValue = false;
            NEXTSYM;
            break;
        default:
            ERROUT(ErrorType::CANNOT_CALL, TOKEN_SYMBOL);
            SKIP;
            TEST(ErrorEnd::CALL_STATEMENT);
            break;
        }
    }
    // ʵ�ڲ�����
    if (SYMBOL_TYPE == SymbolType::LEFT_BRACKET) {
        realParameterListAnalyze(name);
        TEST(ErrorEnd::CALL_STATEMENT);
    }
    if (hasValue)
        semanticAnalyzer.callFunction(name, value);
    else
        semanticAnalyzer.callProcedure(name);
    CLEAR;
END:
    ;
}

// ʵ�ڲ�����
void GrammarAnalyzer::realParameterListAnalyze(const string &name) {
    // �������
    table.decodeKey(name);
    vector<Argument> *args = table.ste->arguments;      // ������
    vector<Argument>::iterator argit;                   // ������ĵ�����
    string value;                                       // ʵ�ε�ֵ
    BasicType type;                                     // ʵ�ε�����
    bool variable;                                      // ʵ���Ƿ��Ǳ������ʽ
    // ȷ�ϲ�����ǿ�
    if (args == NULL) {
        ERROUT(ErrorType::TOO_MANY_ARG, "there should not be real parameters");
        SKIP;
        TEST(ErrorEnd::REAL_PARAMETER_LIST);
    }
    argit = args->begin();
    // ������
    if (SYMBOL_TYPE != SymbolType::LEFT_BRACKET) {
        ERROUT(ErrorType::NO_LEFTBRACKET);
    } else
        NEXTSYM;
    // Ĭ����������
    // ��һ��ʵ�ڲ���
    expressionAnalyze(value, type, variable);
    TEST(ErrorEnd::REAL_PARAMETER_LIST);
    // �����������Ƿ����
    if (argit == args->end()) {
        ERROUT(ErrorType::TOO_MANY_ARG);
        SKIP;
        TEST(ErrorEnd::REAL_PARAMETER_LIST);
    // ���ʵ�����β������е� var �Ƿ�ɶ�Ӧ
    } else if (argit->vary == true && variable == false) {
        ERROUT(ErrorType::UNVARIABLE);
    }
    // ����
    semanticAnalyzer.passParameter(value, argit->type, type);
    argit++;
    while (SYMBOL_TYPE == SymbolType::COMMA) {
        NEXTSYM;
        // ʵ�ڲ���
        expressionAnalyze(value, type, variable);
        TEST(ErrorEnd::REAL_PARAMETER_LIST);
        // �����������Ƿ����
        if (argit == args->end()) {
            ERROUT(ErrorType::TOO_MANY_ARG);
            SKIP;
            TEST(ErrorEnd::REAL_PARAMETER_LIST);
        // ���ʵ�����β������е� var �Ƿ�ɶ�Ӧ
        } else if (argit->vary == true && variable == false) {
            ERROUT(ErrorType::UNVARIABLE);
        }
        // ����
        semanticAnalyzer.passParameter(value, argit->type, type);
        argit++;
    }
    if (argit != args->end()) {
        ERROUT(ErrorType::TOO_LESS_ARG);
    }
    // ������
    if (SYMBOL_TYPE != SymbolType::RIGHT_BRACKET) {
        ERROUT(ErrorType::NO_RIGHTBRACKET);
    } else
        NEXTSYM;
    CLEAR;
END:
    ;
}

// ����
void GrammarAnalyzer::conditionAnalyze(SymbolType &relationOp) {
    // ���ʽ
    string value1, value2;
    BasicType t;
    bool v;
    expressionAnalyze(value1, t, v);
    TEST(ErrorEnd::CONDITION);
    // ��ϵ�����
    relationOperatorAnalyze(relationOp);
    TEST(ErrorEnd::CONDITION);
    // ���ʽ
    expressionAnalyze(value2, t, v);
    TEST(ErrorEnd::CONDITION);
    semanticAnalyzer.compare(value1, value2);
    CLEAR;
END:
    ;
}

// ��ϵ�����
void GrammarAnalyzer::relationOperatorAnalyze(SymbolType &relationOp) {
    switch (SYMBOL_TYPE) {
    case SymbolType::LESS:
    case SymbolType::LESS_EQUAL:
    case SymbolType::GREATER:
    case SymbolType::GREATER_EQUAL:
    case SymbolType::EQUAL:
    case SymbolType::INEQUAL:
        relationOp = SYMBOL_TYPE;
        NEXTSYM;
        break;
    default:
        ERROUT(ErrorType::NO_RELATION);
        relationOp = SymbolType::ILLEGAL;
        SKIP;
        TEST(ErrorEnd::RELATION_OPERATOR);
        break;
    }
    CLEAR;
END:
    ;
}

// �������
void GrammarAnalyzer::conditionStatementAnalyze() {
    // if �ؼ���
    SymbolType condition;   // �ж�����
    string elseLabel = semanticAnalyzer.newLabel("condition_else");
    string endLabel = semanticAnalyzer.newLabel("condition_end");
    if (SYMBOL_TYPE != SymbolType::IF) {
        ERROUT(ErrorType::NO_IF);
    } else {
        NEXTSYM;
    }
    // ����
    conditionAnalyze(condition);
    TEST(ErrorEnd::CONDITION);
    semanticAnalyzer.dissatisfyJump(condition, elseLabel);
    // then �ؼ���
    if (SYMBOL_TYPE != SymbolType::THEN) {
        ERROUT(ErrorType::NO_THEN);
    } else
        NEXTSYM;
    // Ĭ���� then������
    // ���
    statementAnalyze();
    TEST(ErrorEnd::CONDITION_STATEMENT);
    semanticAnalyzer.jumpAnyway(endLabel);
    // ������û�� else �ؼ��֣�else-labe ������
    semanticAnalyzer.label(elseLabel);
    // else �ؼ���
    if (SYMBOL_TYPE == SymbolType::ELSE) {
        NEXTSYM;
        statementAnalyze();
        TEST(ErrorEnd::CONDITION_STATEMENT);
    }
    semanticAnalyzer.label(endLabel);
    CLEAR;
END:
    ;
}

// do-while ���
void GrammarAnalyzer::doWhileStatementAnalyze() {
    SymbolType condition;
    string lab = semanticAnalyzer.newLabel("loop_do_while");
    // do �ؼ���
    if (SYMBOL_TYPE != SymbolType::DO) {
        ERROUT(ErrorType::NO_DO);
    } else
        NEXTSYM;
    // label
    semanticAnalyzer.label(lab);
    // ���
    statementAnalyze();
    TEST(ErrorEnd::DO_WHILE_STATEMENT);
    // whlie �ؼ���
    if (SYMBOL_TYPE != SymbolType::WHILE) {
        ERROUT(ErrorType::NO_WHILE);
        SKIP;
        TEST(ErrorEnd::DO_WHILE_STATEMENT);
    } else
        NEXTSYM;
    // ����
    conditionAnalyze(condition);
    TEST(ErrorEnd::DO_WHILE_STATEMENT);
    // ����ѭ����������תָ��
    semanticAnalyzer.satisfyJump(condition, lab);
    CLEAR;
END:
    ;
}

// for ���
void GrammarAnalyzer::forStatementAnalyze() {
    string step;            // ÿһ��Ҫ�ӵ���ֵ������� TO���� step ӦΪ 1��DOWNTO ӦΪ -1
    SymbolType condition;   // ѭ���ж������������ TO����ӦΪ <=��DOWNTOӦΪ >=
    string forvar;          // ѭ��������
    string start;           // ѭ��������ֵ
    string end;             // ѭ��������ֵ
    string startlabel = semanticAnalyzer.newLabel("loop_for_start");
    string endlabel = semanticAnalyzer.newLabel("loop_for_end");
    BasicType forvartype;
    bool v;
    BasicType t;
    string tmp;
    // for �ؼ���
    if (SYMBOL_TYPE != SymbolType::FOR) {
        ERROUT(ErrorType::NO_FOR);
    } else
        NEXTSYM;
    // ��ʶ����ѭ������
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROUT(ErrorType::NO_IDENTIFIER);
        SKIP;
        TEST(ErrorEnd::FOR_STATEMENT);
    } else {
        table.find(TOKEN_SYMBOL);
        if (table.ste == NULL) {
        // ����Ƿ��ж���
            ERROUT(ErrorType::UNDEFINED);
            SKIP;
            TEST(ErrorEnd::FOR_STATEMENT);
        } else if (table.ste->type != IdentifierType::VAR) {
        // ����ǲ��Ǳ���
            ERROUT(ErrorType::UNVARIABLE, "assign to function is forbid");
            SKIP;
            TEST(ErrorEnd::FOR_STATEMENT);
        } else {
            forvar = table.encodeKey();
            forvartype = table.ste->valueType;
            NEXTSYM;
        }
    }
    // ��ֵ��
    if (SYMBOL_TYPE != SymbolType::ASSIGN) {
        ERROUT(ErrorType::NO_ASSIGN);
    } else
        NEXTSYM;
    // Ĭ���и�ֵ��������
    // ���ʽ��ѭ�������ĳ�ֵ
    expressionAnalyze(start, t, v);
    TEST(ErrorEnd::FOR_STATEMENT);
    // ��ѭ����������ֵ
    semanticAnalyzer.assign(forvar, start, forvartype, t);
    // to | downto �ؼ���
    switch (SYMBOL_TYPE) {
    case SymbolType::TO:
        semanticAnalyzer.intToString(1, step);
        condition = SymbolType::LESS_EQUAL;
        NEXTSYM;
        break;
    case SymbolType::DOWNTO:
        semanticAnalyzer.intToString(-1, step);
        condition = SymbolType::GREATER_EQUAL;
        NEXTSYM;
        break;
    default:
        ERROUT(ErrorType::NO_DOWN_TO);
        SKIP;
        TEST(ErrorEnd::FOR_STATEMENT);
        break;
    }
    // ���ʽ��ѭ����ֹ����
    expressionAnalyze(end, t, v);
    TEST(ErrorEnd::FOR_STATEMENT);
    // do �ؼ���
    if (SYMBOL_TYPE != SymbolType::DO) {
        ERROUT(ErrorType::NO_DO);
    } else
        NEXTSYM;
    // Ĭ���� do������
    semanticAnalyzer.label(startlabel);                     // ѭ����ʼ��ʶ
    semanticAnalyzer.compare(forvar, end);
    semanticAnalyzer.dissatisfyJump(condition, endlabel);   // ����ѭ������
    // ���
    statementAnalyze();                                     // ѭ����
    TEST(ErrorEnd::FOR_STATEMENT);
    semanticAnalyzer.addition(forvar, step, tmp);           // ѭ�������������Լ�
    semanticAnalyzer.assign(forvar, tmp, forvartype, BasicType::INTEGER);
    semanticAnalyzer.jumpAnyway(startlabel);                // ��������ת��ѭ����ʼ
    semanticAnalyzer.label(endlabel);                       // ѭ��������ʶ������
    CLEAR;
END:
    ;
}

// �������
void GrammarAnalyzer::compoundStatementAnalyze() {
    // begin �ؼ���
    if (SYMBOL_TYPE != SymbolType::BEGIN) {
        ERROUT(ErrorType::NO_BEGIN);
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
        ERROUT(ErrorType::NO_END);
    } else
        NEXTSYM;
    CLEAR;
END:
    ;
}

// �����
void GrammarAnalyzer::readStatementAnalyze() {
    // read �ؼ���
    string var;
    if (SYMBOL_TYPE != SymbolType::READ) {
        ERROUT(ErrorType::NO_READ);
        SKIP;
        TEST(ErrorEnd::READ_STATEMENT);
    } else
        NEXTSYM;
    // ������
    if (SYMBOL_TYPE != SymbolType::LEFT_BRACKET) {
        ERROUT(ErrorType::NO_LEFTBRACKET);
    } else
        NEXTSYM;
    // Ĭ���������ţ�����
    // ��ʶ��
    if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
        ERROUT(ErrorType::NO_IDENTIFIER);
        SKIP;
        TEST(ErrorEnd::READ_STATEMENT);
    } else {
        table.find(TOKEN_SYMBOL);
        if (table.ste == NULL) {
        // ����ʶ���Ƿ��ж���
            ERROUT(ErrorType::UNDEFINED, TOKEN_SYMBOL);
        } else if (table.ste->type != IdentifierType::VAR) {
        // ����ǲ��Ǳ���
            ERROUT(ErrorType::UNVARIABLE, TOKEN_SYMBOL);
        } else {
        // ���������ͣ����ɶ�ָ��
            var = table.encodeKey();
            if (table.ste->valueType == BasicType::CHAR)
                semanticAnalyzer.readChar(var);
            else
                semanticAnalyzer.readInteger(var);
        }
    }
    NEXTSYM;
    while (SYMBOL_TYPE == SymbolType::COMMA) {
        NEXTSYM;
        // ��ʶ��
        if (SYMBOL_TYPE != SymbolType::IDENTIFIER) {
            ERROUT(ErrorType::NO_IDENTIFIER);
            SKIP;
            TEST(ErrorEnd::READ_STATEMENT);
        } else {
            table.find(TOKEN_SYMBOL);
            if (table.ste == NULL) {
            // ����ʶ���Ƿ��ж���
                ERROUT(ErrorType::UNDEFINED, TOKEN_SYMBOL);
            } else if (table.ste->type != IdentifierType::VAR) {
            // ����ǲ��Ǳ���
                ERROUT(ErrorType::UNVARIABLE, TOKEN_SYMBOL);
            } else {
            // ���������ͣ����ɶ�ָ��
                var = table.encodeKey();
                if (table.ste->valueType == BasicType::CHAR)
                    semanticAnalyzer.readChar(var);
                else
                    semanticAnalyzer.readInteger(var);
            }
        }
        NEXTSYM;
    }
    // ������
    if (SYMBOL_TYPE != SymbolType::RIGHT_BRACKET) {
        ERROUT(ErrorType::NO_RIGHTBRACKET);
    } else
        NEXTSYM;
    CLEAR;
END:
    ;
}

// д���
void GrammarAnalyzer::writeStatementAnalyze() {
    string name;
    BasicType type;
    bool v;
    // write �ؼ���
    if (SYMBOL_TYPE != SymbolType::WRITE) {
        ERROUT(ErrorType::NO_WRITE);
    } else
        NEXTSYM;
    // ������
    if (SYMBOL_TYPE != SymbolType::LEFT_BRACKET) {
        ERROUT(ErrorType::NO_LEFTBRACKET);
    } else
        NEXTSYM;
    // Ĭ����������
    // ����
    if (SYMBOL_TYPE == SymbolType::DOUBLE_QUOTE) {
        // �ַ���
        stringAnalyze(name);
        TEST(ErrorEnd::WRITE_STATEMENT);
        semanticAnalyzer.writeString(name);
        if (SYMBOL_TYPE == SymbolType::COMMA) {
            NEXTSYM;
            // ���ʽ
            expressionAnalyze(name, type, v);
            TEST(ErrorEnd::WRITE_STATEMENT);
            if (type == BasicType::CHAR)
                semanticAnalyzer.writeChar(name);
            else
                semanticAnalyzer.writeInteger(name);
        }
    } else {                                      
        // ���ʽ
        expressionAnalyze(name, type, v);
        TEST(ErrorEnd::WRITE_STATEMENT);
        if (type == BasicType::CHAR)
            semanticAnalyzer.writeChar(name);
        else
            semanticAnalyzer.writeInteger(name);
    }
    // ������
    if (SYMBOL_TYPE != SymbolType::RIGHT_BRACKET) {
        ERROUT(ErrorType::NO_RIGHTBRACKET);
    } else
        NEXTSYM;
    semanticAnalyzer.newline();
    CLEAR;
END:
    ;
}