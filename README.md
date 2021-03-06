# 問題

3パズルゲーム (縦、横 3つ以上同じ柄を並べたらブロックが消える)

# 考察

任意の 2点間の宝石を交換することが可能。

* 最初の連鎖で消せる最大の数は 5 が最大
    * 任意の 2箇所を消せるので挟み込むような形にすると 12 はいけるはず。
        * 16 も可能なことがわかった（逆T字型を2つ消す)
* 盤面のサイズによる影響はあるか
    * 探索のサイズが上がる、仮にビームサーチを適用した場合にはビーム幅が減少する
* スコア計算が指数的なので消すときはなるべく同時にたくさん消えるようにする。
* コンボ数によるスコアは存在するが線形的な増加。
* 連鎖の型を考えてそれに合わせる？
    * シンプルな探索だとあまりよくなさそう。
        * 合法手の数が単純に多い
        * 8 x 8 の盤面でも 4000 ぐらい（実際には同色のスワップは候補から消すのでこれよりかは少ない）
        * パズドラの場合は指の移動ルートなので移動に制限があるので深く探索することが可能（隣接するマスの 8 方向）
* 焼きなましの方針が良さそう
* 最終的に欲しいのは発火する前の盤面状態
    * 最初の発火の手順が決まっていればどうにでもなる

## スコア計算

合計のスコアは「ラインスコア * コンボ数」で計算される。 コンボ数とラインスコアは独立しているので「コンボの最後あたりでたくさん消えるようにする」といった戦略は取る必要が無い。
落ちコンでコンボは増えても、ラインの長さを増やすのは難しいので、盤面のブロックを使ってラインスコアは取りに行ったほうが良い。

落ちコンが存在しているので、大連鎖を組むと落下による暴発が発生して意図しない連鎖が発生する可能性がある。

# 実装

## グリッドの実装

* 横向きに実装する。(落下処理が早くなる)
* 番兵を入れる (盤外処理が楽になる)

## 各盤面サイズでの連鎖型の構築

* サイズごとに最適な連鎖パターンがあるので、それを組み込みで入れ込む
* スコアを取るための型と連鎖を増やすための型を別々で用意して、この２つをマージすることで生成したい盤面の構築を行う。

## 連鎖処理の高速化

## 現状の盤面から、任意の盤面への変換手順の最適化

  * 変換可能かどうかの確認方法について調べる
  * ナイーブに変換していく場合、途中で暴発が発生しないように注意する必要がある。