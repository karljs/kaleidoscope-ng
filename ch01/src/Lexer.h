#pragma once

#include <cstddef>
#include <expected>
#include <string>
#include <variant>

enum class TokenType {
    END_OF_FILE,
    DEF,
    EXTERN,
    IDENTIFIER,
    NUMBER,
    COMMENT,
};

struct Token {
    TokenType type;
    std::variant<std::wstring, double> data;
};

struct UnknownToken {
    std::wstring unknown_token;
    std::size_t position;
};

class Lexer {
  public:
    Lexer();
    std::expected<Token, UnknownToken> getToken();

  private:
    std::size_t current_position = 0;
};
