// Chatgaiya++ compiler: source language -> executable Python 3
// Build: g++ -std=c++17 -O2 ChatgaiyaPlusPlus.cpp -o chatgaiya.exe

// Keywords:
//   ongko       -> int
//   dhoshomik   -> float
//   kotha       -> string
//   ho          -> bool
//   hasa        -> true
//   misa        -> false
//   zodi        -> if
//   noile       -> else / else if
//   zotokkhon   -> while
//   tham        -> break
//   chol        -> continue
//   lo()        -> input
//   ko()        -> print

#include <bits/stdc++.h>
#include <sys/stat.h>

using namespace std;

class Compiler {
    enum class Type { // Types
        UNKNOWN,
        INT,
        FLOAT,
        STRING,
        BOOL
    };

    struct Token {  // Tokens
        string text;
        int line;
        int col;
    };

    struct Expr {   // Exprerssions
        string code;
        Type type;
        bool inputCall = false;
    };

    // Compiler Data
    vector<Token> ts;
    size_t p = 0;
    int ind = 0;
    unordered_map<string, Type> vars;
    vector<string> py;
    vector<string> errors;
    string fileName;
    bool usesIndexing = false;

    // Identufiers
    static bool identStart(const string& s) {
        return !s.empty() && (isalpha((unsigned char)s[0]) || s[0] == '_');
    }

    // Numbers
    static bool numberStart(const string& s) {
        return !s.empty() && (isdigit((unsigned char)s[0]) || s[0] == '.');
    }

    // Type Check
    static bool isType(const string& s) {
        return
            s == "ongko" ||
            s == "dhoshomik" ||
            s == "kotha" ||
            s == "ho";
    }

    // String type
    static Type typeOf(const string& s) {
        if (s == "ongko")
            return Type::INT;

        if (s == "dhoshomik")
            return Type::FLOAT;

        if (s == "kotha")
            return Type::STRING;

        if (s == "ho")
            return Type::BOOL;

        return Type::UNKNOWN;
    }

    // Numeric
    static bool num(Type t) {
        return
            t == Type::INT ||
            t == Type::FLOAT;
    }

    static string typeName(Type t) {
        if (t == Type::INT)
            return "ongko";

        if (t == Type::FLOAT)
            return "dhoshomik";

        if (t == Type::STRING)
            return "kotha";

        if (t == Type::BOOL)
            return "ho";

        return "unknown";
    }

    static string defaultLiteral(Type t) {
        if (t == Type::FLOAT)
            return "0.0";

        if (t == Type::STRING)
            return "\"\"";

        if (t == Type::BOOL)
            return "False";

        return "0";
    }

    // Rerserve Words for ourr Compiler
    static bool isReservedWord(const string& s) {
        static const unordered_set<string> kws = {
            "shuru_kor", "lo", "ko",
            "ongko", "dhoshomik", "kotha", "ho",
            "hasa", "misa",
            "zodi", "noile", "tokon", "zotokkhon",
            "tham", "chol", "ki_type",
            "False", "None", "True", "and", "as", "assert",
            "async", "await", "break", "class", "continue",
            "def", "del", "elif", "else", "except", "finally",
            "for", "from", "global", "if", "import", "in",
            "is", "lambda", "nonlocal", "not", "or", "pass",
            "raise", "return", "try", "while", "with", "yield"
        };

        return kws.count(s) != 0;
    }

    // Assignment operators
    static bool assignable(Type a, Type b) {
        return
            a == Type::UNKNOWN ||
            b == Type::UNKNOWN ||
            a == b ||
            (
                a == Type::FLOAT &&
                b == Type::INT
            );
    }

    // Output of Python code
    void emit(const string& s) {
        py.push_back(string(ind * 4, ' ') + s);
    }

    Token& cur() {
        static Token e{
            "<eof>",
            0,
            0
        };

        return p < ts.size() ? ts[p] : e;
    }

    bool at(const string& x) {
        return cur().text == x;
    }

    bool take(const string& x) {
        if (at(x)) {
            ++p;
            return true;
        }

        return false;
    }

