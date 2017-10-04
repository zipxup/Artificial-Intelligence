// hw3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<fstream>
#include<string>
#include<map>
#include<list>
#include<vector>
#include<iostream>
using namespace std;

const string NOT = "~";
const string OR = "|";
const string AND = "&";
const string IMPLY = "=>";
const string Left = "(";
const string Right = ")";

struct Clause{
	string name;
	vector<string> args;
};

class Tree
{
public:
	string value;
	Tree *tLeft;
	Tree *tRight;
	Tree(Tree* t): value(t->value), tLeft(t->tLeft), tRight(t->tRight){ }
	Tree(): value(""), tLeft(NULL), tRight(NULL) { }
};
Tree *p = NULL;
multimap<int, Clause> kb_rule;
multimap<int, Clause> kb_fact;
//map<int, string> kb;
//multimap<int, Clause> kb_supply;
//map<string, list<string> >rule_predicate;
//multimap<string, list<string> >fact_predicate;

void distinguish_letters(list<string> letters, int ord){
	string let;
	list<string> var_list;
	list<string> const_list;
	string pred_name = "";
	string var = "";
	string constant = "";
	Clause clus;

	while(!letters.empty()){
		let = letters.front();
		const char *c = let.c_str();
		if(isupper(c[0]) && c[let.size()-1] == '('){
			for(int i = 0; i < let.size()-1; ++i){
				pred_name += c[i];
			}
			clus.name = pred_name;
		}
		if(islower(c[0])){
			for(int i = 0; i < let.size()-1; ++i){
				var += c[i];
			}
			var_list.push_back(var);
			var = "";
		}
		if(isupper(c[0]) && c[let.size()-1] != '('){
			for(int i = 0; i < let.size()-1; ++i){
				constant += c[i];
			}
			const_list.push_back(constant);
			constant = "";
		}
		if(c[let.size()-1] == ')'){
			if(var_list.empty() && isupper(c[0])){
				while(!const_list.empty()){
					clus.args.push_back(const_list.front());
					const_list.pop_front();
				}
				kb_fact.insert(pair<int, Clause>(ord, clus));
			}
			else{
				while(!var_list.empty()){
					clus.args.push_back(var_list.front());
					var_list.pop_front();
				}
				kb_rule.insert(pair<int, Clause>(ord, clus));
			}
			pred_name = "";
			clus.name = "";
			clus.args.clear();
		}
		letters.pop_front();
	}
}
void find_letters(string s, int ord){
	const char *c = s.c_str();
	list<string> letters;
	for(int i = 0; i < s.size(); ){
		int beg = 0, end = 0;
		if(isalpha(c[i])){
			beg = i;
			for(int j = i;;++j){
				if(ispunct(c[j])){
					end = j;
					break;
				}
			}
			string let = "";
			for(int k = beg; k <= end; ++k){
				let += c[k];
			}
			letters.push_back(let);
			i = end + 1;
		}
		else ++i;
	}
	distinguish_letters(letters, ord);
}

string fol_to_pl(string s){
	const char *c = s.c_str();
	string new_s = "";
	for(int i = 0; i < s.size();){
		if(c[i] == '(' && islower(c[i+1])){
			int j;
			for(j = i; j < s.size(); ++j){
				if(c[j] == ')')
					break;
			}
			i = j +1;
		}
		else{
			new_s += c[i];
			++i;
		}
	}
	//cout << new_s << endl;
	return new_s;
}


