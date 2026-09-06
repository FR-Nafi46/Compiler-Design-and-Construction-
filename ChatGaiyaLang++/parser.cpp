#define CHATGAIYA_PARSER_INCLUDED
#include "lexer.cpp"

// Expression
Expr expression() {
    return logicalOr();
}

// Logical OR
Expr logicalOr() {
    Expr a = logicalAnd();

    while (take("||")) {
        Expr b = logicalAnd();

        if (a.type != Type::BOOL || b.type != Type::BOOL) {
            err("|| requires boolean operands");
        }

        a = {
            "(" + a.code + " or " + b.code + ")",
            Type::BOOL
        };
    }

    return a;
}

// Logical AND
Expr logicalAnd() {
    Expr a = bitOr();

    while (take("&&")) {
        Expr b = bitOr();

        if (a.type != Type::BOOL || b.type != Type::BOOL) {
            err("&& requires boolean operands");
        }

        a = {
            "(" + a.code + " and " + b.code + ")",
            Type::BOOL
        };
    }

    return a;
}

// Bitwise OR
Expr bitOr() {
    Expr a = bitXor();

    while (take("|")) {
        Expr b = bitXor();

        if (a.type != Type::INT || b.type != Type::INT) {
            err("| requires integer operands");
        }

        a = {
            "(" + a.code + " | " + b.code + ")",
            Type::INT
        };
    }

    return a;
}

// Bitwise XOR
Expr bitXor() {
    Expr a = bitAnd();

    while (take("^")) {
        Expr b = bitAnd();

        if (a.type != Type::INT || b.type != Type::INT) {
            err("^ requires integer operands");
        }

        a = {
            "(" + a.code + " ^ " + b.code + ")",
            Type::INT
        };
    }

    return a;
}

// Bitwise AND
Expr bitAnd() {
    Expr a = equality();

    while (take("&")) {
        Expr b = equality();

        if (a.type != Type::INT || b.type != Type::INT) {
            err("& requires integer operands");
        }

        a = {
            "(" + a.code + " & " + b.code + ")",
            Type::INT
        };
    }

    return a;
}

// Shift
Expr shift() {
    Expr a = add();

    while (at("<<") || at(">>")) {
        string o = cur().text;
        ++p;

        Expr b = add();

        if (a.type != Type::INT || b.type != Type::INT) {
            err("shift operators require integer operands");
        }

        a = {
            "(" + a.code + " " + o + " " + b.code + ")",
            Type::INT
        };
    }

    return a;
}

// Equality
Expr equality() {
    Expr a = relation();

    while (at("==") || at("!=")) {
        string o = cur().text;
        ++p;

        Expr b = relation();

        if (a.type != b.type && !(num(a.type) && num(b.type))) {
            err("incompatible types in comparison");
        }

        a = {
            "(" + a.code + " " + o + " " + b.code + ")",
            Type::BOOL
        };
    }

    return a;
}

// Relational
Expr relation() {
    Expr a = shift();

    while (at("<") || at(">") || at("<=") || at(">=")) {
        string o = cur().text;
        ++p;

        Expr b = shift();

        if (!num(a.type) || !num(b.type)) {
            err("relational operators require numbers");
        }

        a = {
            "(" + a.code + " " + o + " " + b.code + ")",
            Type::BOOL
        };
    }

    return a;
}

// Addition / Subtraction
Expr add() {
    Expr a = mul();

    while (at("+") || at("-")) {
        string o = cur().text;
        ++p;

        Expr b = mul();

        bool bothNum = num(a.type) && num(b.type);

        bool bothString =
            a.type == Type::STRING &&
            b.type == Type::STRING;

        // "+" also allows one string operand mixed with a
        // non-string, non-unknown operand (int/float/bool).
        // The non-string side is auto-stringified so
        // e.g. "count: " + count works, instead of forcing
        // the user to print pieces separately.
        bool mixedConcat =
            o == "+" &&
            (a.type == Type::STRING) != (b.type == Type::STRING) &&
            a.type != Type::UNKNOWN &&
            b.type != Type::UNKNOWN;

        bool ok = bothNum || bothString || mixedConcat;

        if (!ok) {
            err("'" + o + "' has invalid operand types");
        }

        Type t;
        string aCode = a.code;
        string bCode = b.code;

        if (bothNum) {
            t = (
                a.type == Type::FLOAT ||
                b.type == Type::FLOAT
            ) ? Type::FLOAT : Type::INT;
        } else if (bothString || mixedConcat) {
            t = Type::STRING;

            if (a.type != Type::STRING)
                aCode = "str(" + aCode + ")";

            if (b.type != Type::STRING)
                bCode = "str(" + bCode + ")";
        } else {
            // Invalid combination already reported above.
            // Fall back to INT so the rest of the broken
            // expression still type-checks without cascading
            // unrelated errors.
            t = Type::INT;
        }

        a = {
            "(" + aCode + " " + o + " " + bCode + ")",
            t
        };
    }

    return a;
}

