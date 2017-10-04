#include<cstring>
#include<cstdlib>
#include<map>
#include<list>
#include<vector>
#include<iostream>
#include<sstream>
#include<fstream>
using namespace std;

struct Clause{
	bool is_positive;
	string name;
	vector<string> args;
};

struct Tree{
	string value;
	Tree *tLeft;
	Tree *tRight;
	Tree(Tree* t): value(t->value), tLeft(t->tLeft), tRight(t->tRight){ }
	Tree(): value(""), tLeft(NULL), tRight(NULL) { }
};

struct Unify{
	string var;
	string value;
};
bool and_or = true;

multimap<string, string> kb_cnf;
multimap<string, Clause> kb_rule;
multimap<string, Clause> kb_fact;
multimap<string, Clause> history_res;

string global = "";
int standarder = 0;
int max_rule = 0;
int max_fact = 0;

void seperate_rule_and_fact(list<string> letters, string current){
	string let;
	list<string> var_list;
	string pred_name = "";
	string var = "";
	Clause clus;
	const char *temp = current.c_str();
	char *cur = new char[strlen(temp)+1];
	strcpy(cur, temp);
	const char *d = "|";              
	char *strToken = strtok(cur, d);
	int m = 0;

	bool is_rule = false;
	list<string> t = letters;
	string t1;
	while(!t.empty()){
		t1 = t.front();
		if(islower(t1[0])){
			is_rule = true;
			break;
		}
		t.pop_front();
	}

	while(!letters.empty()){
		stringstream stream;
		stream << standarder;
		string tmp = stream.str();
		let = letters.front();
		const char *c = let.c_str();
		if(isupper(c[0]) && c[let.size()-1] == '('){
			for(int i = 0; i < let.size()-1; ++i){
				pred_name += c[i];
			}
			clus.name = pred_name;
		}
		else{
			for(int i = 0; i < let.size()-1; ++i){
				var += c[i];
			}
			if(islower(c[0])){
				var += tmp;
			}
			var_list.push_back(var);
			var = "";
		}
		if(c[let.size()-1] == ')'){		
			if(strToken != NULL){
				if(strToken[0] == '~'){
					clus.is_positive = 0;
				}
				else clus.is_positive = 1;
			}
			strToken = strtok(NULL,d);
			if(is_rule){
				while(!var_list.empty()){
					clus.args.push_back(var_list.front());
					var_list.pop_front();
				}
				kb_rule.insert(pair<string, Clause>(current, clus));
				++m;
			}
			else{
				while(!var_list.empty()){
					clus.args.push_back(var_list.front());
					var_list.pop_front();
				}
				kb_fact.insert(pair<string, Clause>(current, clus));
				++m;
			}
			pred_name = "";
			clus.name = "";
			clus.args.clear();
		}
		letters.pop_front();
	}
	if(is_rule){
		if(m > max_rule){
			max_rule = m;
		}
	}
	else{
		if(m > max_fact){
			max_fact = m;
		}
	}
	++standarder;
}
void store_sentence(string current, bool is_query){
	//const char *c = s.c_str();
	list<string> letters;
	for(int i = 0; i < current.size(); ){
		int beg = 0, end = 0;
		if(isalpha(current[i])){
			beg = i;
			for(int j = i;;++j){
				if(ispunct(current[j])){
					end = j;
					break;
				}
			}
			string let = "";
			for(int k = beg; k <= end; ++k){
				let += current[k];
			}
			letters.push_back(let);
			i = end + 1;
		}
		else ++i;
	}
	seperate_rule_and_fact(letters, current);
}

