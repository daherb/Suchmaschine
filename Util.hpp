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
      // Copy six-byte characters
      if (((unsigned char)word[pos]&0xfc)==0xfc)
	{
	  result+=word.substr(pos,6);
	  pos+=5;
	}
      // Copy five-byte characters
      else if (((unsigned char)word[pos]&0xf8)==0xf8)
	{
	  result+=word.substr(pos,5);
	  pos+=4;
	}
      // Copy four-byte characters
      else if (((unsigned char)word[pos]&0xf0)==0xf0)
	{
	  result+=word.substr(pos,4);
	  pos+=3;
	}
      // Copy three-byte characters
      else if (((unsigned char)word[pos]&0xe0)==0xe0)
	{
	  result+=word.substr(pos,3);
	  pos+=2;
	}
      // else should be invalid in utf-8
      // Two-byte-character
      else if (((unsigned char)word[pos]&0xc0)==0xc0)
	{
	  switch((unsigned char)word[pos]) 
	    {
	      // Skip all Special characters in this block
	    case 0xc2:
	      break;
	      // Normalize diacritics and special characters
	    case 0xc3:
	      switch((unsigned char)word[pos+1])
		{
		  // Remove some special characters
		case 0x97:
		case 0xb7:
		  break;
		  // Remove all diacritics form a's
		case 0x80:
		case 0x81:
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x85:
		case 0xa0:
		case 0xa1:
		case 0xa2:
		case 0xa3:
		case 0xa4:
		case 0xa5:
		  result+='a';
		  break;
		  // Remove all diacritics form e's
		case 0x88:
		case 0x89:
		case 0x8a:		
		case 0x8b:
		case 0xa8:
		case 0xa9:
		case 0xaa:		
		case 0xab:
		  result+='e';
		  break;
		  // Remove all diacritics form i's
		case 0x8c:
		case 0x8d:
		case 0x8e:
		case 0x8f:
		case 0xac:
		case 0xad:
		case 0xae:
		case 0xaf:
		  result+='i';
		  break;
		  // Remove all diacritics form o's
		case 0x92:
		case 0x93:
		case 0x94:
		case 0x95:
		case 0x96:
		case 0x98:
		case 0xb2:
		case 0xb3:
		case 0xb4:
		case 0xb5:
		case 0xb6:
		case 0xb8:
		  result+='o';
		  break;
		  // Remove all diacritics form u's
		case 0x99:
		case 0x9a:
		case 0x9b:
		case 0x9c:
		case 0xb9:
		case 0xba:
		case 0xbb:
		case 0xbc:
		  result+='u';
		  // Remove all diacritics form c's
		case 0x87:
		case 0xa7:
		  result+='c';
		  break;
		  // Remove all diacritics form n's
		case 0x91:
		case 0xb1:
		  result+='n';
		  break;
		  // Remove all diacritics form y's
		case 0x9d:
		case 0xbd:
		case 0xbf:
		  result+='y';
		  break;
		  // Split ligatures
		case 0x9f:
		  result+="ss";
		  break;
		case 0x86:
		case 0xa6:
		  result+="ae";
		  break;
		}
	      break;
	    default:
	      result+=word.substr(pos,2);  
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
