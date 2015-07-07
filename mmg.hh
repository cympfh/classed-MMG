struct PUnit;
struct Alphabet;

using Pattern = vector<PUnit>;
using Text = vector<Alphabet>;
using Integer = mpz_class;

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

vector<map<PUnit, vi>>
NFA(const Pattern&, int, bool=false);

pair< map<State, map<PUnit, State>>, int >
DFA(const Pattern&, int, bool=false);

Integer
language_size(const Pattern&, int, bool=false);
