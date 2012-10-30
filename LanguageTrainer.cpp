/**
 * @file LanguageTrainer.hpp
 * @author  Herbert Lange <herbert.lange@campus.lmu.de>
 * @version 1.0
 *
 * @section DESCRIPTION
 * Trainer is a small program to train the Language Model needed for Language Idenfification
 */

#include "LanguageRecognizer.hpp"
#include "Util.hpp"

void usage()
{
  cout << "Trainer [-restore <Index-File> ] -store <Index-File> <File_1> <Language_1> ... <File_n> <Language_n>" << endl;
  cout << "Adds the files <File_1> ... <File_n> together with associated language <Language_1> ... <Language_n>"<< endl;
  cout << "into a data file to use it in the search process" << endl;
  cout << "Required command:" << endl;
  cout << "\t -store <Data-File>: Stores the created language data to <Data-File>" << endl;
  cout << "Optional command:" << endl;
  cout << "\t -restore <Data-File>: Restore language data from <Data-File" << endl;
}

int main(int argc, char* argv[])
{
  // Check if at least on command and one file to add and for each filename exactly one language
  if (argc<3||argc%2!=1)
    {
      usage();
    }
  else
    {
      Util u;
      // Language Recognizer
      LanguageRecognizer lang_rec;
      // Checks if filename to store is given
      bool have_filename=false;
      // File name to store
      string filename;
      int file_count=(argc-1)/2;
      // Saves file names and languagess as pairs
      pair<string,string> files[file_count];
      int array_pos=0;
      for(int pos=1;pos<argc;pos+=2)
	{
	  string tmp(argv[pos]);
	  if (tmp==string("-restore"))
	    {
	      lang_rec.restore(argv[pos+1]);
	      file_count--;
	    }
	  else if(tmp==string("-store"))
	    {
	      filename=argv[pos+1];
	      have_filename=true;
	      file_count--;
	    }
	  else
	    {
	      // Save file name
	      files[array_pos].first=tmp;
	      // Save language
	      files[array_pos].second=u.normalize(string(argv[pos+1]));
#ifdef DEBUG
	      cout << files[array_pos].first << "\t" << files[array_pos].second << "\t" << endl;
#endif
	      array_pos++;
	    }
	}
      if (have_filename)
	{	  
#ifdef DEBUG
	  cout << "Filecount: " << file_count << endl;
#endif
	  for(int ct=0; ct<file_count; ct++)
	    {
	      cout << "Adding File: " << files[ct].first << "\tLanguage: " << files[ct].second << "\t" << endl;
	      lang_rec.training(files[ct].second,files[ct].first);
	    }
	  lang_rec.to_file(filename);
	}
      else
	{
	  cerr << "No file name to store the data" << endl;
	}
    } 
}
