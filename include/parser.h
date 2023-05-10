#pragma once

#include "ast.h"
#include <string>
#include <memory>
#include <vector>

namespace logixpr {

enum class TokenType {
    VARIABLE,
    CONSTANT_TRUE,
    CONSTANT_FALSE,
    NOT,
    AND,
    OR,
    IMPLIES,
    BICONDITIONAL,
    LPAREN,
    RPAREN,
    END_OF_INPUT,
    INVALID
};

struct Token {
    TokenType type;
    std::string value;
    size_t position;
    
    Token(TokenType type, const std::string& value, size_t position)
        : type(type), value(value), position(position) {}
};

class ParseError : public std::exception {
private:
    std::string message;
    size_t position;

public:
    ParseError(const std::string& msg, size_t pos);
    const char* what() const noexcept override;
    size_t getPosition() const;
};

class Lexer {
private:
    std::string input;
    size_t position;
    size_t current_char_index;

public:
    explicit Lexer(const std::string& input);
    std::vector<Token> tokenize();

private:
    char currentChar() const;
    char peekChar() const;
    void advance();
    void skipWhitespace();
    Token readVariable();
    Token readOperator();
};

class Parser {
private:
    std::vector<Token> tokens;
    size_t current_token_index;

public:
    explicit Parser(const std::vector<Token>& tokens);
    std::unique_ptr<ASTNode> parse();

private:
    const Token& currentToken() const;
    const Token& peekToken() const;
    void advance();
    bool match(TokenType type);
    void consume(TokenType type);
    
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseBiconditional();
    std::unique_ptr<ASTNode> parseImplication();
    std::unique_ptr<ASTNode> parseDisjunction();
    std::unique_ptr<ASTNode> parseConjunction();
    std::unique_ptr<ASTNode> parseNegation();
    std::unique_ptr<ASTNode> parsePrimary();
};

class ExpressionParser {
public:
    static std::unique_ptr<ASTNode> parse(const std::string& expression);
};

}