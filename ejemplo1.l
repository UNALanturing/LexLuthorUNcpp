%{
//*****PARTE EN C++*****
#include <bits/stdc++.h>
#define NUM 600
#define ID 601
#define CONSTA 602
void showError();

using namespace std;
%}

entero  [0-9]+
id      [a-z]+
constante [A-Z]+

%%
{entero}    {return (NUM);}
{id}        {return (ID);}
{constante} {return(CONSTA);}
%%

int main(){
	freopen("input.txt", "r", stdin);
	for(auto x: yylex()){
		cout << x.first << " " << x.second << '\n';
	}
}

void showError(){
    printf("Other input");
}