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
## 201906 mkouda

---
### Agenda ###

* gdbを使ってpwnしてみよう
---

### 事前準備 ###
[ここ](https://github.com/halkichi0308/gdb-tutorial/raw/master/bin/runme)から実行ファイルをダウンロード

または`github`より

```
https://github.com/halkichi0308/gdb-tutorial/blob/master/bin/runme
```
---
### gdbの使い方 (基本編) ###
* まずはgdb起動
``` s
$ gdb
```

--
- 続いてgdbで実行ファイルを読み込む
``` s
gdb-peda$ file runme
```

--
- ブレークポイントをさします
``` s
gdb-peda$ b main
Breakpoint 1 at 0x80484ea
```
---
### gdbの使い方 (基本編) ###
* 実行します
``` s
gdb-peda$ run
```

--
`run :`ブレークポイントから実行。  
`run <arg>` とすれば引数もつけられます。


--
* 関数にもブレークを張れます。
``` s
gdb-peda$ b printf@plt
Breakpoint 1 at 0x8048350
```

--
* 以下で`b main`と同じようにブレークできます
``` s
gdb-peda$ r
```
---

### ここで一旦セクションについて学ぶ 1/4 ###
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
### セクションとは? 2/4 ###
&nbsp;

* `.data`セクションは`書き換わる事が前提`  
->この辺はアドレスの書き換えが狙いやすい??  

* 逆に`.text`、`.rodata`のバイナリが書き変わることは考えにくい

* `.got`セクションはプログラムがキャッシュを作ることを目的に書き込み可能になっていることがある(`No RELRO`もしくは`Partial RELRO`の状態)  GOT OVerwriteはこういった場合に成立する。

[pltはここを参照](https://tkmr.hatenablog.com/entry/2017/02/28/030528)  

&nbsp;
---
### セクションとは? 3/4 ###

`readelf -r got` や `objdump -h`で見れる  

![image](https://raw.githubusercontent.com/halkichi0308/gdb-tutorial/master/assets/section_objdump.png)

---
### セクションとは? 4/4 ###
* 各セクションの意味

![images](https://raw.githubusercontent.com/halkichi0308/gdb-tutorial/master/assets/section_elem.png)

&nbsp;

参考: http://www.ertl.jp/~takayuki/readings/info/no02.html

---
### gdbの使い方 (実践編)###

runmeからフラグを奪取してみましょう。
---
### gdbの使い方 (実践編)###

#### アプローチの方法 #### 

[1]. 実行してみる

``` s
$ ./runme
```
---
### gdbの使い方 (実践編)###

#### アプローチの方法 #### 

[2]. 実行ファイルの調査

``` s
gdb-peda$ pdisass main
Dump of assembler code for function main:
   0x080484e7 <+0>:	push   ebp
   0x080484e8 <+1>:	mov    ebp,esp
```

`runme`が怪しい...?
``` s
   0x08048526 <+63>:	call   0x804849d <runme>
   0x0804852b <+68>:	mov    DWORD PTR [esp],0x80485e7
   0x08048532 <+75>:	call   0x8048360 <puts@plt>
```

---
### gdbの使い方 (実践編)###

#### アプローチの方法 #### 

[3]. 調査してみる

``` s
gdb-peda$ checksec
CANARY    : disabled
FORTIFY   : disabled
NX        : ENABLED
PIE       : disabled
RELRO     : Partial
```

``` s
gdb-peda$ pdisass runme
```

---
.col-4[

動的に解析してみる  
実行プラグラムの中身はこんな形です。

]

.col-8[

<img src="https://raw.githubusercontent.com/halkichi0308/gdb-tutorial/master/assets/02.png" width="80%">

]

それではgdbを使って解析していきましょう。

---
- gdbの見方
- 現在のレジスタ

![images](https://raw.githubusercontent.com/halkichi0308/gdb-tutorial/master/assets/registor.png)

---
- 現在のフラグ(cmpなどの演算結果)

![images](https://raw.githubusercontent.com/halkichi0308/gdb-tutorial/master/assets/flag.png)

---
- 実行命令

![images](https://raw.githubusercontent.com/halkichi0308/gdb-tutorial/master/assets/function.png)

---
- スタック

![images](https://raw.githubusercontent.com/halkichi0308/gdb-tutorial/master/assets/stack.png)
---

- まずはここまでプログラムを進めてみます
![images](https://raw.githubusercontent.com/halkichi0308/gdb-tutorial/master/assets/01.png)

--
- $eaxにds:0x804a040の値を格納しているようです。


---
- 関数の第一引数にはEAXレジスタ($eax)がよく利用されるので、$eaxの箇所を参考にすると関数に何を渡しているか把握できます。

![images](https://raw.githubusercontent.com/halkichi0308/gdb-tutorial/master/assets/03.png)

---
- 関数の第一引数にはEAXレジスタ($eax)がよく利用されるので、$eaxの箇所を参考にすると関数に何を渡しているか把握できます。

![images](https://raw.githubusercontent.com/halkichi0308/gdb-tutorial/master/assets/03.png)

---
- `0x804a040`番地の値は以下のコマンドで確認できます。
``` s
gdb-peda$ x 0x804a040
0x804a040 <secret>:	0x12345678
```

--
- 現在のEAXレジスタの値は以下のコマンドで確認できます。
``` s
gdb-peda$ x 0x804a040
0x804a040 <secret>:	0x12345678
```

---
![images](https://raw.githubusercontent.com/halkichi0308/gdb-tutorial/master/assets/03.png)

- どうやらrunme関数の前にcmp -> jne が存在するためこのままでは実行できない様です。
---
- cmp命令でEAXレジスタ($eax)と0xffを比較しています。
![images](https://raw.githubusercontent.com/halkichi0308/gdb-tutorial/master/assets/04.png)

---
- cmp命令でEAXレジスタ($eax)と0xffを比較しています。
![images](https://raw.githubusercontent.com/halkichi0308/gdb-tutorial/master/assets/05.png)

---
- このままではjumpしてしまうのでレジスタの値を書き換えます。
``` s
gdb-peda$ set $eax = 0xff
```

---
- $eaxに0xffを格納することでjne命令を回避することができました。
![images](https://raw.githubusercontent.com/halkichi0308/gdb-tutorial/master/assets/06.png)


---
### jmp命令 ###
jmp命令はcmp命令によってセットされたflagの値を参照します。

* jne: (A != B)のときにjmp
* je:  (A == B)のときにjmp
* jg:  (A > B)の時にjmp

--
&nbsp;
- jump not equalと覚えれば簡単。

---
- もうひとつcmp命令があるのでやってみましょう
![images](https://raw.githubusercontent.com/halkichi0308/gdb-tutorial/master/assets/06.png)

---
- runmeを実行できましたか?

(ステップ実行)
``` s
gdb-peda$ s
```
--
- これでFlagを奪取できたかと思います。

---
- ソースはこちら

![images](https://raw.githubusercontent.com/halkichi0308/gdb-tutorial/master/assets/runme.png)

---
- 15文字ずつステップ実行するのは面倒なので
``` s
gdb-peda$ run > dump.txt
```
- とすると実行時に表示した文字列をファイルにダンプしてくれます。


---
### おわり ###



---

