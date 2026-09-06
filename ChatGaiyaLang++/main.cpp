#include "parser.cpp"

int main(int argc, char** argv) {
    if (argc < 2 || argc > 4) {
        cerr << "Usage: " << argv[0] << " <source.cg> [-o output.py]\n";
        return 2;
    }

    string in = argv[1];
    string out;

    if (argc == 4) {
        if (string(argv[2]) != "-o") {
            cerr << "Expected -o before output file\n";
            return 2;
        }

        out = argv[3];
    } 
    else {
        out = in;

        size_t q = out.find_last_of('.');

        if (q != string::npos) {
            out.resize(q);
        }

        out += ".py";
    }

    ifstream f(in); // open source

    if (!f) {
        cerr << "Cannot open source file: "
             << in
             << '\n';

        return 1;
    }

    string src(
        (
            istreambuf_iterator<char>(f)
        ),
        {}
    );

    Compiler c; // Here we compiler out code........

    if (!c.compile(src, in)) {
        c.printErrors();

        cerr << "Compilation failed; "
                "no target file was generated.\n";

        return 1;
    }

    if (!c.writePython(out)) {  // for converting our code into python
        cerr << "Cannot write output file: "
             << out
             << '\n';

        return 1;
    }

    cout << "Compilation successful: "
         << out
         << '\n';

    return 0;
}