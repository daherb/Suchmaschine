/**
 * @file SimpleDocumentList.hpp
 * @author  Herbert Lange <herbert.lange@campus.lmu.de>
 * @version 1.0
 */

#ifndef SIMPLEDOCUMENTLIST_H
#define SIMPLEDOCUMENTLIST_H

#include <string>
#include <set>
#include <iostream>
#include <unordered_map>

#include "DocumentList.hpp"

using namespace std;

class DocumentInfo;

/**
 * The SimpleDocumentList class is the Implementation of a DocumentList<int>, i.e. a DocumentList
 * where Documents are represented as Integers
 */
class SimpleDocumentList : public DocumentList<int>
{
  public:
  /**
   * Function to add a Document to the SimpleDocumentList with count 1
   *
   * @param id Document ID of the Document to add
   */
    void add(int id);
  /**
   * Function to add a Document to the SimpleDocumentList with a specific count
   *
   * @param id Document ID of the Document to add
   * @param count Document Count
   */
    void add(int id, int count);
  /**
   * Function to remove a Document from the SimpleDocumentList
   *
   * @param id Document ID of the Document to add
   */
    void remove(int id);
  /**
   * Function to print the content of this SimpleDocumentList to Standard Out
   */
    void print();
  /**
   * Function to print the content of this SimpleDocumentList to a stream
   * @param *out Pointer to an ostream to print to
   */
    void to_stream(ostream *out);
  /**
   * Function to print the content of this SimpleDocumentList to a stream with the
   * additional data from a DocumentInfo Object.
   * @param *doc_info Pointer to a DocumentInfo Object providing additional Information
   * @param *out Pointer to an ostream to print to
   */
    void to_stream(DocumentInfo *doc_info,ostream *out);
  /**
   * Function to intersect this DocumentList with a second one. 
   * @param dl The DocumentList to intersect with
   * @return Pointer to the intersected DocumentList
   */
    SimpleDocumentList *intersect(SimpleDocumentList dl);  
  /**
   * Function to unify this DocumentList with a second one.
   * @param dl The SimpleDocumentList to unify with
   * @return Pointer to the unified SimpleDocumentList
   */
    SimpleDocumentList *unify(SimpleDocumentList dl); 
  /**
   * Function to complement this DocumentList according to a second one. 
   * @param element The SimpleDocumentList to complement with
   * @return Pointer to the complemented SimpleDocumentList
   */
    SimpleDocumentList *complement(SimpleDocumentList dl); 
    // The list of doc ids and counts
    unordered_map<int,int> doclist;
};


#include "DocumentInfo.hpp"

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


void SimpleDocumentList::to_stream(DocumentInfo *doc_info, ostream *out)
{
#ifdef DEBUG
  cerr << doclist.size() << " Elements" << endl;
#endif
    for (auto it=this->doclist.begin() ; it != this->doclist.end(); it++ )
      *out << "|" << it->first << ":" << doc_info->get(it->first,"filename") << ":" << it-> second << ":" << doc_info->get(it->first,"language");
    *out << "|"; 
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
