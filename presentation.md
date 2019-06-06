title: gdb-tutorial
class: animation-fade
layout: true

<!-- This slide will serve as the base layout for all your slides -->
.bottom-bar[
  {{title}}
]

---

class: impact

# {{title}}
## With a good subtitle :-)

---

### 事前準備 ###
[ここ](https://github.com/halkichi0308/gdb-tutorial/raw/master/bin/runme)から実行ファイルをダウンロード

または`github`より
``` url
https://github.com/halkichi0308/gdb-tutorial/blob/master/bin/runme
```
---
* まずはgdb起動
```
$ gdb
```

--
- 続いてgdbで実行ファイルを読み込む
```
gdb-peda$ file runme
```

--
- ブレークポイントをさします
```
gdb-peda$ b main
Breakpoint 1 at 0x80484ea
```
---
* 実行します
```
gdb-peda$ run
```

--
`run :`ブレークポイントから実行。  
`run <arg>` とすれば引数もつけられます。


--
* 関数にもブレークを張れます。
```
gdb-peda$ b printf@plt
Breakpoint 1 at 0x8048350
```

--
* 以下で`b main`と同じようにブレークできます
```
gdb-peda$ r
```
---

### ここで一旦セクションについて学ぶ 1/3 ###
* セクションとは?
プログラムを構成する要素。

--
.section[

セクション名|説明
:-|:-
.text|機械語となる部分を格納。バイナリはここがメイン
.data|初期値を持つ変数を格納。
.bss|0で初期化される変数。ここに格納された値は全て0になる。
.rodata|定数(const)を格納する。ここは書き換えが起こらない
.plt|動的リンクされた関数。リンカにより生成される
.got.plt|動的リンクのアドレステーブル。ここの値をプログラム実行時にインタプリタが書き換える
]

---
### セクションとは? 2/3 ###
&nbsp;

* `.data`セクションは`書き換わる事が前提`  
->この辺はアドレスの書き換えが狙いやすい??  

* 逆に`.text`、`.rodata`のバイナリが書き変わることは考えにくい

* `.got`セクションはプログラムがキャッシュを作ることを目的に書き込み可能になっていることがある(`No RELRO`もしくは`Partial RELRO`の状態)  GOT OVerwriteはこういった場合に成立する。

[pltはここを参照](https://tkmr.hatenablog.com/entry/2017/02/28/030528)  

&nbsp;
---
### セクションとは? 3/3 ###

`readelf -r got` や `objdump -h`で見れる  

```
$objdump -h runme
```
--

- Create slides with your **favorite text editor**
--

- Focus on your **content**, not the tool
--

- You can finally be **productive**!

---

# There's more

## Syntax highlighting

You can also add `code` to your slides:
```html
<div class="impact">Some HTML code</div>
```

## CSS classes

You can use .alt[shortcut] syntax to apply .big[some style!]

...or just <span class="alt">HTML</span> if you prefer.

---

# And more...

## 12-column grid layout

Use to the included **grid layout** classes to split content easily:
.col-6[
  ### Left column

  - I'm on the left
  - It's neat!
]
.col-6[
  ### Right column

  - I'm on the right
  - I love it!
]

## Learn the tricks

See the [wiki](https://github.com/gnab/remark/wiki) to learn more of what you can do with .alt[Remark.js]
