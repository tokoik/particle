# particle

## 1. C++プロジェクトの新規作成

実験には何かの道具が必要になります。もし、それに使える既存のアプリケーションやサービス、あるいは、他の人が作ったプログラムやライブラリが存在すれば、それを使うに越したことはありません。しかし、実験の内容を満たす道具が無かったり、同じ目的の異なる手法を開発しようとするなら、ある程度道具は自分で作る必要があります。ここではプログラミングの練習も兼ねて、できるだけ何もないところから、目的のプログラムを作成することにします。

### 1.1 新しい C++ プロジェクトの作成

Visual Studio 2022 を起動して「新しいプロジェクトの作成(N)」を選びます。

![新しいプロジェクトの作成](images/fig00.png)

「空のプロジェクト」を作成します。

![空のプロジェクト](images/fig01.png)

### 1.2 main.cpp の追加

「プロジェクト」メニューから「新しい項目の追加(W)...」を選びます。

![新しい項目の追加](images/fig02.png)

追加する項目のファイル名を指定します。ここでは main.cpp とします。

![追加する項目のファイル名は main.cpp](images/fig03.png)

ソースプログラムを入力します。

![ソースプログラムの入力](images/fig04.png)

ソースプログラムの内容は、次のようなものです。何もしないプログラムです。

```cpp
auto main() -> int
{
}
```

### 1.3 プログラムのビルドと実行

「▷ローカル Windows デバッガー」をクリックして、プログラムのビルドと実行を行います。

![プログラムのビルドと実行](images/fig05.png)

ビルドに成功するとプログラムが実行されて、次のようなコンソールウィンドウが現れます。このウィンドウ上で任意のキーをタイプするかクローズボックスをクリックして、ウィンドウを閉じます。

![プログラムの停止](images/fig06.png)

## [ステップ 01](https://github.com/tokoik/particle/blob/step01/README.md)

## 2. ライブラリの追加

