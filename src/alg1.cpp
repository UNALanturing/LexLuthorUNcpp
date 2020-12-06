#include<bits/stdc++.h>
#define LAMBDA '~'
#define CONCAT '`'



using namespace std;

struct Node{
  public:
    char simbol;
    Node *left = NULL;
    Node *right = NULL;
    int index;
    set<int> firstpos;
    set<int> lastpos;
    set<int> followpos;
    bool isNull = false;
    bool nullable;
    bool nullable_computed = false;
    bool firstpos_computed = false;
    bool lastpos_computed = false;
    Node(){
    	isNull = true;
    };

    Node(char s, Node *l, Node *r){
      simbol = s;
      left = l;
      right = r;
    };

    Node(char s, Node *l){
      simbol = s;
      left = l;
    };

    Node(char s, int ind){
      simbol = s;
      index = ind;
    };

    
};




bool nullable(Node &n){
  if(n.nullable_computed) return n.nullable;
  if(n.simbol == LAMBDA || n.simbol == '*') n.nullable = true;
  else if(n.simbol == '|') n.nullable = nullable(*n.left) || nullable(*n.right);
  else if(n.simbol == CONCAT) n.nullable = nullable(*n.left) && nullable(*n.right);
  else n.nullable = false;
  n.nullable_computed = true;
  return n.nullable;
};


set<int> unir(set<int> s1, set<int> s2){
	vector<int> v1(s1.begin(),s1.end());
	vector<int> v2(s2.begin(),s2.end());
	vector<int> v(v1.size() + v2.size());
	auto it = set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), v.begin());
    v.resize(it - v.begin());
    return set<int>(v.begin(), v.end());
};

set<int> firstpos(Node &n){
  if(n.firstpos_computed) return n.firstpos;
  if(n.simbol == LAMBDA);
  else if(n.simbol == '|') n.firstpos = unir(firstpos(*n.left), firstpos(*n.right));
  else if(n.simbol == CONCAT){
    if(nullable(*n.left)) n.firstpos = unir(firstpos(*n.left), firstpos(*n.right));
    else n.firstpos = firstpos(*n.left); 
  }else if(n.simbol == '*') n.firstpos = firstpos(*n.left);
  else n.firstpos.insert(n.index);
  n.firstpos_computed = true;
  return n.firstpos;
};

set<int> lastpos(Node &n){
  if(n.lastpos_computed) return n.lastpos;
  if(n.simbol == LAMBDA);
  else if(n.simbol == '|') n.lastpos = unir(lastpos(*n.left), lastpos(*n.right));
  else if(n.simbol == CONCAT){
    if(nullable(*n.right)) n.lastpos = unir(lastpos(*n.left), lastpos(*n.right));
    else n.lastpos = lastpos(*n.right); 
  }else if(n.simbol == '*') n.lastpos = lastpos(*n.left);
  else n.lastpos.insert(n.index);
  n.lastpos_computed = true;
  return n.lastpos;
};

void computeFirstAndLastpos(Node &n){
  if(n.simbol == CONCAT || n.simbol == '|' || n.simbol == '*') computeFirstAndLastpos(*n.left);
  if(n.simbol == CONCAT || n.simbol == '|' ) computeFirstAndLastpos(*n.right);
  firstpos(n);
  lastpos(n);
};


//Función auxiliar de la funcion followpos para insertar una posicion dentro del conjunto followpos de una hoja con indice index.
void insertFollowpos(Node &n, int index, set<int> numeros){
  if(n.simbol == CONCAT || n.simbol == '|' || n.simbol == '*' ){
    insertFollowpos(*n.left, index, numeros);
    if(n.simbol != '*') insertFollowpos(*n.right, index, numeros);
  }else if(n.index == index){
    for(int x : numeros){
      n.followpos.insert(x); 
    }
  }
};

