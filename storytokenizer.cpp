//This file was provided by the instructor and modified by Johnathan Teav
#include "storytokenizer.h"

bool PassageTokenizer::hasNextSection() const
{
  return pos < pass.size();
}

SectionToken PassageTokenizer::nextSection()
{
  unsigned int start, end;
  SectionToken ret;

  if (pos >= pass.size())
  {
    lastsect = TEXT;
    return ret;
  }
  
  //Handle blocks separately
  start = pass.find(BLOCK_START, pos);
  if ((lastsect == IF || lastsect == ELSEIF || lastsect == ELSE) && start != string::npos)
  {
    end = start+1;
    unsigned int level = 1;
    while (level > 0 && end < pass.size())
    {
      if (pass.substr(end, BLOCK_START.size()) == BLOCK_START)
        level++;
      else if (pass.substr(end, BLOCK_START.size()) == BLOCK_END)
        level--;
      end++;
    }

    if (level == 0)
      lastsect = BLOCK; 
    else
      lastsect = TEXT; //Treat a bad block as text?
    ret = SectionToken(pass.substr(start, end - start), BLOCK);
    pos = end;
    return ret;
  }
  else
  {
    unsigned int nextset = pass.find(SET_START, pos);
    unsigned int nextgoto = pass.find(GOTO_START, pos);
    unsigned int nextlink = pass.find(LINK_START, pos);
    unsigned int nextif = pass.find(IF_START, pos);
    unsigned int nextelseif = pass.find(ELSEIF_START, pos);
    unsigned int nextelse = pass.find(ELSE_START, pos);

    unsigned int nexttok = nextset;
    if (nextgoto < nexttok)
      nexttok = nextgoto;
    if (nextgoto < nexttok)
      nexttok = nextgoto;
    if (nextlink < nexttok)
      nexttok = nextlink;
    if (nextif < nexttok)
      nexttok = nextif;
    if (nextelseif < nexttok)
      nexttok = nextelseif;
    if (nextelse < nexttok)
      nexttok = nextelse;

    end = string::npos;
    lastsect = TEXT;
    if (nexttok > pos)
      end = nexttok;
    else if (nexttok == nextlink)
    {
      end = pass.find(LINK_END, nextlink) + LINK_END.size();
      lastsect = LINK;
    }
    else if (nexttok != string::npos)
    {
      end = pass.find(COMMAND_END, pos) + 1;
      if (nexttok == nextgoto)
        lastsect = GOTO;
      else if (nexttok == nextset)
        lastsect = SET;
      else if (nexttok == nextif)
        lastsect = IF;
      else if (nexttok == nextelseif)
        lastsect = ELSEIF;
      else if (nexttok == nextelse)
        lastsect = ELSE;
      else
        lastsect = TEXT;
    }

    if (end != string::npos)
      ret = SectionToken(pass.substr(pos, end - pos), lastsect);
    else
      ret = SectionToken(pass.substr(pos), lastsect);
    pos = end;
    return ret;
  }
}

//Function to insert SectionTokens into a vector
void PassageToken::addSections(){
  pt = new PassageTokenizer(getText());
  SectionToken sec;
  while (pt->hasNextSection()){
    sec = pt->nextSection();
    sections.push_back(sec);
    if(sec.getType() == LINK){
      addLinks(sec);
    }
  }
}

//Function to insert Links into a vector
void PassageToken::addLinks(SectionToken stok){
  //Remove the brackets
  int index = stok.getText().find("[[");
  index+=2;
  string text;
  for(; stok.getText()[index] != ']'; index++){
    text += stok.getText()[index];
  }
  links.push_back(text);
}

bool StoryTokenizer::hasNextPassage() const
{
  return story.find(PASSAGE_START_TAG, pos) != string::npos;
}

PassageToken StoryTokenizer::nextPassage()
{
  unsigned int next = story.find(PASSAGE_START_TAG, pos);
  unsigned int start = story.find(PASSAGE_START_TAG_CLOSE, next) + 1;
  unsigned int end = story.find(PASSAGE_END_TAG, start);

  if (next == string::npos || end == string::npos)
    return PassageToken();
  else
  {
    pos = end;
    unsigned int namestart = story.find(PASSAGE_NAME_START, next) + PASSAGE_NAME_START.size();
    unsigned int nameend = story.find(PASSAGE_NAME_END, namestart);
    if (namestart == string::npos || nameend == string::npos)
      return PassageToken(story.substr(start, end - start));
    else
      return PassageToken(story.substr(namestart, nameend - namestart), story.substr(start, end - start));
  }
}