// Multiplication / Division / Mod
Expr mul() {
    Expr a = unary();

    while (at("*") || at("/") || at("%")) {
        string o = cur().text;
        ++p;

        Expr b = unary();

        if (!num(a.type) || !num(b.type)) {
            err("arithmetic operators require numbers");
        }

        Type t = (
            a.type == Type::FLOAT ||
            b.type == Type::FLOAT ||
            o == "/"
        ) ? Type::FLOAT : Type::INT;

        a = {
            "(" + a.code + " " + o + " " + b.code + ")",
            t
        };
    }

    return a;
}

// Unary
Expr unary() {
    // NOT
    if (take("!")) {
        Expr a = unary();

        if (a.type != Type::BOOL) {
            err("! requires a boolean");
        }

        return {
            "(not " + a.code + ")",
            Type::BOOL
        };
    }

    // NEGATIVE
    if (take("-")) {
        Expr a = unary();

        if (!num(a.type)) {
            err("unary - requires a number");
        }

        return {
            "(-" + a.code + ")",
            a.type
        };
    }

    // POSITIVE
    if (take("+")) {
        Expr a = unary();

        if (!num(a.type)) {
            err("unary + requires a number");
        }

        return a;
    }

    return postfix();
}

// Postfix (string indexing: s[i])
Expr postfix() {
    Expr a = primary();

    while (take("[")) {
        Expr idx = expression();

        need("]");

        if (a.type != Type::STRING) {
            err("indexing is only supported on kotha (string) values");
        }

        if (idx.type != Type::INT) {
            err("string index must be ongko (int)");
        }

        usesIndexing = true;

        a = {
            "_cg_charat(" + a.code + ", " + idx.code + ")",
            Type::STRING
        };
    }

    return a;
}

// Primary
Expr primary() {
    // Parentheses
    if (take("(")) {
        Expr a = expression();

        need(")");

        return {
            "(" + a.code + ")",
            a.type,
            a.inputCall
        };
    }

    // Boolean
    if (at("hasa") || at("misa")) {
        bool v = at("hasa");
        ++p;

        return {
            v ? "True" : "False",
            Type::BOOL
        };
    }

    // Input
    if (at("lo")) {
        ++p;

        need("(");
        need(")");

        return {
            "input()",
            Type::STRING,
            true
        };
    }

    // Type checking - ki_type(expr)
    // Resolves at compile time to the static type of expr,
    // since Chatgaiya++ is statically typed.
    if (at("ki_type")) {
        ++p;

        need("(");

        Expr inner = expression();

        need(")");

        return {
            "\"" + typeName(inner.type) + "\"",
            Type::STRING
        };
    }

    // String
    if (cur().text.size() >= 2 && cur().text[0] == '"') {
        string x = cur().text;
        ++p;

        return {
            x,
            Type::STRING
        };
    }

    // Number
    if (numberStart(cur().text)) {
        string x = cur().text;
        ++p;

        return {
            x,
            x.find('.') != string::npos
                ? Type::FLOAT
                : Type::INT
        };
    }

    // Variable
    if (identStart(cur().text)) {
        string x = cur().text;
        ++p;

        if (!vars.count(x)) {
            err("undeclared variable '" + x + "'");

            return {
                x,
                Type::UNKNOWN
            };
        }

        return {
            x,
            vars[x]
        };
    }

    // Error
    err("expected expression");

    if (!eof() && !at(")") && !at(";") && !at("\n")) {
        ++p;
    }

    return {
        "0",
        Type::UNKNOWN
    };
}

// Input cast
string inputCast(Type t, const string& x) {
    // INTEGER INPUT
    if (t == Type::INT) {
        return "int(input())";
    }

    // FLOAT INPUT
    if (t == Type::FLOAT) {
        return "float(input())";
    }

    // BOOLEAN INPUT
    if (t == Type::BOOL) {
        return "(" + x + ".strip().lower() in ('hasa','true','1'))";
    }

    // STRING INPUT
    return x;
}

// Statement
void statement() {
    // Empty line / semicolon
    if (at(";") || at("\n")) {
        ++p;
        return;
    }

    // Include
    if (at("shuru_kor") || at("#include")) {
        while (!eof() && !at(";") && !at("\n")) {
            ++p;
        }

        separators();
        return;
    }

    // Declaration
    if (isType(cur().text)) {
        declaration();
        return;
    }

    // Print
    if (take("ko")) {
        need("(");

        Expr e = expression();

        need(")");
        need(";");

        emit("print(" + e.code + ")");
        return;
    }

    // IF
    if (take("zodi")) {
        ifStatement();
        return;
    }

    // WHILE
    if (take("zotokkhon")) {
        whileStatement();
        return;
    }

    // BREAK
    if (take("tham")) {
        need(";");

        emit("break");
        return;
    }

    // CONTINUE
    if (take("chol")) {
        need(";");

        emit("continue");
        return;
    }

    // Variable statement
    if (identStart(cur().text)) {
        string n = cur().text;
        ++p;

        if (!vars.count(n)) {
            err("undeclared variable '" + n + "'");
        }

        // Assignment
        if (take("=")) {
            Expr e = expression();

            need(";");

            Type t =
                vars.count(n)
                    ? vars[n]
                    : Type::UNKNOWN;

            if (!e.inputCall && !assignable(t, e.type)) {
                err(
                    "cannot assign " +
                    typeName(e.type) +
                    " to " +
                    typeName(t)
                );
            }

            string rhs =
                e.inputCall
                    ? inputCast(t, e.code)
                    : e.code;

            emit(n + " = " + rhs);
            return;
        }

        // Increment / decrement
        if (take("++") || take("--")) {
            string op = ts[p - 1].text;

            need(";");

            if (vars.count(n) && !num(vars[n])) {
                err(op + " requires a number");
            }

            emit(
                n +
                (
                    op == "++"
                        ? " += 1"
                        : " -= 1"
                )
            );

            return;
        }

        err("expected assignment or increment");

        recover();
        return;
    }

    // Unknown statement
    err("unexpected token");
    recover();
}

