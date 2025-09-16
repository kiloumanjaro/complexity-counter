#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm> 
#include <unordered_set>
#include <algorithm> 

using namespace std;

string cleanLine(string line) {
    line.erase(remove(line.begin(), line.end(), ';'), line.end());
    static const unordered_set<string> keywords = {
        "int", "string", "float", "double", "char", "bool",
        "long", "short", "void", "using", "namespace", "std"
    };

    stringstream ss(line);
    string word;
    string result;

    while (ss >> word) {
        if (keywords.find(word) != keywords.end()) {
            continue;
        }
        if (!result.empty()) result += " ";
        result += word;
    }
    return result;
}


void readLine(string line, vector<string>& results, bool& inIf, bool& inFor, bool& inElse) {
    stringstream ss(line);
    string code;

    while (getline(ss, code, ',')){
        code.erase(0, code.find_first_not_of(' '));

        if (inIf) {
            results.push_back("if statements:");
            results.push_back(cleanLine(code));
            inIf = false;
            continue;
        }

        if (inFor) {
            if (code.find("}") != string::npos) {
                results.push_back("statements:");
                inFor = false;
                continue;
            }
            results.push_back("for statements:");
            results.push_back(cleanLine(code));
            continue;
        }
        if (inElse) {
            if (code.find("}") != string::npos) {
                inElse = false;
                continue;
            }
            results.push_back("else statements:");
            results.push_back(cleanLine(code));
            continue;
        }
        
        if (code.find("else") != string::npos) {
            inElse = true;
            continue;
        }

        for (size_t i = 0; i < code.size(); i++) {
            char ch = code[i];

            if (ch == '=') {
                results.push_back(cleanLine(code));
            }
            else if (ch == '+' || ch == '/' || ch == '>' | ch == '<' | ch == ':' ) {
                results.push_back(cleanLine(code));
            }
            if (ch == '(') {
                size_t close = code.find(')', i);
                if (close != string::npos) {
                    string left = code.substr(0, i);  
                    string inside = code.substr(i + 1, close - i - 1); 

                    if (left == "if") {
                        results.push_back("if:");
                        results.push_back("condition: " + inside);
                        inIf = true; 
                    }
                    else if (left == "for") {
                        results.push_back("for:");

                        // Split initializer; condition; update
                        stringstream condSS(inside);
                        string part;
                        vector<string> parts;
                        while (getline(condSS, part, ';')) {
                            part.erase(0, part.find_first_not_of(' '));
                            part.erase(remove(part.begin(), part.end(), ';'), part.end());
                            if (!part.empty()) parts.push_back(part);
                        }
                        if (parts.size() >= 1) results.push_back("initializer: " + cleanLine(parts[0]));
                        if (parts.size() >= 2) results.push_back("condition: " + cleanLine(parts[1]));
                        if (parts.size() >= 3) results.push_back("update: " + cleanLine(parts[2]));

                        inFor = true;
                    }
                    break;
                }
            }
            else if (ch == '-') {
                // check if this '-' is a unary sign
                if (i == 0 || code[i-1] == '=') {
                    continue; 
                }
                results.push_back(cleanLine(code)); // count as subtraction
            }
        }
    }
}

vector<string> uniqueLines(const vector<string>& input) {
    vector<string> output;
    unordered_set<string> seen;

    for (const auto& line : input) {
        if (seen.find(line) == seen.end()) {
            output.push_back(line);
            seen.insert(line);
        }
    }
    return output;
}

int main(){
    int lines;
    int i;
    int tc;
    string line;
    vector<string> code;
    bool inIf = false; 
    bool inFor = false;
    bool inElse = false;


    cin >> lines;
    cin.ignore(); 
    for (i=0 ; i<lines; i++) {
        getline(cin, line);
        readLine(line, code, inIf, inFor, inElse);
    }

    vector<string> filtered = uniqueLines(code);

    if(!(filtered[0] == "for:" || filtered[0] == "if:")){
            cout << "statements:" << endl;
    }


    for (const string& c : filtered) {
        cout << c << endl;
    }

}