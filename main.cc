#include "./mmg.cc"
using namespace std;

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

  // 行くぜ
  auto result = kmmg(K, docs);
  for (auto& p: result) {
    cout << p << endl;
  }

  return 0;
}
