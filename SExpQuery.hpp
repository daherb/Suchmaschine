/**
 * @file SExpQuery.hpp
 * @author  Herbert Lange <herbert.lange@campus.lmu.de>
 * @version 1.0
 */

#ifndef SEXPQUERY_H
#define SEXPQUERY_H

#include <vector>
#include <string>

#ifdef DEBUG
#include <iostream>
#endif

#include "SimpleIndex.hpp"
#include "Query.hpp"
#include "Util.hpp"

using namespace std;

enum states { PreNode, ParseOperator, PostOperator, ParseOperand };

/**
 * The Node class is the foundation of a tree
 */
class Node
{
  public:
/**
 * Pointer to the Parent Node
 */
    Node *parent;
/**
 * List of Pointers to Child Nodes
 */
    vector<Node *> children;
/**
 * A Value stored in the Node
 */
    string value;
/**
 * Constructor only setting the Parent Node
 * @param *p Pointer to the Parent Node
 */
    Node(Node *p) { parent=p; }
/**
 * Constructor setting the Parent Node and Node Value
 * @param *p Pointer to the Parent Node
 * @param v Value to be stored in Node
 */
    Node(Node *p, string v) { parent=p; value=v; }
/**
 * Function to print the whole Subtree of the Node
 * @param in String used for Indentation
 */
    void printTree(string in);
};

void Node::printTree(string in)
{
  cout << in << value << '\n';
  for (auto it=children.begin(); it!=children.end(); ++it)
    {
      (*it)->printTree(in+"  ");
    }
}

/**
 * The SExpQuery class is the Implementation of a basic Query<int> Parser. A SExpQuery String
 * is quite similar to Lisp-like expressions. The form of a Query Expression is (o e1 e2) with
 * o one of AND, NOT, OR and e1,e2 either Document Terms or Query Expressions.
 */
class SExpQuery : public Query<int>
{
  public: 
/**
 * Constructor to incorporate an SimpleIndex for the Evaluation and to set the result to NULL
 *
 * @param *i Pointer to the SimpleIndex to be used for Evaluation of the SExpQuery
 */
    SExpQuery(SimpleIndex *i);
/**
 * Function to parse a Query and store the resulting SimpleDocumentList internally. 
 *
 * @param query The Query string to be evaluated
 */
    void parse(string query);
/**
 * Funtion to return the resulting SimpleDocumentList
 *
 * @return SimpleDocumentList containing the documents selected by a previous query
 */
    SimpleDocumentList *get_result();
  private:
    Util u;
    SimpleDocumentList *interpret(Node *tree);
    Node *stree;
  // The resulting DocumentList
    SimpleDocumentList *result;
    SimpleIndex *inverted_index;
};

SExpQuery::SExpQuery(SimpleIndex *i)
{
  stree=NULL;
  result=NULL;
  inverted_index=i;
}
// (query (and (or winter sea) (meta language deutsch)))
void SExpQuery::parse(string query)
{
  unsigned int pos=0;
  Node *curnode=NULL;
  int curstate;
  string op="";
  string operand="";
  for ( pos = 0; pos < query.length(); pos++)
    {
      if (query[pos]=='(')
	{
	  curstate=ParseOperator;
	  if (curnode==NULL)
	    curnode=new Node(curnode);
	  else
	    {
	      Node *oldnode=curnode;
	      curnode=new Node(oldnode);
	      oldnode->children.push_back(curnode);
	    }
	}
      else if (curstate==PreNode && (query[pos]==' ' || query[pos]==')'))
	{
	}
      else if (curstate==ParseOperator)
	{
	  if (query[pos]!=' ' && query[pos]!=')')
	    {
	      op+=query[pos];
	    }
	  else
	    {
	      curstate=PostOperator;
	      curnode->value=u.normalize(op);
	      op="";
	    }
	}
      else if (curstate==PostOperator)
	{
	  if (query[pos]!=' ')
	    {
	      curstate=ParseOperand;
	      operand=query[pos];
	    }
	}
      else if (curstate==ParseOperand)
	{
	  if (query[pos]!=' ' && query[pos]!=')')
	    {
	      operand+=query[pos];
	    }
	  else
	    {
	      curstate=PostOperator;
	      curnode->children.push_back(new Node(curnode,u.normalize(operand)));
	      operand="";
	    }
	}
      else
	{
	  // Should never be the case
	  cerr << "Impossible case " << curstate << " while parsing query" << endl;
	}
      if (query[pos]==')')
	{
	  if (curnode->parent!=NULL)
	    {
	      curnode=curnode->parent;
	    }
	 
	  curstate=PostOperator;
	}
    }
  stree=curnode;
  while (stree->parent!=NULL)
    stree=stree->parent;
#ifdef DEBUG
  stree->printTree("");
#endif
  result=interpret(stree);
}

SimpleDocumentList *SExpQuery::interpret(Node *tree)
{
  if (tree->children.size()>0)
    {
      if (tree->value=="and")
	{
	  return interpret(tree->children[0])->intersect((*interpret(tree->children[1])));
	}
      else if (tree->value == "or")
	{
	  return interpret(tree->children[0])->unify((*interpret(tree->children[1])));
	}
      else if (tree->value == "not")
	{
	  cout << (*(SimpleDocumentList *)inverted_index->get_doc_ids()).doclist.size() << endl;
	  return interpret(tree->children[0])->complement((*(SimpleDocumentList *)inverted_index->get_doc_ids()));
	}
      else if (tree->value == "meta")
	{
	  return (SimpleDocumentList *) inverted_index->doc_info.get_all(tree->children[0]->value,tree->children[1]->value);
	}
      else
	{
	  cerr << "Impossible case while interpreting tree" << endl;
	}
    }
  else if (tree->children.size()==0)
    { 
      return (SimpleDocumentList *) inverted_index->retrieve(tree->value);;
    }
  return NULL;
}
SimpleDocumentList *SExpQuery::get_result()
{
  // Do some ranking stuff: language, term count etc.
  return result;
}

#endif
