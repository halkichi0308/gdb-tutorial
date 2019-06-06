# gdb-tutorial

---
### 事前準備 ###
ここから実行ファイルをダウンロード

---
* まずはgdb起動
```
$ gdb
```

---
* 実行ファイルを読み込んで
``` sh
gdb-peda$ file runme
```

* ブレークポイントをさします
``` sh
gdb-peda$ b main
Breakpoint 1 at 0x80484ea
```
===
