#include "Lexer.h"
#include <iostream>

const wchar_t* tokenTypeName(TokenType type) {
    switch (type) {
        case TokenType::END_OF_FILE: return L"EOF";
        case TokenType::DEF: return L"DEF";
        case TokenType::EXTERN: return L"EXTERN";
        case TokenType::IDENTIFIER: return L"IDENTIFIER";
        case TokenType::NUMBER: return L"NUMBER";
        case TokenType::COMMENT: return L"COMMENT";
        default: return L"UNKNOWN";
    }
}

int main() {
    Lexer lexer;
    
    std::wcout << L"Chapter 01 - Lexer\n";
    std::wcout << L"==================\n";
    std::wcout << L"Reading tokens from stdin:\n\n";
    
    auto result = lexer.getToken();
    while (result.has_value()) {
        Token token = result.value();
        
        std::wcout << L"Token: " << tokenTypeName(token.type);
        
#ifdef DEBUG_OUTPUT
        std::wcout << L" (type=" << static_cast<int>(token.type) << L")";
#endif
        
        if (std::holds_alternative<std::wstring>(token.data)) {
            std::wcout << L" = \"" << std::get<std::wstring>(token.data) << L"\"";
        } else if (std::holds_alternative<double>(token.data)) {
            std::wcout << L" = " << std::get<double>(token.data);
        }
        std::wcout << L"\n";
        
        if (token.type == TokenType::END_OF_FILE) {
            break;
        }
        
        result = lexer.getToken();
    }
    
    if (!result.has_value()) {
        std::wcout << L"Unknown: '" << result.error().unknown_token 
                   << L"' at pos " << result.error().position << L"\n";
    }
    
    return 0;
}
