#include <iostream>
#include <stack>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

void printFile(istream& dictfile);
bool balanceFile(istream& dictfile);
bool isMatching(char, char);
void getChar(char& c, istream& infile, int& i);

struct charAndLineNumber
{
    int lineNumber;
    char openCharacter;
};
int main() {
    ifstream fin;
    string fileName;

    cout << "Please enter filename for C++ code: ";
    cin >> fileName;

    fin.open(fileName.c_str());

    if (!fin) {
        cout << "File not found!" << endl;
        return (0);
    }
    printFile(fin);
    if (balanceFile(fin))
        cout << "balance ok" << endl;
}

void printFile(istream& fin)
{
    int i = 1;
    string s;
    while (fin.peek() != EOF)
    {
        getline(fin, s);
        cout << i << " " << s << endl;
        i++;
    }
    fin.clear();
    fin.seekg(0, fin.beg);
}

bool balanceFile(istream& infile) {
    stack <charAndLineNumber> openingCharsStack;
    char c, d, e, f;
    bool balanced = true;
    int lineNumber = 1;
    while (infile.peek() != EOF) 
    {
        getChar(c, infile, lineNumber); // get char from the file.

        // checking for /*
        if (c == '/' && infile.peek() == '*') 
        {
            infile.get(c);
            int matchLine = lineNumber; //saves the line number of the opening char.
            getChar(c, infile, lineNumber); //get char after  /*
            while ((c != '*' || infile.peek() != '/') && infile.peek() != EOF) 
            {
                getChar(c, infile, lineNumber); //get char until reached */
            }
            if (infile.peek() == EOF) //if reached end of file, /* is not balanced
            {
                balanced = false;
                cout << "unmatched /* symbol on line " << matchLine << endl;
                break;
            }
            cout << "pair matching /* and */" << endl;
        }
        else
        {
            if (c == '/' && infile.peek() == '/')
            {
                string skipLine;
                getline(infile, skipLine); //if chars are //, skip a line.
            }
        }

        //checking for "" and ''
        if (c == '"' || c == '\'') { 
            char tempChar;
            tempChar = c;
            int tempLineNumber = lineNumber;
            getChar(c, infile, lineNumber);
            while (c != tempChar && infile.peek() != EOF) { //get char until met closing char.
                getChar(c, infile, lineNumber);
                if (c == '\n') {   //"" and '' are unbalanced with newline.
                    balanced = false;
                    cout << "unmatched " << tempChar << " symbol on line " << tempLineNumber << endl;
                    break;
                }
            }
            if (c == '\n') {
                break;
            }
            if (c != tempChar) {
                balanced = false;
                cout << "unmatched " << tempChar << " symbol on line " << tempLineNumber << endl;
                break;
            }
            cout << "pair matching " << tempChar << " and " << tempChar << endl;
        }

        //checking for ( and { and [
        if (c == '(' || c == '{' || c == '[') { 
            charAndLineNumber a;
            a.lineNumber = lineNumber; 
            a.openCharacter = c;
            openingCharsStack.push(a);         // if ( or { or [ found, push to stack a struct of line number and an opening char.
        }
        else if (c == ')' || c == '}' || c == ']')
        {
            if (openingCharsStack.empty()) // if no matching opening character in stack, it is not balanced.
            {
                cout << "unmatched " << c << " symbol on line " << lineNumber << endl;
                balanced = false;
                break;
            }
            charAndLineNumber b;
            b = openingCharsStack.top(); 
            openingCharsStack.pop(); // (checking every opening chars) pops to find whether its pair exists or not.
            if (isMatching(b.openCharacter, c)) //if found closing char, the pair exists.
            {
                cout << "pair matching " << b.openCharacter << " and " << c << endl;
            }
            else {  //if not found closing char, the pair do not exists.
                balanced = false;
                cout << "unmatched " << b.openCharacter << " symbol on line " << b.lineNumber << endl;
                break;
            }
        }
    }
    if (!openingCharsStack.empty() && balanced == true) { // if there is any opening char left in the stack at the EOF, it is unbalanced.
        cout << "unmatched " << openingCharsStack.top().openCharacter << " symbol on line " << openingCharsStack.top().lineNumber << endl;
        balanced = false;
    }
    return balanced;
}

bool isMatching(char a, char b)
{
    if (a == '{') {
        if (b == '}')
            return true;
        else
            return false;
    }
    if (a == '(') {
        if (b == ')')
            return true;
        else
            return false;
    }
    if (a == '[') {
        if (b == ']')
            return true;
        else
            return false;
    }
    return false;
}

void getChar(char& c, istream& infile, int& i)
{
    infile.get(c);
    if (c == '\n')
        i++;
}