int find_split(string str, int start, int end){
	int tag = -1;
	int pair = 0, count = 0;
	int loc = -1;
	for(int i = start; i <= end; ++i){
		if(str[i] == '('){
			++pair;
			++count;
		}
		if(str[i] == ')'){
			--pair;
		}
		if(pair == 0 && count != 0){
			loc = i;
			break;
		}
	}
	if(loc == end && str[start] == '(' && str[end] == ')'){
		++start;
		--end;
	}

	int is_in_braket = 0;  
	int more_grade = 0;  
	for (int i = start; i <= end; i++)  
	{  
		if (str[i] == '(')  
			++is_in_braket;  
		else if (str[i] == ')')  
			--is_in_braket;  

		if ((str[i] == '|' || str[i] == '='|| str[i] =='&' )  && is_in_braket == 0)  
		{  
			more_grade = 1;  
			tag = i;
		}  
		else if ((str[i] == '~')  && more_grade == 0  && is_in_braket == 0)  
		{  
			tag = i; 
		}  
	}  
	return tag; 
}
Tree *create_tree(string str, int start, int end){
	Tree *pTree = NULL;
	bool flag = true;
	for(int i = start; i <= end; ++i){
		if(str[i] == '~' || str[i] == '&' || str[i] == '|' || str[i] == '='){
			flag = false;
			break;
		}
	}

	if(flag){
		pTree = new Tree;
		if(str[start] == '(' && str[end] == ')'){
			++start;
			--end;
		}
		for(int i = start; i <= end; ++i){
			pTree->value += str[i];
		}
		//cout << pTree->value << endl ;
		pTree->tLeft = NULL;
		pTree->tRight = NULL;
	}
	else{
		int tag = find_split(str, start, end);
		//cout << c[tag] << endl;
		pTree = new Tree;
		pTree->value = str[tag];
		int pair = 0, count = 0;
		int loc = -1;
		for(int i = start; i <= end; ++i){
			if(str[i] == '('){
				++pair;
				++count;
			}
			if(str[i] == ')'){
				--pair;
			}
			if(pair == 0 && count != 0){
				loc = i;
				break;
			}
		}
		if(loc == end && str[start] == '(' && str[end] == ')'){
			++start;
			--end;
		}
		//cout << start << " " << end << endl;
		if(str[tag] == '='){
			pTree->value += str[tag+1];
			pTree->tLeft = create_tree(str, start, tag-1);
			pTree->tRight = create_tree(str, tag+2, end);
		}
		else if(str[tag] == '~'){
			pTree->tLeft = NULL;
			pTree->tRight = create_tree(str, tag+1, end);
		}
		else{
			pTree->tLeft = create_tree(str, start, tag-1);
			pTree->tRight = create_tree(str, tag+1, end);
		}
	}
	return pTree;
}

int compare_prior(string op1, string op2){
	const char *temp = op2.c_str();
	if(isalpha(temp[0])){
		return 0;
	}
	else{
		if(op2 == "~"){
			return 0;
		}
		else if(op1 == "&" && op2 == "&"){
			return 0;
		}
		else if(op1 == "|" && op2 == "|"){
			return 0;
		}
		else 
			return 1;
	}
}
void print_tree(Tree *pTree){
	int flag_l = 0, flag_r = 0;  
	if (pTree != NULL){  
		int ret;  
		if (pTree->tLeft != NULL) {  
			ret = compare_prior(pTree->value, pTree->tLeft->value);  
			if (ret == 1){  
				//cout << '('; 
				global += "(";
				flag_l = 1;  
			}  
		}  
		print_tree(pTree->tLeft);  
		if (flag_l == 1){  
			//cout << ')';
			global += ")";
			flag_l = 0;  
		}  

		//cout << pTree->value; 
		global += pTree->value;

		if (pTree->tRight != NULL){  
			ret = compare_prior(pTree->value, pTree->tRight->value);  
			if (ret == 1){  
				//cout << '(';  
				global += "(";
				flag_r = 1;  
			}  
		}  
		print_tree(pTree->tRight);  
		if (flag_r == 1){  
			// cout <<  ')'; 
			global += ")";
			flag_r = 0;  
		}  
	}  
}
Tree *delete_imply(Tree *pTree){
	Tree *temp = NULL;
	if(pTree != NULL){
		if(pTree->value == "=>"){
			pTree->value = "|";
			temp = new Tree(pTree->tLeft);
			pTree->tLeft->value = "~";
			pTree->tLeft->tLeft = NULL;
			pTree->tLeft->tRight = temp;
		}
		delete_imply(pTree->tLeft);
		delete_imply(pTree->tRight);
	}
	return pTree;
}

Tree *move_not_inward(Tree *pTree){
	Tree *temp = NULL; 
	if(pTree != NULL){
		if(pTree->value == "~"){
			pTree->tLeft = new Tree;
			if(pTree->tRight->value == "&" || pTree->tRight->value == "|"){
				temp = new Tree(pTree->tRight);
				if(temp->value == "&"){
					pTree->value = "|";
				}
				else if (temp->value == "|"){
					pTree->value = "&";
				}
				pTree->tLeft->value = "~";
				pTree->tLeft->tLeft = NULL;
				pTree->tLeft->tRight = temp->tLeft;
				pTree->tRight->value = "~";
				pTree->tRight->tLeft = NULL;
				pTree->tRight->tRight = temp->tRight;
			}
			else if(pTree->tRight->value == "~"){
				temp = new Tree(pTree->tRight->tRight);  
				pTree->value = temp->value; // cannot pTree = temp, because the address of pTree will change, the ancestor of pTree don't know the address has been changed.
				pTree->tLeft = temp->tLeft;
				pTree->tRight = temp->tRight;
			}
			else{
				pTree->tLeft = NULL;
			}
		}
		move_not_inward(pTree->tLeft);
		move_not_inward(pTree->tRight);	
	}
	return pTree;
}

