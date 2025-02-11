#include <iostream>
#include <cstring>
#include <cctype>
#include <unordered_set>

class SyntaxChecker {
private: 
    char src[100];
    const char *delimiters;
    char *tokens[40];
    bool state;
    int toknum;

    void removeLastLetter(char *word) {
        size_t len = std::strlen(word);
        if (len > 0) {
            word[len - 1] = '\0'; // Remove last character
        }
    }
    
    void fixCommaSpacing(char* input) {
        int len = std::strlen(input);
        char* result = new char[len * 3]; // Allocate more space to accommodate added spaces
        int j = 0;
        
        for (int i = 0; i < len; ++i) {
            // Insert a space before if it's not the first character and there's no space before
            if (i > 0 && (input[i] == '(' || input[i] == ',' || input[i] == ')' || input[i] == '=') && input[i - 1] != ' ') {
                result[j++] = ' ';
            }
    
            result[j++] = input[i];
    
            // Insert a space after if there's no space after
            if ((input[i] == '(' || input[i] == ',' || input[i] == ')' || input[i] == '=') && i + 1 < len && input[i + 1] != ' ') {
                result[j++] = ' ';
            }
        }
    
        result[j] = '\0'; // Null-terminate the string
    
        std::strcpy(input, result);
        delete[] result;
    }


    void tokenize() {
        toknum = 0;
        delimiters = " ";
        fixCommaSpacing(src);
        char *token = strtok(src, delimiters);
        while (token != NULL) {
            std::cout << token << "\n";
            if (token[strlen(token)-1] == ';' && strlen(token) != 1) {
                removeLastLetter(token); 
                tokens[toknum] = strdup(token); // Store original token
                tokens[++toknum] = strdup(";"); // Store ";" separately
            }
            else if (token[strlen(token)-1] == ','  && strlen(token) != 1) {
                removeLastLetter(token); 
                tokens[toknum] = strdup(token); // Store original token
                tokens[++toknum] = strdup(","); // Store "," separately
            }
            else if (token[strlen(token)-1] == '(') {
                removeLastLetter(token); 
                tokens[toknum] = strdup(token); // Store original token
                tokens[++toknum] = strdup("("); // Store "(" separately
            }
            else if (token[strlen(token)-1] == ')') {
                removeLastLetter(token); 
                tokens[toknum] = strdup(token); // Store original token
                tokens[++toknum] = strdup(")"); // Store ")" separately
            }
            else
                tokens[toknum] = strdup(token);
            token = std::strtok(NULL, delimiters);
            toknum++;
        }
    }
    
    void variableAssignment() { //  handles variable to variable assignment
        std::unordered_set<std::string> seenTokens;
        for (int i = 1; i < toknum; i++) { // Start from 1 to check previous token
            if (tokens[i - 1] == std::string("=") && seenTokens.find(tokens[i]) != seenTokens.end()) {
                // Allocate new memory for modified token
                std::string modifiedToken = "'" + std::string(tokens[i]) + "'";
                tokens[i] = strdup(modifiedToken.c_str()); // strdup allocates new memory
            } else {
                seenTokens.insert(tokens[i]);
            }
        }
    }
    
