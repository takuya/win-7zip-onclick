## 7-Zip をワンタッチで展開したい

レジストリ新規作成をして、登録

`7-Zip.desktop` というProgIdを作り、`zip`の関連付けを変更する。
```
Windows Registry Editor Version 5.00

[HKEY_CURRENT_USER\SOFTWARE\Classes\7-Zip.desktop]

[HKEY_CURRENT_USER\SOFTWARE\Classes\7-Zip.desktop\DefaultIcon]
@="\"C:\\Program Files (x86)\\Lhaplus\\LplsIcon.dll\",101"

[HKEY_CURRENT_USER\SOFTWARE\Classes\7-Zip.desktop\shell]

[HKEY_CURRENT_USER\SOFTWARE\Classes\7-Zip.desktop\shell\open]

[HKEY_CURRENT_USER\SOFTWARE\Classes\7-Zip.desktop\shell\open\command]
@="\"C:\\Users\\takuya\\.app\\7zipToDesktop.exe\" %1"

[HKEY_CURRENT_USER\SOFTWARE\Classes\.zip]
@="7-Zip.desktop"


```


## 展開先を指定したい。エクスプローラで開いてほしい

古のLhaplus という解凍ソフトは、展開先を指定することが出来た。

また、展開後にエクスプローラーを開いてくれて便利だった。

しかし、lhaplus は緩やかに死んでいるように思える。

仕方ないので、ラッパーを作ることにした。

## やってないこと

- インストーラーの作成とかやってない。
- 設定画面とか作ってない

ただ、デスクトップへZip展開してくれるだけ。またエクスプローラーが開いて、くれる

ソースコードがあるのだから、出力先はビルドで変えたらいいだけですしね。

## 制限事項

C++ 17 以上でコンパイルすること。

## ライセンス

GPLv3