//Sets a variable and its value to an unordered map.
void Story::setVariable(SectionToken stok){
  string set = stok.getText();
  int index = set.find('$');
  string setVar;
  for(;set[index] != ' '; index++){
    setVar += set[index];
  }

  bool setBool;
  if(set.find("false") != string::npos){
    setBool = false;
  }
  else if(set.find("true") != string::npos){
    setBool = true;
  }

  if(variables.find(setVar) == variables.end()){
    pair<string, bool> setPair(setVar, setBool);
    variables.insert(setPair);
  }
  else{
    variables[setVar] = setBool;
  }
}

//Function to handle IF section types
bool Story::ifSection(SectionToken stok){
  string set = stok.getText();
  int index = set.find('$');
  string setVar;
  for(;set[index] != ' '; index++){
    setVar += set[index];
  }

  bool setBool;
  if(set.find("false") != string::npos){
    setBool = false;
  }
  else if(set.find("true") != string::npos){
    setBool = true;
  }

  if(variables[setVar] == setBool){
    return true;
  }
  else{
    return false;
  }
}

//Function to handle blocks
void Story::block(string stok){
  PassageTokenizer pt(stok);
  SectionToken sec;
  while (pt.hasNextSection()){
    sec = pt.nextSection();
    bool ifSec;

    //Statements handle set variables, if, else if, and else sections
    if(sec.getType() == IF){
      ifSec = ifSection(sec);
      sec = pt.nextSection();
      if(ifSec){
        cout << sec.getText();
      }
    }
    else if(sec.getType() == ELSEIF){
      ifSec = ifSection(sec);
      sec = pt.nextSection();
      if(ifSec){
        cout << sec.getText();
      }
    }
    else if(sec.getType() == ELSE){
      sec = pt.nextSection();
      if(!ifSec){
        cout << sec.getText();
      }
    }
    else{
      cout << sec.getText();
    }
  }
}

//Function to set everything up
void Story::setUp(){
  StoryTokenizer st(story);

  //Loop to insert all passages into an unordered map
  while (st.hasNextPassage()){
    PassageToken ptok = st.nextPassage();
    pair<string, PassageToken> newPair(ptok.getName(), ptok);
    passages.insert(newPair);

    //Insert the passage's sections into a vector
    ptok.addSections();
  }  
}

//Function to run everything
void Story::play(int in){
  string passageName;
  if(in == 1){
    passageName = "start";
  }
  else{
    passageName = "New Game";
  }
  setUp();

  for(auto sec: passages[passageName].sections){
    if(sec.getType() == SET){
      setVariable(sec);
    }
    else{
      cout << sec.getText();
    }
  }
  cout << endl;

  //Display the links as options
  cout << "Enter:\n";
  int i = 1;
  for(auto link: passages[passageName].links){
    cout << i << " for " << link << endl;
    i++;
  }

  //User picks a link
  int input;
  cin >> input;
  string choice = passages[passageName].links[input-1];

  //Loop for the rest of the passages
  while(true){
    for(int i = 0; i < passages[choice].sections.size(); i++){
      bool ifSec;
      //Statements handle set variables, if, else if, and else sections
      if(passages[choice].sections[i].getType() == SET){
        setVariable(passages[choice].sections[i]);
      }
      else if(passages[choice].sections[i].getType() == IF){
        ifSec = ifSection(passages[choice].sections[i]);
        i++;
        if(ifSec){
          cout << passages[choice].sections[i].getText();
        }
      }
      else if(passages[choice].sections[i].getType() == ELSEIF){
        ifSec = ifSection(passages[choice].sections[i]);
        i++;
        if(ifSec){
          cout << passages[choice].sections[i].getText();
        }
      }
      else if(passages[choice].sections[i].getType() == ELSE){
        i++;
        if(!ifSec){
          if(passages[choice].sections[i].getType() == BLOCK){
            block(passages[choice].sections[i].getText());
          }
          else{
            cout << passages[choice].sections[i].getText();
          }
        }
      }
      else if(passages[choice].sections[i].getType() == BLOCK){
        block(passages[choice].sections[i].getText());
      }
      else{
        cout << passages[choice].sections[i].getText();
      }
    }
    cout << endl;
    
    //Display the new options
    if(passages[choice].links.empty()){
      break;
    }
    cout << "Enter:\n";
    int j = 1;
    for(auto link: passages[choice].links){
      cout << j << " for " << link << endl;
      j++;
    }

    //Get the user's choice
    cin >> input;
    choice = passages[choice].links[input-1];
  }
}