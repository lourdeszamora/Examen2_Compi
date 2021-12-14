#include "ast.h"
#include <map>
#include <iostream>

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
    return "If statement code generation\n";
}

void MethodInvocationExpr::genCode(Code &code){
    
}

string AssignationStatement::genCode(){
    return "Assignation statement code generation\n";
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
    return "Method definition code generation\n";
}