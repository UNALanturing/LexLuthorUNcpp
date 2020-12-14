%{
//*****PARTE EN C++*****
#include <bits/stdc++.h>
#define NUM 600
#define ID 601
#define CONSTA 602
void showError();

using namespace std;
%}

entero  (0|1|2|3|4|5|6|7|8|9)*(0|1|2|3|4|5|6|7|8|9)
id      (a|s|d)*(a|s|d)
constante (A|S|D)*(A|S|D)

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