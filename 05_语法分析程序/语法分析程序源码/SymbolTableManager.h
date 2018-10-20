/*
    SymbolTableManager.h
    ���ű�������Ľӿںͱ�������
*/
#ifndef SYMBOL_TABLE_MANAGER__H
#define SYMBOL_TABLE_MANAGER__H

#include "type.h"

class SymbolTableManager {
public:
    static bool found;
    static string id;
    static IdentifierType idtype;      
    static BasicType vtype;

    static SymbolTableManager  &initialSymbolTableManager();

};

#endif // !SYMBOL_TABLE_MANAGER__H