    bool eof() {
        return at("<eof>");
    }

    void err(const string& s) {
        errors.push_back(
            fileName +
            ":" +
            to_string(cur().line) +
            ":" +
            to_string(cur().col) +
            ": " +
            s +
            " (near '" +
            cur().text +
            "')"
        );
    }

    bool need(const string& x) {
        if (take(x))
            return true;

        err("expected '" + x + "'");

        return false;
    }
    
    // skip new lines and separatos
    void separators() {
        while (at(";") || at("\n")) {
            ++p;
        }
    }

    // Finding Errors
    void recover() {
        size_t before = p;

        while (!eof() && !at(";") && !at("\n") && !at("}")){
            ++p;
        }

        if (at(";") || at("\n")) {
            ++p;
        }

        if (p == before && at("}")) {
            ++p;
        }
    }

    
    // Lexer of my Compiler
    void lex(const string& s) {
        int line = 1;
        int col = 1;

        auto add = [&](string x, int l, int c) {
            ts.push_back({
                move(x),
                l,
                c
            });
        };

        for (size_t i = 0; i < s.size();) {
            char c = s[i];
            // Space
            if (c == ' ' || c == '\t' || c == '\r') {
                ++i;
                ++col;
                continue;
            }

            // Newline
            if (c == '\n') {
                add("\n", line, col);

                ++i;
                ++line;

                col = 1;

                continue;
            }

            // Comments
            if (c == '/' &&i + 1 < s.size() && s[i + 1] == '/'){
                while (i < s.size() && s[i] != '\n') {
                    ++i;
                    ++col;
                }
                continue;
            }

            int l = line;
            int c0 = col;

            // identifier/keywords
            if (isalpha((unsigned char)c) ||c == '_') {
                size_t j = i + 1;

                while (j < s.size() && (isalnum((unsigned char)s[j]) || s[j] == '_')) {
                    ++j;
                }

                add(s.substr(i, j - i), l, c0);

                col += int(j - i);
                i = j;

                continue;
            }

            // Number
            if (isdigit((unsigned char)c) ||
                (c == '.' && i + 1 < s.size() && isdigit((unsigned char)s[i + 1]))) {

                size_t j = i;
                bool dot = false;

                while (j < s.size() &&
                       (isdigit((unsigned char)s[j]) ||
                       (!dot && s[j] == '.'))) {

                    if (s[j] == '.')
                        dot = true;

                    ++j;
                }

                add(s.substr(i, j - i), l, c0);

                col += int(j - i);
                i = j;

                continue;
            }

            // String
            if (c == '"' || c == '\'') {
                char q = c;
                size_t j = i + 1;
                string v;
                bool closed = false;

                while (j < s.size()) {

                    if (s[j] == q) {
                        closed = true;
                        break;
                    }

                    if (s[j] == '\\' && j + 1 < s.size()) {
                        v += s[j++];
                        v += s[j++];
                    }
                    else {
                        v += s[j++];
                    }
                }

                if (!closed) {
                    errors.push_back(
                        fileName +
                        ":" +
                        to_string(l) +
                        ":" +
                        to_string(c0) +
                        ": unterminated string literal"
                    );
                }

                add("\"" + v + "\"", l, c0);

                size_t end = closed ? j + 1 : j;

                col += int(end - i);
                i = end;

                continue;
            }


            // Operators of two char
            string two = i + 1 < s.size() ? s.substr(i, 2) : "";

            if (two == "==" ||
                two == "!=" ||
                two == "<=" ||
                two == ">=" ||
                two == "&&" ||
                two == "||" ||
                two == "++" ||
                two == "--" ||
                two == "<<" ||
                two == ">>") {

                add(two, l, c0);

                i += 2;
                col += 2;
            }
            else {
                add(string(1, c), l, c0);
                ++i;
                ++col;
            }
        }

        ts.push_back({"<eof>", line, col});
    }

#ifndef CHATGAIYA_PARSER_INCLUDED
};
#endif