    void checkVariable() {
        int stage = 0; // 0 - expect type, 1 - expect identifier, 2 - expect comma or semicolon
        for (int i = 0; i < toknum; i++) {
            int type = getTypeVariable(tokens[i]);

            if (stage == 0) { // Expecting a data type
                if (type == 1) {
                    stage = 0;
                }
                if (type != 0) {
                    std::cout << "Syntax Error: Expected data type at token " << i + 1 << "\n";
                    std::cout << "INVALID VARIABLE DECLARATION\n";
                    state = false;
                    return;
                }
                std::cout << "Next Up is Stage 1 (Found Data Type)\n";
                stage = 1;
            } else if (stage == 1) { // Expecting an identifier
                if (type != 2) {
                    std::cout << "Syntax Error: Expected identifier at token " << i + 1 << "\n";
                    std::cout << "INVALID VARIABLE DECLARATION\n";
                    state = false;
                    return;
                }
                std::cout << "Next Up is Stage 2 (Found Identifier)\n";
                stage = 2;
            } else if (stage == 3) {
                variableAssignment();
                if (type != 6) {
                    std::cout << type << std::endl;
                    std::cout << "Syntax Error: Expected number at token " << i + 1 << "\n";
                    std::cout << "INVALID VARIABLE DECLARATION\n";
                    state = false;
                    return;                   
                }
                std::cout << "Next Up is Stage 2 (Found Number)\n";
                stage = 2;
            } else if (stage == 2) { // Expecting a comma or semicolon
                if (type == 3) { // Comma found, expect another identifier
                    stage = 1;
                    std::cout << "Goes Back to Stage 1 (Found Comma)\n";;
                } else if (type == 4) { // Semicolon found, end statement
                    std::cout << "Passed Final Stage (Found Semicolon)\n";                    
                    std::cout << "VALID VARIABLE DECLARATION\n";
                    return;
                } else if (type == 5) { // equal sign found, expect number 
                    stage = 3;
                    std::cout << "Next Up is Stage 3 (Found Eqaul Sign)\n";
                } else {
                    std::cout << "Syntax Error: Expected ',' or ';' at token " << i + 1 << "\n";
                    std::cout << "INVALID VARIABLE DECLARATION\n";
                    state = false;
                    return;
                }
            } 

        }
        
        std::cout << "Syntax Error: Incomplete statement\n";
    }

    void checkFunction() {
        int stage = 0; // 0 - expect type, 1 - expect identifier, 2 - expect comma or semicolon
        for (int i = 0; i < toknum; i++) {
            int type = getTypeFunction(tokens[i]);

            if (stage == 0) { // Expecting a data type
                if (type == 1) {
                    stage = 0;
                }
                if (type != 0 && !(tokens[i] != nullptr && tokens[i][0] == '\0')){
                    std::cout << "Syntax Error: Expected data type at token " << i + 1 << "\n";
                    std::cout << "INVALID FUNCTION DECLARATION\n";
                    state = false;
                    return;
                }
                std::cout << "Next Up is Stage 1 (Found Data Type)\n";
                stage = 1;

                
            } else if (stage == 1) { // Expecting an identifier
                if (type != 2 && tokens[i][0] != ')' && tokens[i][0] != ',') {
                    std::cout << "Syntax Error: Expected identifier at token " << i + 1 << "\n";
                    std::cout << "INVALID FUNCTION DECLARATION\n";
                    state = false;
                    return;
                }
                else if (tokens[i][0] == ',') {
                    std::cout << "FIND ANOTHER DATA TYPE\n";
                    stage = 0;
                }
                else {
                    std::cout << "Next Up is Stage 2 (Found Identifier)\n";
                    stage = 2;
                }

            } else if (stage == 3) {
                if (type != 6) {
                    std::cout << "Syntax Error: Expected number at token " << i + 1 << "\n";
                    std::cout << "INVALID FUNCTION DECLARATION\n";
                    state = false;
                    return;                   
                }
                std::cout << "Next Up is Stage 2 (Found Number)\n";
                stage = 2;
            } else if (stage == 2) { // Expecting a '(' or ')' or ','
                if (type == 3) { // '(' found, expect another data type
                    stage = 0;
                    std::cout << "Goes Back to Stage 0 (Found '('))\n";;
                } else if (type == 4) { // Semicolon found, end statement
                    std::cout << "Passed Final Stage (Found Semicolon)\n";                    
                    std::cout << "VALID FUNCTION DECLARATION\n";
                    return;
                } else if (type == 5) { // ')' found, expect a semicolon
                    stage = 2;
                    std::cout << "Next Up is Stage 2 (Found ')')\n";
                } else if (type == 7) { // ',' found, expect a data type
                    stage = 0;
                    std::cout << "Next Up is Stage 0 (Found ',')\n";
                } else {
                    std::cout << "Syntax Error: Expected ';' at token " << i + 1 << "\n";
                    std::cout << "INVALID FUNCTION DECLARATION\n";
                    state = false;
                    return;
                }
            } 

        }
        
        std::cout << "Syntax Error: Incomplete statement\n";
    }
    

