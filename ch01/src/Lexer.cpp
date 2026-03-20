#include "Lexer.h"
#include <unordered_map>
#include <utf8proc.h>

namespace {
const std::unordered_map<std::string_view, TokenType> keyword_map = {
    {"def", TokenType::DEF}, {"extern", TokenType::EXTERN}};

bool is_unicode_alpha(utf8proc_int32_t cp) {
    const auto cat = utf8proc_category(cp);
    return cat == UTF8PROC_CATEGORY_LU || cat == UTF8PROC_CATEGORY_LL ||
           cat == UTF8PROC_CATEGORY_LT || cat == UTF8PROC_CATEGORY_LM ||
           cat == UTF8PROC_CATEGORY_LO;
}
} // namespace

Lexer::Lexer(std::string source) : source_(std::move(source)) {}

utf8proc_int32_t Lexer::peek_codepoint() const {
    if (byte_pos_ >= source_.size())
        return -1;
    utf8proc_int32_t cp;
    utf8proc_iterate(
        reinterpret_cast<const utf8proc_uint8_t *>(source_.data() + byte_pos_),
        source_.size() - byte_pos_, &cp);
    return cp;
}

utf8proc_int32_t Lexer::consume_codepoint() {
    if (byte_pos_ >= source_.size())
        return -1;
    utf8proc_int32_t cp;
    utf8proc_ssize_t bytes = utf8proc_iterate(
        reinterpret_cast<const utf8proc_uint8_t *>(source_.data() + byte_pos_),
        source_.size() - byte_pos_, &cp);
    if (bytes > 0)
        byte_pos_ += bytes;
    return cp;
}

std::expected<Token, UnknownToken> Lexer::getToken() {
    // Skip whitespace
    auto cp = peek_codepoint();
    while (cp != -1 && (cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r' ||
                        cp == '\f' || cp == '\v')) {
        consume_codepoint();
        cp = peek_codepoint();
    }

    std::size_t token_start = byte_pos_;
    cp = consume_codepoint();

    if (cp == -1) {
        return Token{TokenType::END_OF_FILE, ""};
    }

    // Comments
    if (cp == '#') {
        while (cp != -1 && cp != '\n') {
            cp = consume_codepoint();
        }
        return getToken(); // recurse to get next real token
    }

    // Numbers
    if (utf8proc_category(cp) == UTF8PROC_CATEGORY_ND) {
        std::string number;
        uint8_t buf[4];
        utf8proc_ssize_t len = utf8proc_encode_char(cp, buf);
        number.append(reinterpret_cast<char *>(buf), len);

        cp = peek_codepoint();
        while (cp != -1 && utf8proc_category(cp) == UTF8PROC_CATEGORY_ND) {
            consume_codepoint();
            len = utf8proc_encode_char(cp, buf);
            number.append(reinterpret_cast<char *>(buf), len);
            cp = peek_codepoint();
        }

        if (cp == '.') {
            consume_codepoint();
            len = utf8proc_encode_char('.', buf);
            number.append(reinterpret_cast<char *>(buf), len);
            cp = peek_codepoint();
            while (cp != -1 && utf8proc_category(cp) == UTF8PROC_CATEGORY_ND) {
                consume_codepoint();
                len = utf8proc_encode_char(cp, buf);
                number.append(reinterpret_cast<char *>(buf), len);
                cp = peek_codepoint();
            }
        }

        return Token{TokenType::NUMBER, std::stod(number)};
    }

    // Identifiers
    if (is_unicode_alpha(cp) || cp == '_') {
        std::string identifier;
        uint8_t buf[4];
        utf8proc_ssize_t len = utf8proc_encode_char(cp, buf);
        identifier.append(reinterpret_cast<char *>(buf), len);

        cp = peek_codepoint();
        while (cp != -1 &&
               (is_unicode_alpha(cp) ||
                utf8proc_category(cp) == UTF8PROC_CATEGORY_ND || cp == '_')) {
            consume_codepoint();
            len = utf8proc_encode_char(cp, buf);
            identifier.append(reinterpret_cast<char *>(buf), len);
            cp = peek_codepoint();
        }

        auto it = keyword_map.find(identifier);
        if (it != keyword_map.end()) {
            return Token{it->second, identifier};
        }
        return Token{TokenType::IDENTIFIER, identifier};
    }

    // Unknown token
    uint8_t buf[4];
    utf8proc_ssize_t len = utf8proc_encode_char(cp, buf);
    std::string unknown(reinterpret_cast<char *>(buf), len);
    return std::unexpected(UnknownToken{unknown, token_start});
}