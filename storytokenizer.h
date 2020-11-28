//This file was provided by the instructor and modified by Johnathan Teav
#ifndef __STORYTOKENIZER_H
#define __STORYTOKENIZER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
using namespace std;

enum section_t {LINK, GOTO, SET, IF, ELSEIF, ELSE, BLOCK, TEXT};

const string PASSAGE_START_TAG = "<tw-passagedata";
const string PASSAGE_START_TAG_CLOSE = ">";
const string PASSAGE_END_TAG   = "</tw-passagedata>";
const string LINK_START = "[[";
const string LINK_SEPARATOR = "-&gt;";
const string LINK_END   = "]]";
const string GOTO_START = "(go-to:";
const string SET_START = "(set:";
const string IF_START = "(if:";
const string ELSEIF_START = "(else-if:";
const string ELSE_START = "(else:";
const string COMMAND_END = ")";
const string BLOCK_START = "[";
const string BLOCK_END   = "]";

const string PASSAGE_NAME_START = "name=\"";
const string PASSAGE_NAME_END = "\"";
const string GOTO_NAME_START = "&quot;";
const string GOTO_NAME_END = "&quot;";

//SectionToken contains information on the section.
class SectionToken{
private:
  string text;
  section_t type;
public:
  SectionToken() : type(TEXT) {}
  SectionToken(string str, section_t t) : text(str), type(t) {}
  section_t getType() const {return type;}
  const string& getText() const {return text;}
};

//PassageTokenizer breaks up the passages into SectionTokens.
class PassageTokenizer{
private:
  string pass;
  unsigned int pos;
  section_t lastsect;
public:
  PassageTokenizer() : pos(0) {}
  PassageTokenizer(string str) : pass(str), pos(0), lastsect(TEXT) {}
  bool hasNextSection() const;
  SectionToken nextSection();
};

//PassageToken contains information on each passage.
class PassageToken{
private:
  string pass;
  string name;
  PassageTokenizer *pt;
public:
  PassageToken() {addSections();}
  PassageToken(const string& str) : pass(str) {addSections();}
  PassageToken(const string& n, const string& p) : name(n), pass(p) {addSections();}
  const string& getName() const {return name;}
  const string& getText() const {return pass;}
  void addSections();
  void addLinks(SectionToken stok);
  
  vector<SectionToken> sections;
  vector<string> links;
};

//StoryTokenizer breaks up the story into PassageTokens.
class StoryTokenizer{
private:
  string story;
  unsigned int pos;
public:
  StoryTokenizer() : pos(0) {}
  StoryTokenizer(const string& s) : story(s), pos(0) {}
  bool hasNextPassage() const;
  PassageToken nextPassage();
};

//The main class that starts everything.
class Story{
private:
  string story;
  unordered_map<string, PassageToken> passages;
  unordered_map<string, bool> variables;
public:
  Story(string text) {story = text;}
  void setVariable(SectionToken stok);
  bool ifSection(SectionToken stok);
  void block(string stok);
  void setUp();
  void play(int in);
};

#endif