#include "../mmg.cc"
using namespace std;

pair<string, string> split_word_pos(string&s)
{
  string w, p;
  const int n = s.size();
  int i;
  for (i = n - 2; i >= 0; --i) {
    if (s[i] == '_') break;
  }
  assert(i > 0);
  w = s.substr(0, i);
  p = s.substr(i + 1);
  return make_pair(w, p);
}

Text to_sentence(string s)
{
  stringstream sin(s);
  Text t;
  for (;;) {
    string s; sin >> s;
    if (!sin) break;
    auto pr = split_word_pos(s);
    t.push_back( Alphabet(pr.second, pr.first) );
  }
  return t;
}

void test(Pattern&p, int ell) {
  cout << "C^" << ell << " ( " << p << " ) = "
    << language_size(p, ell, true) << endl;
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
    string ln;
    loop {
      getline(cin, ln);
      if (not cin) break;
      Text text = to_sentence(ln);
      if (DEBUG) trace(text);
      docs.push_back(text);
    }
  }
  init(docs);

  // 行くぜ
  {
    Pattern p = {
      PUnit(),
      PUnit(), PUnit("A", "a"), PUnit()
    };
    test(p, 4);
    DFA(p, 4, true);
  }

  return 0;
}
