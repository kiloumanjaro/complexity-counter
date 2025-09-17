#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm> 
#include <unordered_set>

using namespace std;

// This function is still needed for the new function
string cleanLine(string line) {
    // Trim leading and trailing whitespace
    line.erase(0, line.find_first_not_of(' '));
    line.erase(line.find_last_not_of(' ') + 1);

    // Check if the line starts with "if" or "for"
    if (line.rfind("if", 0) == 0 || line.rfind("for", 0) == 0) {
        // If it does, ensure the next character is a colon
        size_t space_pos = line.find(' ');
        if (space_pos != string::npos) {
            string after_space = line.substr(space_pos + 1);
            if (after_space.rfind(":", 0) != 0) {
                // If it's not a colon, return an empty string
                return "";
            }
        } else {
            // No space found after 'if' or 'for', return an empty string
            return "";
        }
    }

    // Existing logic for removing semicolons and keywords
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
// The new function that handles 'if' and 'for' logic
void analyzeParentheticalStatements(const string& code, size_t i, vector<string>& results, bool& inIf, bool& inFor) {
    size_t close = code.find(')', i);
    if (close != string::npos) {
        string left = code.substr(0, i);
        string inside = code.substr(i + 1, close - i - 1);

        if (left == "if") {
            results.push_back("if:");
            results.push_back("condition: " + inside);
            inIf = true;
        } else if (left == "for") {
            results.push_back("for:");
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
    }
}
void readLine(string line, vector<string>& results, bool& inIf, bool& inFor, bool& inElse) {
    stringstream ss(line);
    string code;

    while (getline(ss, code, ',')){
        code.erase(0, code.find_first_not_of(' '));

        if (inIf) {
            results.push_back("if statements:");
            results.push_back(cleanLine(code));
            
            // Check for nested loops or ifs within the if block
            for (size_t i = 0; i < code.size(); i++) {
                if (code[i] == '(') {
                    analyzeParentheticalStatements(code, i, results, inIf, inFor);
                    break;
                }
            }

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
            
            // Check for nested loops or ifs within the for block
            for (size_t i = 0; i < code.size(); i++) {
                if (code[i] == '(') {
                    analyzeParentheticalStatements(code, i, results, inIf, inFor);
                    break;
                }
            }
            
            continue;
        }
        
        // ... (rest of your existing code remains the same)
        if (inElse) {
            if (code.find("}") != string::npos) {
                inElse = false;
                continue;
            }
            results.push_back("else statements:");
            results.push_back(cleanLine(code));
            // Check for nested loops or ifs within the else block
            for (size_t i = 0; i < code.size(); i++) {
                if (code[i] == '(') {
                    analyzeParentheticalStatements(code, i, results, inIf, inFor);
                    break;
                }
            }
            continue;
        }

        if (code.find("else") != string::npos) {
            inElse = true;
            continue;
        }

        for (size_t i = 0; i < code.size(); i++) {
            char ch = code[i];

            if (ch == '(') {
                analyzeParentheticalStatements(code, i, results, inIf, inFor);
                break;
            }
            else if (ch == '=') {
                results.push_back(cleanLine(code));
            }
            else if (ch == '+' || ch == '/' || ch == '>' | ch == '<' | ch == ':') {
                results.push_back(cleanLine(code));
            }
            else if (ch == '-') {
                if (i == 0 || code[i - 1] == '=') {
                    continue;
                }
                results.push_back(cleanLine(code));
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
    string line;
    vector<string> code;
    bool inIf = false; 
    bool inFor = false;
    bool inElse = false;

    cin >> lines;
    cin.ignore(); 
    for (int i = 0; i < lines; i++) {
        getline(cin, line);
        readLine(line, code, inIf, inFor, inElse);
    }

    vector<string> filtered = uniqueLines(code);

    if (!(filtered.empty() || filtered[0] == "for:" || filtered[0] == "if:")) {
        cout << "statements:" << endl;
    }

    for (const string& c : filtered) {
        cout << c << endl;
    }

    return 0;
}