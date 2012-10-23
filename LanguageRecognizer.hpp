#ifndef _LANGUAGE_HPP
#define _LANGUAGE_HPP

#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <set>
#include <sstream>
#include <climits>
#include "Util.hpp"

#define TOP_FACTOR 20

using namespace std;

class LanguageRecognizer {
  public:
    void training(string language, string filename);
    void training(string language, ifstream *in);
    set<string> *recognize(string line);
    set<string> *recognize(istream* in);
    void to_file(string filename);
    void to_stream(ostream *out);
    void store();
    void restore(string filename);
  private:
    void trim(string language);
    // Hash to recognize languages
    unordered_map<string,set<string>> knowsyourlanguage;
    // Generate complete trigram frequency list
    unordered_map<string,unordered_map<string,int>> trigram_freq;
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
  //  // Generate complete trigram frequency list
  //  unordered_map<string,int> trigram_freq;
  while(!in->eof())
    {
      string word;
      *in >> word;
      string nword=normalize(word);
      if (nword.length()<3)
	{
	  if (nword.length()>0)
	    trigram_freq[nword][language]++;
#ifdef DEBUG
	  cout << nword << "\t" << trigram_freq[nword][language] << endl;
#endif
	}
      else
	{
	  for(unsigned int pos=0;pos<nword.length()-2;pos++)
	    {
	      string tri=nword.substr(pos,3);
	      trigram_freq[tri][language]++;
#ifdef DEBUG
	      cout << tri << "\t" << trigram_freq[tri][language] << endl;
	      //	      cout << "Got " << trigram_freq.size() << " elements in trigram frequency list" << endl;  
#endif
	    }
	}
    }
  trim(language);
}
void LanguageRecognizer::trim(string language)
{
  // Trim result list 
  size_t toplen=trigram_freq.size()*TOP_FACTOR/100;
#ifdef DEBUG
  cout << "Keeping top " << toplen << " of " << trigram_freq.size() << " elements" << endl;
#endif
  pair<string,int> top_list[toplen];
  for ( auto it = trigram_freq.begin(); it != trigram_freq.end(); ++it )
    {
#ifdef DEBUG
      cout << "Iterator " << it->first << "\t" << it->second[language] << endl;
#endif
      int smallest_pos=0;
      int smallest=INT_MAX;
      for(unsigned int pos=0;pos<toplen;pos++)
	{
	  if (top_list[pos].second<smallest)
	    {
	      smallest=top_list[pos].second;
	      smallest_pos=pos;
	    }
	}
      if (top_list[smallest_pos].second<it->second[language])
	{
	  // Set trigram to new value
	  top_list[smallest_pos].first=it->first;
	  // Set count to new value
	  top_list[smallest_pos].second=it->second[language];
#ifdef DEBUG
	  cout << "Setting position " << smallest_pos << " to " << top_list[smallest_pos].first << " with count " << top_list[smallest_pos].second << endl;
#endif
	}
    }
  // Store trimmed list with language
  for(unsigned int pos=0;pos<toplen;pos++)
    {
#ifdef DEBUG
      //      cout << "Got " << knowsyourlanguage.size() << " elements in trigram list" << endl;  
      cout << "Adding "<< top_list[pos].first << " with language " << language << endl;  
#endif
      knowsyourlanguage[top_list[pos].first].insert(language);
    }
}

set<string> *LanguageRecognizer::recognize(string line)
{
#ifdef DEBUG
  cout << "Got " << knowsyourlanguage.size() << " elements in trigram list" << endl;
  cout << "Recognizing string :" << endl << line << endl;
#endif
  stringstream linestream(line,ios::in);
  return recognize((istream *)&linestream);
}

set<string> *LanguageRecognizer::recognize(istream *in)
{
#ifdef DEBUG
  cout << "Recognizing instream :" << endl;
#endif
  unordered_map<string,int> lang_count;
  // Count all the trigrams
  while(!in->eof())
    {
      string word;
      *in >> word;
      string nword=normalize(word);
      if (nword.length()<3)
	{
	  if (nword.length()>0)
	    {
	      for ( auto it = knowsyourlanguage[nword].begin(); it != knowsyourlanguage[nword].end(); ++it )
		{
		  lang_count[*it]++;
#ifdef DEBUG
		  cout << nword << "\t" << *it << "\t" << lang_count[*it] << "#"<< endl;
#endif
		}
	    }
	}
      else
	{
	  for(unsigned int pos=0;pos<word.length()-2;pos++)
	    {
	      string tri=word.substr(pos,3);
	      for ( auto it = knowsyourlanguage[tri].begin(); it != knowsyourlanguage[tri].end(); ++it )
		{
		  lang_count[*it]++;
#ifdef DEBUG
		  cout << tri << "\t" << *it << "\t" << lang_count[*it] << endl;
#endif
		}
	    }
	}
    }
  // Find maximum trigram count
  int maxcount=0;
  for ( auto it = lang_count.begin(); it != lang_count.end(); ++it )
    {
      if (it->second>maxcount)
	maxcount=it->second;
    }
#ifdef DEBUG
  cout << "Maximum per-language trigram count " << maxcount << endl;
#endif
  // Find all languages with maximum trigram count
  set<string> *res=new set<string>();
#ifdef DEBUG
  cout << "Possible language candidates: ";
#endif
  for ( auto it = lang_count.begin(); it != lang_count.end(); ++it )
    {
      if (it->second==maxcount)
	{
	  res->insert(it->first);
#ifdef DEBUG
	  cout << it-> first;
#endif
	}
    }
#ifdef DEBUG 
  cout << endl;
#endif
  return res;
}

void LanguageRecognizer::to_file(string filename)
{
  ofstream outfile;
  outfile.open(filename);
  to_stream(&outfile);
}

void LanguageRecognizer::to_stream(ostream *out)
{
  for ( auto wordit = trigram_freq.begin(); wordit != trigram_freq.end(); ++wordit )
    {
      *out << wordit->first << "\t|";
      for (auto langit=wordit->second.begin(); langit != wordit->second.end(); ++langit )
	{
	  if (langit->first.length()>0 && langit->second>0)
	    *out << langit->first << ":" << langit->second << "|";
	}
      *out << endl;
    }
}

void LanguageRecognizer::store()
{
  to_file("language.dat");
}

void LanguageRecognizer::restore(string filename)
{
  set<string> langlist;
  ifstream in;
  in.open(filename.c_str());
  while(!in.eof())
    {
      string line;
      getline(in,line);
      stringstream linestream(line);
      string trigram;
      string langs;
      getline(linestream,trigram,'\t');
      getline(linestream,langs,'\t');
      stringstream langstream(langs);
      while (!langstream.eof())
	{
	  string langcount;
	  getline(langstream,langcount,'|');
	  stringstream langcountstream(langcount);
	  string lang, count;
	  getline(langcountstream,lang,':');
	  getline(langcountstream,count,':');
	  trigram_freq[trigram][lang]=atoi(count.c_str());
	  //	  knowsyourlanguage[trigram].insert(lang);
	  langlist.insert(lang);
	}
    }
  in.close();
  for (auto it=langlist.begin(); it!=langlist.end(); ++it)
    {
      if (it->length()>1)
	trim(*it);
    }
}
#endif
