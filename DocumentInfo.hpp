
class DocumentInfo
{
  public:
    void set_language(int id, string language);
    void set_filename(int id, string filename);
    string get_language(int id);
    string get_filename(int id);
  private:
  // Hash table with document id as key and pair of language and filename as value
    unordered_map<int id, pair<string,string>> doc_info;
};

void DocumentInfo::set_language(int id, string language)
{
  doc_info[id].first=language;
}

void DocumentInfo::set_filename(int id, string filename)
{
  doc_info[id].second=filename;
}

string DocumentInfo::get_language(int id)
{
  return doc_info[id].first;
}

string DocumentInfo::get_filename(int id)
{
  return doc_info[id].second;
}