void followpos(Node &root, Node &n){
  if(n.simbol == CONCAT){
    for(int i : (*n.left).lastpos){
      if(!(*n.right).firstpos.empty()) insertFollowpos(root, i, set<int>((*n.right).firstpos.begin(), (*n.right).firstpos.end()));
    }
  }else if(n.simbol == '*'){
    for(int i : n.lastpos){
      if(!n.firstpos.empty()) insertFollowpos(root, i, set<int>(n.firstpos.begin(), n.firstpos.end()));
    }
  }
};

void computeFollowpos(Node &root, Node &n){
	if(n.simbol == CONCAT || n.simbol == '|' || n.simbol == '*') computeFollowpos(root,*n.left);
	if(n.simbol == CONCAT || n.simbol == '|' ) computeFollowpos(root,*n.right);
	followpos(root, n);
};

string setToString(set<int> s){
	string ans = "";
	for(int x : s){
		ans += to_string(x);
	}
	return ans;
}

class AFD{
	public:
		//Atributos
		int initial_state;
		int num_states;
		set<char> alfabeto;
		int **transitions;//matriz de transicion
		set<int> final_states;//estados de aceptacion
		unordered_map<char,int> number_simbol;//Dicionario que asocia cada simbolo del alfabeto con su numero correspondiente en la matriz de transicion

		//Constructor que implementa el algoritmo 1 de la seccion 3.9 libro de Aho
		AFD(Node &root){
			nullable(root);
			computeFirstAndLastpos(root);
			computeFollowpos(root,root);
			unordered_map<int, set<int>> flp;
			unordered_map<int, char> lc;
			int lastleave;
			leaves(root,flp,lc,lastleave);
			set<string> dstates;
			set<string> fstates;
			dstates.insert(setToString(root.firstpos));
			vector<tuple<string,char,string>> dtran;
			queue<set<int>> q;
			q.push(root.firstpos);
			while(!q.empty()){
				set<int> s = q.front();
				q.pop();
				for(char a : alfabeto){
					set<int> u;
					for(int p : s){
						if(lc[p]==a) u = unir(flp[p],u);
					}
					string su = setToString(u);
					if(!u.empty() && !dstates.count(su)){
						q.push(u);
						dstates.insert(su);//nuevo estado
						if(u.count(lastleave)) fstates.insert(su);// estados de aceptacion
					}
					dtran.push_back( make_tuple(setToString(s), a, su) );//nueva transicion
				}
				
			}
			
			cout <<"states: " << '\n';
			for(string state : dstates){
				cout << state << '\n';
			}
			cout << "\nTransitions:" << '\n';
			for(auto transition : dtran){
				cout << "(" << get<0>(transition) << " , " << get<1>(transition) << " )  --> " << get<2>(transition) << '\n'; 
			}
			
			//establecemos el numero total de estados
			num_states = dstates.size();
			
			int i =0;
			//diccionario que asocia string de estado con su numero.
			unordered_map<string, int> number_state;
			for(string state : dstates){
				number_state[state] = i;
				i++;
			}

			//diccionario que asocia cada simbolo del alfabeto con su numero correspondiente.
			i=0;
			for(char a : alfabeto){
				number_simbol[a] = i;
				i++;
			}

			//establecemos el estado inicial
			initial_state = number_state[setToString(root.firstpos)];
			
			//establecemos los estados de aceptacion
			for(string s : fstates) final_states.insert(number_state[s]);

			//inicializamos la matriz de transicion
			int num_simbols = alfabeto.size();
			transitions = new int*[num_states];
			for(int i=0; i<num_states;i ++){
				transitions[i] = new int[num_simbols];
			}
			//inicializamos todas las transiciones en -1 
			for(i=0; i<num_states;i++){
				for(int j=0;j<num_simbols; j++){
					transitions[i][j] = -1;
				}
			}
			//Actualizamos la matriz con las transiciones validas.
			for(tuple<string, char, string> t : dtran){
				transitions[number_state[get<0>(t)]][number_simbol[get<1>(t)]] = number_state[get<2>(t)];
			}

		};

