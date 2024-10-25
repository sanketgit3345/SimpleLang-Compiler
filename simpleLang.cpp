#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <string>

using namespace std;

#define MAX_TOKEN_LEN 100

// Define token types
enum TokenType {
    TOKEN_INT, TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_ASSIGN,
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_IF, TOKEN_EQUAL, TOKEN_LBRACE, TOKEN_RBRACE,
    TOKEN_SEMICOLON, TOKEN_EOF, TOKEN_UNKNOWN
};

// Token structure
struct Token {
    TokenType type;
    char text[MAX_TOKEN_LEN];
};

// Lexer function to get the next token from the input file
void getNextToken(ifstream &file, Token &token) {
    char c;
    while (file.get(c)) {
        if (isspace(c)) continue;

        if (isalpha(c)) {
            int len = 0;
            token.text[len++] = c;
            while (file.get(c) && isalnum(c)) {
                if (len < MAX_TOKEN_LEN - 1) token.text[len++] = c;
            }
            file.unget();
            token.text[len] = '\0';

            if (strcmp(token.text, "int") == 0) token.type = TOKEN_INT;
            else if (strcmp(token.text, "if") == 0) token.type = TOKEN_IF;
            else token.type = TOKEN_IDENTIFIER;
            return;
        }

        if (isdigit(c)) {
            int len = 0;
            token.text[len++] = c;
            while (file.get(c) && isdigit(c)) {
                if (len < MAX_TOKEN_LEN - 1) token.text[len++] = c;
            }
            file.unget();
            token.text[len] = '\0';
            token.type = TOKEN_NUMBER;
            return;
        }

        switch (c) {
            case '=': token.type = TOKEN_ASSIGN; token.text[0] = '='; token.text[1] = '\0'; return;
            case '+': token.type = TOKEN_PLUS; token.text[0] = '+'; token.text[1] = '\0'; return;
            case '-': token.type = TOKEN_MINUS; token.text[0] = '-'; token.text[1] = '\0'; return;
            case '{': token.type = TOKEN_LBRACE; token.text[0] = '{'; token.text[1] = '\0'; return;
            case '}': token.type = TOKEN_RBRACE; token.text[0] = '}'; token.text[1] = '\0'; return;
            case ';': token.type = TOKEN_SEMICOLON; token.text[0] = ';'; token.text[1] = '\0'; return;
        }
    }
    token.type = TOKEN_EOF;
    token.text[0] = '\0';
}

// Abstract Syntax Tree (AST) Node structure
struct Node {
    string value;
    Node* left;
    Node* right;
};

// Create a new AST node
Node* createNode(const string& value) {
    Node* newNode = new Node;
    newNode->value = value;
    newNode->left = newNode->right = nullptr;
    return newNode;
}

// Sample parsing function for expressions (e.g., a = b + c)
Node* parseExpression() {
    Node* node = createNode("=");
    node->left = createNode("a");
    node->right = createNode("+");
    node->right->left = createNode("b");
    node->right->right = createNode("c");
    return node;
}

// Print Abstract Syntax Tree (AST)
void printAST(Node* node) {
    if (node == nullptr) return;
    printAST(node->left);
    cout << node->value << " ";
    printAST(node->right);
}

// Generate assembly from AST
void generateAssembly(Node* node) {
    if (node == nullptr) return;

    if (node->value == "=") {
        generateAssembly(node->right); // Generate code for the right side first
        cout << "STORE " << node->left->value << endl;
    } else if (node->value == "+") {
        generateAssembly(node->left);
        generateAssembly(node->right);
        cout << "ADD" << endl;
    } else {
        cout << "LOAD " << node->value << endl;
    }
}

// Main function
int main() {
    ifstream file("input.txt");
    if (!file.is_open()) {
        cerr << "Failed to open file" << endl;
        return 1;
    }

    cout << "Tokenizing..." << endl;
    Token token;
    do {
        getNextToken(file, token);
        cout << "Token: " << token.type << ", Text: " << token.text << endl;
    } while (token.type != TOKEN_EOF);

    file.close();

    // Simulate parsing an expression
    cout << "\nParsing and Generating AST..." << endl;
    Node* ast = parseExpression();
    
    cout << "\nAbstract Syntax Tree (AST): ";
    printAST(ast);

    // Generate assembly code
    cout << "\n\nGenerated Assembly:\n";
    generateAssembly(ast);

    return 0;
}
