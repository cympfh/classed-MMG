struct PUnit;
struct Alphabet;

using Pattern = vector<PUnit>;
using Text = vector<Alphabet>;

// initial with `init`
vector<Text> docs;
int alphabet_size = -1;
map<string, int> class_size;
set<Alphabet> vocabulary;
// set<string> pos_vocabulary;

bool preceq(Text&, Pattern&);
void init(vector<Text>&_docs, bool=false);
vector<Pattern> kmmg(int K);

using State = long long unsigned int;

Integer
language_size(const Pattern&, int, bool=false);
