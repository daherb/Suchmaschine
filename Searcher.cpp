#include <iostream>
#include "SimpleIndex.hpp"
#include "BooleanQuery.hpp"

#define LEMMALEX_GERMAN string("lemmalex.deutsch")
using namespace std;

void usage()
{
  cout << "Searcher <Command_1> <Parameter_1> ... <Command_n> <Parameter_n>" << endl;
  cout << "Search engine to manage a file index and search it" << endl;
  cout << "Possible commands:" << endl;
  cout << "\t -add <Filename>: Adds a file to the Index" << endl;
  cout << "\t -query <\"term1 [op1 term2 ...]\">: Query the Index" << endl;
  cout << "\t -store <Filename>: Store the internal index to a file" << endl;
  cout << "\t -restore <Filename>: Restore the internal index from a file" << endl;
  cout << "\t -langfile <Filename>: Restore the language data from a file" << endl;
}

int main(int argc, char *argv[])
{
  // Check if at least on command and for each command exactly one parameter
  if (argc<3||argc%2!=1)
    {
      usage();
    }
  else
    {
      // Number of commands given by command line
      int command_count=(argc-1)/2;
      // Check if index is present
      bool have_index=false;
      // Our index
      SimpleIndex index;
      // Load our lemma lexicon
      index.load_lemma_lex("deutsch",LEMMALEX_GERMAN);
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
	      have_index=true;
	      ifstream infile;
	      infile.open(commands[pos].second);
	      index.restore_index(&infile);
	    }
	}
      // Check for restore language data
      for (int pos=0;pos<command_count;pos++)
	{
	  if (commands[pos].first=="-langfile")
	    {
	      cout << "INIT: Loading language data from " << commands[pos].second << endl;
	      index.lang_rec.restore(commands[pos].second);
	    }
	}
      // Check for add files
      for (int pos=0;pos<command_count;pos++)
	{
	  if (commands[pos].first=="-add")
	    {
	      cout << "INIT: Adding file " << commands[pos].second << endl;
	      // Say that we have an index
	      have_index=true;
	      // Insert file into Index
	      index.insert(commands[pos].second);
#ifdef DEBUG
	      index.print();
#endif
	    }
	}
      // Check for queries
      for (int pos=0;pos<command_count;pos++)
	{
	  if (commands[pos].first=="-query")
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
		  BooleanQuery q(&index);
		  q.parse(commands[pos].second);
		  // Get and print results
		  SimpleDocumentList *r=q.get_result();
#ifdef DEBUG
		  cerr << "R size" << r->doclist.size() << endl;
		  cerr << "R " << r << endl; 
#endif
		  cout << "Result for query \"" << commands[pos].second << "\":" << endl;
		  //r->print();
		  for (auto it=r->doclist.begin(); it!=r->doclist.end();++it)
		    {
		      cout << index.doc_info.get(it->first,"filename") << "\t" << it->second << "\t" << index.doc_info.get(it->first,"language") << endl;
		    }
		}
	    }
	}
      // Check for store index
      for (int pos=0;pos<command_count;pos++)
	{
	  if (commands[pos].first=="-store")
	    {
	      if (!have_index)
		{
		  cout << "No data to store" << endl;
		  return -1;
		}
	      else
		{
		  index.to_file(commands[pos].second);
		}
	    }
	}
    }
#ifdef DEBUG
  cin.getline(NULL,0);
#endif
}
