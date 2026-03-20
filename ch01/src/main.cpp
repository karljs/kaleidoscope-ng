#include "Lexer.h"
#include <iostream>

const std::string tokenTypeName(TokenType type) {
    switch (type) {
    case TokenType::END_OF_FILE:
        return "EOF";
    case TokenType::DEF:
        return "DEF";
    case TokenType::EXTERN:
        return "EXTERN";
    case TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case TokenType::NUMBER:
        return "NUMBER";
    case TokenType::COMMENT:
        return "COMMENT";
    default:
        return "UNKNOWN";
    }
}

int main() {
    // read the whole input into a buffer, because iterating
    // over a utf8 stream is out of scope
    std::string source{std::istreambuf_iterator<char>(std::cin),
                       std::istreambuf_iterator<char>()};
    Lexer lexer(source);

    std::cout << "Chapter 01 - Lexer\n";
    std::cout << "==================\n";
    std::cout << "Reading tokens from stdin:\n\n";

    auto result = lexer.getToken();
    while (result.has_value()) {
        Token token = result.value();

        std::cout << "Token: " << tokenTypeName(token.type);

#ifdef DEBUG_OUTPUT
        std::cout << " (type=" << static_cast<int>(token.type) << ")";
#endif

        if (std::holds_alternative<std::string>(token.data)) {
            std::cout << " = \"" << std::get<std::string>(token.data) << "\"";
        } else if (std::holds_alternative<double>(token.data)) {
            std::cout << " = " << std::get<double>(token.data);
        }
        std::cout << "\n";

        if (token.type == TokenType::END_OF_FILE) {
            break;
        }

        result = lexer.getToken();
    }

    if (!result.has_value()) {
        std::cout << "Unknown: '" << result.error().unknown_token << "' at pos "
                  << result.error().position << "\n";
    }

    return 0;
}
