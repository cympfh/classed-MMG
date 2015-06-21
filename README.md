# classed MMG


```
rho(p) =
  <A> -> a/A
  <> -> <A>
  <> -> <> <>
```

## 長さに上限を与えた下での生成言語サイズによるヒューリスティック

なんだかんだあって、結局パターンに含まれる `a/A` `<A>` の数のマイナスを
優先度とする `priority_queue` にした

## 使い方

### 入力フォーマット

```bash
% cat input
x x b c d e f
S S V C D E F
y b c d e f
S V C D E F
```

1行目に1文目、2行目に1文目の単語POSの列  
以下同様に、
(2n-1)行目にn文目、(2n)行目にn文目の単語POSの列

### 出力例

```
% mmg < input
<S> <> c/C d/D e/E f/F
```

読み方をメモしておく。  
`<S>` は `S` というPOSの語がちょうど1つ入る変数。  
`<>` は任意の語の列が、ただし長さが1以上の列が入る変数。  
`c/C` は `C` というPOSの語である `c` というアルファベット。  


### デバッグモード (冗長モード)

```bash
% mmg -D < input
>>> text = x/S x/S b/V c/C d/D e/E f/F
>>> text = y/S b/V c/C d/D e/E f/F
tighten: <>
 -> <> <>
 -> <S> <>
 -> <S> <> <>
 -> <S> <> <F>
 -> <S> <> f/F
 -> <S> <> <> f/F
 -> <S> <> <E> f/F
 -> <S> <> e/E f/F
 -> <S> <> <> e/E f/F
 -> <S> <> <D> e/E f/F
 -> <S> <> d/D e/E f/F
 -> <S> <> <> d/D e/E f/F
 -> <S> <> <C> d/D e/E f/F
 -> <S> <> c/C d/D e/E f/F
tighten end
<S> <> c/C d/D e/E f/F
```

今はこんなんだけど、今後増えるかもしれない


