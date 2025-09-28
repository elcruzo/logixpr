#include "parser.h"
#include <stdexcept>
#include <cctype>
#include <unordered_map>

namespace logixpr {

ParseError::ParseError(const std::string& msg, size_t pos) : message(msg), position(pos) {}

const char* ParseError::what() const noexcept {
    return message.c_str();
}

size_t ParseError::getPosition() const {
    return position;
}

Lexer::Lexer(const std::string& input) : input(input), position(0), current_char_index(0) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (position < input.length()) {
        skipWhitespace();
        if (position >= input.length()) break;
        
        char ch = currentChar();
        
        if (ch == 'T' || ch == 'F') {
            if (ch == 'T') {
                tokens.emplace_back(TokenType::CONSTANT_TRUE, "T", position);
            } else {
                tokens.emplace_back(TokenType::CONSTANT_FALSE, "F", position);
            }
            advance();
        } else if (std::isalpha(ch) || ch == '_') {
            tokens.push_back(readVariable());
        } else if (ch == '(') {
            tokens.emplace_back(TokenType::LPAREN, "(", position);
            advance();
        } else if (ch == ')') {
            tokens.emplace_back(TokenType::RPAREN, ")", position);
            advance();
        } else {
            tokens.push_back(readOperator());
        }
    }
    
    tokens.emplace_back(TokenType::END_OF_INPUT, "", position);
    return tokens;
}

char Lexer::currentChar() const {
    return position < input.length() ? input[position] : '\0';
}

char Lexer::peekChar() const {
    return (position + 1) < input.length() ? input[position + 1] : '\0';
}

void Lexer::advance() {
    if (position < input.length()) {
        position++;
        current_char_index++;
    }
}

void Lexer::skipWhitespace() {
    while (position < input.length() && std::isspace(currentChar())) {
        advance();
    }
}

Token Lexer::readVariable() {
    size_t start_pos = position;
    std::string var_name;
    
    while (position < input.length() && (std::isalnum(currentChar()) || currentChar() == '_')) {
        var_name += currentChar();
        advance();
    }
    
    return Token(TokenType::VARIABLE, var_name, start_pos);
}

Token Lexer::readOperator() {
    size_t start_pos = position;
    char ch = currentChar();
    
    static const std::unordered_map<std::string, TokenType> operators = {
        {"!", TokenType::NOT},
        {"~", TokenType::NOT},
        {"¬", TokenType::NOT},
        {"&", TokenType::AND},
        {"&&", TokenType::AND},
        {"∧", TokenType::AND},
        {"|", TokenType::OR},
        {"||", TokenType::OR},
        {"∨", TokenType::OR},
        {"->", TokenType::IMPLIES},
        {"→", TokenType::IMPLIES},
        {"<->", TokenType::BICONDITIONAL},
        {"↔", TokenType::BICONDITIONAL}
    };
    
    std::string op_str;
    op_str += ch;
    advance();
    
    if (ch == '-' && currentChar() == '>') {
        op_str += currentChar();
        advance();
    } else if (ch == '<' && currentChar() == '-' && peekChar() == '>') {
        op_str += currentChar();
        advance();
        op_str += currentChar();
        advance();
    } else if ((ch == '&' || ch == '|') && currentChar() == ch) {
        op_str += currentChar();
        advance();
    }
    
    auto it = operators.find(op_str);
    if (it != operators.end()) {
        return Token(it->second, op_str, start_pos);
    }
    
    return Token(TokenType::INVALID, op_str, start_pos);
}

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current_token_index(0) {}

std::unique_ptr<ASTNode> Parser::parse() {
    auto result = parseExpression();
    if (currentToken().type != TokenType::END_OF_INPUT) {
        throw ParseError("Unexpected token after expression", currentToken().position);
    }
    return result;
}

const Token& Parser::currentToken() const {
    return tokens[current_token_index];
}

const Token& Parser::peekToken() const {
    size_t next_index = current_token_index + 1;
    return next_index < tokens.size() ? tokens[next_index] : tokens.back();
}

void Parser::advance() {
    if (current_token_index < tokens.size() - 1) {
        current_token_index++;
    }
}

bool Parser::match(TokenType type) {
    return currentToken().type == type;
}

void Parser::consume(TokenType type) {
    if (!match(type)) {
        throw ParseError("Expected different token type", currentToken().position);
    }
    advance();
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    return parseBiconditional();
}

std::unique_ptr<ASTNode> Parser::parseBiconditional() {
    auto left = parseImplication();
    
    while (match(TokenType::BICONDITIONAL)) {
        advance();
        auto right = parseImplication();
        left = std::make_unique<BinaryOpNode>(NodeType::BICONDITIONAL, std::move(left), std::move(right));
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::parseImplication() {
    auto left = parseDisjunction();
    
    if (match(TokenType::IMPLIES)) {
        advance();
        auto right = parseImplication();
        return std::make_unique<BinaryOpNode>(NodeType::IMPLIES, std::move(left), std::move(right));
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::parseDisjunction() {
    auto left = parseConjunction();
    
    while (match(TokenType::OR)) {
        advance();
        auto right = parseConjunction();
        left = std::make_unique<BinaryOpNode>(NodeType::OR, std::move(left), std::move(right));
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::parseConjunction() {
    auto left = parseNegation();
    
    while (match(TokenType::AND)) {
        advance();
        auto right = parseNegation();
        left = std::make_unique<BinaryOpNode>(NodeType::AND, std::move(left), std::move(right));
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::parseNegation() {
    if (match(TokenType::NOT)) {
        advance();
        auto operand = parseNegation();
        return std::make_unique<UnaryOpNode>(NodeType::NOT, std::move(operand));
    }
    
    return parsePrimary();
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    if (match(TokenType::VARIABLE)) {
        auto var_name = currentToken().value;
        advance();
        return std::make_unique<VariableNode>(var_name);
    }
    
    if (match(TokenType::CONSTANT_TRUE)) {
        advance();
        return std::make_unique<ConstantNode>(true);
    }
    
    if (match(TokenType::CONSTANT_FALSE)) {
        advance();
        return std::make_unique<ConstantNode>(false);
    }
    
    if (match(TokenType::LPAREN)) {
        advance();
        auto expr = parseExpression();
        consume(TokenType::RPAREN);
        return expr;
    }
    
    throw ParseError("Expected variable, constant, or parenthesized expression", currentToken().position);
}

std::unique_ptr<ASTNode> ExpressionParser::parse(const std::string& expression) {
    Lexer lexer(expression);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    return parser.parse();
}

}