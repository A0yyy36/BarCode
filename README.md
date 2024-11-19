# バーコードリーダー
## 概要
このプログラムは，Arduino Unoとフォトリフレクタを用いて作成したハードウェアに対して，正確にバーコードを読み取るためのプログラムです．このプログラムを実行することで，バーコードを正確に読み取ることが出来ます．プログラムはC/C++をベースにしているArduino言語で記述しました．

## 作成経緯
大学3年生の前期に履修した「情報工学実験3」の授業において，Arduino Unoを使用してペン型バーコードリーダーを制作しました．この授業では，学生自身が計画を立案し，実装および検証を行うことが求められました．
作成したペン型バーコードリーダーは，読み取り部分が直接バーコードに触れる設計であるため，手振れや読み取り速度の変動により精度が低下するという課題がありました．そこで，ソフトウェア面での解決策として，以下のアルゴリズムの設計と実装を取り組みを行いました．
1. バーコードの基準幅を動的に調整するアルゴリズム
2. 読み取り時の誤差を軽減する近似アルゴリズム
これらの取り組みにより，全桁の正確な読み取りを実現しました．

## 回路図
図1は，Arduino Unoとフォトリフレクタを用いたバーコードリーダーの回路図を示しています．
<p align="center">
  <img src="https://github.com/A0yyy36/BarCode/blob/main/img/Schematic.png" />
</p>
<p align="center">図1 Arduino Unoとフォトリフレクタを用いたバーコードリーダー</p>

## プログラム説明
### メインプログラム
図2は，メインプログラム(BarCode.ino)のフローチャートを示しています．
<p align="center">
  <img src="https://github.com/A0yyy36/BarCode/blob/main/img/Flowchart.png" />
</p>
<p align="center">図2 BarCord.inoのフローチャート</p>

### 基準幅更新プログラム
このプログラムは，読み取り速度の変動に影響されることなく，動的にバーコードの比率を正確に算出します．基準幅更新プログラムのフローチャートを図3に示しています．
<p align="center">
  <img src="https://github.com/A0yyy36/BarCode/blob/main/img/UpdateBar.png" />
</p>
<p align="center">図3 基準幅更新プログラムのフローチャート</p>

### 近似配列格納プログラム
このプログラムは，読み取りエラーが発生しても正しい値に補正するアルゴリズムです．近似配列格納プログラムのフローチャートを図4に示しています．
<p align="center">
  <img src="https://github.com/A0yyy36/BarCode/blob/main/img/Approximate.png" />
</p>
<p align="center">図4 近似配列格納プログラムのフローチャート</p>
