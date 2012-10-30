/**
 * @file DocumentInfo.hpp
 * @author  Herbert Lange <herbert.lange@campus.lmu.de>
 * @version 1.0
 */
#ifndef _DOCUMENTINFO_HPP
#define _DOCUMENTINFO_HPP

#include <string>
#include <unordered_map>

#include "DocumentList.hpp"

using namespace std;

class SimpleDocumentList;

/**
 * The DocumentInfo class is a class to store and query Meta-data for Document IDs.
 */
class DocumentInfo
{
  public:
 /**
  * Function to store Meta-data for a Document.
  *
  * @param id The Document ID to associate the Meta-data with
  * @param tag The Identifier for the Data field
  * @param value The Value to store in the Cell determined by Document ID and Tag
  */
    void set(int id, string tag, string value);
 /**
  * Function to retrieve Meta-date stored for Document ID with Tag
  *
  * @param id Document ID the Data is stored for
  * @param tag Tag for the Meta-data
  * @return The value of the Meta-data stored for Document ID with Tag
  */
    string get(int id, string tag);
 /**
  * Function to retrieve all Documents where the Meta-date with the selected Tag has the given Value.
  *
  * @param tag Tag for the Meta-data
  * @param value Value to filter the Meta-data for
  * @return Pointer to a List of all Documents where the Tag has the Value
  */
    SimpleDocumentList *get_all(string tag, string value); /**< Gets all documents where the metadata tag has the value */
  private:
  /** Document information table. Hash of Hashes with document id as first key, 
    * Meta tag as second key and string as value of the Meta data
    */
    unordered_map<int,unordered_map<string,string>> doc_info;
};

#include "SimpleDocumentList.hpp"

void DocumentInfo::set(int id, string tag, string value)
{
  doc_info[id][tag]=value;
}


string DocumentInfo::get(int id, string tag)
{
  return doc_info[id][tag];
}

SimpleDocumentList *DocumentInfo::get_all(string tag, string value)
{
  SimpleDocumentList *res=new SimpleDocumentList();
  for (auto it=doc_info.begin(); it!=doc_info.end(); ++it)
    {
      if (it->second[tag]==value)
	{
	  res->add(it->first);
	}
    }
  return res;
}
#endif
