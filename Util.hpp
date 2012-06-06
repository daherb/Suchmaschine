#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

// Normalize a word (still a lot of work to do)
string normalize(string word)
{
  string result("");
  for(unsigned int pos=0; pos<word.length();pos++)
    {
      
      // Handle UTF-8
      // Two-byte-character
      if (((unsigned char)word[pos]&0xc0)==0xc0)
	{
	  switch((unsigned char)word[pos]) 
	    {
	      // Skip all Special characters in this block
	    case 0xc2:
	      break;
	      // Normalize umlaut
	    case 0xc3:
	      switch((unsigned char)word[pos+1])
		{
		case 0x8a:
		case 0xa4:
		  result+='a';
		  break;
		case 0x96:
		case 0xb6:
		  result+='o';
		case 0x9c:
		case 0xbc:
		  result+='u';
		case 0x9f:
		  result+="ss";
		}
	    }
	  pos++;
	}
      // Single-byte-character
      else if ((unsigned char)word[pos]<128)
	{ 
	  // Delete special characters
	  switch ((unsigned char)word[pos]) 
	    {
	    case '!':
	    case '"':
	    case '#':
	    case '$':
	    case '%':
	    case '&':
	    case '\'':
	    case '(':
	    case ')':
	    case '*':
	    case '+':
	    case ',':
	    case '-':
	    case '.':
	    case '/':
	    case ':':
	    case ';':
	    case '<':
	    case '>':
	    case '=':
	    case '?':
	    case '@':
	    case '[':
	    case '\\':
	    case ']':
	    case '^':
	    case '_':
	    case '`':
	    case '{':
	    case '|':
	    case '}':
	    case '~':
	      break;
	    default:
	      // Capital letters -> convert to small letters
	      if ((unsigned char)word[pos]<0x61)
		{
		  result+=word[pos]-'A'+'a';
		}
	      // Small letters
	      else
		{
		  result+=word[pos];
		}
	    }
	}
      // else skip (utf-8 characters with three or four bytes)
    }
  return result;
}

// Split string at spaces and eliminate multiple spaces
vector<string> *tokenize(string line)
{
  // Insert space to ensure termination
  line+=' ';
  // List of split words
  vector<string> *result=new vector<string>();
  // Current word
  string word="";
  // Position of last separator (space)
  int seppos=-1;
  // For all letters in the line
  for (int pos=0 ; pos < (int)line.length(); pos++ )
    {
      // Separator
      if ((line[pos]==' '))
	{
	  // At least one letter between spaces
	  if (seppos<pos-1)
	    {
	      // Save word and start again with new empty one
	      result->push_back(word);
	      word="";
	      // Save position of last space
	      seppos=pos;
	    }
	  // else skip double spaces etc.
	  else
	    {
	      seppos++;
	    }
	}
      // Ordinary letter -> add to word
      else 
	{
	  word+=line[pos];
	}
    }
  return result;
}
#endif
