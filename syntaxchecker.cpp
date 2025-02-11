#include <iostream>
#include <cstring>
#include <cctype>
#include <unordered_set>

class SyntaxChecker {
private: 
    char src[300];
    const char *delimiters;
    char *tokens[100];
    bool state;
    int toknum;

    void removeLastLetter(char *word) {
        size_t len = std::strlen(word);
        if (len > 0) {
            word[len - 1] = '\0'; 
        }
    }
    
    void fixCommaSpacing(char* input) {
        int len = std::strlen(input);
        char* result = new char[len * 3]; // allocate more space to accommodate added spaces
        int j = 0;
        
        for (int i = 0; i < len; ++i) {
            result[j++] = input[i];
    
            // insert a space if there's no space
            if ((input[i] == '(' || input[i] == ',' || input[i] == ')' || input[i] == '=') && i + 1 < len && input[i + 1] != ' '&& input[i - 1] != ' ' ) {
                result[j++] = ' ';
            }
            if ((input[i] == '=') && i + 1 < len && input[i + 1] != ' ')  {
                result[j++] = ' ';
            }
        }
        result[j] = '\0';
        std::strcpy(input, result);
        delete[] result;
    }

    void tokenize() {
        toknum = 0;
        delimiters = " ";
        fixCommaSpacing(src);
        char *token = strtok(src, delimiters);
        while (token != NULL) {
            //std::cout << token << "\n";
            if (token[strlen(token)-1] == ';' && strlen(token) != 1) {
                removeLastLetter(token); 
                tokens[toknum] = strdup(token); // store original token
                tokens[++toknum] = strdup(";"); // store ";" separately
            }
            else if (token[strlen(token)-1] == ','  && strlen(token) != 1) {
                removeLastLetter(token); 
                tokens[toknum] = strdup(token); // store original token
                tokens[++toknum] = strdup(","); // store "," separately
            }
            else if (token[strlen(token)-1] == '(') {
                removeLastLetter(token); 
                tokens[toknum] = strdup(token); // store original token
                tokens[++toknum] = strdup("("); // store "(" separately
            }
            else if (token[strlen(token)-1] == ')') {
                removeLastLetter(token); 
                tokens[toknum] = strdup(token); // store original token
                tokens[++toknum] = strdup(")"); // store ")" separately
            }
            else if ((token[strlen(token)-1] == '=') && (((token[strlen(token)-2] >= 'A') && (token[strlen(token)-2] <= 'Z')) || ((token[strlen(token)-2] >= 'a') && (token[strlen(token)-2] <= 'z')))) {
                removeLastLetter(token); 
                tokens[toknum] = strdup(token); // store original token
                tokens[++toknum] = strdup("="); // store "=" separately
            }
            else
                tokens[toknum] = strdup(token);
            token = std::strtok(NULL, delimiters);
            toknum++;
        }
    }
    
    void variableAssignment() { //  handles variable to variable assignment
        std::unordered_set<std::string> seenTokens;
        for (int i = 1; i < toknum; i++) {
            if (tokens[i - 1] == std::string("=") && seenTokens.find(tokens[i]) != seenTokens.end()) {
                std::string modifiedToken = "'" + std::string(tokens[i]) + "'";
                tokens[i] = strdup(modifiedToken.c_str()); // allocates new memory
            } else {
                seenTokens.insert(tokens[i]);
            }
        }
    }
    
    void checkVariable() {
        int stage = 0; // [0] expect datatype, [1] expect identifier, [2] expect comma or semicolon, [3] expect number or variable assignment
        for (int i = 0; i < toknum; i++) {
            int type = getTypeVariable(tokens[i]);

            if (stage == 0) { // expecting a data type
                if (type == 1) {
                    stage = 0;
                }
                if (type != 0) {
                    //std::cout << "expected data type at token " << i + 1 << "\n";
                    std::cout << "INVALID VARIABLE DECLARATION\n";
                    state = false;
                    return;
                }
                stage = 1;
            } else if (stage == 1) { // expecting an identifier
                if (type != 2) {
                    //std::cout << "expected identifier at token " << i + 1 << "\n";
                    std::cout << "INVALID VARIABLE DECLARATION\n";
                    state = false;
                    return;
                }
                stage = 2;
            } else if (stage == 3) { // expecting a number assignment
                variableAssignment();
                if (type != 6) {
                    //std::cout << "expected number at token " << i + 1 << "\n";
                    std::cout << "INVALID VARIABLE DECLARATION\n";
                    state = false;
                    return;                   
                }
                stage = 2;
            } else if (stage == 2) { // expecting a comma or semicolon
                if (type == 3) { // comma found, expect another identifier
                    stage = 1;
                } else if (type == 4) { // semicolon found, end statement;                    
                    std::cout << "VALID VARIABLE DECLARATION\n";
                    return;
                } else if (type == 5) { // equal sign found, expect a number or variable 
                    stage = 3;
                } else {
                    //std::cout << "expected ',' or ';' at token " << i + 1 << "\n";
                    std::cout << "INVALID VARIABLE DECLARATION\n";
                    state = false;
                    return;
                }
            } 
        }
        std::cout << "INVALID VARIABLE DECLARATION\n";
    }

