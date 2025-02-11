    void variableAssignment() {
        std::unordered_set<std::string> seenTokens;
        for (int i = 0; i < toknum; i++) {
            if (seenTokens.find(tokens[i]) != seenTokens.end()) {
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