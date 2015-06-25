#include "./mmg.cc"
using namespace std;

void test(Pattern&p, int ell) {
  cout << "C^" << ell << " ( " << p << " ) = " << language_size(p, ell, true) << endl;
}

int main(int argc, char*argv[])
{
  // parse args
  {
    for (int i = 1; i < argc; ++i) {
      string s (argv[i]);
      if (s == "-D") {
        DEBUG = true;
        continue;
      }
      if (s == "-K") {
        K = str_to_int(string(argv[i+1]));
        if (DEBUG) trace(K);
      }
    }
  }

  // read documents
  vector<Text> docs;
  {
    string sentence;
    string poss;
    loop {
      getline(cin, sentence);
      getline(cin, poss);
      if (not cin) break;
      auto s = as_words(sentence);
      auto p = as_words(poss);
      if (s.size() != p.size()) {
        cerr << "wrong numbers: #words should equal to #pos" << endl;
        cerr << "  (" << s.size() << ") " << sentence << endl;
        cerr << "  (" << p.size() << ") " << poss << endl;
        return 1;
      }

      Text text;
      rep (i, s.size()) { text.push_back(Alphabet(p[i], s[i])); }
      if (DEBUG) trace(text);
      docs.push_back(text);
    }
  }

  kmmg(0, docs);

  // 行くぜ
  {
    Pattern p = {
      PUnit(), PUnit("A", "a"), PUnit()
    };
    test(p, 4);
  }

  return 0;
}
