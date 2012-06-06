#ifndef _LANGUAGE_HPP
#define _LANGUAGE_HPP

#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>

#define TOP_FACTOR 10 
using namespace std;

class State;

class Transition {
  public:
    State *target;
    set<string> labels;
};

class State {
  public:
    State(string value);
    set<string> values;
    vector<Transition *> arcs;
};

class Automaton {
  public:
    State start_state;
    void add(string language,string label);
};

class LanguageRecognizer {
  public:
    void training(string language,string filename);
    void training(string language,ifstream *in);
    string recognize(string line);
  private:
    Automaton knowsyourlanguage;
};

Transition::Transition(string label, State *t)
{
  labels.insert(label);
  target=t;
}

State::State(string value)
{
  values.insert(value);
}

void Automaton::add(string language, string label)
{
  // Empty automaton
  if(start_state.transitions.size()==0)
    {
      start_state.transitions.push_back(new Transition(string label, new State(language)));
    }
  else
    {
      for (auto it=start_state.transitions.begin() ; it < start_state.transitions.end(); it++ )
	{
	  // Already label of a transition to a different language
	  if ((it->labels.count(label)>0)&&(it->target->values.count(language)==0)) 
	    {
	      State *s=new State();
	      s->values=it->target->values;
	      s->values.insert(language);

	      start_state.transitions.push_back(new Transition(
	    }
	}
    }
}

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
  unordered_map<string,int> trigram_freq;
  while(!in->eof())
    {
      string word;
      cin >> word;
      for(int pos=0;pos<word.length()-2;pos++)
	{
	  string tri=word.substr(pos,3);
	  trigram_freq[tri]++;
#ifdef DEBUG
	  cout << tri << endl;
#endif
	}
    }
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
	      smallest=top_list[pos];
	      smallest_pos=pos;
	    }
	}
      if (top_list[smallest_pos].second<it->second)
	{
	  top_list[smallest_pos].first=it->first;
	  top_list[smallest_pos].second=it->second;
	  break;
	}
    }
}

#endif
