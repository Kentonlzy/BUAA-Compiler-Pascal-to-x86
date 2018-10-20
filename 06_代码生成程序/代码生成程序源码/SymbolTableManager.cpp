/*
    SymbolTableManager.cpp
    ʵ�ַ��ű����֯�͹���
*/
#include "SymbolTableManager.h"
using namespace std;

static STE root;                        // ���η��ű�ĸ��������������루�������򣩣�����
static map<string, string> stringMap;   // �����������õ����ַ���

// ����ģʽ���ٵĹ�����
SymbolTableManager &SymbolTableManager::initialSymbolTableManager() {
    static SymbolTableManager theSymbolTableManager;
    return theSymbolTableManager;
}

// �����Ĺ��캯��
SymbolTableManager::SymbolTableManager(): ste(NULL), position(&root) {}

// �� position �����ƽ�һ��
// ���뱣֤ block �Ѿ���ȷ���뵽���ű���
// �߼��ϣ�block ����Ϊ�������������
void SymbolTableManager::goIn(const string &block) {
    map<string, STE*>::iterator it = position->childs->find(block);
    if (it != position->childs->end())
        position = it->second;
}

// �� position �����˻�һ��
void SymbolTableManager::goOut() {
    if (position->father != NULL)
        position = position->father;
}

// �жϱ��е�ǰ���Ƿ����������
bool SymbolTableManager::isHere(const string &id) {
    if (position->childs != NULL)
        return position->childs->count(id) > 0 || id == position->name;
    return id == position->name;
}

// ����鵽���� true
// �Ѳ鵽�ı����ָ�븳�� ste
// ��û�в鵽���� ste ��Ϊ NULL ������ false
bool SymbolTableManager::find(const string &id) {
    STE *ps = position->childs != NULL ? position : position->father;
    while (ps != NULL) {
        map<string, STE *>::iterator it = ps->childs->find(id);
        if (it != ps->childs->end()) {
            ste = it->second;
            return true;
        }
        ps = ps->father;
    }
    ste = NULL;
    return false;
}

// ���, �� ste ���뵽���е�ǰ�㣨position�㣩
// �ڴ�֮ǰ����ȷ�� isHere(ste->name) == false
void SymbolTableManager::insert() {
    ste->father = position;
    if (position->childs == NULL)
        position->childs = new map<string, STE *>;
    position->childs->insert(pair<string, STE *>(ste->name, ste));
}

static char *pointToStr(const void *point) {
    static char str[9];
    std::sprintf(str, "%08X", point);
    return str;
}

// �� ste ת��ΪΨһ��ʶ
string &SymbolTableManager::encodeKey() {
    static string idkey;
    idkey.clear();
    switch (ste->type) {
    case IdentifierType::ARRAY:
        idkey.append("a_");
        break;
    case IdentifierType::FUNCTION:
        idkey.append("f_");
        break;
    case IdentifierType::PROCEDURE:
        idkey.append("p_");
        break;
    case IdentifierType::VAR:
        idkey.append("v_");
        break;
    default:
        idkey.append("c_");
        break;
    }
    idkey.append(ste->name);
    idkey.append("_");
    idkey.append(pointToStr(ste));
    return idkey;
}

// �� position ת��ΪΨһ��ʶ
string &SymbolTableManager::encodePosition() {
    static string key;
    if (position == &root) {
        key = "_main";
        return key;
    } else {
        ste = position;
        return encodeKey();
    }        
}

// ��Ψһ��ʶת��Ϊ ste
void SymbolTableManager::decodeKey(const string &idkey) {
    if (idkey == "_main")
        ste = &root;
    else
        std::sscanf(idkey.substr(idkey.size()-8).c_str(), "%08X", ste);
}

void SymbolTableManager::addString(const string &name, const string &str) {
    stringMap.insert(pair<string, string>(name, str));
}

map<string, string> &SymbolTableManager::getStringMap() {
    return stringMap;
}

void SymbolTableManager::defineArguments(STE *te, vector<Argument> &args) {
    // ���ԭ������
    if (te->arguments != NULL)
        delete te->arguments;
    te->arguments = new vector<Argument>(args);
    // ȷ���ӽڵ� map ��Ч
    if (te->childs == NULL)
        te->childs = new map<string, STE *>;
    // ����Ѳ�����Ϊ te �ľֲ�����
    for (vector<Argument>::iterator it = args.begin(); it != args.end(); it++) {
        STE *nte = new STE;
        nte->name = it->name;
        nte->type = IdentifierType::VAR;
        nte->valueType = it->type;
        nte->father = te;
        te->childs->insert(pair<string, STE *>(it->name, nte));
    }
}