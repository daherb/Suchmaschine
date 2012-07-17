#ifndef _DOCUMENTINFO_HPP
#define _DOCUMENTINFO_HPP

#include <string>
#include <unordered_map>

#include "DocumentList.hpp"

using namespace std;

class SimpleDocumentList;

class DocumentInfo
{
  public:
    void set(int id, string tag, string value);
    string get(int id, string tag);
    SimpleDocumentList *get_all(string tag, string value);
  private:
  // Hash of Hashes with document id as first key, Meta tag as second key and string as value of the Meta data
  //unordered_map<int, pair<string,string>> doc_info;
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

//DocumentList<int> *DocumentInfo::get_all(string tag, string value)
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
