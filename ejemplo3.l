%{
#include<bits/stdc++.h>
using namespace std;

%}

%%

is|am|are|were|was|be|being|been|do|does|did|will|would|should|can|could|has|have|had|go {cout << yytext << ": is a verb\n";}
very|simply|gently|quietly|calmly|angrily {cout << yytext << ": is an adverb\n";}
to|from|behind|above|below|between|below {cout << yytext << ": is a preposition\n";}
if|then|and|but|or {cout << yytext << ": is a conjunction\n";}
their|my|your|his|her|its {cout << yytext << ": is a adjective\n";}
I|you|he|she|we|they {cout << yytext << ": is a pronoun\n";}
[a-zA-Z]+ {cout << yytext << ": don't recognize, might be a noun\n";}

%%

int main(){
	freopen("input.txt", "r", stdin);
	yylex();
}