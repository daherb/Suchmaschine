#ifndef _DOCUMENTINFO_HPP
#define _DOCUMENTINFO_HPP
class DocumentInfo
{
  public:
    void set_language(int id, string language);
  void set(int id, string tag, string filename);
  string get(int id, string tag);
  private:
  // Hash of Hashes with document id as first key, Meta tag as second key and string as value of the Meta data
  //unordered_map<int, pair<string,string>> doc_info;
    unordered_map<int,unordered_map<string,string>> doc_info;
};

void DocumentInfo::set(int id, string tag, string value)
{
  doc_info[id][tag]=value;
}


string DocumentInfo::get(int id, string tag)
{
  return doc_info[id][tag];
}

#endif