Tree *distribute_and_over_or(Tree *pTree){
	Tree *temp1 = NULL, *temp2 = NULL, *temp3 = NULL, *temp4 = NULL;
	if(pTree != NULL){
		if(pTree->value == "|"){
			if(pTree->tLeft->value == "&"){
				temp1 = new Tree(pTree->tRight);
				temp2 = new Tree(pTree->tLeft->tRight);
				temp3 = new Tree(temp1);
				temp4 = new Tree(pTree->tLeft->tLeft);

				pTree->value = "&";
				pTree->tLeft = new Tree;
				pTree->tLeft->value = "|";
				pTree->tLeft->tRight = temp1;
				pTree->tLeft->tLeft = temp4;
				pTree->tRight = new Tree;
				pTree->tRight->value = "|";
				pTree->tRight->tLeft = temp2;
				pTree->tRight->tRight = temp3;
				and_or = true;
			}
			else if (pTree->tRight->value == "&"){
				temp1 = new Tree(pTree->tLeft);
				temp2 = new Tree(pTree->tRight->tLeft);
				temp3 = new Tree(temp1);
				temp4 = new Tree(pTree->tRight->tRight);

				pTree->value = "&";
				pTree->tLeft = new Tree;
				pTree->tLeft->value = "|";
				pTree->tLeft->tLeft = temp3;
				pTree->tLeft->tRight = temp2 ;
				pTree->tRight = new Tree;
				pTree->tRight->value = "|";
				pTree->tRight->tLeft = temp1;
				pTree->tRight->tRight = temp4;
				and_or = true;
			}
		}
		distribute_and_over_or(pTree->tLeft);
		distribute_and_over_or(pTree->tRight);
	}
	return pTree;
}

string to_cnf(string s){
	string temp = "";
	for(int i = 0; i < s.size(); ++i){   // eliminate space, which may cause problems
		if(s[i] != ' '){
			temp += s[i];
		}
	}
	Tree *pTree = create_tree(temp, 0, temp.size()-1);
	pTree = delete_imply(pTree);
	pTree = move_not_inward(pTree);
	while(and_or){
		and_or = false;
		pTree = distribute_and_over_or(pTree);
	}
	and_or = true;
	print_tree(pTree);

	const char *t_cnf = global.c_str();
	char *cnf = new char[strlen(t_cnf)+1];
	strcpy(cnf, t_cnf);
	const char *d = "&";              
	char *strToken = strtok(cnf, d);
	while(strToken != NULL){
		string stc = "";
		if(strToken[0] == '('){
			for(int i = 1; i < strlen(strToken)-1; ++i){
				stc += strToken[i];
			}
		}
		else stc = strToken;
		kb_cnf.insert(pair<string, string>(s, stc));
		strToken = strtok(NULL,d);
	}
	global = "";
	return s;
}


string negate_query(string q){
	string new_q = "~(";
	for(int i = 0; i < q.size(); ++i){
		new_q += q[i];
	}
	new_q += ")";
	Tree *pTree = create_tree(new_q, 0, new_q.size()-1);
	pTree = move_not_inward(pTree);
	print_tree(pTree);
	new_q = global;
	//cout << new_q <<endl;
	global = "";
	return new_q;
}

