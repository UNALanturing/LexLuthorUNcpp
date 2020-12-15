%{
//*****PARTE EN C++*****
#include <bits/stdc++.h>
#define NUM 600
#define OPERADOR 601
#define MAS 700
#define MENOS 701
#define POR 702
#define DIV 703

void showError();
int operatorID();

using namespace std;

%}

entero  [0-9]+
operador [\\+\\-x/]

%%
{entero}    {return (NUM);}
{operador}  {yylval = operatorID();cout << "lexema: " << yytext << " yylval: " << yylval << '\n'; return (OPERADOR);}

%%

int main(){
	freopen("input.txt", "r", stdin);
	for(auto x: yylex()){
		cout << x.first << " " << x.second << '\n';
	}
}

int operatorID(){
	if(yytext == "+") return MAS;
	if(yytext == "-") return MENOS;
	if(yytext == "x") return POR;
	if(yytext == "/") return DIV;
	return -1;
}

void showError(){
    printf("Other input");
}