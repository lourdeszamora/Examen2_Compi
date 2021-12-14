#include "ast.h"
#include <map>
#include <iostream>

map<string, float> variables = {}; 
map<string, MethodDefinition *> methods = {}; 

void FloatExpr::genCode(Code &code){
}

void SubExpr::genCode(Code &code){
}

void DivExpr::genCode(Code &code){
}

void IdExpr::genCode(Code &code){
}

string ExprStatement::genCode(){

}

string IfStatement::genCode(){
  
}

void MethodInvocationExpr::genCode(Code &code){
    
}

string AssignationStatement::genCode(){
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

}

string ReturnStatement::genCode(){

}