OpenGL を使ったアプリケーションの作成を手助けするために、[GLFW](https://www.glfw.org/)、[GLEW](https://glew.sourceforge.net/)、[GLM](https://github.com/g-truc/glm) の 3 つのライブラリを作成したプロジェクトに追加します。これらは、それぞれの Web ページからソースファイルやコンパイル済みのバイナリファイルを入手することができますが、Visual Studio では [NuGet](https://www.nuget.org/) というパッケージマネージャを使って、作成したプロジェクトに組み込むことができます。

### 2.1 GLFW の追加

OpenGL はプラットフォームに依存しないグラフィックスライブラリですが、アプリケーションが OpenGL を使ってグラフィックス表示をしようとするときには、プラットフォームに依存した手順でお膳立てをしてやる必要があります。GLFW は、そのお膳立てをするためのレームワーク（ソフトウェア開発を手助けするための枠組み）です。これをインストールするために、まず「プロジェクト」メニューから「NuGetパッケージの管理(N)...」を選びます。

![NuGetパッケージの管理](images/fig07.png)

「参照」を選んで「glfw」を検索し、glfwのパッケージを選んで「インストール」をクリックします。

![GLFW の追加](images/fig08.png)

「適用」をクリックします。

![適用](images/fig09.png)

### 2.2 GLEW の追加

同様にして、GLEW というライブラリをインストールします。これは Windows が標準ではサポートしていない OpenGL のバージョン 1.1 より後の機能を有効にします。「glew」を検索して最新版の2.2.0 を選び、「インストール」をクリックします。なお、前記の GLEW のプロジェクトのサイトでは最新版は 2.1.0 になっていますが、GitHub 上の最新版は 2.2.0 になっています (2025 年 5 月 23 日時点)。

![GLEW の追加](images/fig10.png)

### 2.3 GLM の追加

最後に、GLM をインストールします。これは OpenGL を使ってプログラミングする際に便利なデータ型や数学関数を集めたものです。「glm」を検索し、glmのパッケージを選んで「インストール」をクリックします。

![GLM の追加](images/fig11.png)

NuGet のタブはもう使わないので閉じて構いません。

![NuGet のタブを閉じる](images/fig12.png)

## [ステップ 02](https://github.com/tokoik/particle/blob/step02/README.md)

## 3. ウィンドウ関連の処理

ウィンドウの作成やマウス・キーボード入力などの処理には GLFW を使いますが、これらの処理をプログラムのあちこちに書くと、メインの処理が読みづらくなるので、Window というラッパークラスにまとめます。このあたりのことについては「GLFWによるOpenGL入門」を復習してください。

### 3.1 Window クラスの追加

「プロジェクト」メニューから「クラスの追加(C)...」を選びます。

![クラスの追加](images/fig13.png)

「クラス(L)」名は「Window」にします。「.h ファイル」と 「.cpp ファイル」のファイル名はクラス名から自動的に設定されますが、ここでは「インライン(I)」にチェックを入れて、「.h ファイル」だけを作成します。

![クラス名の指定](images/fig14.png)

### 3.2 ヘッダファイルの追加

Windows.h に以下の内容を追加します。この最初で OpenGL 関連のヘッダファイルを `#include` します。OpenGL を使うときは GL/gl.h を`#include` するのが一般的でしたが、Windows が正式にサポートしている OpenGL のバージョンは 1.1 までで、それ以降の新しい機能はビデオカードのドライバが提供していても定義されていません。そのため、ここでは代わりに GL/glew.h を使います。

また、このプログラムでは Windows の機能を使う必要があるウィンドウの生成や管理、またキーボードやマウスなどによるイベントの処理などに GLFW を使いますので、GLFW/glfw3.h を#include します。なお、このヘッダファイルは内部で GL/gl.h を `#include` するので、GLEW を使う場合は、この前に GL/glew.h を `#include` する必要があります。

```cpp
#pragma once

// OpenGL 関連
#include <GL/glew.h>
#include <GLFW/glfw3.h>
```

このプログラムでは、OpenGL / GLSL に対応したデータ型を取り扱うことのできる数学関数ライブラリ [GLM](https://github.com/g-truc/glm) を使用します。そのヘッダファイルもここで `#include` します。なお、`_USE_MATH_DEFINES` は GLM と直接関係ありませんが、これを定義しないと Visual Studio では C 言語や C++ 言語でよく使われる `M_PI` などの数学定数が定義されません。また、GLM の以前のバージョンではデフォルトでは角度の単位が °(度) なので、`GLM_FORCE_RADIANS` を定義してラジアンに切り替えます。

```cpp
// GLM 関連
#if !defined(_USE_MATH_DEFINES)
#  define _USE_MATH_DEFINES
#endif
#if !defined(GLM_FORCE_RADIANS)
#  define GLM_FORCE_RADIANS
#endif
#include <GLM/glm.hpp>
```

### 3.3 Window クラスのプライベートメンバ

`Window` クラスのプライベートメンバには、ウィンドウを作成したときに作られるウィンドウの識別子 `window` と、ウィンドウのサイズ `size` を保持するようにします。`window` の内容は開いたウィンドウが破棄されるまで変化しないので `const` にしておきます。また `size` は投影変換行列を作る時などに頻繁に使われる画面の縦横比（アスペクト比）や、マウスカーソルのウィンドウ上での相対的な位置の算出に必要になるので、ウィンドウのサイズが変更されたときに算出して保持するようにします。`glm::dvec2` は数値の精度が `double` 型の 2 次元ベクトルのデータ型です。数値の精度が `float` 型の 2 次元ベクトルは `glm::vec2` です。

```cpp
///
/// ウィンドウ関連の処理クラス
///
class Window
{
  /// ウィンドウの識別子
  GLFWwindow* const window;

  /// ウィンドウのサイズ
  glm::dvec2 size;
```

### 3.4 ウィンドウサイズ変更時の処理

ウィンドウのサイズを変更したときに呼び出すメソッド `resize()` を定義します。このメソッドはコールバック関数として用いるので、`static` メソッドにする必要があります。しかし、`static` メソッドはインスタンスに属するメンバ変数の `window` や `size` を参照することはできません。このメソッドはサイズが変更されたウィンドウに対して呼び出されるので、対象のオブジェクト自体は存在します。

そこで後述するコンストラクタにおいて、ウィンドウのオブジェクトを生成したときに、その実体を指す `this` ポインタを GLFW のライブラリの方に保存しておきます。そして、`resize()` が呼び出されたときにライブラリから `this` ポインタを取り出し、それを介してオブジェクトのメンバ変数にアクセスします。

なお、`resize()` の引数の `width` と `height` には、ウィンドウのディスプレイ上の幅と高さが与えられますが、これらと OpenGL のフレームバッファのサイズは、必ずしも一致していません。近年の高密度のディスプレイを採用した PC の中には、画面上のサイズよりも高い解像度のフレームバッファをもつものがあり (macOS)、そのような PC で `width` と `height` を使ってビューポートを設定すると、ビューポートが開いたウィンドウより小さく設定されてしまいます。そのため、`glfwGetFramebufferSize()` を使って実際のフレームバッファのサイズを調べます。

```cpp
  ///
  /// ウィンドウのサイズ変更時の処理
  ///
  /// @param[in] window サイズ変更の対象のウィンドウの識別子
  /// @param[in] width サイズ変更後のウィンドウの幅
  /// @param[in] height サイズ変更後のウィンドウの高さ
  ///
  /// @note glfwSetWindowSizeCallback() で登録するコールバック関数
  ///
  static auto resize(GLFWwindow* window, int width, int height) -> void
  {
    // window が保持するインスタンスの this ポインタを得る
    const auto instance{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };

    // インスタンスからの呼び出しでなければ戻る
    if (instance == nullptr) return;

    // インスタンスのウィンドウのサイズを更新する
    instance->size = { width, height };

    // フレームバッファの大きさ
    int fbWidth, fbHeight;

    // フレームバッファの大きさを得る
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

    // フレームバッファ全体をビューポートにする
    glViewport(0, 0, fbWidth, fbHeight);
  }
```

### 3.5 コンストラクタ

メンバ変数の `window` と `size` はコンストラクタの初期化しリストで初期化します。`window` は `glfwCreateWindow()` で作成したウィンドウの識別子を保持します。`size` は作成したウィンドウのサイズで、これはコンストラクタ内で呼び出している `resize()` でも設定しますが、プライベートメンバは初期化しておくべきなので、作成したウィンドウの縦横比で初期化しておきます。

コピーコンストラクタは削除しておきます。`Window` クラスはメンバの `window` は、ディスプレイ上のウィンドウという「実際のオブジェクト」を示しています。したがって `Window` クラスに複数のインスタンスが存在すると、同じウィンドウに対して異なる状態を持たせることが可能になり、矛盾が生じます。また、デストラクタで `glfwDestroyWindow()` を呼び出してウィンドウを破棄するようにしていると、一つのインスタンスを破棄した後、残りのインスタンスを破棄する際に実際のオブジェクトであるウィンドウ」が存在しなくなるため、プログラムをクラッシュさせるようなエラーを引き起こしてしまいます。

これと同様に、インスタンスのコピーを禁止するために、代入演算子も削除しておきます。`Window` クラスのメンバの `window` は `const` にしているので、もとよりインスタンスに別のインスタンスを代入をすることができませんが、代入演算子を削除することによって、インスタンスのコピーの禁止を明確に示しておきます。

```cpp
public:

  ///
  /// コンストラクタ
  ///
  /// @param[in] width ウィンドウの幅
  /// @param[in] height ウィンドウの高さ
  /// @param[in] title ウィンドウのタイトル
  ///
  Window(int width = 640, int height = 480, const char* title = "GLFW Window") :

    // ウィンドウを生成して識別子を保存する
    window{ glfwCreateWindow(width, height, title, nullptr, nullptr) },

    // 開いたウィンドウのサイズを保存する
    size{ width, height }
  {
    // ウィンドウが開けなければ戻る
    if (window == nullptr) return;

    // 現在のウィンドウを処理対象にする
    glfwMakeContextCurrent(window);

    // 表示はディスプレイのリフレッシュレートに同期させる
    glfwSwapInterval(1);

    // このインスタンスの this ポインタを記録しておく
    glfwSetWindowUserPointer(window, this);

    // ウィンドウのサイズ変更時に呼び出す処理を登録する
    glfwSetWindowSizeCallback(window, resize);

    // 開いたウィンドウに初期設定を適用する
    resize(window, width, height);
  }

  // コピーコンストラクタは使用しない
  Window(const Window& window) = delete;

  // ムーブコンストラクタはデフォルトのものを使用する
  Window(Window&& window) = default;

  ///
  /// デストラクタ
  ///
  virtual ~Window()
  {
    // ウィンドウを破棄する
    glfwDestroyWindow(window);
  }

  // 代入演算子は使用しない
  Window& operator=(const Window& window) = delete;

  // ムーブ代入演算子はデフォルトのものを使用する
  Window& operator=(Window&& window) = default;
```

### 3.6 そのほかのメンバ関数

そのほかのメンバ関数を定義します。`get()` メソッドは、対象のウィンドウの識別子を取り出します。この戻り値が `nullptr` でないことを調べれば、ウィンドウが生成されたことを確認できます。

```cpp
  ///
  /// ウィンドウの識別子を取り出す
  ///
  /// @return ウィンドウの識別子
  ///
  auto get() const
  {
    // ウィンドウの識別子を返す
    return window;
  }
```

描画ループの継続判定を行うメソッドを定義します。これを `bool` をオーバーライドして定義することにより、インスタンスを `if ( ... )` や `while ( ... )` の `( ... )` の中などの論理コンテキストで評価したときに呼び出すことができます。

この中では、まず `glfwPollEvents()` によりマウスやキーボードの操作などにより発生したイベントを取り出し、その結果を `glfwWindowShouldClose()` により判定して、ウィンドウが閉じられた場合には、ループを継続しないとして `false` を返します。`glfwPollEvents()` や `glfwWaitEvents()` を使ってイベントの取り出しを行わないと、例えばウィンドウが閉じられたこと（これもイベント）を `glfwWindowShouldClose()` が検出できません。

```cpp
  ///
  /// 描画ループの継続判定
  ///
  /// @return 描画を継続する場合は true
  ///
  explicit operator bool() const
  {
    // イベントを取り出す
    glfwPollEvents();

    // ウィンドウを閉じないなら true を返す
    return glfwWindowShouldClose(window) == GLFW_FALSE;
  }
  ```

  処理対象のウィンドウ `window` に対する描画が完了した後に `glfwSwapBuffers()` を呼び出すメソッドを定義します。

```cpp
  ///
  /// ダブルバッファリング
  ///
  auto swapBuffers() const
  {
    // カラーバッファを入れ替える
    glfwSwapBuffers(window);
  }
```

そのほか、処理対象のウィンドウのサイズや縦横比を調べるメソッドを用意しておきます。

```cpp
  ///
  /// ウィンドウのサイズを取り出す
  ///
  /// @return ウィンドウのサイズ
  ///
  const auto& getSize() const
  {
    // ウィンドウのサイズを返す
    return size;
  }

  ///
  /// ウィンドウの縦横比を取り出す
  ///
  /// @return ウィンドウの縦横比
  ///
  auto getAspect() const
  {
    // ウィンドウのサイズから縦横比を計算して返す
    return static_cast<GLfloat>(size.x / size.y);
  }
};
```

## [ステップ 03](https://github.com/tokoik/particle/blob/step03/README.md)

## 4. OpenGL の処理

GLFW と GLEW を使って OpenGL を使うアプリケーションを作成する基本的な手順を main.cpp に組み込みます。

### 4.1 main.cpp の変更

main.cpp を次のように書き換えます。まず、Windows の OpenGL のライブラリファイル openg32.lib をリンクする設定を埋め込みます。これは Visual Studio のプロジェクトの「設定」でも可能ですが、その手順を説明するのが面倒なので、`#pragma` を使ってソースプログラムに埋め込みます。その後、3. で作成した Window.h を `#include` します。

`Window` クラスのオブジェクト `window` が真の間、描画ループを継続するようにします。

```cpp
// Windows の OpenGL ライブラリをリンクする
#pragma comment(lib, "opengl32.lib")

// ウィンドウ関連の処理
#include "Window.h"

// 標準ライブラリ
#include <iostream>
```

メインプログラムでは、最初に `glfwInit()` で GLFW を初期化します。これが成功したら、プログラムの終了時に忘れずに `glfwTerminate()` を実行するように、これを `atexit()` で登録しておきます。

```cpp
///
/// メインプログラム
///
/// @return プログラムが正常に終了した場合は 0
///
auto main() -> int
{
  // GLFW を初期化する
  if (glfwInit() == GL_FALSE)
  {
    // GLFW の初期化に失敗したのでエラーメッセージを出して
    std::cerr << "Can't initialize GLFW" << std::endl;

    // 終了する
    return EXIT_FAILURE;
  }

  // プログラム終了時の処理を登録する
  atexit(glfwTerminate);
```

`Window` クラスのインスタンスを一つ生成し、ウィンドウを開きます。そのウィンドウの識別子を取り出し、`nullptr` でないことを確認します。

```cpp
  // ウィンドウを作成する
  Window window;

  // ウィンドウが作成できなかったら
  if (window.get() == nullptr)
  {
    // エラーメッセージを出して
    std::cerr << "Can't create GLFW window." << std::endl;

    // 終了する
    return EXIT_FAILURE;
  }
```

ウィンドウの作成に成功すれば、OpenGL の（レンダリング）コンテキストが作成されているので、`glewInit()` を呼び出して OpenGL の新しい機能を有効にします。なお、その前に `glewExperimental` という大域変数に `GL_TRUE` を代入しておき、すべての OpenGL の機能が使えるようにしておきます。

```cpp
  // GLEW の初期化時にすべての API のエントリポイントを見つけるようにして
  glewExperimental = GL_TRUE;

  // GLEW を初期化する
  if (glewInit() != GLEW_OK)
  {
    // GLEW の初期化に失敗したのでエラーメッセージを出して
    std::cerr << "Can't initialize GLEW" << std::endl;

    // 終了する
    return EXIT_FAILURE;
  }
```

`glClear(GL_COLOR_BUFFER_BIT ...)` でウィンドウ内でクリアするときの色を `glClearColor()` で指定した後、ループの継続条件として `Window` クラスのインスタンスである `window` を論理コンテキストで評価します。ループ内では `glClear()` でカラーバッファとデプスバッファを消去した後、最後に `window.swapBuffers()` を呼び出して、カラーバッファの入れ替えをします。このときに、ループ内で発生したウィンドウサイズの変更やマウス・キーボード操作などのイベントの取り出しを行います。

```cpp
  // 背景色を指定する
  glClearColor(0.2f, 0.3f, 0.4f, 0.0f);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
    // ウィンドウを消去する
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }
}
```

### 4.2 エラーチェックの追加

OpenGL のライブラリ関数、すなわち API (Application Program Interface) は、正しい設定や順序で呼び出さなければ、エラーが発生して期待した通りに動作しません。このエラーはプログラムの実行時に発生しますが、エラーメッセージなどは表示されないため、どこでどのようなエラーが発生しているのかを知るのは困難です。また、エラー発生やその内容は API の呼び出し直後に `glGetError()` を使って調べることができますが、 プログラム中の複数の場所にこの関数を置くと、どこに置いたものがエラーを検出したのかがわからなくなってしまいます。

そこでマクロを使って、エラーが発生した場所を付加してエラーメッセージを表示するマクロ関数 `errorcheck()` を定義します。この関数はデバッグビルドの時だけ有効になるようにし、リリースビルドでは何もしないようにします。これは errorcheck.h というヘッダファイルに置きます。

```cpp
#pragma once

#if defined(_DEBUG)
///
/// OpenGL のエラーをチェックする
///
/// @param[in] name エラー発生時に標準エラー出力に出力するファイル名などの文字列。nullptr なら何も出力しない。
/// @oaram[in] line エラー発生時に標準エラー出力に出力する行番号などの整数値。
///
extern auto _errorcheck(const char* name, unsigned int line) -> void;

///
/// OpenGL のエラーの発生を検知したときにソースファイル名と行番号を表示する。
///
/// @def errorcheck()
///
/// @note このマクロを置いた場所（より前）で OpenGL のエラーが発生していた時に、
/// このマクロを置いたソースファイル名と行番号を出力する。
/// リリースビルド時には無視する。
///
#  define errorcheck() _errorcheck(__FILE__, __LINE__)
#else
#  define errorcheck()
#endif
```

マクロ関数 `errorcheck()` が展開されて呼び出される実際の関数 `_errorcheck()` は、`glGetError()` の戻り値からエラーメッセージを生成し、引数に与えられたファイル名 `name` と行番号 `line` を付加して、標準エラー出力 `std::cerr` に出力します。

```cpp
// デバッグ用関数の定義
#include "errorcheck.h"

// デバッグモードのときだけ定義する
#if defined(_DEBUG)

// OpenGL の関数や定数の宣言
#include <GL/glew.h>

// エラーメッセージの出力用
#include <iostream>

///
/// OpenGL のエラーをチェックする
///
/// @param[in] name エラー発生時に標準エラー出力に出力するファイル名などの文字列。nullptr なら何も出力しない。
/// @oaram[in] line エラー発生時に標準エラー出力に出力する行番号などの整数値。
///
auto _errorcheck(const char* name, unsigned int line) -> void
{
  const GLenum error{ glGetError() };

  if (error != GL_NO_ERROR)
  {
    if (name != nullptr && *name != '\0')
    {
      std::cerr << name;
      if (line > 0) std::cerr << " (" << line << ")";
      std::cerr << ": ";
    }

    switch (error)
    {
    case GL_INVALID_ENUM:
      std::cerr << "An unacceptable value is specified for an enumerated argument" << std::endl;
      break;
    case GL_INVALID_VALUE:
      std::cerr << "A numeric argument is out of range" << std::endl;
      break;
    case GL_INVALID_OPERATION:
      std::cerr << "The specified operation is not allowed in the current state" << std::endl;
      break;
    case GL_OUT_OF_MEMORY:
      std::cerr << "There is not enough memory left to execute the command" << std::endl;
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      std::cerr << "The specified operation is not allowed current frame buffer" << std::endl;
      break;
    default:
      std::cerr << "An OpenGL error has occured: " << std::hex << std::showbase << error << std::endl;
      break;
    }
  }
}
#endif
```

このヘッダファイルをエラーチェックを行うソースプログラム、ここでは main.cpp で `#include` します。

```cpp
// Windows の OpenGL ライブラリをリンクする
#pragma comment(lib, "opengl32.lib")

// ウィンドウ関連の処理
#include "Window.h"

// OpenGL のエラーチェック
#include "errorcheck.h"

// 標準ライブラリ
#include <iostream>
```

実際にエラーチェックを行いたいところに `errorcheck()` を置きます。

```cpp
  // 背景色を指定する
  glClearColor(0.2f, 0.3f, 0.4f, 0.0f);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
    // ウィンドウを消去する
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // OpenGL のエラーが発生していないかチェックする
    errorcheck();

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }
}
```

## [ステップ 04](https://github.com/tokoik/particle/blob/step04/README.md)

## 5. シェーダの読み込み処理

shader.cpp というファイルを作成し、そこにシェーダ関連の関数を定義します。また、定義した関数を main.cpp から参照できるように、外部から参照する関数の宣言を shader.h というヘッダファイルに置きます。

### 5.1 shader.cpp の作成

main.cpp の作成と同様に、「プロジェクト」メニューから「新しい項目の追加(W)...」を選び、shader.cpp というファイルを作成します。

### 5.2 ヘッダファイルの読み込み

この shader.cpp で定義する関数の内容と、その関数の宣言するヘッダファイル shader.h の内容が矛盾しないように、shader.h を最初に `#include` しておきます。また、この処理ではコンソール入力やファイル入力、および動的配列 `std::vector` を使いますから、それらを宣言するヘッダファイルも も `#include` しておきます。

```cpp
// シェーダ関連の処理の実装
#include "shader.h"

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <sstream>
```

### 5.3 シェーダのコンパイル結果を表示する関数

シェーダのコンパイルは、プログラムの実行中に OpenGL のライブラリを使って行います。したがって、エラーメッセージを表示する場合も、ライブラリが保持しているメモリから取り出す必要があります。この関数は他のところから使用されないので、`static` にしておきます。

```cpp
///
/// シェーダオブジェクトのコンパイル結果を表示する
///
/// @param[in] shader シェーダオブジェクト名
/// @param[in] str コンパイルエラーが発生した場所を示す文字列
/// @return コンパイル結果が正常ならば GL_TRUE、異常ならば GL_FALSE
///
static auto printShaderInfoLog(GLuint shader, const std::string& str) -> GLboolean
{
  // コンパイル結果を取得する
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) std::cerr << "Compile Error in " << str << std::endl;

  // シェーダのコンパイル時のログの長さを取得する
  GLsizei bufSize;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

  // シェーダのコンパイル時のログがあるなら
  if (bufSize > 1)
  {
    // ログの内容を取得する
    std::string infoLog(bufSize, '\0');
    GLsizei length;
    glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);

    // ログの内容を表示する
    std::cerr << infoLog << std::endl;
  }

  // コンパイル結果を返す
  return status;
}
```

### 5.4 シェーダのリンク結果を表示する関数

コンパイルされたシェーダオブジェクトのリンクもプログラムの実行中に OpenGL のライブラリを使って行いますから、そのエラーメッセージもメモリから取り出す必要があります。この関数も他のところから使用されないので、`static` にしておきます。

```cpp
///
/// プログラムオブジェクトのリンク結果を表示する
///
/// @param[in] program プログラムオブジェクト名
/// @return リンク結果が正常ならば GL_TRUE、異常ならば GL_FALSE
///
static auto printProgramInfoLog(GLuint program) -> GLboolean
{
  // リンク結果を取得する
  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) std::cerr << "Link Error." << std::endl;

  // シェーダのリンク時のログの長さを取得する
  GLsizei bufSize;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

  // シェーダのリンク時のログがあるなら
  if (bufSize > 1)
  {
    // ログの内容を取得する
    std::string infoLog(bufSize, '\0');
    GLsizei length;
    glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);

    // ログの内容を表示する
    std::cerr << infoLog << std::endl;
  }

  // リンク結果を返す
  return status;
}
```

### 5.5 シェーダオブジェクトを作成する関数

シェーダのソースをコンパイル・リンクして作成したシェーダプログラムの実体を「プログラムオブジェクト」と言います。この手順は、次のようになります。

1. `glCreateProgram()` を使って、プログラムオブジェクトを作成します。
2. `glCreateShader(引数)` でシェーダオブジェクトを作成します。「引数」には、バーテックスシェーダのシェーダオブジェクトを作成するときは `GL_VERTEX_SHADER`、フラグメントシェーダのシェーダオブジェクトを作成するときは `GL_FRAGMENT_SHADER` を指定します。
3. `glShaderSource()` を使ってシェーダのソースプログラムをシェーダオブジェクトのメモリに読み込みます。ソースプログラムは単一の文字列もしくは文字列の配列 (1 行 1 文字列) です。
4. `glCompileShader()` を使って読み込んだソースプログラムをコンパイルします。
5. バーテックスシェーダ `GL_VERTEX_SHADER` とフラグメントシェーダ `GL_FRAGMENT_SHADER` がコンパイルできたら、それぞれをプログラムオブジェクトに組み込んだのち、`glLinkProgram()` で各シェーダオブジェクトをリンクします。

バーテックスシェーダとフラグメントシェーダのシェーダオブジェクトの作成手順は同じですから、ソースプログラムをコンパイルしてシェーダオブジェクトを作成し、プログラムオブジェクトに組み込む関数を作ります。この関数も他のところから使用されないので、`static` にしておきます。

```cpp
///
/// シェーダオブジェクトを作成してプログラムオブジェクトに組み込む
///
/// @param program シェーダオブジェクトを組み込むプログラムオブジェクトの名前
/// @param src シェーダのソースプログラムの文字列
/// @param msg メッセージに追加する文字列
/// @param type シェーダの種類（GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER）
/// @return シェーダオブジェクトの作成とプログラムオブジェクトへの組み込みに成功したら true
///
static auto createShader(GLuint program, const std::string& src, const std::string& msg,
  GLenum type) -> bool
{
  // プログラムオブジェクトが作成できていなかったら
  if (program == 0)
  {
    // エラーメッセージを表示して戻る
    std::cerr << "Error: Could not create program object." << std::endl;
    return false;
  }

  // シェーダオブジェクトを作成する
  const auto shader{ glCreateShader(type) };

  // シェーダオブジェクトが作成できなかったら
  if (shader == 0)
  {
    // エラーメッセージを表示して戻る
    std::cerr << "Error: Could not create shader object." << std::endl;
    return false;
  }

  // シェーダオブジェクトにソースプログラムの文字列を設定してコンパイルする
  auto ptr{ src.c_str() };
  glShaderSource(shader, 1, &ptr, nullptr);
  glCompileShader(shader);

  // コンパイル時のメッセージを表示してコンパイル結果を得る
  const auto status{ printShaderInfoLog(shader, msg) != GL_FALSE };

  // エラーが無ければシェーダオブジェクトをプログラムオブジェクトに組み込む
  if (status) glAttachShader(program, shader);

  // シェーダオブジェクトに削除マークを付ける
  glDeleteShader(shader);

  // コンパイル結果を返す
  return status;
}
```

### 5.6 プログラムオブジェクトを作成する関数

`createProgram()` はバーテックスシェーダとフラグメントシェーダの両方のソースプログラムを受け取ってコンパイル・リンクし、プログラムオブジェクトを作成します。このリンクの際、フラグメントシェーダのプログラムオブジェクトは必ずしも必要ありません。これは Transform Feedback などで `glEnable(GL_RASTERIZER_DISCARD)` としてラスタライザを無効にしたときに、フラグメントシェーダが不要になるためです。そのため、フラグメントシェーダのソースプログラムが空文字列のときは、バーテックスシェーダのシェーダオブジェクトだけをリンクするようにしています。

```cpp
///
/// プログラムオブジェクトを作成する
///
/// @param[in] vsrc バーテックスシェーダのソースプログラムの文字列
/// @param[in] fsrc フラグメントシェーダのソースプログラムの文字列
/// @param[in] vmsg バーテックスシェーダのコンパイル時のメッセージに追加する文字列
/// @param[in] fmsg フラグメントシェーダのコンパイル時のメッセージに追加する文字列
/// @return 作成したプログラムオブジェクトの名前、作成できなかった場合は 0
///
/// @note この関数は、
/// バーテックスシェーダとフラグメントシェーダの両方のソースプログラムを受け取りますが、
/// フラグメントシェーダのソースプログラムは空文字列でも構いません。
/// これは Transform Feedback を使用する場合などで、
/// glEnable(GL_RASTERIZER_DISCARD) としてラスタライザを無効にしたときには、
/// フラグメントシェーダが不要になるためです。
/// なお、バーテックスシェーダのソースプログラムがコンパイルエラーになった場合は、
/// フラグメントシェーダのコンパイルは行いません。
///
auto createProgram(const std::string& vsrc, const std::string& fsrc,
  const std::string& vmsg, const std::string& fmsg) -> GLuint
{
  // バーテックスシェーダのソースの文字列が与えられていなければ
  if (vsrc.empty())
  {
    // エラーメッセージを表示して 0 を返す
    std::cerr << "Error: No vertex shader specified." << std::endl;
    return 0;
  }

  // 空のプログラムオブジェクトを作成する
  const auto program{ glCreateProgram() };

  // バーテックスシェーダの作成と組み込みに成功したら
  if (createShader(program, vsrc, vmsg, GL_VERTEX_SHADER))
  {
    // フラグメントシェーダがないかフラグメントシェーダの作成と組み込みに成功したら
    if (fsrc.empty() || createShader(program, fsrc, fmsg, GL_FRAGMENT_SHADER))
    {
      // プログラムオブジェクトをリンクして
      glLinkProgram(program);

      // エラーがなければプログラムオブジェクトを返す
      if (printProgramInfoLog(program)) return program;
    }
  }

  // エラーのときはプログラムオブジェクトを削除して 0 を返す
  glDeleteProgram(program);
  return 0;
}
```

### 5.7 ソースファイルを読み込む関数

`glCompileShader()` は `glShaderSource()` により文字列として与えられたソースプログラムを読み込んでコンパイルします。また、これらを使って定義した `createProgram()` も、ソースプログラムを文字列で受け取ります。これは、シェーダのソースプログラムを C++ のソースプログラム中に文字列として埋め込む場合は問題ないのですが、それだとシェーダだけを書き換えていろいろ試したい場合には、書き換えるたびにプログラムのビルドが必要になって不便です。

そこで、シェーダのソースプログラムを外部のテキストファイルに書いて、それをプログラムの実行時に読み込んで使うようにします。ソースファイルの読み込みに使う関数は、例えば次のようなものです。この関数も他のところから使用されないので、`static` にしておきます。

```cpp
///
/// シェーダのソースファイルを読み込む
///
/// @param[in] name シェーダのソースファイル名
/// @return 読み込んだソースファイルの文字列、読み込めなければ ""
///
static auto readShaderSource(const std::string& name) -> std::string
{
  // ソースファイルを開く
  std::ifstream file(name, std::ios::binary);

  // ソースファイルが開けなかったら
  if (file.fail())
  {
    // エラーメッセージを表示してから読み込み失敗として戻る
    std::cerr << "Error: Can't open source file: " << name << std::endl;
    return "";
  }

  // ファイル全体を読み込む
  std::stringstream buffer;
  buffer << file.rdbuf();

  // 読み込みエラーが起きていたら
  if (file.fail())
  {
    // エラーメッセージを表示してから読み込み失敗として戻る
    std::cerr << "Error: Can't read source file: " << name << std::endl;
    return "";
  }

  // 読み込んだ文字列を返す（file はデストラクタで自動的に閉じられる）
  return buffer.str();
}
```

### 5.8 シェーダのソースファイルを読み込んでプログラムオブジェクトを作成する関数

以上の関数を使って、外部のソースファイルを読み込んで、プログラムオブジェクトを作成します。

```cpp
///
/// シェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
///
/// @param[in] vert バーテックスシェーダのソースファイル名
/// @param[in] frag フラグメントシェーダのソースファイル名
/// @return 作成したプログラムオブジェクトの名前、作成できなかった場合は 0
///
auto loadProgram(const std::string& vert, const std::string& frag) -> GLuint
{
  // バーテックスシェーダのソースファイル名が与えられていなければ
  if (vert.empty())
  {
    // エラーメッセージを表示して 0 を返す
    std::cerr << "Error: No shader source file specified." << std::endl;
    return 0;
  }

  // バーテックスシェーダのソースファイルを読み込む
  const std::string vsrc{ readShaderSource(vert) };

  // バーテックスシェーダのソースファイルが読み込めなければ 0 を返す
  if (vsrc.empty()) return 0;

  // フラグメントシェーダのソースの文字列（初期値は empty）
  std::string fsrc;

  // フラグメントシェーダのソースファイル名が指定されていたら
  if (!frag.empty())
  {
    // フラグメントシェーダのソースファイルを読み込んで
    fsrc = readShaderSource(frag);

    // 読み込めていなかったら 0 を返す
    if (fsrc.empty()) return 0;
  }

  // プログラムオブジェクトを作成する
  return createProgram(vsrc, fsrc, vert, frag);
}
```

### 5.9 shader.h の作成

shader.cpp で定義した関数を他のソースファイルのプログラムから参照するときに使うヘッダファイルを作成します。の作成と同様に、「プロジェクト」メニューから「新しい項目の追加(W)...」を選び、shader.h というファイルを作成します。これに以下の内容を追加します。

```cpp
// シェーダ関連の宣言は gl.h に含まれていないので glew.h を使う
#include <GL/glew.h>

// 標準ライブラリ
#include <string>

///
/// プログラムオブジェクトを作成する
///
/// @param[in] vsrc バーテックスシェーダのソースプログラムの文字列
/// @param[in] fsrc フラグメントシェーダのソースプログラムの文字列
/// @param[in] vert バーテックスシェーダのコンパイル時のメッセージに追加する文字列
/// @param[in] frag フラグメントシェーダのコンパイル時のメッセージに追加する文字列
/// @return 作成したプログラムオブジェクトの名前、作成できなかった場合は 0
///
extern auto createProgram(const std::string& vsrc, const std::string& fsrc,
  const std::string& vert = "vertex shader", const std::string& frag = "fragment shader") -> GLuint;

///
/// シェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
///
/// @param[in] vert バーテックスシェーダのソースファイル名
/// @param[in] frag フラグメントシェーダのソースファイル名
/// @return 作成したプログラムオブジェクトの名前、作成できなかった場合は 0
///
extern auto loadProgram(const std::string& vert, const std::string& frag) -> GLuint;
```

## [ステップ 05](https://github.com/tokoik/particle/blob/step05/README.md)

## 6. シェーダのソースプログラム

Visual Studio を使って、バーテックスシェーダのソースファイル point.vert と、フラグメントシェーダのソースファイル point.frag を作成します。

### 6.1 バーテックスシェーダ

「プロジェクト(P)」メニューの「新しい項目の追加(W)...」を選び、ファイル名に point.vert を指定してください。".vert" という拡張子のファイルは Visual Studio のテンプレートにないので、ファイル名は拡張子まで指定してください。このファイルでは、頂点の位置 `position` にモデルビュー投影変換行列 `mc` をかけて `gl_Position` に代入します。

```glsl
#version 430 core

// 頂点の位置
layout (location = 0) in vec4 position;

// モデルビュー投影変換行列
uniform mat4 mc;

void main()
{
  // 頂点の位置をクリッピング座標系に変換する
  gl_Position = mc * position;
}
```

### 6.2 フラグメントシェーダ

フ「プロジェクト(P)」メニューの「新しい項目の追加(W)...」を選び、ファイル名に point.frag を指定してください。".frag" という拡張子のファイルは Visual Studio のテンプレートにないので、ファイル名は拡張子まで指定してください。このファイルでは、そのフラグメントの色 `vec4(1.0)` すなわち白色を、フラグメントの色の `out` 変数 `color` に代入します。

```glsl
#version 430 core

// フラグメントの色
layout (location = 0) out vec4 color;

void main()
{
  // フラグメントの色として白色を出力する
  color = vec4(1.0);
}
```

### 6.3 シェーダのソースファイルのプロジェクトへの組み込み

シェーダのソースファイルはプロジェクトファイルと同じフォルダに入れて置くだけで構わないのですが、Visual Studio から編集しやすいように、プロジェクトのソリューションエクスプローラーに登録します。ソリューションエクスプローラーでプロジェクト名 (particle) を選択した後、右クリックして現れるポップアップメニューか「プロジェクト(P)」メニューの「新しいフィルター(F)」を選んでください。

![新しいフィルター](images/fig15.png)

フィルターの名前は「シェーダー ファイル」のようなものにしておきます。

![フィルターの名前](images/fig16.png)

作成したフォルダ（シェーダー ファイル）に point.vert と point.frag をドラッグ＆ドロップするか、このフォルダを選択した状態で「プロジェクト(P)」のメニューから「既存の項目の追加(G)...」を選択して、point.vert と point.frag を選択してください。

![既存の項目の追加](images/fig17.png)

point.vert と point.frag がソリューションエクスプローラーの「シェーダー ファイル」の中に入ります。

![シェーダー ファイルの内容](images/fig18.png)

## [ステップ 06](https://github.com/tokoik/particle/blob/step06/README.md)