		//Funcion auxiliar para obtener la informacion de las hojas del arbol sintactico usadas en el constructor.
		void leaves(Node &n, unordered_map<int, set<int>> &flp, unordered_map<int, char> &lc, int &lv){
			if(n.simbol == CONCAT || n.simbol == '|'){ leaves(*n.left,flp,lc,lv); leaves(*n.right,flp,lc,lv);}
			else if(n.simbol == '*') leaves(*n.left,flp,lc,lv);
			else{
				flp[n.index] = n.followpos;
				lc[n.index] = n.simbol;
				if(n.simbol != '#') alfabeto.insert(n.simbol);
				else lv = n.index;
			} 
		};

		//Imprimir informacion del automata:
		void info(){
			cout << "Alfabeto: \n{";
			for(char a : alfabeto) cout << a << " ";
			cout << "}\nNumero de estados: " << num_states << '\n';
			cout << "Estado inicial: " << initial_state << '\n';
			cout << "Estados de aceptacion: " << "\n{";
			for(int x : final_states) cout << x << " ";
			cout << "}\nMatriz de transicion: " << '\n';

			for(int i=0; i<num_states; i++){
				for(int j=0; j<alfabeto.size(); j++){
					cout << transitions[i][j] << " ";
				}
				cout << '\n';
			}
		};
};



Node* parseRegex(string regex){
	stack<Node*> s;
	Node* root = NULL;
	int leaf_index = 1;
	try{
		for(int i = 0; i<regex.length(); i++){
			if(regex[i] == '('){
				if(root == NULL || regex[i-1] == '(') s.push(NULL);
				else if(regex[i-1] == '|') s.push(new Node('|', root));
				else s.push(new Node(CONCAT, root));
				root = NULL;			
			}else if(regex[i] == ')'){
				if(s.empty()) throw i;
				Node* snode = s.top();
				s.pop();
				if(snode == NULL);
				else{
					snode -> right = root;
					root = snode;
				}
			}else if(regex[i] == '*'){
				if(root == NULL) throw i;
				if(root -> simbol == '*');
				else if(root -> simbol == '|' || root -> simbol == CONCAT){
					if(regex[i-1] == ')') root = new Node('*', root);
					else{
						root -> right = new Node('*', root -> right);
					}
				}
			}else if(regex[i] == '|'){
				if(root == NULL) throw i;
				else if(regex[i-1] != '|'){
					root = new Node('|', root);
				}else throw i;
			}else{
				if(root == NULL) root = new Node(regex[i], leaf_index++);
				else if(regex[i-1] != '|') root = new Node(CONCAT, root, new Node(regex[i], leaf_index++));
				else root -> right = new Node(regex[i], leaf_index++);
			}	
		}
		if(!s.empty()) throw (int) regex.length();
	}catch(int e){
		cout << "Error parsing " << regex << " at index " << e << '\n';
	}
	return root;
};

void printInOrder(struct Node* root){
	if(root == NULL) return;
	else if(root -> simbol == CONCAT || root -> simbol == '|' || root -> simbol == '*'){
		cout << "("; printInOrder(root -> left); cout << ")";
		if(root -> simbol != CONCAT) cout << root -> simbol;
		if(root -> simbol == '|' || root -> simbol == CONCAT) cout << "("; printInOrder(root -> right); cout << ")";
	}else{
		cout << root -> simbol;
	}
};

int main(){
	
	//Arbol sintactico del ejemplo 3.56 del libro de Aho
	//Hojas
	Node h1('a',1);
	Node h2('b',2);
	Node h3('a',3);
	Node h4('b',4);
	Node h5('b',5);
	Node h6('#',6);
	//Nodos internos
	Node n7('|',&h1,&h2);
	Node n8('*',&n7);
	Node n9(CONCAT,&n8, &h3);
	Node n10(CONCAT,&n9,&h4);
	Node n11(CONCAT,&n10,&h5);
	Node root(CONCAT,&n11,&h6);
	printInOrder(&root);
	cout << '\n';
	/*
	AFD automata(root);
	automata.info();
	*/
	Node* root1 = parseRegex("(a|b)*abb#");
	//Node* root = new Node('|', new Node('a',1), new Node('b',2));
	//cout << p -> right -> simbol << '\n';
	printInOrder(root1);
	cout << '\n';
	return 0;
}