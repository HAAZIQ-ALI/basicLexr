/*
Hey there! 👋 This is a program that reads code and breaks it down into smaller pieces.
Think of it like reading a sentence and identifying each word and punctuation mark!

For example, if you have: "let x = 42 + 5"
This program will break it down into:
- "let" (a special command word)
- "x" (a variable name)
- "=" (equals sign)
- "42" (a number)
- "+" (plus sign)
- "5" (another number)

This process is called "lexical analysis" or "tokenization", and the program that does
this is called a "lexer". It's the first step in making a programming language!
*/

#include <iostream>   // This lets us print things to the screen
#include <string>     // This lets us work with text
#include <vector>     // This lets us make lists of things

// This is like making a list of all the different types of "things" we can find in our code
// Just like how in English we have nouns, verbs, and adjectives!
enum class TokenType {
    NUMBER,         // Like: 42, 100, 1234
    IDENTIFIER,     // Variable names like: x, y, myVariable
    EQUALS,         // The = sign
    PLUS,          // The + sign
    MINUS,         // The - sign
    STAR,          // The * sign (for multiplication)
    SLASH,         // The / sign (for division)
    OPEN_PAREN,    // The ( symbol
    CLOSE_PAREN,   // The ) symbol
    LET,           // The word "let" (for creating variables)
    ILLEGAL,       // Something we don't understand
    END_OF_FILE    // Marks the end of our code
};

// This is like a container that holds two things:
// 1. What TYPE of thing we found (from the list above)
// 2. The actual TEXT we found
// For example: type=NUMBER, literal="42"
struct Token {
    TokenType type;
    std::string literal;
    
    // This is a constructor - it helps us create new tokens easily
    Token(TokenType t, std::string l) : type(t), literal(l) {}
};

// This is our main "lexer" class - it does all the work of reading the code
// and breaking it into tokens
class Lexer {
private:
    std::string input;      // The code we're reading
    size_t position;        // Where we are right now in the code
    size_t readPosition;    // Where we're going to read next
    char ch;               // The current character we're looking at

    // This function helps us move forward one character in our code
    // It's like moving your finger along while reading text!
    void readChar() {
        if (readPosition >= input.length()) {
            ch = 0;  // If we reach the end, we use 0 to mark it
        } else {
            ch = input[readPosition];
        }
        position = readPosition;
        readPosition++;
    }

    // This function skips over spaces, tabs, and new lines
    // Just like how we ignore extra spaces when reading!
    void skipWhitespace() {
        while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
            readChar();
        }
    }

    // This function reads a whole number
    // If it sees "123", it won't stop at "1", but reads all three digits
    std::string readNumber() {
        size_t startPos = position;
        while (isdigit(ch)) {  // isdigit checks if something is a number (0-9)
            readChar();
        }
        return input.substr(startPos, position - startPos);
    }

    // This function reads words (like variable names or special keywords)
    // It keeps going as long as it sees letters or underscores
    std::string readIdentifier() {
        size_t startPos = position;
        while (isalpha(ch) || ch == '_') {  // isalpha checks if something is a letter (a-z, A-Z)
            readChar();
        }
        return input.substr(startPos, position - startPos);
    }

public:
    // This is called when we create a new lexer
    // It sets everything up and gets ready to start reading
    Lexer(std::string input) : input(input), position(0), readPosition(0) {
        readChar();  // Read the first character right away
    }

    // This is the main function that figures out what each piece of code is
    // It's like identifying whether something is a word, number, or symbol
    Token nextToken() {
        skipWhitespace();  // First, skip any spaces

        // Create a token - assume it's illegal until we know what it is
        Token tok(TokenType::ILLEGAL, std::string(1, ch));

        // This is like a big "if" statement that checks what character we're looking at
        switch (ch) {
            case '=':
                tok = Token(TokenType::EQUALS, "=");
                break;
            case '+':
                tok = Token(TokenType::PLUS, "+");
                break;
            case '-':
                tok = Token(TokenType::MINUS, "-");
                break;
            case '*':
                tok = Token(TokenType::STAR, "*");
                break;
            case '/':
                tok = Token(TokenType::SLASH, "/");
                break;
            case '(':
                tok = Token(TokenType::OPEN_PAREN, "(");
                break;
            case ')':
                tok = Token(TokenType::CLOSE_PAREN, ")");
                break;
            case 0:
                tok = Token(TokenType::END_OF_FILE, "");
                break;
            default:
                // If it's not a special character, check if it's a number or word
                if (isdigit(ch)) {
                    std::string num = readNumber();
                    return Token(TokenType::NUMBER, num);
                } else if (isalpha(ch) || ch == '_') {
                    std::string ident = readIdentifier();
                    // Check if the word is "let" - it's special!
                    if (ident == "let") {
                        return Token(TokenType::LET, ident);
                    }
                    return Token(TokenType::IDENTIFIER, ident);
                }
        }

        readChar();  // Move to the next character
        return tok;
    }
};

// This function helps us convert token types into text we can read
// It's just for printing and debugging
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::EQUALS: return "EQUALS";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::STAR: return "STAR";
        case TokenType::SLASH: return "SLASH";
        case TokenType::OPEN_PAREN: return "OPEN_PAREN";
        case TokenType::CLOSE_PAREN: return "CLOSE_PAREN";
        case TokenType::LET: return "LET";
        case TokenType::ILLEGAL: return "ILLEGAL";
        case TokenType::END_OF_FILE: return "EOF";
        default: return "UNKNOWN";
    }
}

// This is where our program starts!
int main() {
    // Let's try our lexer with a simple piece of code
    std::string input = "let x = 42 + (15 - 3)";
    Lexer lexer(input);

    // Keep getting tokens until we reach the end
    Token tok = lexer.nextToken();
    while (tok.type != TokenType::END_OF_FILE) {
        // Print each token we find
        std::cout << "Type: " << tokenTypeToString(tok.type) 
                  << ", Literal: '" << tok.literal << "'" << std::endl;
        tok = lexer.nextToken();
    }

    return 0;
}

/*
When you run this program with the input "let x = 42 + (15 - 3)", it will:
1. Find "let" and recognize it as a special keyword
2. Find "x" and recognize it as a variable name
3. Find "=" and recognize it as an equals sign
4. Find "42" and recognize it as a number
5. Find "+" and recognize it as a plus sign
6. Find "(" and recognize it as an opening parenthesis
7. Find "15" and recognize it as a number
8. Find "-" and recognize it as a minus sign
9. Find "3" and recognize it as a number
10. Find ")" and recognize it as a closing parenthesis

This is exactly what a computer needs to do first when trying to understand
and run your code! Cool, right? 😎
*/