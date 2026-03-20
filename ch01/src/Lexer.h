#pragma once

#include <cstddef>
#include <expected>
#include <string>
#include <utf8proc.h>
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
    std::variant<std::string, double> data;
};

struct UnknownToken {
    std::string unknown_token;
    std::size_t position;
};

class Lexer {
  public:
    explicit Lexer(std::string source);
    std::expected<Token, UnknownToken> getToken();

  private:
    utf8proc_int32_t peek_codepoint() const;
    utf8proc_int32_t consume_codepoint();

    std::string source_;
    std::size_t byte_pos_ = 0;
};