multimap<string, Clause> analyze_query(string str){
	Clause clus;
	string t_str = "";
	int begin = -1, end = -1;
	bool is_name = false;
	multimap<string, Clause> curr_res;
	const char *temp = str.c_str();
	char *c_neg = new char[strlen(temp)+1];
	strcpy(c_neg, temp);
	const char *d = "|";              
	char *strToken = strtok(c_neg, d);

	while(strToken != NULL){
		int i = 0;
		if(strToken[0] == '~'){
			clus.is_positive = false;
			i = 1;
		}
		else {
			clus.is_positive = true;
			i = 0;
		}
		int size = strlen(strToken);
		while(i < strlen(strToken)){
			if(isalpha(strToken[i])){
				begin = i;
				for(int j = i+1; j < strlen(strToken); ++j){
					if(strToken[j] == '('){
						end = j;
						is_name = true;
						break;
					}
					else if(strToken[j] == ',' || strToken[j] == ')'){
						end = j;
						is_name = false;
						break;
					}
				}
				i = end + 1;
				t_str = "";
				for(int k = begin; k < end; ++k){
					t_str += strToken[k];
				}
				if(is_name){
					clus.name = t_str;
				}
				else clus.args.push_back(t_str);
			}
			else ++i;
		}
		history_res.insert(pair<string, Clause>(str, clus));
		curr_res.insert(pair<string, Clause>(str, clus));
		clus.args.clear();
		strToken = strtok(NULL,d);
	}
	return curr_res;
}
string unify_part(Clause res2, multimap<string, Clause> temp, multimap<string, Clause> curr_res){
	string final = "";
	multimap<string, Clause>::iterator i;
	bool flag = true;
	for(i = temp.begin(); i != temp.end(); ++i){
		if(i->second.name.compare(res2.name) == 0 && i->second.is_positive != res2.is_positive 
			&& i->second.args == res2.args){
				flag = false;
		}
		else flag = true;
		if(flag){
			if(i != temp.begin()){
				final += "|";
			}
			if(!i->second.is_positive){
				final += "~";
			}
			final += i->second.name + "(";
			for(int k = 0; k < i->second.args.size(); ++k){
				if(k > 0){
					final += ",";
				}
				final += i->second.args[k];
			}
			final += ")";
		}
	}
	for(i = curr_res.begin(); i != curr_res.end(); ++i){
		if(i->second.name.compare(res2.name) == 0 && i->second.is_positive == res2.is_positive
			&& i->second.args == res2.args){
				flag = false;
		}
		else flag = true;
		if(flag){
			if(final != ""){
				final += "|";
			}
			if(i->second.is_positive == 0){
				final += "~";
			}

			final += i->second.name + "(";
			for(int k = 0; k < i->second.args.size(); ++k){
				if(k > 0){
					final += ",";
				}
				final += i->second.args[k];
			}
			final += ")";
		}

	}

	return final;
}
string unify_rule(string res1, Clause res2, multimap<string, Clause> curr_res){
	multimap<string, Clause> temp;
	multimap<string, Clause>::iterator i, beg, end;
	beg = kb_rule.lower_bound(res1);
	end = kb_rule.upper_bound(res1);
	Unify u;
	vector<Unify> uni;
	bool flag = true;
	int count = 0;

	for(i = beg; i != end; ++i){
		temp.insert(pair<string, Clause>(i->first, i->second));
	}
	for(i = temp.begin(); i != temp.end(); ++i){
		if(i->second.name.compare(res2.name) == 0 && i->second.is_positive != res2.is_positive){
			for(int k = 0; k < res2.args.size(); ++k){
				if(islower(i->second.args[k][0])){
					u.var = i->second.args[k];
					u.value = res2.args[k];
					i->second.args[k] = res2.args[k];
					uni.push_back(u);
					++count;
				}
				else if(isupper(i->second.args[k][0])){
					if(isupper(res2.args[k][0]) && i->second.args[k].compare(res2.args[k]) == 0){
						++count;
					}
					else if(islower(res2.args[k][0])){
						u.var = res2.args[k];
						u.value = i->second.args[k];
						res2.args[k] = i->second.args[k];
						uni.push_back(u);
						++count;
					}
				}
			}
		}
	}
	if(count != res2.args.size()){
		return res2.name;
	}
	for(i = curr_res.begin(); i != curr_res.end(); ++i){
		for(int k = 0; k < uni.size(); ++k){
			for(int h = 0; h < i->second.args.size(); ++h){
				if(i->second.args[h].compare(uni[k].var) == 0){
					i->second.args[h] = uni[k].value;
				}
			}
		}
	}
	for(i = temp.begin(); i != temp.end(); ++i){
		for(int k = 0; k < uni.size(); ++k){
			for(int h = 0; h < i->second.args.size(); ++h){
				if(i->second.args[h].compare(uni[k].var) == 0){
					i->second.args[h] = uni[k].value;
				}
			}
		}
	}
	return unify_part(res2, temp, curr_res);
}
string unify_fact(string res1, Clause res2, multimap<string, Clause> curr_res){
	multimap<string, Clause> temp;
	multimap<string, Clause>::iterator i, beg, end;
	beg = kb_fact.lower_bound(res1);
	end = kb_fact.upper_bound(res1);
	int count = 0;
	Unify u;
	vector<Unify> uni;
	bool flag = true;

	for(i = beg; i != end; ++i){
		temp.insert(pair<string, Clause>(i->first, i->second));
	}
	for(i = temp.begin(); i != temp.end(); ++i){
		if(i->second.name.compare(res2.name) == 0 && i->second.is_positive != res2.is_positive){
			for(int k = 0; k < res2.args.size(); ++k){
				if(i->second.args[k].compare(res2.args[k]) == 0){
					++count;
				}
				else if(islower(res2.args[k][0])){
					u.var = res2.args[k];
					u.value = i->second.args[k];
					res2.args[k] = i->second.args[k];
					uni.push_back(u);
					++count;
				}
			}
		}
	}
	if(count != res2.args.size()){
		return res2.name;
	}
	for(i = curr_res.begin(); i != curr_res.end(); ++i){
		for(int k = 0; k < uni.size(); ++k){
			for(int h = 0; h < i->second.args.size(); ++h){
				if(i->second.args[h].compare(uni[k].var) == 0){
					i->second.args[h] = uni[k].value;
				}
			}
		}
	}
	return unify_part(res2, temp, curr_res);
}

