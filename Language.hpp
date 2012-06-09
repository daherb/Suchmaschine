#ifndef _LANGUAGE_HPP
#define _LANGUAGE_HPP

#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <set>
#include <sstream>
#include "Util.hpp"

#define TOP_FACTOR 10 
using namespace std;

class LanguageRecognizer {
  public:
    void training(string language, string filename);
    void training(string language, ifstream *in);
    set<string> *recognize(string line);
    set<string> *recognize(istream* in);
  private:
  unordered_map<string,set<string>> knowsyourlanguage;
};

void LanguageRecognizer::training(string language, string filename)
{
#ifdef DEBUG
  cout << "Adding file " << filename << " with language " << language << " to recognizer" << endl;
#endif
  // Open filestream
  ifstream infile;
  infile.open(filename,ifstream::in);
  // Call the other insert method with the filestream
  training(language,&infile);
}

void LanguageRecognizer::training(string language, ifstream *in)
{
#ifdef DEBUG
  cout << "Start training for " << language << endl;
#endif
  // Generate complete trigram frequency list
  unordered_map<string,int> trigram_freq;
  while(!in->eof())
    {
      string word;
      *in >> word;
      string nword=normalize(word);
      if (nword.length()<3)
	{
	  trigram_freq[nword]++;
#ifdef DEBUG
	  cout << nword << endl;
#endif
	}
      else
	{
	  for(int pos=0;pos<nword.length()-2;pos++)
	    {
	      string tri=nword.substr(pos,3);
	      trigram_freq[tri]++;
#ifdef DEBUG
	      cout << tri << endl;
#endif
	    }
	}
    }
  // Trim result list 
  size_t toplen=trigram_freq.size()/TOP_FACTOR;
  pair<string,int> top_list[toplen];
  for ( auto it = trigram_freq.begin(); it != trigram_freq.end(); ++it )
    {
      int smallest_pos=0;
      int smallest=0;
      for(int pos=0;pos<toplen;pos++)
	{
	  if (top_list[pos].second<smallest)
	    {
	      smallest=top_list[pos].second;
	      smallest_pos=pos;
	    }
	}
      if (top_list[smallest_pos].second<it->second)
	{
	  // Set trigram to new value
	  top_list[smallest_pos].first=it->first;
	  // Set count to new value
	  top_list[smallest_pos].second=it->second;
	  break;
	}
    }
  // Store trimmed list with language
  for(int pos=0;pos<toplen;pos++)
    {
      knowsyourlanguage[top_list[pos].first].insert(language);
    }
}

set<string> *LanguageRecognizer::recognize(string line)
{
  stringstream linestream(line,ios::in);
  recognize((istream *)&linestream);
}

set<string> *LanguageRecognizer::recognize(istream *in)
{
  unordered_map<string,int> lang_count;
  // Count all the trigrams
  while(!in->eof())
    {
      string word;
      *in >> word;
      string nword=normalize(word);
      if (nword.length()<3)
	{
	  for ( auto it = knowsyourlanguage[nword].begin(); it != knowsyourlanguage[nword].end(); ++it )
	    lang_count[*it]++;
#ifdef DEBUG
	  cout << nword << endl;
#endif
	}
      else
	{
	  for(int pos=0;pos<word.length()-2;pos++)
	    {
	      string tri=word.substr(pos,3);
	      for ( auto it = knowsyourlanguage[tri].begin(); it != knowsyourlanguage[tri].end(); ++it )
		lang_count[*it]++;
#ifdef DEBUG
	      cout << tri << endl;
#endif
	    }
	}
    }
  // Find maximum trigram count
  int maxcount=0;
  for ( auto it = lang_count.begin(); it != lang_count.end(); ++it )
    {
      if (it->second>maxcount)
	maxcount=it->second>maxcount;
    }
  // Find all languages with maximum trigram count
  set<string> *res=new set<string>();
  for ( auto it = lang_count.begin(); it != lang_count.end(); ++it )
    {
      if (it->second==maxcount)
	res->insert(it->first);
    }
  return res;
}

#endif