int find_split(const char *c, int start, int end){
	int tag = -1;
	int pair = 0, count = 0;
	int loc = -1;
	for(int i = start; i <= end; ++i){
		if(c[i] == '('){
			++pair;
			++count;
		}
		if(c[i] == ')'){
			--pair;
		}
		if(pair == 0 && count != 0){
			loc = i;
			break;
		}
	}
	if(loc == end && c[start] == '(' && c[end] == ')'){
		++start;
		--end;
	}

	int is_in_braket = 0;  
	int more_grade = 0;  
	for (int i = start; i <= end; i++)  
	{  
		if (c[i] == '(')  
			++is_in_braket;  
		else if (c[i] == ')')  
			--is_in_braket;  

		if ((c[i] == '|' || c[i] == '='|| c[i] =='&' )  && is_in_braket == 0)  
		{  
			more_grade = 1;  
			tag = i;
		}  
		else if ((c[i] == '~')  && more_grade == 0  && is_in_braket == 0)  
		{  
			tag = i; 
		}  
	}  
	return tag; 
}
Tree *create_tree(const char *c, int start, int end){
	Tree *pTree = NULL;
	bool flag = true;
	for(int i = start; i <= end; ++i){
		if(c[i] == '~' || c[i] == '&' || c[i] == '|' || c[i] == '='){
			flag = false;
			break;
		}
	}

	if(flag){
		pTree = new Tree;
		for(int i = start; i <= end; ++i){
			pTree->value += c[i];
		}
		//cout << pTree->value << endl ;
		pTree->tLeft = NULL;
		pTree->tRight = NULL;
	}
	else{
		int tag = find_split(c, start, end);
		//cout << c[tag] << endl;
		pTree = new Tree;
		pTree->value = c[tag];
		int pair = 0, count = 0;
		int loc = -1;
		for(int i = start; i <= end; ++i){
			if(c[i] == '('){
				++pair;
				++count;
			}
			if(c[i] == ')'){
				--pair;
			}
			if(pair == 0 && count != 0){
				loc = i;
				break;
			}
		}
		if(loc == end && c[start] == '(' && c[end] == ')'){
			++start;
			--end;
		}
		//cout << start << " " << end << endl;
		if(c[tag] == '='){
			pTree->value += c[tag+1];
			pTree->tLeft = create_tree(c, start, tag-1);
			pTree->tRight = create_tree(c, tag+2, end);
		}
		else if(c[tag] == '~'){
			pTree->tLeft = NULL;
			pTree->tRight = create_tree(c, tag+1, end);
		}
		else{
			pTree->tLeft = create_tree(c, start, tag-1);
			pTree->tRight = create_tree(c, tag+1, end);
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
    if (pTree != NULL)  
    {  
        int ret;  
        if (pTree->tLeft != NULL) //非叶子节点  
        {  
            ret = compare_prior(pTree->value, pTree->tLeft->value);  
            if (ret == 1) //当前节点的优先级大于左子树, 左子树需要加括号, 防止误解  
            {  
                cout << '(';  
                flag_l = 1;  
            }  
        }  
        print_tree(pTree->tLeft);  
        if (flag_l == 1)  
        {  
            cout << ')';  
            flag_l = 0;  
        }  
  
        cout << pTree->value;  
  
        if (pTree->tRight != NULL) //非叶子结点  
        {  
            ret = compare_prior(pTree->value, pTree->tRight->value);  
            if (ret == 1)//当前节点的优先级大于右子树, 又子树需要加括号, 防误解  
            {  
				cout << '(';  
                flag_r = 1;  
            }  
        }  
        print_tree(pTree->tRight);  
        if (flag_r == 1)  
        {  
            cout <<  ')';  
            flag_r = 0;  
        }  
    }  
}
Tree *delete_imply(Tree *pTree){
	Tree *temp = NULL;
	if(pTree != NULL){
		if(pTree->value == "=>"){
			pTree->value = "|";
			temp = new Tree;
			//temp = pTree->tLeft;
			//cout << temp->value<< endl;
			temp->value = pTree->tLeft->value;
			temp->tLeft = pTree->tLeft->tLeft;
			temp->tRight = pTree->tLeft->tRight;
			pTree->tLeft->value = "~";
			pTree->tLeft->tLeft = NULL;
			pTree->tLeft->tRight = temp;
			//cout << pTree->tLeft->tRight->value << temp->value;
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
				temp = new Tree;
				temp->value = pTree->tRight->value;
				temp->tLeft = pTree->tRight->tLeft;
				temp->tRight = pTree->tRight->tRight;
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
				temp = new Tree;  
				temp = pTree->tRight->tRight;
				pTree->value = temp->value; // cannot pTree = temp, because the address of pTree will change, the ancestor of pTree don't know the address has been changed.
				pTree->tLeft = temp->tLeft;
				pTree->tRight = temp->tRight;
			}
			else 
				pTree->tLeft = NULL;
		}
		move_not_inward(pTree->tLeft);
		move_not_inward(pTree->tRight);	
	}
	return pTree;
}

Tree *distribute_and_over_or(Tree *pTree){
	Tree *temp1 = NULL, *temp2 = NULL, *temp3 = NULL;
	if(pTree != NULL){
		if(pTree->value == "|"){
			if(pTree->tLeft->value == "&"){
				temp1 = new Tree(pTree->tRight);
				temp2 = new Tree(pTree->tLeft->tRight);
				temp3 = new Tree(temp1);

				pTree->value = "&";
				pTree->tLeft->value = "|";
				pTree->tLeft->tRight = temp1;
				pTree->tRight->value = "|";
				pTree->tRight->tLeft = temp2;
				pTree->tRight->tRight = temp3;
			}
			else if (pTree->tRight->value == "&"){
				temp1 = new Tree(pTree->tLeft);
				temp2 = new Tree(pTree->tRight->tLeft);
				temp3 = new Tree(temp1);

				pTree->value = "&";
				pTree->tLeft->value = "|";
				pTree->tLeft->tLeft = temp1;
				pTree->tLeft->tRight = temp2 ;
				pTree->tRight->value = "|";
				pTree->tRight->tLeft = temp3;
			}
		}
		//Tree *l_temp = pTree->tLeft;
		//Tree *r_temp = pTree->tRight;
		distribute_and_over_or(pTree->tLeft);
		distribute_and_over_or(pTree->tRight);
	}
	return pTree;
}

string to_cnf(string s){
	//s = fol_to_pl(s);
	//cout << s << endl;
	string temp = "";
	const char* c = s.c_str();
	for(int i = 0; i < s.size(); ++i){   // eliminate space, which may cause problems
		if(c[i] != ' '){
			temp += c[i];
			//cout << temp << endl;
		}
	}
	//cout << temp << temp.size()<<  endl;
	c = temp.c_str();
	Tree *pTree = create_tree(c, 0, temp.size()-1);
	//cout << pTree->value;
	//s = delete_imply(pTree);
	pTree = delete_imply(pTree);
	//cout << endl;
	//print_tree(pTree);
	//cout << s << endl;
	//delete_imply(pTree);
	//cout << endl;
	pTree = move_not_inward(pTree);
	//print_tree(pTree);
	//cout << endl;
	p = pTree;
	pTree = distribute_and_over_or(pTree);
	print_tree(pTree);
	cout << endl;
	return s;
}


bool ask(string q){
	bool flag = true;
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
		find_letters(sentence[i], i);
	}
	multimap<int, Clause>::const_iterator it;
	int previous = -1, current;
	for(it = kb_rule.begin(); it != kb_rule.end(); ++it){
		current = it->first;
		if(previous != current){
			//cout << sentence[current] <<endl;
			sentence[current] = to_cnf(sentence[current]);
		}
		previous = current;
	}
	/*multimap<int, Clause>::const_iterator it1; 
	for (it1 = kb_rule.begin(); it1 != kb_rule.end(); ++it1){
	cout << it1 ->first << " " << it1->second.name;
	for(int i = 0; i < it1->second.args.size(); ++i){
	cout << it1->second.args[i];
	}
	cout << endl;
	}
	cout << endl;*/

	bool result;
	for(int i = 0; i < nq; ++i){
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

