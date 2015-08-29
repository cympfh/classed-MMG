#include "../mmg.cc"
using namespace std;

int main(int argc, char*argv[])
{
  {
    Text t = { Alphabet("NN", "name") };
    Pattern p = { PUnit("NN", "name") };
    Pattern p2 = { PUnit("NN") };
    Pattern p3 = { PUnit() };
    Pattern q = { PUnit("NN", "name"), PUnit() };
    Pattern r = { PUnit(), PUnit("NN", "name") };
    assert(preceq(t, p));
    assert(preceq(t, p2));
    assert(preceq(t, p3));
    assert(not preceq(t, q));
    assert(not preceq(t, r));
  }

  {
    Text t = { Alphabet("NN", "name"), Alphabet("BE", "is") };
    Pattern p = { PUnit("NN", "name") };
    Pattern q = { PUnit("NN", "name"), PUnit("BE") };
    Pattern r = { PUnit("NN", "name"), PUnit("BE", "are") };
    assert(not preceq(t, p));
    assert(preceq(t, q));
    assert(not preceq(t, r));
  }

  {
    Text t = {
      Alphabet("A", "a"), Alphabet("B", "b"),
      Alphabet("A", "a"), Alphabet("B", "b"),
      Alphabet("A", "a"), Alphabet("B", "b")
    };
    assert(preceq(t, { PUnit("A"), PUnit(), PUnit("B") }));
    assert(preceq(t, { PUnit("A"), PUnit("B"), PUnit(), PUnit("B") }));
    assert(preceq(t, { PUnit("A"), PUnit(), PUnit("A"), PUnit("B") }));
    assert(preceq(t, { PUnit(), PUnit("A"), PUnit("B") }));
    assert(preceq(t, { PUnit("A"), PUnit("B"), PUnit() }));
    assert(preceq(t, { PUnit(), PUnit("A"), PUnit("B"), PUnit() }));
    assert(preceq(t, { PUnit("A"), PUnit(), PUnit("B"), PUnit(), PUnit("B") }));
    assert(preceq(t, { PUnit(), PUnit("A"), PUnit("B"), PUnit() }));
    assert(preceq(t, { PUnit("A"), PUnit("B"), PUnit("A"), PUnit("B"), PUnit("A"), PUnit("B") }));
    assert(preceq(t, { PUnit(), PUnit("B"), PUnit("A"), PUnit("B"), PUnit("A"), PUnit("B") }));
    assert(preceq(t, { PUnit("A"), PUnit("B"), PUnit("A"), PUnit(), PUnit("A"), PUnit("B") }));
    assert(preceq(t, { PUnit("A"), PUnit("B"), PUnit("A"), PUnit("B"), PUnit("A"), PUnit() }));

    assert(not preceq(t, { PUnit("C"), PUnit("B"), PUnit() }));
    assert(not preceq(t, { PUnit("A"), PUnit("B") }));
    assert(not preceq(t, { PUnit(), PUnit("B"), PUnit("B"), PUnit() }));
    assert(not preceq(t, { PUnit(), PUnit("A"), PUnit("B"), PUnit("B") }));
    assert(not preceq(t, { PUnit("B"), PUnit(), PUnit("A"), PUnit("B") }));
    assert(not preceq(t, { PUnit("A"), PUnit("B"), PUnit(), PUnit("A"), PUnit("B"), PUnit("A"), PUnit("B") }));
  }

  {
    Text t = {
      Alphabet("A", "a"), Alphabet("B", "b"),
      Alphabet("A", "a"), Alphabet("B", "b"),
      Alphabet("A", "a"), Alphabet("B", "b"),
      Alphabet("A", "a"), Alphabet("B", "b"),
      Alphabet("A", "a"), Alphabet("B", "b")
    };
    Pattern p = { PUnit("A"), PUnit("B"), PUnit(), PUnit("A"), PUnit("B"), PUnit("A"), PUnit("B") };
    const int N = 10000;
    auto a = clock();
    for (int i=0;i<N;++i) preceq(t, p);
    auto b = clock();
    for (int i=0;i<N;++i) DP_preceq(t, p);
    auto c = clock();
    cout << (b-a) << " / " << (c-b) << endl;
  }
  return 0;
}
