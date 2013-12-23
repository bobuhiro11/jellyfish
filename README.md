jellyfish
========

小さなlisp処理系です．
まだRnRSに従っていません．
現状では[こんなプログラム](https://github.com/bobuhiro11/jellyfish/blob/master/test/test_integ.scm)が動いてくれます．

Requires
--------

字句・構文解析に，bisonとflexを使っています．

- [bison](http://www.gnu.org/software/bison/)
- [flex](http://flex.sourceforge.net/)

Usage
--------

- インストール

```
$ git clone git@github.com:bobuhiro11/jellyfish.git
$ cd jellyfish
$ make
```

- サンプルコードの実行

```
$ make test     # あるいは，$ bin/jellyfish test/test_integ.scm
```

- 対話式インタプリタ

```
$ make run      # あるいは，$ bin/jellyfish
```

- schemeソースコードの実行

```
$ bin/jellyfish filename.scm
```

License
-------

MIT License
