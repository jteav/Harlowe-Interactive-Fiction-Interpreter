//This file was provided by the instructor and modified by Johnathan Teav
#include <iostream>
#include <fstream>
#include "storytokenizer.h"
using namespace std;

int main()
{
  string line, story;

  ifstream myfile ("test-example.html");

  if(myfile.is_open()){
    getline(myfile, line);
    while (myfile && line != "</html>")
    {
      story += line + '\n';
      getline(myfile, line);
    }

    StoryTokenizer st(story);
    int pass = 0;
    while (st.hasNextPassage())
    {
      PassageToken ptok = st.nextPassage();
      pass++;
      cout << "Passage " << pass << ":  " << ptok.getText() << endl;

      PassageTokenizer pt(ptok.getText());
      while (pt.hasNextSection())
      {
        SectionToken stok = pt.nextSection();
        switch (stok.getType())
        {
        case LINK:
          cout << "  Link:  ";
          break;
        case SET:
          cout << "  Set:  ";
          break;
        case GOTO:
          cout << "  Go-to:  ";
          break;
        case IF:
          cout << "  If:  ";
          break;
        case ELSEIF:
          cout << "  Else-if:  ";
          break;
        case ELSE:
          cout << "  Else:  ";
          break;
        case BLOCK:
          cout << "  Block:  ";
          break;
        case TEXT:
          cout << "  Text:  ";
          break;
        default:
          cout << "  Unknown token:  ";
        }
        cout << stok.getText() << endl;
      }
    }
  }
  else{
    cout << "Error opening file\n";
  }

  return 0;
}