// Variable declaration
void declaration() {
    Type t = typeOf(cur().text);
    ++p;

    if (!identStart(cur().text)) {
        err("expected variable name");

        recover();
        return;
    }

    string n = cur().text;
    ++p;

    if (isReservedWord(n)) {
        err(
            "'" +
            n +
            "' is a reserved word and cannot be used as a variable name"
        );

        recover();
        return;
    }

    if (vars.count(n)) {
        err("variable already declared: '" + n + "'");
    } else {
        vars[n] = t;
    }

    string rhs = defaultLiteral(t);

    if (take("=")) {
        Expr e = expression();

        if (!e.inputCall && !assignable(t, e.type)) {
            err(
                "cannot initialize " +
                typeName(t) +
                " with " +
                typeName(e.type)
            );
        }

        rhs =
            e.inputCall
                ? inputCast(t, e.code)
                : e.code;
    }

    need(";");

    emit(n + " = " + rhs);
}

// Block
void block() {
    if (take("{")) {
        ++ind;

        separators();

        while (!eof() && !at("}")) {
            statement();
            separators();
        }

        if (at("}")) {
            ++p;
        } else {
            err("expected '}'");
        }

        --ind;
    } else {
        ++ind;

        // The lexer stores newlines as tokens, so without
        // skipping them here, statement() would see a bare
        // "\n" first, treat it as a no-op empty statement,
        // and return immediately. It would then be parsed
        // back in the caller's loop as an ordinary top-level
        // statement, unindented and executed unconditionally.
        separators();
        statement();

        --ind;
    }
}

// Boolean condition
bool boolCondition(const Expr& e) {
    if (e.type != Type::BOOL && e.type != Type::UNKNOWN) {
        err("condition must be boolean");
    }

    return true;
}

// IF / ELSE IF / ELSE
void ifStatement(bool elseIf = false) {
    // zodi (condition)
    need("(");

    Expr c = expression();

    need(")");

    boolCondition(c);

    emit(
        string(elseIf ? "elif " : "if ") +
        c.code +
        ":"
    );

    block();

    // The lexer stores newline as a token.
    // Therefore we must remove newline/semicolon
    // before checking for "noile".
    separators();

    // noile
    if (take("noile")) {
        // Skip newline after noile if present
        separators();

        // noile zodi (...)
        if (take("zodi")) {
            ifStatement(true);
        }

        // noile {...}
        else {
            emit("else:");
            block();
        }
    }

    // tokon (else)
    else if (take("tokon")) {
        emit("else:");
        block();
    }
}

// While
void whileStatement() {
    need("(");

    Expr c = expression();

    need(")");

    boolCondition(c);

    emit("while " + c.code + ":");
    block();
}

public:

// Compile
bool compile(const string& source, const string& name) {
    fileName = name;

    ts.clear();
    vars.clear();

    py = {
        "#!/usr/bin/env python3",
        "# Generated by Chatgaiya++ compiler",
        ""
    };

    errors.clear();
    usesIndexing = false;

    p = 0;
    ind = 0;

    lex(source);

    while (!eof()) {
        separators();

        if (!eof()) {
            statement();
        }
    }

    // If any s[i] indexing was compiled, splice the runtime
    // helper in right after the header comment lines so
    // generated code stays valid Python top-to-bottom.
    // An out-of-range index returns "" instead of crashing
    // the generated program with an unhandled IndexError.
    if (usesIndexing) {
        py.insert(
            py.begin() + 3,
            {
                "def _cg_charat(s, i):",
                "    try:",
                "        return s[i]",
                "    except (IndexError, TypeError):",
                "        return \"\"",
                ""
            }
        );
    }

    return errors.empty();
}

// Write Python file
bool writePython(const string& path) {
    ofstream f(path);

    if (!f)
        return false;

    for (auto& x : py) {
        f << x << '\n';
    }

    f.close();

    return chmod(path.c_str(), 0755) == 0;
}

// Print errors
void printErrors() const {
    for (auto& x : errors) {
        cerr << "Error: " << x << '\n';
    }
}
};
