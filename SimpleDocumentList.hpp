#ifndef SIMPLEDOCUMENTLIST_H
#define SIMPLEDOCUMENTLIST_H

#include <set>
#include <iostream>
#include "DocumentList.hpp"

class SimpleDocumentList : public DocumentList<int>
{
  public:
    ~SimpleDocumentList() {};
  // Add document id and term frequency
    void add(int id);
    void add(int id, int count);
  // Remove document id
    void remove(int id);
  // Print list of document ids in the list
    void print();
    void to_stream(ostream *out);
  // All the neccessary set operations
    SimpleDocumentList *intersect(SimpleDocumentList dl);  
    SimpleDocumentList *unify(SimpleDocumentList dl); 
    SimpleDocumentList *complement(SimpleDocumentList dl); 
  //#ifndef DEBUG
  //  private:
  //#endif
  // The set of doc ids
    unordered_map<int,int> doclist;
};


void SimpleDocumentList::add(int id)
{
  doclist[id]++;
}

void SimpleDocumentList::add(int id, int count)
{
  doclist[id]+=count;
}

void SimpleDocumentList::remove(int id)
{
  doclist.erase(id);
}

SimpleDocumentList *SimpleDocumentList::intersect(SimpleDocumentList dl)
{
  SimpleDocumentList *result=new SimpleDocumentList();
  // For all doc ids in dl
  for (auto it=dl.doclist.begin();it!=dl.doclist.end();it++)
  {
    // If doc id in dl and in here -> add it to result
    if (this->doclist.count(it->first)>0)
      result->doclist[it->first]+=it->second; 
  }
  return result;
}


SimpleDocumentList *SimpleDocumentList::unify(SimpleDocumentList dl)
{
  SimpleDocumentList *result=new SimpleDocumentList();
  // Add all doc ids in dl to result
  for (auto it=dl.doclist.begin();it!=dl.doclist.end();it++)
  {
    result->doclist[it->first]+=it->second;
  }
  // Add all doc ids in here to results -> the set avoids duplicates
  for (auto it=this->doclist.begin();it!=this->doclist.end();it++)
  {
    result->doclist[it->first]+=it->second;
  }
  return result; 
}

SimpleDocumentList *SimpleDocumentList::complement(SimpleDocumentList dl)
{
  SimpleDocumentList *result=new SimpleDocumentList();
  // For all elements in the universe dl
  for (auto it=dl.doclist.begin();it!=dl.doclist.end();it++)
  {
    // If element not in here -> add to result
    if (this->doclist.count(it->first)==0)
      result->doclist[it->first]+=it->second;
  }
  return result; 
}


void SimpleDocumentList::to_stream(ostream *out)
{
#ifdef DEBUG
  cerr << doclist.size() << " Elements" << endl;
#endif
    for (auto it=this->doclist.begin() ; it != this->doclist.end(); it++ )
      *out << "|" << it-> first << ":" << it-> second;
    *out << "|"; 
}

void SimpleDocumentList::print()
{
  to_stream(&cout);
  cout << endl;
}
#endif
