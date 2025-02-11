    void fixCommaSpacing(char* input) {
        int len = std::strlen(input);
        char* result = new char[len * 2]; // Allocate more space to accommodate added spaces
        int j = 0;
        
        for (int i = 0; i < len; ++i) {
            result[j++] = input[i];
            if ((input[i] == '(' || input[i] == ',' ||  input[i] == ')' || input[i] == '=')  && i + 1 < len && input[i + 1] != ' ') {
                result[j++] = ' ';
            }
        }
        result[j] = '\0'; // Null-terminate the string
        
        std::strcpy(input, result);
        delete[] result;
    }
