#ifndef SIMPLEINDEX_H
#define SIMPLEINDEX_H

#include <unordered_map>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Util.hpp"
#include "Index.hpp"
#include "SimpleDocumentList.hpp"
#include "DocumentInfo.hpp"
#include "Language.hpp"

using namespace std;

class SimpleIndex: public Index<int>
{
  public:
    DocumentInfo doc_info;
    LanguageRecognizer lang_rec;
  // Insert files either by name or by filestream
    void insert(string filename) {Index<int>::insert(filename); };
    void insert(ifstream *in, string filename);
  // Get document list by key
    DocumentList<int> *retrieve(string key);
  // Get all doc ids in the index
    DocumentList<int> *get_doc_ids();
    void print();
    void to_stream(ostream *out);
    void to_file(string filename);
    void restore_index(ifstream *infile);
    void load_lemma_lex(string language, string filename);
  private:
  // Index as a unoredered map (quite like hash map) with strings as keys and 
  // pair of term count (int) and document list
    unordered_map<string,pair<int,SimpleDocumentList>> inverted_index;
  // List of all doc ids
    SimpleDocumentList doc_ids;
  // First key language, second key full form, value lemma
    unordered_map<string,unordered_map<string,string>> lemma_forward;
  // First keylanguage, second key lemma, value set of all full forms
    unordered_map<string,unordered_map<string,set<string>>> lemma_backward;
};


void SimpleIndex::insert(ifstream *in, string filename)
{
  // Get next doc id
  int docid=doccount++;
  // Save filename info
  doc_info.set(docid,"filename",filename);
  // Get document language ...
  ifstream tempstream;
  tempstream.open(filename);
  set<string> *langs=lang_rec.recognize(&tempstream);
  // Only set language if result is unambigous
  string lang;
  if (langs->size()==1)
    lang=*(langs->begin());
  else
    lang="";
  tempstream.close();
  doc_info.set(docid,"language",lang);
#ifdef DEBUG
  cerr << "Got " << lang << endl;
#endif 
  // Store in the list of all doc ids
  doc_ids.add(docid);
  string word;
  // Read the whole file word by word to the end
  while(!in->eof())
  {
    *in >> word;
    string nword=normalize(word);
    // Add to index
    if (lemma_forward[lang][nword]!="")
      {
#ifdef DEBUG
	cout << "Got lemma " << lemma_forward[lang][nword] << " for word " << nword << endl;
#endif
	for (auto it=lemma_backward[lang][lemma_forward[lang][nword]].begin(); it!=lemma_backward[lang][lemma_forward[lang][nword]].end(); ++it)
	  {
	    inverted_index[*it].first++;
	    inverted_index[*it].second.add(docid);
	  }
      }
    else
      {
	inverted_index[nword].first++;
	inverted_index[nword].second.add(docid);
      }
  }
}

DocumentList<int> *SimpleIndex::retrieve(string key)
{
  return &inverted_index[key].second;
}

DocumentList<int> *SimpleIndex::get_doc_ids()
{
  return &doc_ids;
}

void SimpleIndex::to_stream(ostream *out)
{
  for ( auto it = inverted_index.begin(); it != inverted_index.end(); ++it )
    {
      *out << it->second.first << "\t";
      it->second.second.to_stream(&doc_info,out);
      *out << "\t" << it->first << endl;
    }
}
void SimpleIndex::print()
{
  to_stream(&cout);
}

void SimpleIndex::to_file(string filename)
{
  ofstream outfile;
  outfile.open(filename);
  to_stream(&outfile);
}

void SimpleIndex::restore_index(ifstream *infile)
{
  while(!infile->eof())
    {
      string line;
      getline(*infile,line);
      stringstream linestream(line);
      string scount,sdoclist,sword;
      getline(linestream,scount,'\t');
      getline(linestream,sdoclist,'\t');
      getline(linestream,sword);
      inverted_index[sword].first+=atoi(scount.c_str());
      stringstream docstream(sdoclist);
      while(!docstream.eof())
	{
	  string sdocinfo;
	  string sdocnum, sdocfilename, sdoccount, sdoclang;
	  getline(docstream,sdocinfo,'|');
	  stringstream docinfostream(sdocinfo);
	  getline(docinfostream,sdocnum,':');
	  getline(docinfostream,sdocfilename,':');
	  getline(docinfostream,sdoccount,':');
	  getline(docinfostream,sdoclang,':');
	  if (sdocnum!=""&& sdoccount!="")
	    {
	      int docnum=atoi(sdocnum.c_str());
	      inverted_index[sword].second.add(docnum,atoi(sdoccount.c_str()));
	      if (sdocfilename!="")
		{
		  doc_info.set(docnum,"filename",sdocfilename);
		}
	      else
		{
		  doc_info.set(docnum,"filename",string("Lost in translation"));
		}
	      if (sdoclang!="")
		{
		  doc_info.set(docnum,"language",sdoclang);
		}
	    }
	}
    }
}

void SimpleIndex::load_lemma_lex(string language, string filename)
{
  cerr << "INIT: loading lemma lexicon " << filename << endl;
  ifstream in;
  in.open(filename.c_str());
  while(!in.eof())
    {
      string full, lemma, nfull, nlemma;
      in >> full;
      in >> lemma;
      nfull=normalize(full);
      nlemma=normalize(lemma);
      lemma_forward[language][nfull]=nlemma;
      lemma_backward[language][nlemma].insert(nfull);
      lemma_backward[language][nlemma].insert(nlemma);
    }
}
#endif
