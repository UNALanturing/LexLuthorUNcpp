%{
//*****PARTE EN C++*****
#include <bits/stdc++.h>
#define NOMBRE 600
#define PATRONOMICO_HOMBRE 601
#define PATRONOMICO_MUJER 602
void showError();

using namespace std;
%}

nombre [A-Z][a-z]+

%%

{nombre}vich {return (PATRONOMICO_HOMBRE);}
{nombre}oba {return (PATRONOMICO_MUJER);}
{nombre} {return(NOMBRE);}
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