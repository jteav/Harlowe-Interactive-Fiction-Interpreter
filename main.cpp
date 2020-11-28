//Johnathan Teav
#include "storytokenizer.h"
using namespace std;

int main(){
  cout << "|---------------------------------------------------------|" << endl;
	cout << "|                                                         |" << endl;
	cout << "|                                                         |" << endl;
	cout << "|                   INTERACTIVE FICTION                   |" << endl;
	cout << "|                                                         |" << endl;
	cout << "|                        SIMULATOR                        |" << endl;
	cout << "|                                                         |" << endl;
	cout << "|                                                         |" << endl;
	cout << "|---------------------------------------------------------|" << endl << endl;

  int input;
  cout << "Enter:\n1 for short story\n2 for long story: ";
  cin >> input;

  string fileName, line, story;
  
  if(input == 1){
    fileName = "test-example.html";
  }
  else if(input == 2){
    fileName = "if.html";
  }
  else{
    cout << "Invalid response!\n";
    return 0;
  }

  ifstream myfile (fileName);

  if(myfile.is_open()){
    getline(myfile, line);
    while (myfile && line != "</html>"){
      story += line + '\n';
      getline(myfile, line);
    }

    Story st(story);
    st.play(input);
  }
  else{
    cout << "Error opening file\n";
  }

  return 0;
}
