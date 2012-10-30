/**
 * @file BooleanQuery.hpp
 * @author  Herbert Lange <herbert.lange@campus.lmu.de>
 * @version 1.0
 */

#ifndef BOOLEANQUERY_H
#define BOOLEANQUERY_H

#include <vector>
#include <string>

#ifdef DEBUG
#include <iostream>
#endif

#include "Query.hpp"
#include "Util.hpp"

using namespace std;

/**
 * The BooleanQuery class is the Implementation of a basic Query<int> Parser. A BooleanQuery String
 * consists of Document Terms connected by the boolean Operators AND, OR and NOT. The Query String
 * is evaluated from left to right.
 */
class BooleanQuery : public Query<int>
{
  public: 
/**
 * Constructor to incorporate an SimpleIndex for the Evaluation and to set the result to NULL
 *
 * @param *i Pointer to the SimpleIndex to be used for Evaluation of the BooleanQuery
 */
    BooleanQuery(SimpleIndex *i);
/**
 * Function to parse a Query and store the resulting SimpleDocumentList internally. The Query is evaluated from left to right.
 *
 * @param query The Query string to be evaluated. Consisting of search terms and the boolean Operators AND, OR and NOT
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
  // List of terms in query
    vector<string> term_list;
  // List of operators in query
    vector<string> op_list;
  // The resulting DocumentList
    SimpleDocumentList *result;
    SimpleIndex *inverted_index;
};

BooleanQuery::BooleanQuery(SimpleIndex *i)
{
  result=NULL;
  inverted_index=i;
}

void BooleanQuery::parse(string query)
{
  vector<string> *v=tokenize(query);
  for (int pos=0; pos<(int)v->size();pos++)
    {
      string nword=u.normalize(v->at(pos));
#ifdef DEBUG
      cout << nword << endl;
#endif
      if ((nword=="and")||(nword=="or")||(nword=="not"))
	{
	  op_list.push_back(nword);
	}
      else
	{
	  term_list.push_back(nword);
	}
    }
  // Clean-up temporary vector
  delete v;

  // Position in term list
  int termpos=0;
  // Resulting document list after one operation
  SimpleDocumentList *resdl=NULL;
  // Pointer to old document list (for clean-up)
  SimpleDocumentList *olddl;
  for(int pos=0; pos<(int)op_list.size();pos++)
    {
      // Get first operand for the operations (either from resdl or from retrieving the first term)
      SimpleDocumentList *dl1;
      if (resdl==NULL)
	{
	  dl1=(SimpleDocumentList *) inverted_index->retrieve(term_list[termpos]);
	  termpos++;
	}
      else
	{
	  dl1=resdl;
	}
      // And-Operator
      if (op_list[pos]=="and")
	{
	  // Get second operand
	  SimpleDocumentList *dl2=(SimpleDocumentList *) inverted_index->retrieve(term_list[termpos]);
	  // Save pointer for clean-up
	  olddl=resdl;

	  resdl=dl1->intersect(*dl2);
	  // Clean-up
	  if (olddl!=NULL)
	    delete olddl;
	  // Next term
	  termpos++;
	}
      // Or-Operator (quite like above)
      else if(op_list[pos]=="or")
	{
	  SimpleDocumentList *dl2=(SimpleDocumentList *) inverted_index->retrieve(term_list[termpos]);
	  olddl=resdl;
	  // Unify the Document Lists
	  resdl=dl1->unify(*dl2);
	  if (olddl!=NULL)
	    delete olddl;
	  termpos++;
	    }
      // Not-Operator
      else 
	{
	  olddl=resdl;
	  // Get list of all documents in index to compute complement
	  SimpleDocumentList *dl2=(SimpleDocumentList *) inverted_index->get_doc_ids();
	  // Complement the Document Lists
	  resdl=dl1->complement(*dl2);
	  if (olddl!=NULL)
	    delete olddl;
	}
#ifdef DEBUG
      cerr << "Resdl size " << resdl->doclist.size() << endl;
#endif
    }
  // Only a single term -> retrieve document list from index
  if (op_list.size()==0)
    {
      resdl=(SimpleDocumentList *) inverted_index->retrieve(term_list[termpos]);
      // Workaround to get the results: A \cup A=A
      resdl=resdl->unify(*resdl);
#ifdef DEBUG
      cerr << "Resdl size " << resdl->doclist.size() << endl;
      cerr << "Resdl " << resdl << endl; 
#endif
    }
#ifdef DEBUG
    cerr << resdl << endl;
#endif
  // Save result in object
  result=resdl;
#ifdef DEBUG
  cerr << "Result size " << result->doclist.size() << endl;
  cerr << "Result " << result << endl; 
#endif
}

SimpleDocumentList *BooleanQuery::get_result()
{
  // Do some ranking stuff: language, term count etc.
  return result;
}

#endif
