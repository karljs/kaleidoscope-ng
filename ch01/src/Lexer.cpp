#include "Lexer.h"
#include <cwctype>
#include <iostream>
#include <locale>
#include <unordered_map>

namespace {
const std::unordered_map<std::wstring_view, TokenType> keyword_map = {
    {L"def", TokenType::DEF}, {L"extern", TokenType::EXTERN}};
}

Lexer::Lexer() {
    std::locale::global(std::locale(""));
    std::wcin.imbue(std::locale());
}

std::expected<Token, UnknownToken> Lexer::getToken() {
    auto skip_while = [this](auto predicate) {
        wint_t c;
        while ((c = std::wcin.peek()) != WEOF && predicate(c)) {
            std::wcin.get();
            current_position++;
        }
    };

    auto consume = [this]() -> wint_t {
        auto c = std::wcin.get();
        if (c != WEOF)
            current_position++;
        return c;
    };

    skip_while([](wint_t c) { return std::iswspace(c); });

    wint_t c = consume();

    if (c == WEOF) {
        return Token{TokenType::END_OF_FILE, L"\0"};
    }

    if (c == L'#') {
        skip_while([](wint_t c) { return c != L'\n'; });
        consume();
        return getToken();
    }

    if (std::iswdigit(c)) {
        std::wstring number{static_cast<wchar_t>(c)};

        while (std::iswdigit(std::wcin.peek())) {
            number += static_cast<wchar_t>(consume());
        }

        if (std::wcin.peek() == L'.') {
            number += static_cast<wchar_t>(consume());
            while (std::iswdigit(std::wcin.peek())) {
                number += static_cast<wchar_t>(consume());
            }
        }

        return Token{TokenType::NUMBER, std::stod(number)};
    }

    if (std::iswalpha(c)) {
        std::wstring identifier{static_cast<wchar_t>(c)};

        for (wint_t next = std::wcin.peek();
             next != WEOF && (std::iswalnum(next) || next == L'_');
             next = std::wcin.peek()) {
            identifier += static_cast<wchar_t>(consume());
        }

        if (auto it = keyword_map.find(identifier); it != keyword_map.end()) {
            return Token{it->second, identifier};
        }
        return Token{TokenType::IDENTIFIER, std::move(identifier)};
    }

    return std::unexpected(UnknownToken{std::wstring(1, c), current_position});
}
