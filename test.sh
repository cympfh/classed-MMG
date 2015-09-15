for K in `seq 1 33`; do
  ./mmg -U -K $K < input/wikiquote > /tmp/a
  a=`cat /tmp/a | wc -l`
  b=`cat /tmp/a | grep '<>' | wc -l`
  ./mmg -K $K < input/wikiquote > /tmp/a
  c=`cat /tmp/a | wc -l`
  d=`cat /tmp/a | grep '<>' | wc -l`

  echo "| $K | $b/$a | $d/$c |"
done
