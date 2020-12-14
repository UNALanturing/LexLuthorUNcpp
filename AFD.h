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
		vector<char> alp;//Alphabet <-- is ordered
		vector< vector<int> > transitions;//matriz de transicion
		set<int> final_states;//estados de aceptacion
		vector<int> f;//Accepted states
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
			/*
			cout <<"states: " << '\n';
			for(string state : dstates){
				cout << state << '\n';
			}
			cout << "\nTransitions:" << '\n';
			for(auto transition : dtran){
				cout << "(" << get<0>(transition) << " , " << get<1>(transition) << " )  --> " << get<2>(transition) << '\n'; 
			}
			*/
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
				alp.push_back(a);
				i++;
			}


			//establecemos el estado inicial
			initial_state = number_state[setToString(root.firstpos)];
			
			//establecemos los estados de aceptacion
			for(string s : fstates) final_states.insert(number_state[s]);
			for(int i=0; i<num_states; i++){
				f.push_back(final_states.count(i));
			}

			
			//inicializamos la matriz de transicion
			int num_simbols = alfabeto.size();
			/*
			transitions = new int*[num_states];
			for(int i=0; i<num_states;i ++){
				transitions[i] = new int[num_simbols];
			}
			*/
			for(int i=0; i<num_states; i++){
				vector<int> aux(num_simbols,-1);
				transitions.push_back(aux);
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

		AFD(int s, int q, vector<char> alph, vector<int> fs, vector< vector<int> > table){
			initial_state = s;
			num_states = q;
			alp = alph;
			alfabeto = set<char>(alp.begin(), alp.end());
			f = fs;
			for(int i=0; i<fs.size(); i++){
				if(fs[i]) final_states.insert(i);
			}
			transitions = table;
		}
		
		bool procesarString(string cadena){
			int current_state = initial_state;
			for(char c : cadena){
				if(alfabeto.count(c) && transitions[current_state][number_simbol[c]] != -1){
					current_state = transitions[current_state][number_simbol[c]];
				}else return 0;
			}
			return final_states.count(current_state);
		}

		//Imprimir informacion del automata:
		void info(){
			cout << "Alfabeto: \n{";
			for(char a : alfabeto) cout << a << " ";
			cout << "}\nNumero de estados: " << num_states << '\n';
			cout << "Estado inicial: " << initial_state << '\n';
			cout << "Estados de aceptacion: " << "\n{";
			for(int x : final_states) cout << x << " ";
			cout << "}\n{";
			for(int x : f) cout << x << " ";
			cout << "}\nMatriz de transicion: " << '\n';

			for(int i=0; i<num_states; i++){
				for(int j=0; j<alfabeto.size(); j++){
					cout << transitions[i][j] << " ";
				}
				cout << '\n';
			}
		};
};

AFD minAFD(AFD aut) {
	int n = aut.num_states; //number of nodes in the original automaton
	vector <int> father(n, -1); //Representative node of each partition
	map <int, vector <int> > pi; //Current partitions (1 acceptance and 0 non acceptance)
	vector <int> aux (2, -1);
	for (int i = 0; i < n; ++i) {
		if (aux[aut.f[i]] == -1) aux[aut.f[i]] = i;
	}
	for (int i = 0; i < n; ++i) {
		pi[aux[aut.f[i]]].push_back(i);
		father[i] = aux[aut.f[i]];
	}
	while (1) { //While there are new partitions
		map < int, vector <int>> newPar; //Every new partition formed
		vector < vector <int> > tran (n, vector <int> (aut.alp.size())); //transition to representative node
		for (auto g: pi) { //for each group G in PI (current paritition)
			map <vector <int> , vector <int> > curPar;//Transitions for the current partition (in order to not merge partitions)
			for (int s : g.second) { // for every node in group G
				for (int i = 0; i < aut.alp.size(); ++i) { //check transition with every char in alphabet
					tran[s][i] = father[aut.transitions[s][i]]; 
				}
				curPar[tran[s]].push_back(s);
			}
			for (auto cp : curPar) {
				newPar[cp.second[0]] = cp.second;
				for (int i : cp.second) father[i] = cp.second[0];
			}
		}
		if (newPar == pi) break;
		pi = newPar;
	}
	//PI contains all groups of equivalent states
	int s = father[aut.initial_state]; //New initial state is the representative node of initial state's group
	int q = pi.size(); //Size of PI equals number of minimum states
	vector <int> f(q, 0);//New vector of acceptance states
	for (int i = 0; i < q; ++i) f[i] = aut.f[father[i]]; //Acceptance states remin as accepted states
	vector < vector <int> > table(q, vector <int> (aut.alp.size())); //New table of transitions
	int idx = 0; //current state's index
	for (auto s : pi) { //
		for (int i = 0; i < aut.alp.size(); ++i) { //Adding new transitions;
			table[idx][i] = father[aut.transitions[s.first][i]];
		}
		idx++;
	}
	return AFD(s, q, aut.alp, f, table); //Return new minimum states automaton
}

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


AFD buildAFD(string s){
	//Aplicar acá a s la función que traduce las expresiones regulares!!
	s += '#';
	Node* root = parseRegex(s);
	return AFD(*root);
}

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

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
};

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
};

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
};