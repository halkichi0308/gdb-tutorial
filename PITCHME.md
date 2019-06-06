# gdb-tutorial

**勉強会用**  
[こちら](https://gitpitch.com/halkichi0308/gdb-tutorial)でSlideを開いてくださいませ。

+++
### 事前準備 ###
ここから実行ファイルをダウンロード

+++
* まずはgdb起動
```
$ gdb
```

+++
* 実行ファイルを読み込んで
``` sh
gdb-peda$ file runme
```

* ブレークポイントをさします
``` sh
gdb-peda$ b main
Breakpoint 1 at 0x80484ea
```
+++
* 実行します
```
gdb-peda$ run
```
`run :`ブレークポイントから実行。  
`run <arg>` とすれば引数もつけられます。  

+++
* ステップオーバー/ステップイン  

ステップオーバー: サブルーチンの中はデバッグしない
```
gdb$ ni
```
usage: `printf`とか既に挙動がわかっている関数の中はデバッグしない。


ステップイン: サブルーチンの中にも入ってデバッグする  
```
gdb$ si
```
+++
