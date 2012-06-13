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

// Class to parse a query and store the resulting document list
class BooleanQuery : public Query<int>
{
  public: 
  // Parse a boolean query
    void parse(string query, SimpleIndex i);
  // Return the resulting document list
    SimpleDocumentList *get_result();
  private:
  // List of terms in query
    vector<string> term_list;
  // List of operators in query
    vector<string> op_list;
  // The resulting DocumentList
    SimpleDocumentList *result=NULL;
};

void BooleanQuery::parse(string query,SimpleIndex i)
{
  vector<string> *v=tokenize(query);
  for (int pos=0; pos<(int)v->size();pos++)
    {
      string nword=normalize(v->at(pos));
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
	  dl1=(SimpleDocumentList *) i.retrieve(term_list[termpos]);
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
	  SimpleDocumentList *dl2=(SimpleDocumentList *) i.retrieve(term_list[termpos]);
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
	  SimpleDocumentList *dl2=(SimpleDocumentList *) i.retrieve(term_list[termpos]);
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
	  SimpleDocumentList *dl2=(SimpleDocumentList *) i.get_doc_ids();
	  // Complement the Document Lists
	  resdl=dl1->complement(*dl2);
	  if (olddl!=NULL)
	    delete olddl;
	}
    }
  // Only a single term -> retrieve document list from index
  if (op_list.size()==0)
    {
      resdl=(SimpleDocumentList *) i.retrieve(term_list[termpos]);
    }
#ifdef DEBUG
  printf("%p\n",resdl);
#endif
  // Save result in object
  result=resdl;
}

SimpleDocumentList *BooleanQuery::get_result()
{
  // Do some ranking stuff: language, term count etc.
  return result;
}

#endif
