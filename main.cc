#include "./mmg.cc"
using namespace std;

int main(int argc, char*argv[])
{

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
        cerr << "  " << sentence << endl;
        cerr << "  " << poss << endl;
      }

      Text text;
      rep (i, s.size()) { text.push_back(Alphabet(p[i], s[i])); }
      trace(text);
      docs.push_back(text);
    }
  }

  // 行くぜ
  auto result = kmmg(1, docs);
  for (auto& p: result) {
    cout << p << endl;
  }

  return 0;
}
