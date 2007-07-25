#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef void (*funcptr)(char **args);

struct node
{
	node *rchild;
	node *lchild;
	node *parent;
	string *val;
	funcptr fxn;
	
	node(char *value)
	{
		val = new string(value);
	}
	node(char *value, funcptr funct)
	{
		val = new string(value);
		fxn = funct;
	}
	~node()
	{
		delete val;
		if(rchild){delete rchild;}
		if(lchild){delete lchild;}
	}
};

int insertnode(node *head, node *n)
{
	if( !(n->val->compare(*(head->val))) )
	{
		if( !(head->lchild) )
		{
			head->lchild = n;
			n->lchild = NULL;
			n->rchild = NULL;
		} else {
			insertnode(head->lchild, n);
		}
	} else {
		if (head->rchild == NULL)
		{
			head->rchild = n;
			n->lchild = NULL;
			n->rchild = NULL;
		} else {
			insertnode(head->rchild, n);
		}
	}
}

node *getNodeByKey(string *val, node *head)
{
	if( head->val->compare(*val) == 0 )
	{
		cout << "Found key " << *(val) << endl;
		return head;
	}
	
	if( head->lchild != NULL && !(val->compare(*(head->val))) )
	{
		return getNodeByKey(val, head->lchild);
		
	} else if (head->rchild != NULL ) {
		return getNodeByKey(val, head->rchild);
		
	} else {
		return NULL;
	}
	return NULL;
}

funcptr getFxnPointer(string *val, void *ptr, node *head)
{
	node *result = getNodeByKey(val, head);
	if (!result) return NULL;
	return result->fxn;
}

void printTree(node *head)
{
	cout << *(head->val) << endl;
	if(head->lchild){printTree(head->lchild);}
	if(head->rchild){printTree(head->rchild);}
}

void printFive(char **args)
{
	cout << "Five!" << endl;
}

void printFour(char **args)
{
	cout << "Four!" << endl;
}

void printThree(char **args)
{
	cout << "Three!" << endl;
}

void printTwo(char **args)
{
	cout << "Two!" << endl;
}

void printOne(char **args)
{
	cout << "One!" << endl;
}

void searchIncompleteString(string *searchString, vector<node *> *results, node *head)
{
	
	if( 	searchString->length() < head->val->length() || 
		searchString->compare( *(head->val) ) == 0	)
	{
		if( searchString->compare(head->val->substr(0,searchString->length())) == 0 )
		{
			results->push_back(head);
		}
	}
	
	if(head->lchild){searchIncompleteString(searchString, results, head->lchild);}
	if(head->rchild){searchIncompleteString(searchString, results, head->rchild);}
}

int main( int argv, char **argc)
{	
	
	/* Create the root node */
	node *treetop = new node(argc[0]);

	// Make a node and insert it
	insertnode(treetop, new node("five", &printFive) );
	insertnode(treetop, new node("four", &printFour) );
	insertnode(treetop, new node("three", &printThree) );
	insertnode(treetop, new node("two", &printTwo) );
	insertnode(treetop, new node("one", &printOne) );
	
	string *search = new string("f");
	vector<node *> *rs = new vector<node *>();
	searchIncompleteString(search, rs, treetop);
	
	cout << rs->size() << endl;
	
	for(int i = 0; i < rs->size(); i++)
	{
		cout << *((*rs)[i]->val) << endl;
	}
	
	
	/* Deletion of the root node cascades, removing the tree */
	delete search;
	delete rs;
	delete treetop;
	return 0;
}