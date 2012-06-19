#ifndef SIMPLEDOCUMENTLIST_H
#define SIMPLEDOCUMENTLIST_H

#include <set>
#include <iostream>
#include "DocumentList.hpp"

class SimpleDocumentList : public DocumentList<int>
{
  public:
  // Add document id and term frequency
  void add(int id);
  // Remove document id
    void remove(int id);
  // Print list of document ids in the list
    void print();
    void to_stream(ostream *out);
  // All the neccessary set operations
    SimpleDocumentList *intersect(SimpleDocumentList dl);  
    SimpleDocumentList *unify(SimpleDocumentList dl); 
    SimpleDocumentList *complement(SimpleDocumentList dl); 
  private:
  // The set of doc ids
  unordered_map<int,int> doclist;
};


void SimpleDocumentList::add(int id)
{
  doclist[id]++;
}

void SimpleDocumentList::remove(int id)
{
  doclist.erase(id);
}

SimpleDocumentList *SimpleDocumentList::intersect(SimpleDocumentList dl)
{
  /*  SimpleDocumentList *result=new SimpleDocumentList();
  // For all doc ids in dl
  for (auto it=dl.doclist.begin();it!=dl.doclist.end();it++)
  {
    // If doc id in dl and in here -> add it to result
    if (this->doclist.count(*it)>0)
      result->add(*it);
  }
  return result;*/
}


SimpleDocumentList *SimpleDocumentList::unify(SimpleDocumentList dl)
{
  /*  SimpleDocumentList *result=new SimpleDocumentList();
  // Add all doc ids in dl to result
  for (auto it=dl.doclist.begin();it!=dl.doclist.end();it++)
  {
    result->add(*it);
  }
  // Add all doc ids in here to results -> the set avoids duplicates
  for (it=this->doclist.begin();it!=this->doclist.end();it++)
  {
    result->add(*it);
  }
  return result; */
}

SimpleDocumentList *SimpleDocumentList::complement(SimpleDocumentList dl)
{
  /*  SimpleDocumentList *result=new SimpleDocumentList();
  // For all elements in the universe dl
  for (auto it=dl.doclist.begin();it!=dl.doclist.end();it++)
  {
    // If element not in here -> add to result
    if (this->doclist.count(*it)==0)
      result->add(*it);
  }
  return result; */
}


void SimpleDocumentList::to_stream(ostream *out)
{
  /*  for (auto it=this->doclist.begin() ; it != this->doclist.end(); it++ )
    *out << "|" << *it;
    *out << "|";*/
}

void SimpleDocumentList::print()
{
  to_stream(&cout);
  cout << endl;
}
#endif
