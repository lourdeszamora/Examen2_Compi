#include "ast.h"
#include <iostream>
#include <sstream>
#include <set>
#include "asm.h"

const char * floatTemps[] = {"$f0",
                            "$f1",
                            "$f2",
                            "$f3",
                            "$f4",
                            "$f5",
                            "$f6",
                            "$f7",
                            "$f8",
                            "$f9",
                            "$f10",
                            "$f11",
                            "$f12",
                            "$f13",
                            "$f14",
                            "$f15",
                            "$f16",
                            "$f17",
                            "$f18",
                            "$f19",
                            "$f20",
                            "$f21",
                            "$f22",
                            "$f23",
                            "$f24",
                            "$f25",
                            "$f26",
                            "$f27",
                            "$f28",
                            "$f29",
                            "$f30",
                            "$f31"
                        };

#define FLOAT_TEMP_COUNT 32
set<string> intTempMap;
set<string> floatTempMap;
int labelCounter = 0;

map<string,string> codeGenerationVars;

extern Asm assemblyFile;

int globalStackPointer = 0;

string saveState(){ 
    set<string>::iterator it = floatTempMap.begin();
    stringstream ss;
    ss<<"sw $ra, " <<globalStackPointer<< "($sp)\n";
    globalStackPointer+=4;
    return ss.str();
}
string retrieveState(string state){
    std::string::size_type n = 0;
    string s = "sw";
    while ( ( n = state.find( s, n ) ) != std::string::npos )
    {
    state.replace( n, s.size(), "lw" );
    n += 2;
    globalStackPointer-=4;
    }
    return state;
}

string getNewLabel(string prefix){
    stringstream ss;
    ss<<prefix << labelCounter;
    labelCounter++;
    return ss.str();
}

void releaseFloatTemp(string temp){
    floatTempMap.erase(temp);
}

void releaseRegister(string temp){
    releaseFloatTemp(temp);
}

string getFloatTemp(){
    for (int i = 0; i < FLOAT_TEMP_COUNT; i++)
    {
        if(floatTempMap.find(floatTemps[i]) == floatTempMap.end()){
            floatTempMap.insert(floatTemps[i]);
            return string(floatTemps[i]);
        }
    }
    cout<<"No more float registers!"<<endl;
    return "";
}

void releaseFloatTemp(string temp){
    floatTempMap.erase(temp);
}

void FloatExpr::genCode(Code &code){
}

void SubExpr::genCode(Code &code){
}

void DivExpr::genCode(Code &code){
}

void IdExpr::genCode(Code &code){
}

string ExprStatement::genCode(){
    return "Expr statement code generation\n";
}

string IfStatement::genCode(){
    string endIfLabel = getNewLabel("endif");
    Code exprCode;
    this->conditionalExpr->genCode(exprCode);
    stringstream code;
    code << exprCode.code << endl;
    
    code << "bc1f "<< endIfLabel <<endl;
    list<Statement *>::iterator it = this->trueStatement.begin();
    for (int  i = 0; i < this->trueStatement.size(); i++)
    {
        code<< (*it)->genCode() <<endl;
        it++;
    }
    code<< endIfLabel <<" :"<< endl;
    list<Statement *>::iterator it = this->falseStatement.begin();
    for (int  i = 0; i < this->falseStatement.size(); i++)
    {
        code<< (*it)->genCode() <<endl;
        it++;
    }
    releaseRegister(exprCode.place);
    
    return code.str();
}

void MethodInvocationExpr::genCode(Code &code){
    list<Expr *>::iterator it = this->expressions.begin();
    list<Code> codes;
    stringstream ss;
    Code argCode;
    while (it != this->expressions.end())
    {
        (*it)->genCode(argCode);
        ss << argCode.code <<endl;
        codes.push_back(argCode);
        it++;
    }

    int i = 0;
    list<Code>::iterator placesIt = codes.begin();
    while (placesIt != codes.end())
    {
        releaseRegister((*placesIt).place);
        ss << "mfc1 $a"<<i<<", "<< (*placesIt).place<<endl;
        i++;
        placesIt++;
    }

    ss<< "jal "<< this->id<<endl;
    string reg;
    reg = getFloatTemp();
    ss << "mtc1 $v0, "<< reg<<endl;
    
    code.code = ss.str();
    code.place = reg;
}

string AssignationStatement::genCode(){
    Code rightSideCode;
    stringstream ss;
    this->value->genCode(rightSideCode);
    ss<< rightSideCode.code <<endl;
    const string name = this->id;
    if(name != ""){
        
        if(codeGenerationVars.find(name) == codeGenerationVars.end()){
        
            ss << "s.s "<<rightSideCode.place << ", "<<name <<endl;
        }
        releaseRegister(rightSideCode.place);
    }
    return ss.str();
}

void GteExpr::genCode(Code &code){
}

void LteExpr::genCode(Code &code){
}

void EqExpr::genCode(Code &code){
}

void ReadFloatExpr::genCode(Code &code){
    
}

string PrintStatement::genCode(){
    return "Print statement code generation\n";
}

string ReturnStatement::genCode(){
    return "Return statement code generation\n";
}

string MethodDefinitionStatement::genCode(){
    if(this->stmts.empty())
        return "";
    int stackPointer = 4;
    stringstream code;
    code<< this->id << ": "<< endl;
    string state = saveState();
    code << state <<endl;
    if(this->params.size() > 0){
        list<string>::iterator it = this->params.begin();
        for(int i = 0; i< this->params.size(); i++){
            code << "sw $a"<<i<<", "<< stackPointer<<"($sp)"<<endl;
            codeGenerationVars.insert(*it,*it);
            stackPointer +=4;
            globalStackPointer +=4;
            it++;
        }
    }
    list<Statement *>::iterator it = this->stmts.begin();
    for (int  i = 0; i < this->stmts.size(); i++)
    {
        code<< (*it)->genCode() <<endl;
        it++;
    }
    stringstream sp;
    int currentStackPointer = globalStackPointer;
    sp << endl<<"addiu $sp, $sp, -"<<currentStackPointer<<endl;
    code << retrieveState(state);
    code << "addiu $sp, $sp, "<<currentStackPointer<<endl;
    code <<"jr $ra"<<endl;
    codeGenerationVars.clear();
    string result = code.str();
    result.insert(id.size() + 2, sp.str());
    return result;
}