    void checkFunction() {
        int stage = 0; // [0] expect datatype, [1] expect identifier, [2] expect comma or semicolon, [3] expect number or variable assignment
        for (int i = 0; i < toknum; i++) {
            int type = getTypeFunction(tokens[i]);

            if (stage == 0) { // expecting a data type
                if (type == 1) {
                    stage = 0;
                }
                if (type != 0 && !(tokens[i] != nullptr && tokens[i][0] == '\0')){
                    //std::cout << "expected data type at token " << i + 1 << "\n";
                    std::cout << "INVALID FUNCTION DECLARATION\n";
                    state = false;
                    return;
                }
                stage = 1;
            } else if (stage == 1) { // expecting an identifier
                if (type != 2 && tokens[i][0] != ')' && tokens[i][0] != ',') {
                    //std::cout << "expected identifier at token " << i + 1 << "\n";
                    std::cout << "INVALID FUNCTION DECLARATION\n";
                    state = false;
                    return;
                }
                else if (tokens[i][0] == ',') { // expecting another datatype
                    stage = 0;
                }
                else {
                    stage = 2;
                }
            } else if (stage == 3) { // expecting a number assignment
                if (type != 6) {
                    //std::cout << "expected number at token " << i + 1 << "\n";
                    std::cout << "INVALID FUNCTION DECLARATION\n";
                    state = false;
                    return;                   
                }
                stage = 2;
            } else if (stage == 2) { // expecting a '(' or ')' or ','
                if (type == 3) { // '(' found, expect another data type
                    stage = 0;
                } else if (type == 4) { // semicolon found, end statement
                    std::cout << "VALID FUNCTION DECLARATION\n";
                    return;
                } else if (type == 5) { // ')' found, expect a semicolon
                    stage = 2;
                } else if (type == 7) { // ',' found, expect a data type
                    if (getTypeVariable(tokens[i+1]) == 0) {
                        stage = 0;
                    }
                    else {
                        stage = 1;
                    }
                } else {
                    //std::cout << "expected ';' at token " << i + 1 << "\n";
                    std::cout << "INVALID FUNCTION DECLARATION\n";
                    state = false;
                    return;
                }
            } 
        }
        std::cout << "INVALID FUNCTION DECLARATION\n";
    }

    bool isTokenUnique(const char* str) {
        int count = 0;
        for (int i = 0; i < toknum; i++) {
            if (strcmp(tokens[i], str) == 0) {
                if (i > 0 && (strcmp(tokens[i - 1], "=") == 0)) {
                    continue;
                }
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
    
        if (str[i] == '+' || str[i] == '-') {
            i++;
        }
        if (str[i] == '\0') return false; // string cannot be just a sign
        for (; str[i] != '\0'; i++) {
            if (std::isdigit(str[i])) {
                hasDigits = true;
            } else if (std::isalpha(str[i]) || str[i] == '\'') {
                hasLetters = true;
            } else if (str[i] == '.') {
                if (hasDecimal || hasLetters) return false; // invalid if more than one decimal or if a letter exists
                hasDecimal = true;
            } else {
                return false; // any other character is invalid
            }
        }
    
        // if there is a decimal, there must be at least one digit before or after it
        if (hasDecimal && !hasDigits) return false;
    
        // a mix of letters and a decimal is invalid (e.g., "12.3abc")
        if (hasLetters && hasDecimal) return false;
    
        return true;
    }
    
    int getTypeVariable(const char* str) {
        if (strcmp(str, "int") == 0 || strcmp(str, "char") == 0 || 
            strcmp(str, "float") == 0 || strcmp(str, "double") == 0)
            return 0;
        else if (strcmp(str, " ") == 0)
            return 1;
        else if (isIdentifier(str) && isTokenUnique(str))  // check if `str` is a valid identifier
            return 2;
        else if (strcmp(str, ",") == 0)
            return 3;
        else if (strcmp(str, ";") == 0)
            return 4;
        else if (strcmp(str, "=") == 0)
            return 5;
        else if (isNumberOrWord(str)) {
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
        else if (isIdentifier(str))  // check if 'str' is a valid identifier
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
            std::strcpy(src, input + 2); // skip the first character and the space
            tokenize();
            if (input[0] == '1') {
                checkVariable();
            } else {
                checkFunction();
            }
        }
    }
};

int main() {
    int n;
    std::cin >> n;
    std::cin.ignore(); 

    for (int i = 0; i < n; ++i) {
        char input[300];
        std::cin.getline(input, 300);
        SyntaxChecker checker(input);
    }

    return 0;
}