bool resolution(string str){
	if(str == ""){
		return true;
	}
	multimap<string, Clause>::iterator i, j;
	for(i = kb_fact.begin(); i != kb_fact.end(); ++i){
		if(str.compare(i->first) == 0){
			return false;
		}
	}
	if(!history_res.empty()){  //if str appears again, it shows there is a loop, then return false;
		for(i = history_res.begin(); i != history_res.end(); ++i){
			if(str.compare(i->first) == 0){
				return false;
			}
		}
	}
	multimap<string, Clause> curr_res = analyze_query(str);
	int count = 0;
	for(i = curr_res.begin(); i != curr_res.end(); ++i){
		++count;
	}
	if(count > max_rule+max_fact-2){
		return false;
	}
	string final = "", res1 = "";
	Clause res2 ;
	bool flag = true;
	for(i = curr_res.begin(); i != curr_res.end(); ++i){
		for(j = kb_fact.begin(); j != kb_fact.end(); ++j){
			if(j->first.find(i->second.name) != string::npos){
				if(i->second.is_positive != j->second.is_positive && i->second.name.compare(j->second.name) == 0){
					res1 = j->first;
					res2 = i->second;
					final = unify_fact(res1, res2, curr_res);
				}
				if(final.compare(res2.name)){
					flag = resolution(final);
					if(flag){
						return true;
					}
				}
			}
		}
	}
	if(flag){
		for(i = curr_res.begin(); i != curr_res.end(); ++i){
			for(j = kb_rule.begin(); j != kb_rule.end(); ++j){
				if(j->first.find(i->second.name) != string::npos){
					if(i->second.is_positive != j->second.is_positive && i->second.name.compare(j->second.name) == 0){
						res1 = j->first;
						res2 = i->second;
						final = unify_rule(res1, res2, curr_res);
					}
					if(final.compare(res2.name)){
						flag = resolution(final);
						if(flag){
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
bool ask(string q){
	string neg = negate_query(q);
	bool flag = resolution(neg);
	multimap<string, Clause>::iterator i;
	for(i = kb_fact.begin(); i != kb_fact.end(); ++i){
		if(q.compare(i->first) == 0){
			return flag;
		}
	}
	if(flag){
		multimap<string, Clause> temp = analyze_query(q);
		for(i = temp.begin(); i != temp.end(); ++i){
			kb_fact.insert(pair<string, Clause>(i->first, i->second));
		}
	}
	return flag;
}

int main(){
	ifstream input("input.txt");
	ofstream output("output.txt");

	string str;
	getline(input, str);
	int nq = atoi(str.c_str());
	string *query = new string[nq];
	string s;
	for(int i = 0; i < nq; ++i){
		getline(input, query[i]);
	}
	getline(input, str);
	int ns = atoi(str.c_str());
	string *sentence = new string[ns];
	for(int i = 0; i < ns; ++i){
		getline(input, sentence[i]);
		sentence[i] = to_cnf(sentence[i]);
	}

	multimap<string, string>::const_iterator it;
	for(it = kb_cnf.begin(); it != kb_cnf.end(); ++it){
		store_sentence(it->second, false);
	}
	bool result = true;
	for (int i = 0; i < nq; ++i){
		history_res.clear();
		result = ask(query[i]);
		if(result){
			output << "TRUE" << endl;
		}
		else{
			output << "FALSE" << endl;
		}
	}
	return 0;
}