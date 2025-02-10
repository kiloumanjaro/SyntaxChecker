#include <iostream>
#include <cstring>
#include <cctype>

class SyntaxChecker {
private: 
    char src[100];
    const char *delimiters;
    char *tokens[10];
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
        char* result = new char[len * 2]; // Allocate more space to accommodate added spaces
        int j = 0;
        
        for (int i = 0; i < len; ++i) {
            result[j++] = input[i];
            if (input[i] == ',' && i + 1 < len && input[i + 1] != ' ') {
                result[j++] = ' ';
            }
        }
        result[j] = '\0'; // Null-terminate the string
        
        std::strcpy(input, result);
        delete[] result;
    }

    void tokenize() {
        delimiters = " ";
        toknum = 0;
        fixCommaSpacing(src);
        char *token = strtok(src, delimiters);
        while (token != NULL) {
            if (token[strlen(token)-1] == ';') {
                std::cout << "True \n";
                removeLastLetter(token); 
                tokens[toknum] = strdup(token); // Store original token
                tokens[++toknum] = strdup(";"); // Store ";" separately
            }
            else if (token[strlen(token)-1] == ',') {
                std::cout << "True \n";
                removeLastLetter(token); 
                tokens[toknum] = strdup(token); // Store original token
                tokens[++toknum] = strdup(","); // Store ";" separately
            }
            else
                tokens[toknum] = strdup(token);

            std::cout << token << "\n";
            token = std::strtok(NULL, delimiters);
            toknum++;
        }
    }
    
    void check() {
        int stage = 0; // 0 - expect type, 1 - expect identifier, 2 - expect comma or semicolon
        for (int i = 0; i < toknum; i++) {
            int type = getType(tokens[i]);
            std::cout << type << "\n";

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
                stage = 1;
            } else if (stage == 1) { // Expecting an identifier
                if (type != 2) {
                    std::cout << "Syntax Error: Expected identifier at token " << i + 1 << "\n";
                    std::cout << "INVALID VARIABLE DECLARATION\n";
                    std::cout << "Pass Stage 0\n";
                    state = false;
                    return;
                }
                std::cout << "Pass Stage 1\n";
                stage = 2;
            } else if (stage == 3) {
                if (type != 6) {
                    std::cout << "Syntax Error: Expected number at token " << i + 1 << "\n";
                    std::cout << "INVALID VARIABLE DECLARATION\n";
                    std::cout << "Pass Stage 0\n";
                    state = false;
                    return;                   
                }
                std::cout << "found number\n";
                stage = 2;
            } else if (stage == 2) { // Expecting a comma or semicolon
                std::cout << "Pass Stage 1\n";
                if (type == 3) { // Comma found, expect another identifier
                    stage = 1;
                    std::cout << "balik Stage 1\n";
                } else if (type == 4) { // Semicolon found, end statement
                    std::cout << "VALID VARIABLE DECLARATION\n";
                    std::cout << "Pass Stage 2\n";
                    return;
                } else if (type == 5) { // equal sign found, expect number 
                    stage = 3;
                    std::cout << "balik Stage 3\n";
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

    bool isNumber(const char* str) {
        if (!str || *str == '\0') return false;
    
        int i = 0;
        bool hasDecimal = false;
    
        // Check for optional sign
        if (str[i] == '+' || str[i] == '-') {
            i++;
        }
    
        bool hasDigits = false;
    
        for (; str[i] != '\0'; i++) {
            if (str[i] >= '0' && str[i] <= '9') {
                hasDigits = true;
            } else if (str[i] == '.') {
                if (hasDecimal) return false; // More than one decimal point is invalid
                hasDecimal = true;
            } else {
                return false; // Any other character is invalid
            }
        }
    
        return hasDigits; // Must have at least one digit
    }


    int getType(const char* str) {
        if (strcmp(str, "int") == 0 || strcmp(str, "char") == 0 || 
            strcmp(str, "float") == 0 || strcmp(str, "double") == 0)
            return 0;
        else if (strcmp(str, " ") == 0)
            return 1;
        else if (isIdentifier(str))  // Check if `str` is a valid identifier
            return 2;
        else if (strcmp(str, ",") == 0)
            return 3;
        else if (strcmp(str, ";") == 0)
            return 4;
        else if (strcmp(str, "=") == 0)
            return 5;
        else if (isNumber(str)) 
            return 6;          
        else
            return 7;
    }


    
public:
    SyntaxChecker(const char *input) {
        std::strcpy(src, input);
        tokenize();
        check(); 
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