    bool isTokenUnique(const char* str) { // handles variable redeclaration 
        int count = 0;
        for (int i = 0; i < toknum; i++) {
            if (strcmp(tokens[i], str) == 0) {
                count++;
                if (count > 1) {
                    return false;
                }
            }
        }
        return true;
    }
    

    bool isIdentifier(const char* str) {
        if (!str) return false;
    
        if (!(str[0] >= 'A' && str[0] <= 'Z') && !(str[0] >= 'a' && str[0] <= 'z') && str[0] != '_') 
            return false;
    
        for (int i = 1; str[i] != '\0'; i++) {
            if (!(str[i] == '_' || (str[i] >= 'A' && str[i] <= 'Z') || 
                  (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= '0' && str[i] <= '9'))) {
                return false;
            }
        }
        
        return true;
    }

    bool isNumberOrWord(const char* str) {
        if (!str || *str == '\0') return false;
    
        int i = 0;
        bool hasDecimal = false;
        bool hasDigits = false;
        bool hasLetters = false;
    
        // Check for optional sign
        if (str[i] == '+' || str[i] == '-') {
            i++;
        }
    
        if (str[i] == '\0') return false; // String cannot be just a sign
    
        for (; str[i] != '\0'; i++) {
            if (std::isdigit(str[i])) {
                hasDigits = true;
            } else if (std::isalpha(str[i]) || str[i] == '\'') {
                hasLetters = true;
            } else if (str[i] == '.') {
                if (hasDecimal || hasLetters) return false; // Invalid if more than one decimal or if a letter exists
                hasDecimal = true;
            } else {
                return false; // Any other character is invalid
            }
        }
    
        // If there is a decimal, there must be at least one digit before or after it
        if (hasDecimal && !hasDigits) return false;
    
        // A mix of letters and a decimal is invalid (e.g., "12.3abc")
        if (hasLetters && hasDecimal) return false;
    
        return true;
    }
    
    int getTypeVariable(const char* str) {
        if (strcmp(str, "int") == 0 || strcmp(str, "char") == 0 || 
            strcmp(str, "float") == 0 || strcmp(str, "double") == 0)
            return 0;
        else if (strcmp(str, " ") == 0)
            return 1;
        else if (isIdentifier(str) && isTokenUnique(str))  // Check if `str` is a valid identifier
            return 2;
        else if (strcmp(str, ",") == 0)
            return 3;
        else if (strcmp(str, ";") == 0)
            return 4;
        else if (strcmp(str, "=") == 0)
            return 5;
        else if (isNumberOrWord(str)) {
            std::cout << "yurr" << std::endl;
            return 6;     
        }
        else 
            return 7;
    }


    int getTypeFunction(const char* str) {
        if (strcmp(str, "int") == 0 || strcmp(str, "char") == 0 || 
            strcmp(str, "float") == 0 || strcmp(str, "double") == 0 || strcmp(str, "void") == 0)
            return 0;
        else if (strcmp(str, " ") == 0)
            return 1;
        else if (isIdentifier(str))  // Check if `str` is a valid identifier
            return 2;
        else if (strcmp(str, "(") == 0)
            return 3;
        else if (strcmp(str, ";") == 0)
            return 4;
        else if (strcmp(str, ")") == 0)
            return 5;
        else if (isNumberOrWord(str)) 
            return 6;     
        else if (strcmp(str, ",") == 0)
            return 7;          
        else
            return 8;
    }

    
public:
    SyntaxChecker(const char *input) {
        if (input[0] == '1' || input[0] == '2') {
            std::strcpy(src, input + 2); // Skip the first character and the space
            tokenize();
            if (input[0] == '1') {
                checkVariable();
            } else {
                checkFunction();
            }
        }
    }

    void printTokens() const {
        for (int i = 0; i < toknum; i++) {
            std::cout << i + 1 << ":" << "[" << tokens[i] << "]" << std::endl;
        }
    } 


};

int main() {
    char input[100];

    std::cout << "Enter a statement to check syntax: ";
    std::cin.getline(input, 100);

    SyntaxChecker checker(input);
    checker.printTokens();

    return 0;
}