#include <iostream>
#include "SimpleIndex.hpp"
// #include "BooleanQuery.hpp"
#include "SExpQuery.hpp"

#define LEMMALEX_GERMAN string("lemmalex.deutsch")
using namespace std;

class Searcher
{
  public:
    Searcher();
    // Restore the search index
    void restore_index(string file);
    // Load language data for language recognition
    void load_language_data(string file);
    // Add file to search index
    void add_file(string file);
    // Query the search index
    int query(string q);
    // Store the search index into a file
    int store_index(string file);
  private:
    // Our index
    SimpleIndex index;
    // Check if index is present
    bool have_index;
};

Searcher::Searcher()
{
  have_index=false;
  // Load our lemma lexicon
  index.load_lemma_lex("deutsch",LEMMALEX_GERMAN);
}

// Restore the search index
void Searcher::restore_index(string file)
{
  have_index=true;
  ifstream infile;
  infile.open(file);
  index.restore_index(&infile);
}

// Load language data for language recognition
void Searcher::load_language_data(string file)
{
  index.lang_rec.restore(file);
}

// Add file to search index
void Searcher::add_file(string file)
{
  // Say that we have an index
  have_index=true;
  // Insert file into Index
  index.insert(file);
#ifdef DEBUG
  index.print();
#endif
}

// Query the search index
int Searcher::query(string qs)
{
  // No work to do if no Index present
  if (! have_index)
    {
      cout << "No data to query" << endl;
      return -1;
    }
  else
    {
      // Execute query
      //BooleanQuery q(&index);
      Query<int> *q=new SExpQuery(&index);
      q->parse(qs);
      // Get and print results
      SimpleDocumentList *r=(SimpleDocumentList*)q->get_result();
#ifdef DEBUG
      cerr << "R size" << r->doclist.size() << endl;
      cerr << "R " << r << endl; 
#endif
      cout << "Result for query \"" << qs << "\":" << endl;
      //r->print();
      for (auto it=r->doclist.begin(); it!=r->doclist.end();++it)
	{
	  cout << index.doc_info.get(it->first,"filename") << "\t" << it->second << "\t" << index.doc_info.get(it->first,"language") << endl;
	}
    }
  return 1;
}

// Store the search index into a file
int Searcher::store_index(string file)
{
  if (!have_index)
    {
      cout << "No data to store" << endl;
      return -1;
    }
  else
    {
      index.to_file(file);
      return 1;
    }
}

void usage()
{
  cout << "Searcher <Command_1> <Parameter_1> ... <Command_n> <Parameter_n>" << endl;
  cout << "Search engine to manage a file index and search it" << endl;
  cout << "Possible commands:" << endl;
  cout << "\t -add <Filename>: Adds a file to the Index" << endl;
  cout << "\t -query <\"(s-Expression)\">: Query the Index with a s-Expression." << endl; 
  cout << "\t\t s-Expression can consist of a single search term or an operator and operands." << endl;
  cout << "\t\tPossible operators are and, or, not, meta. Operands can be search terms or s-Expressions." << endl;
  cout << "\t\tmeta can be used to query meta informations like language." << endl;
  cout << "\t -store <Filename>: Store the internal index to a file" << endl;
  cout << "\t -restore <Filename>: Restore the internal index from a file" << endl;
  cout << "\t -langfile <Filename>: Restore the language data from a file" << endl;
}

int main(int argc, char *argv[])
{
  Searcher search;
  // Check if at least on command and for each command exactly one parameter
  if (argc<3||argc%2!=1)
    {
      usage();
    }
  else
    {
      // Number of commands given by command line
      int command_count=(argc-1)/2;
      // Saves commands and parameters as pairs
      pair<string,string> commands[command_count];
      for(int pos=1;pos<argc;pos+=2)
	{
	  // Save command
	  commands[pos/2].first=string(argv[pos]);
	  // Save parameter
	  commands[pos/2].second=string(argv[pos+1]);
#ifdef DEBUG
	  cout << commands[pos/2].first << "\t" << commands[pos/2].second << "\t" << endl;
#endif
	}
      // Check for restore index
      for (int pos=0;pos<command_count;pos++)
	{
	  if (commands[pos].first=="-restore")
	    {
	      cout << "INIT: Loading index from " << commands[pos].second << endl;
	      search.restore_index(commands[pos].second);
	    }
	}
      // Check for restore language data
      for (int pos=0;pos<command_count;pos++)
	{
	  if (commands[pos].first=="-langfile")
	    {
	      cout << "INIT: Loading language data from " <<  commands[pos].second << endl;
	      search.load_language_data(commands[pos].second);
	    }
	}
      // Check for add files
      for (int pos=0;pos<command_count;pos++)
	{
	  if (commands[pos].first=="-add")
	    {
	      cout << "INIT: Adding file " << commands[pos].second << endl;
	      search.add_file(commands[pos].second);
	    }
	}
      // Check for queries
      for (int pos=0;pos<command_count;pos++)
	{
	  if (commands[pos].first=="-query")
	    {
	      search.query(commands[pos].second);
	    }
	}
      // Check for store index
      for (int pos=0;pos<command_count;pos++)
	{
	  if (commands[pos].first=="-store")
	    {
	      search.store_index(commands[pos].second);
	    }
	}
    }
#ifdef DEBUG
  cin.getline(NULL,0);
#endif
}
