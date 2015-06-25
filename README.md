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
% cat sample-input
x_S x_S b_V c_C d_D e_E f_F
y_S b_V c_C d_D e_E f_F
```

一行が一文.
単語とその品詞とを `\_` でもって連接したものを空白区切りで書く.

`xx_SS` という単位を `string` で受け取って、
長さをnとしたとき、n-2文字目からさかのぼって最初に見つけた `_` で2つに区切る.

### 出力例

```
% mmg < sample-input
<S> <> c/C d/D e/E f/F
```

読み方をメモしておく。  
`<S>` は `S` というPOSの語がちょうど1つ入る変数。  
`<>` は任意の語の列が、ただし長さが1以上の列が入る変数。  
`c/C` は `C` というPOSの語である `c` というアルファベット。  

### デバッグモード (冗長モード)

```bash
% mmg -D < sample-input
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


