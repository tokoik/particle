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

### [ステップ 01](https://github.com/tokoik/particle/tree/step01)

## 2. ライブラリの追加

OpenGL を使ったアプリケーションの作成を手助けするために、[GLFW](https://www.glfw.org/)、[GLEW](https://glew.sourceforge.net/)、[GLM](https://github.com/g-truc/glm) の3つのライブラリを作成したプロジェクトに追加します。これらは、それぞれの Web ページからソースファイルやコンパイル済みのバイナリファイルを入手することができますが、Visual Studio では [NuGet](https://www.nuget.org/) というパッケージマネージャを使って、作成したプロジェクトに組み込むことができます。

### 2.1 GLFW の追加

OpenGL はプラットフォームに依存しないグラフィックスライブラリですが、アプリケーションが OpenGL を使ってグラフィックス表示をしようとするときには、プラットフォームに依存した手順でお膳立てをしてやる必要があります。GLFW は、そのお膳立てをするためのレームワーク（ソフトウェア開発を手助けするための枠組み）です。これをインストールするために、まず「プロジェクト」メニューから「NuGetパッケージの管理(N)...」を選びます。

![NuGetパッケージの管理](images/fig07.png)

「参照」を選んで「glfw」を検索し、glfwのパッケージを選んで「インストール」をクリックします。

![GLFW の追加](images/fig08.png)

「適用」をクリックします。

![適用](images/fig09.png)

### 2.2 GLEW の追加

同様にして、GLEW というライブラリをインストールします。これは Windows が標準ではサポートしていない OpenGL のバージョン 1.1 より後の機能を有効にします。「glew」を検索して最新版の2.2.0 を選び、「インストール」をクリックします。なお、前記の GLEW のプロジェクトのサイトでは最新版は 2.1.0 になっていますが、GitHub 上の最新版は 2.2.0 になっています (2025年5月23日時点)。

![GLEW の追加](images/fig10.png)

### 2.3 GLM の追加

最後に、GLM をインストールします。これは OpenGL を使ってプログラミングする際に便利なデータ型や数学関数を集めたものです。「glm」を検索し、glmのパッケージを選んで「インストール」をクリックします。

![GLM の追加](images/fig11.png)

NuGet のタブはもう使わないので閉じて構いません。

![NuGet のタブを閉じる](images/fig12.png)

### [ステップ 02](https://github.com/tokoik/particle/tree/step02)

## 3. ウィンドウ関連の処理

ウィンドウの作成やマウス・キーボード入力などの処理には GLFW を使いますが、これらの処理をプログラムのあちこちに書くと、メインの処理が読みづらくなるので、Window というラッパークラスにまとめます。このあたりのことについては「GLFWによるOpenGL入門」を復習してください。

### 3.1 Window クラスの追加

「プロジェクト」メニューから「クラスの追加(C)...」を選びます。

![クラスの追加](images/fig13.png)

「クラス(L)」名は「Window」にします。「.h ファイル」と 「.cpp ファイル」のファイル名はクラス名から自動的に設定されますが、ここでは「インライン(I)」にチェックを入れて、「.h ファイル」だけを作成します。

![クラス名の指定](images/fig14.png)

### 3.2 Window.h の内容

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
#define _USE_MATH_DEFINES
#define GLM_FORCE_RADIANS
#include <GLM/glm.hpp>
```

`Window` クラスのプライベートメンバには、ウィンドウを作成したときに作られるウィンドウの識別子 `window` と、ウィンドウのサイズ `size` を保持するようにします。`window` の内容は開いたウィンドウが破棄されるまで変化しないので `const` にしておきます。また `size` は投影変換行列を作る時などに頻繁に使われる画面の縦横比（アスペクト比）や、マウスカーソルのウィンドウ上での相対的な位置の算出に必要になるので、ウィンドウのサイズが変更されたときに算出して保持するようにします。`glm::dvec2` は数値の精度が `double` 型の2次元ベクトルのデータ型です。数値の精度が `float` 型の2次元ベクトルは `glm::vec2` です。

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

ウィンドウのサイズを変更したときに呼び出すメソッド `resize()` を定義します。このメソッドはコールバック関数として用いるので、`static` メソッドにする必要があります。しかし、`static` メソッドはメンバ変数の `window` や `aspect` を参照することはできません。このメソッドはサイズが変更されたウィンドウに対して呼び出されるので、対象のオブジェクト自体は存在します。

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
  Window(const Window& draw) = delete;

  ///
  /// デストラクタ
  ///
  virtual ~Window()
  {
    // ウィンドウを破棄する
    glfwDestroyWindow(window);
  }

  // 代入演算子は使用しない
  Window& operator=(const Window& draw) = delete;
```

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

  ///
  /// 描画ループの継続判定
  ///
  /// @return 描画を継続する場合は true
  ///
  explicit operator bool()
  {
    // イベントを取り出す
    glfwWaitEvents();

    // ウィンドウを閉じるなら false を返す
    if (glfwWindowShouldClose(window)) return false;

    // ウィンドウを閉じない
    return true;
  }

  ///
  /// ダブルバッファリング
  ///
  auto swapBuffers() const
  {
    // カラーバッファを入れ替える
    glfwSwapBuffers(window);
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

### [ステップ 03](https://github.com/tokoik/particle/tree/step03)

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

### [ステップ 04](https://github.com/tokoik/particle/tree/step04)

## 5. シェーダの読み込み処理

shader.cpp というファイルを作成し、そこにシェーダ関連の関数を定義します。また、定義した関数を main.cpp から参照できるように、外部から参照する関数の宣言を shader.h というヘッダファイルに置きます。

### 5.1 shader.cpp の作成

main.cpp の作成と同様に、「プロジェクト」メニューから「新しい項目の追加(W)...」を選び、shader.cpp というファイルを作成します。

#### ヘッダファイルの読み込み

この shader.cpp で定義する関数の内容と、その関数の宣言するヘッダファイル shader.h の内容が矛盾しないように、shader.h を最初に `#include` しておきます。また、この処理ではコンソール入力やファイル入力、および動的配列 `std::vector` を使いますから、それらを宣言するヘッダファイルも も `#include` しておきます。

```cpp
// シェーダ関連の処理
#include "shader.h"

// 標準ライブラリ
#include <iostream>
#include <fstream>
#include <vector>
```

#### シェーダのコンパイル結果を表示する関数

シェーダのコンパイルは、プログラムの実行中に OpenGL のライブラリを使って行います。したがって、エラーメッセージを表示する場合も、ライブラリが保持しているメモリから取り出す必要があります。この関数は他のところから使用されないので、`static` にしておきます。

```cpp
///
/// シェーダオブジェクトのコンパイル結果を表示する
///
/// @param[in] shader シェーダオブジェクト名
/// @param[in] str コンパイルエラーが発生した場所を示す文字列
/// @return コンパイル結果が正常ならば GL_TRUE、異常ならば GL_FALSE
///
static auto printShaderInfoLog(GLuint shader, const char* str) -> GLboolean
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
    std::vector<GLchar> infoLog(bufSize);
    GLsizei length;
    glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);

    // ログの内容を表示する
    std::cerr << &infoLog[0] << std::endl;
  }

  // コンパイル結果を返す
  return status;
}
```

#### シェーダのリンク結果を表示する関数

コンパイルされたシェーダオブジェクトのリンクもプログラムの実行中に OpenGL のライブラリを使って行いますから、そのエラーメッセージもメモリから取り出す必要があります。この関数も他のところから使用されないので、`static` にしておきます。

```cpp
///
/// プログラムオブジェクトのリンク結果を表示する
///
/// @param[in] program プログラムオブジェクト名
/// @return リンク結果が正常ならば GL_TRUE、異常ならば GL_FALSE
///
static auto printProgramInfoLog(GLuint program)
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
    std::vector<GLchar> infoLog(bufSize);
    GLsizei length;
    glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);

    // ログの内容を表示する
    std::cerr << &infoLog[0] << std::endl;
  }

  // リンク結果を返す
  return static_cast<GLboolean>(status);
}
```

#### プログラムオブジェクトを作成する関数

シェーダのソースをコンパイル・リンクして作成したシェーダプログラムの実体を「プログラムオブジェクト」と言います。この手順は、次のようになります。

1. `glCreateProgram()` を使って、プログラムオブジェクトを作成します。
2. `glCreateShader(引数)` でシェーダオブジェクトを作成します。「引数」には、バーテックスシェーダのシェーダオブジェクトを作成するときは `GL_VERTEX_SHADER`、フラグメントシェーダのシェーダオブジェクトを作成するときは `GL_FRAGMENT_SHADER` を指定します。
3. `glShaderSource()` を使ってシェーダのソースプログラムをシェーダオブジェクトのメモリに読み込みます。ソースプログラムは単一の文字列もしくは文字列の配列 (1行1文字列) です。
4. `glCompileShader()` を使って読み込んだソースプログラムをコンパイルします。
5. バーテックスシェーダ `GL_VERTEX_SHADER` とフラグメントシェーダ `GL_FRAGMENT_SHADER` がコンパイルできたら、`glLinkProgram()` で各シェーダオブジェクトをリンクします。

この `createProgram()` はバーテックスシェーダとフラグメントシェーダの両方のソースプログラムを受け取りますが、フラグメントシェーダのソースプログラムは `nullptr` でも構いません。これは Transform Feedback などで `glEnable(GL_RASTERIZER_DISCARD)` としてラスタライザを無効にしたときに、フラグメントシェーダが不要になるためです。

なお、バーテックスシェーダのソースプログラムがエラーになった場合も、エラーチェックのためにフラグメントシェーダはコンパイルするようにしています。したがって、どちらかのソースプログラムでエラーが発生したことは、リンク時に検出して報告することになります。

```cpp
///
/// プログラムオブジェクトを作成する
///
/// @param[in] vsrc バーテックスシェーダのソースプログラムの文字列
/// @param[in] fsrc フラグメントシェーダのソースプログラムの文字列
/// @param[in] vert バーテックスシェーダのコンパイル時のメッセージに追加する文字列
/// @param[in] frag フラグメントシェーダのコンパイル時のメッセージに追加する文字列
/// @return 作成したプログラムオブジェクトの名前、作成できなかった場合は 0
///
auto createProgram(const char* vsrc, const char* fsrc,
  const char* vert, const char* frag) -> GLuint
{
  // バーテックスシェーダのソースの文字列が与えられていなければ
  if (vsrc == nullptr)
  {
    // エラーメッセージを表示して 0 を返す
    std::cerr << "Error: Source file not specified." << std::endl;
    return 0;
  }

  // 空のプログラムオブジェクトを作成する
  const GLuint program{ glCreateProgram() };

  // プログラムオブジェクトが作成できなかったら
  if (program == 0)
  {
    // エラーメッセージを表示して 0 を返す
    std::cerr << "Error: Could not create program object." << std::endl;
    return 0;
  }

  // バーテックスシェーダのシェーダオブジェクトを作成する
  const auto vertShader{ glCreateShader(GL_VERTEX_SHADER) };

  // シェーダオブジェクトにソースプログラムの文字列を設定してコンパイルする
  glShaderSource(vertShader, 1, &vsrc, nullptr);
  glCompileShader(vertShader);

  // エラーが無ければシェーダオブジェクトをプログラムオブジェクトに組み込む
  if (printShaderInfoLog(vertShader, vert)) glAttachShader(program, vertShader);

  // シェーダオブジェクトに削除マークをつけておく
  glDeleteShader(vertShader);

  // フラグメントシェーダのソースの文字列が与えられていれば
  if (fsrc != nullptr)
  {
    // フラグメントシェーダのシェーダオブジェクトを作成する
    const auto fragShader{ glCreateShader(GL_FRAGMENT_SHADER) };

    // シェーダオブジェクトにソースプログラムの文字列を設定してコンパイルする
    glShaderSource(fragShader, 1, &fsrc, nullptr);
    glCompileShader(fragShader);

    // エラーが無ければシェーダオブジェクトをプログラムオブジェクトに組み込む
    if (printShaderInfoLog(fragShader, frag)) glAttachShader(program, fragShader);

    // シェーダオブジェクトに削除マークをつけておく
    glDeleteShader(fragShader);
  }

  // プログラムオブジェクトをリンクしてエラーがなければプログラムオブジェクトを返す
  glLinkProgram(program);
  if (printProgramInfoLog(program)) return program;

  // エラーのときはプログラムオブジェクトを削除して 0 を返す
  glDeleteProgram(program);
  return 0;
}
```

#### ソースファイルを読み込む関数

`glCompileShader()` は `glShaderSource()` により文字列として与えられたソースプログラムを読み込んでコンパイルします。また、これらを使って定義した `createProgram()` も、ソースプログラムを文字列で受け取ります。これは、シェーダのソースプログラムを C++ のソースプログラム中に文字列として埋め込む場合は問題ないのですが、それだとシェーダだけを書き換えていろいろ試したい場合には、書き換えるたびにプログラムのビルドが必要になって不便です。

そこで、シェーダのソースプログラムを外部のテキストファイルに書いて、それをプログラムの実行時に読み込んで使うようにします。ソースファイルの読み込みに使う関数は、例えば次のようなものです。

```cpp
///
/// シェーダのソースファイルを読み込む
///
/// @param[in] name シェーダのソースファイル名
/// @param[out] buffer 読み込んだソースファイルのテキスト
/// @return 読み込みに成功したら true、失敗したら false
///
static auto readShaderSource(const char* name,
  std::vector<GLchar>& buffer) -> bool
{
  // ソースファイル名が nullptr だったら読み込み失敗を返す
  if (name == nullptr) return false;

  // ソースファイルを開く
  std::ifstream file(name, std::ios::binary);

  // ソースファイルが開けなかったら
  if (file.fail())
  {
    // エラーメッセージを表示して
    std::cerr << "Error: Can't open source file: " << name << std::endl;

    // 読み込み失敗
    return false;
  }

  // ファイルの末尾に移動し現在位置（＝ファイルサイズ）を得る
  file.seekg(0L, std::ios::end);
  const auto length{ static_cast<GLsizei>(file.tellg()) };

  // ファイルサイズのメモリを確保
  buffer.resize(length + 1);

  // ファイルを先頭から読み込む
  file.seekg(0L, std::ios::beg);
  file.read(buffer.data(), length);

  // 読み込んだ文字列の末尾にヌル文字を追加する
  buffer[length] = '\0';

  // 読み込みが失敗していたら
  if (file.fail())
  {
    // エラーメッセージを表示して
    std::cerr << "Error: Could not read souce file: " << name << std::endl;

    // ファイルを閉じて
    file.close();

    // 読み込み失敗を返す
    return false;
  }

  // 読み込みが成功したらファイルを閉じて
  file.close();

  // 読み込み成功を返す
  return true;
}
```

#### シェーダのソースファイルを読み込んでプログラムオブジェクトを作成する関数

以上の関数を使って、外部のソースファイルを読み込んで、プログラムオブジェクトを作成します。

```cpp
///
/// シェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
///
/// @param[in] vert バーテックスシェーダのソースファイル名
/// @param[in] frag フラグメントシェーダのソースファイル名
/// @return 作成したプログラムオブジェクトの名前、作成できなかった場合は 0
///
auto loadProgram(const char* vert, const char* frag) -> GLuint
{
  // バーテックスシェーダのソースファイルを読み込む
  std::vector<GLchar> vsrc;
  const auto vstat{ readShaderSource(vert, vsrc) };

  // フラグメントシェーダのソースファイルを読み込む
  std::vector<GLchar> fsrc;
  const auto fstat{ readShaderSource(frag, fsrc) };

  // 両方のソースファイルが読み込めたらプログラムオブジェクトを作成する
  return vstat && fstat ? createProgram(vsrc.data(), fsrc.data(), vert, frag) : 0;
}
```

### 5.2 shader.h の作成

shader.cpp で定義した関数を他のソースファイルのプログラムから参照するときに使うヘッダファイルを作成します。の作成と同様に、「プロジェクト」メニューから「新しい項目の追加(W)...」を選び、shader.h というファイルを作成します。これに以下の内容を追加します。

```cpp
// シェーダ関連の宣言は gl.h に含まれていないので glew.h を使う
#include <GL/glew.h>

///
/// プログラムオブジェクトを作成する
///
/// @param[in] vsrc バーテックスシェーダのソースプログラムの文字列
/// @param[in] fsrc フラグメントシェーダのソースプログラムの文字列
/// @param[in] vert バーテックスシェーダのコンパイル時のメッセージに追加する文字列
/// @param[in] frag フラグメントシェーダのコンパイル時のメッセージに追加する文字列
/// @return 作成したプログラムオブジェクトの名前、作成できなかった場合は 0
///
extern auto createProgram(const char* vsrc, const char* fsrc,
  const char* vert = "vertex shader", const char* frag = "fragment shader") -> GLuint;

///
/// シェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
///
/// @param[in] vert バーテックスシェーダのソースファイル名
/// @param[in] frag フラグメントシェーダのソースファイル名
/// @return 作成したプログラムオブジェクトの名前、作成できなかった場合は 0
///
extern auto loadProgram(const char* vert, const char* frag) -> GLuint;
```

### [ステップ 05](https://github.com/tokoik/particle/tree/step05)

## 6. シェーダのソースプログラム

Visual Studio を使って、バーテックスシェーダのソースファイル point.vert と、フラグメントシェーダのソースファイル point.frag を作成します。

### 6.1 バーテックスシェーダ

「プロジェクト(P)」メニューの「新しい項目の追加(W)...」を選び、ファイル名に point.vert を指定してください。".vert" という拡張子のファイルは Visual Studio のテンプレートにないので、ファイル名は拡張子まで指定してください。このファイルでは、頂点位置の頂点属性 `position` にモデルビュー投影変換行列 `mc` をかけて `gl_Position` に代入します。

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

### [ステップ 06](https://github.com/tokoik/particle/tree/step06)

## 7. 図形関連の処理

図形関連の処理を行う `Object` という構造体を追加します。クラスにせずに構造体にするのは、メンバ変数が `const` で読み取りしか行わないため、`private` にする理由があまりないと考えたからです。このあたりは、それでもクラスにしてメンバを `public` にすべきだという考え方もありますので、どっちにするかは任せます。

### 7.1 Object.h の作成

`Object` 構造体を定義するヘッダファイルを作成します。「プロジェクト」メニューから「新しい項目の追加(W)...」を選んで Object.h というファイルを作成します。このファイルでも最初に GL/glew.h を `#include` します。

```cpp
#pragma once

// バッファオブジェクト関連の宣言は gl.h に含まれていないので glew.h を使う
#include <GL/glew.h>
```

メンバには頂点配列オブジェクトの名前（番号）を保持する `vao` と、それに組み込む頂点バッファオブジェクトの名前を保持する `vbo` を追加します。これらは一度作成すしたら変更することはないので、`const` にしておきます。

```cpp
///
/// 頂点配列オブジェクト
///
struct Object
{
  /// 頂点配列オブジェクト名
  const GLuint vao;

  /// 頂点バッファオブジェクト名
  const GLuint vbo;
```

コンストラクタでは `vao` と `vbo` を作成します。引数は `vbo` のメモリを確保するサイズと、そこに最初に格納するデータを指すポインタです。`Window` クラスと同じ理由で、コピーコンストラクタと代入演算子は削除しておきます。

```cpp
  ///
  /// コンストラクタ
  ///
  /// @param[in] size データサイズ
  /// @param[in] data データ
  ///
  Object(GLsizeiptr size, const void* data = nullptr);

  // コピーコンストラクタは使用しない
  Object(const Object& draw) = delete;

  ///
  /// デストラクタ
  ///
  virtual ~Object();

  // 代入演算子は使用しない
  Object& operator=(const Object& object) = delete;
};
```

### 7.2 Object.cpp の作成

「プロジェクト」メニューから「新しい項目の追加(W)...」を選んで Object.cpp というファイルを作成し、そこに `Object` 構造体のコンストラクタを定義します。コンストラクタでは、点（頂点）を格納する頂点バッファオブジェクトを組み込んだ頂点配列オブジェクトを作成します。この手順は次のようになります。

1. `glGenVertexArrays()` を使って、頂点配列オブジェクトを一つ作り、メンバ変数 `vao` に名前を格納します。
2. 次に `glGenBuffers()` を使って、頂点バッファオブジェクト `GL_ARRAY_BUFFER` を一つ作り、メンバ変数 `vbo` に名前を格納します。
3. `glBindVertexArray()` を使って、作った頂点配列オブジェクト `vao` を結合します。
4. `glBindBuffer()` を使って、作った頂点バッファオブジェクト `vbo` を結合します。これで頂点バッファオブジェクトが頂点配列オブジェクトに組み込まれます。
5. `glBufferData()` を使って、作った頂点配列オブジェクトに GPU 上のメモリを割り当てます。割り当てるメモリのサイズは、3次元の点の座標値 (x, y, z) を点の数 `count` 個分です。 また、そこに引数 `position` の内容をコピーします。`position` が `nullptr` なら、データのコピーを行わずにメモリの確保だけを行います。

この頂点バッファオブジェクトは、`glVertexAttribPointer()` を使って、バーテックスシェーダの `location` が 0 の `in` 変数 (attribute 変数) から 3 要素の `float` 型として入力されるようにします。また、`glEnableVertexAttribArray()` を使って、`location` が 0 の `in` 変数を有効にします。

なお、`vao` と `vbo` は初期化リストで初期化しますが、頂点配列オブジェクトを作成する `glGenVertexArrays()` や頂点バッファオブジェクトを作成する `glGenBuffers()` は引数にポインタを指定する必要があるので、ラムダ関数でラップしています。

```cpp
///
/// 図形関連の処理の実装
/// 
#include "Object.h"

///
/// コンストラクタ
///
/// @param[in] size データサイズ
/// @param[in] data データ
///
Object::Object(GLsizeiptr size, const void* data) :

  // 頂点配列オブジェクトを作成して vao を初期化する
  vao{ []() { GLuint vao; glGenVertexArrays(1, &vao); return vao; }() },

  // 頂点バッファオブジェクトを作成して vbo を初期化する
  vbo{ []() { GLuint vbo; glGenBuffers(1, &vbo); return vbo; }() }
{
  // 頂点配列オブジェクトを結合する
  glBindVertexArray(vao);

  // 頂点バッファオブジェクトを結合して頂点配列オブジェクトに組み込む
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // 頂点バッファオブジェクトのメモリを確保して頂点位置データを転送する
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);

  // 結合されている頂点バッファオブジェクトのインデックスを 0 番にする
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // 0 番の頂点バッファオブジェクトを有効にする
  glEnableVertexAttribArray(0);

  // 頂点配列オブジェクトの結合を解除する
  glBindVertexArray(0);

  // 頂点バッファオブジェクトの結合を解除する
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
```

デストラクタでは、作成した頂点配列オブジェクト `vao` と頂点バッファオブジェクト `vbo` を削除します。

```cpp
///
/// デストラクタ
///
Object::~Object()
{
  // 頂点配列オブジェクトを削除する
  glDeleteVertexArrays(1, &vao);

  // 頂点バッファオブジェクトを削除する
  glDeleteBuffers(1, &vbo);
}
```

### [ステップ 07](https://github.com/tokoik/particle/tree/step07)

## 8. 描画処理

### 8.1 main.cpp への組み込み

これまでに作成したシェーダのプログラムオブジェクト作成関数 `loadProgram()` や `Object` 構造体を、main.cpp で使えるようにします。shader.cpp で定義した関数 `loadProgram()` を main.cpp で使用するために、その宣言を格納した shader.h を `#include` します。また、図形のデータを保持する `Object` 構造体を使用するために、Object.h を `#include` します。図形は 10,000 個の点（粒子群）とし、その初期値を乱数で与えるために random も `#include` します。

このプログラムでは、GLM の `glm::vec4` や `glm::mat4` のようなデータ型の変数などからデータが格納されている場所のポインタを取り出す `glm::value_ptr()` を使うので、それが宣言されている GLM/gtc/type_ptr.hpp を `#include` します。また、平行移動の変換行列を求める `glm::translate()` やビュー（視野）変換行列を作るのに便利な `glm::lookAt()`、透視投影変換行列を作るのに便利な `glm::perspective()` などが宣言されている GLM/gtc/matrix_transform.hpp も `#include` します。

```cpp
// Windows の OpenGL ライブラリをリンクする
#pragma comment(lib, "opengl32.lib")

// ウィンドウ関連の処理
#include "Window.h"

// OpenGL のエラーチェック
#include "errorcheck.h"

// シェーダの読み込み処理
#include "shader.h"

// 図形関連の処理
#include "Object.h"

// 粒子数
const auto PARTICLE_COUNT{ 10000 };

// 乱数
#include <random>

// GLM 関連
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>

// 標準ライブラリ
#include <iostream>
```

### 8.2 プログラムオブジェクトの作成

GLFW の初期化が完了して OpenGL の新しい機能が使えるようになったら、シェーダのプログラムオブジェクトを作成します。プログラムオブジェクトが作成できなかったことを考えて、プログラムオブジェクトの名前 `program` が 0 でないことを確認した方が良いですね。そのあと、実行時に値を設定する必要がある変換行列の `uniform` 変数 `mc` の場所を取得しておきます。

```cpp
///
/// メインプログラム
///
/// @return プログラムが正常に終了した場合は 0
///
auto main() -> int
{
  //
  // 中略
  //  

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

  // プログラムオブジェクトを作成する
  const auto program{ loadProgram("point.vert", "point.frag") };

  // プログラムオブジェクトが作成できなかったら
  if (program == 0)
  {
    // エラーメッセージを出して
    std::cerr << "Can't create program object." << std::endl;

    // 終了する
    return EXIT_FAILURE;
  }

  // uniform 変数 mc の場所を取得する
  const auto mcLoc{ glGetUniformLocation(program, "mc") };
```

### 8.3 図形の作成

図形は `Object` 構造体の変数を宣言することで作成します。コンストラクタの引数は頂点バッファオブジェクトとして確保する GPU 上のメモリのサイズです。この頂点バッファオブジェクトをメインメモリにマップし、`glm::vec3` 型の配列とみなして、一つ一つの要素に ±1 の乱数を格納します。

```cpp
  // 図形を作成する
  Object object{ sizeof(glm::vec3) * PARTICLE_COUNT };

  // 乱数生成器を初期化する
  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());

  // [-1.0f, 1.0f) の範囲の一様乱数の生成器を用意する
  std::uniform_real_distribution<GLfloat> dist(-1.0f, 1.0f);

  // 粒子の初期位置を設定する
  glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
  const auto position{ static_cast<glm::vec3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)) };
  for (auto i{ 0 }; i < PARTICLE_COUNT; ++i)
  {
    // ランダムな位置を設定する
    position[i] = { dist(engine), dist(engine), dist(engine) };
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
```

### 8.4 描画ループの開始

ウィンドウの背景色を指定して、描画ループを開始します。`Window` クラスのインスタンス `window` を論理コンテキストで評価して、描画ループの継続条件とします。また、ループの最初でウィンドウのカラーバッファとデプスバッファを消去します。

```cpp
  // 背景色を指定する
  glClearColor(0.2f, 0.3f, 0.4f, 0.0f);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
    // ウィンドウを消去する
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```

### 8.5 プログラムオブジェクトの指定

描画に使用するシェーダのプログラムオブジェクトを指定します。また、現在のウィンドウのアスペクト比を使って投影 (プロジェクション) 変換行列 `projection` を求め、それに視野変換行列 `view` を乗じたものを、このプログラムオブジェクトの `uniform` 変数 `mc` に設定します。投影変換の画角は 60°、図形の描画範囲は -3±1 なので、更に ±1 の余裕を持たせて near = 1、far = 5 に設定します。

```cpp
  // 背景色を指定する
  glClearColor(0.2f, 0.3f, 0.4f, 0.0f);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
    // ウィンドウを消去する
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // プログラムオブジェクトを指定する
    glUseProgram(program);

    // ビュー変換行列を設定する
    const auto view{ glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)) };

    // 投影変換行列を設定する
    const auto projection{ glm::perspective(glm::radians(60.0f), window.getAspect(), 1.0f, 5.0f) };

    // uniform 変数 mc に値を設定する
    glUniformMatrix4fv(mcLoc, 1, GL_FALSE, glm::value_ptr(projection * view));
```

### 8.6 図形の描画

描画する図形の頂点配列オブジェクトを指定して、点で描画します。

```cpp
    // 図形を指定する
    glBindVertexArray(object.vao);

    // 図形を描画する
    glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

    // OpenGL のエラーが発生していないかチェックする
    errorcheck();

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }
}
```

### 8.8 実行結果

実行すると、こんな感じになります。

![実行結果](images/fig19.png)

### [ステップ 08](https://github.com/tokoik/particle/tree/step08)

## 9. オブジェクトの回転

マウスのドラッグで図形をくるくる回転できるようにします。このような操作は「トラックボール」と呼ばれることがあります。この機能は `Window` クラスに実装しますので、Window.h を編集します。

### 9.1 クォータニオンを使う

回転の処理には、特にゲームやロボティクスでは、クォータニオン（四元数）という数を使います。GLM では、クォータニオンは `glm::quot`（数値の精度が `float` 型の場合）や `glm::dquot`（数値の精度が `double` 型の場合）というデータ型で扱うことができます。これらを使うために GLM/gtc/quaternion.hpp を `#include` します。また、配列のデータ型 `std::array` も使うので、array も `#include` します。

```cpp
// GLM 関連
#define _USE_MATH_DEFINES
#define GLM_FORCE_RADIANS
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>

// 標準ライブラリ
#include <array>
```

### 9.2 マウスボタンを使う

マウス操作に使うマウスボタンの識別子（番号）を格納する変数 `button` と現在のマウスカーソルの位置 `cursor` を、プライベートメンバに追加します。GLFW では整数値の 0 である `GLFW_MOUSE_BUTTON_0` から `GLFW_MOUSE_BUTTON_LAST` の、`GLFW_MOUSE_BUTTON_LAST + 1` 個のマウスボタンの識別子を扱うことができますから、`button` はどのボタンも押していないことを表す -1 で初期化します。

また、マウスボタンを押した位置 `start`、マウスのドラッグによる回転を表すクォータニオンを格納する `rotation`、そしてクォータニオンの `rotation` から求めた回転の変換行列を格納する `model` を、それぞれ `GLFW_MOUSE_BUTTON_LAST + 1` 個の配列で用意します。このほか、ドラッグ中の回転を一時的に保持するクォータニオン `trackball` を用意します。

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

  /// 操作しているマウスボタン
  int button{ -1 };

  /// マウスカーソルの現在位置
  glm::dvec2 cursor{};

  /// ボタンごとのマウスボタンを押した位置
  std::array<glm::dvec2, GLFW_MOUSE_BUTTON_LAST + 1> start{};

  /// ボタンごとの回転
  std::array<glm::dquat, GLFW_MOUSE_BUTTON_LAST + 1> rotation{};

  /// ボタンごとのモデル変換行列
  std::array<glm::mat4, GLFW_MOUSE_BUTTON_LAST + 1> model{};

  /// トラックボール処理の途中経過
  glm::dquat trackball{};
```

### 9.3 マウスボタンを操作したときの処理

`mouse()` というメソッドにマウスボタンを操作したときの処理を記述します。このメソッドもコールバック関数として使用しますので、`static` にします。しががって、このメソッドでも `resize()` と同様に、`glfwGetWindowUserPointer()` でウィンドウの識別子 `window` からインスタンスのポインタを取り出します。

```cpp
  ///
  /// マウスボタンの操作時の処理
  ///
  /// @param[in] window マウスボタンの操作を受け付けるウィンドウの識別子
  /// @param[in] button 押されたマウスボタンの識別子
  /// @param[in] action マウスボタンの状態
  /// @param[in] mods マウスボタンの状態に影響する修飾キー (Shift, Ctrl, Alt)
  ///
  /// @note glfwSetMouseButtonCallback() で登録するコールバック関数
  ///
  static void mouse(GLFWwindow* window, int button, int action, int mods)
  {
    // window が保持するインスタンスの this ポインタを得る
    const auto instance{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };

    // インスタンスからの呼び出しでなければ戻る
    if (instance == nullptr) return;
```

そして、マウスボタンを押したときに、押したボタンの識別子と、その位置を保存します。また、マウスボタンを離したときには、マウスボタンの識別子としてどのボタンを押していないことを示す -1 を格納し、その時の回転のクォータニオンを保存します。

```cpp
    // マウスボタンを押していたら
    if (action != GLFW_RELEASE)
    {
      // 押したマウスボタンを記録する
      instance->button = button;

      // ドラッグ開始時のカーソル位置を保存する
      instance->start[button] = instance->cursor;
    }
    else
    {
      // マウスボタンを離したことを記録する
      instance->button = -1;

      // ドラッグ終了時の回転を保存する
      instance->rotation[button] = instance->trackball;
    }
  }
```

### 9.4 コンストラクタの修正

コンストラクタでは、`glfwSetMouseButtonCallback()` でマウスボタンを操作したときに呼び出されるコールバック関数に `mouse()` を登録します。また、すべてのマウスボタンについて、回転 `rotation` を単位クォータニオン `glm::quat(1.0f, 0.0f, 0.0f, 0.0f)` で初期化し、回転のモデル変換行列を格納する `model` を単位行列 `glm::mat4(1.0f)` で初期化します。

```cpp
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

    // マウスボタンの操作時に呼び出す処理を登録する
    glfwSetMouseButtonCallback(window, mouse);

    // 全てのボタンの回転を初期化する
    std::fill(rotation.begin(), rotation.end(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

    // 全てのボタンのモデル変換行列を初期化する
    std::fill(model.begin(), model.end(), glm::mat4(1.0f));

    // ウィンドウのサイズ変更時に呼び出す処理を登録する
    glfwSetWindowSizeCallback(window, resize);

    // 開いたウィンドウに初期設定を適用する
    resize(window, width, height);
  }
```

### 9.5 ループの継続判定処理の修正

マウスカーソルの現在位置などの状態は `glfwWaitEvents()` の呼び出しで取得されますから、これを呼び出している `bool()` 内でマウスカーソルの位置を `cursor` に保存します。

```cpp
  ///
  /// 描画ループの継続判定
  ///
  /// @return 描画を継続する場合は true
  ///
  explicit operator bool()
  {
    // イベントを取り出す
    glfwWaitEvents();

    // ウィンドウを閉じるなら false を返す
    if (glfwWindowShouldClose(window)) return false;

    // マウスの現在位置を保存する
    glfwGetCursorPos(window, &cursor[0], &cursor[1]);
```

マウスのボタンが押されていたら、現在のマウスカーソルの位置の、マウスボタンを押した位置からの変位 (`dx`, `dy`) を求めます。OpenGL のスクリーンの座標系は上方向が正なのに対して、マウスカーソルの座標系は下方向が正なので、変位の上下は反転します。また、これをウィンドウのサイズで割って、ウィンドウの大きさにかかわらず 0～1 の値となるようにします。

この変位の長さを角度に用い、変位の方向に直交する方向を回転軸に用いて回転のクォータニオンを決定し、`trackball` に保存します。また、このクォータニオンの回転の変換行列も `model` に求めておきます。

```cpp
    // いずれかのマウスボタンが押されていたら
    if (button >= GLFW_MOUSE_BUTTON_LEFT)
    {
      // マウスの相対変位
      const auto dx{ (cursor.x - start[button].x) / size.x };
      const auto dy{ (start[button].y - cursor.y) / size.y };

      // マウスポインタの位置のドラッグ開始位置からの距離
      const auto length{ hypot(dx, dy) };

      // マウスポインタの位置がドラッグ開始位置から移動していれば
      if (length > 0.0)
      {
        // マウスの移動方向と直交するベクトルを回転軸にする
        const auto axis{ glm::normalize(glm::dvec3(-dy, dx, 0.0)) };

        // マウスの移動量を回転角とした回転を現在の回転と合成する
        trackball = glm::angleAxis(length * M_PI, axis) * rotation[button];

        // 合成した回転の四元数から回転の変換行列を求める
        model[button] = glm::mat4_cast(static_cast<glm::quat>(trackball));
      }
    }

    // ウィンドウを閉じない
    return true;
  }
```

### 9.6 モデル変換行列の取り出し

このモデル変換行列 `model` を取り出すメソッド `getModel()` を追加します。後の都合で、このメソッドは `const` にしません。

```cpp
  ///
  /// ウィンドウの縦横比を取り出す
  ///
  /// @return ウィンドウの縦横比
  ///
  auto getAspect()
  {
    return static_cast<GLfloat>(size.x / size.y);
  }

  ///
  /// モデル変換行列を取り出す
  ///
  /// @param[in] button マウスボタンの識別子
  /// @return モデル変換行列
  ///
  const auto& getModel(int button)
  {
    // 指定したボタンに割り当てたモデル変換行列を返す  
    return model[button];
  }
};
```

### [ステップ 09](https://github.com/tokoik/particle/tree/step09)

## 10. オブジェクトの移動

マウスホイールの操作で図形を前後に移動できるようにします。この機能は `Window` クラスに実装しますので、Window.h を編集します。

### 10.1 マウスホイールの回転量

マウスホイールの回転量を保持するプライベートメンバ `scroll` を追加します。この初期値は 0 にします（"0.0, 0.0" を省略しても 0 になります）。GLFW では、マウスホイールの回転量は2次元で得られます。

```cpp
///
/// ウィンドウ関連の処理クラス
///
class Window
{
  /// ウィンドウの識別子
  GLFWwindow* const window;

  // 中略

  /// トラックボール処理の途中経過
  glm::dquat trackball{};

  /// マウスホイールの回転量
  glm::dvec2 scroll{ 0.0, 0.0 };
```

### 10.2 マウスホイールを操作したときの処理

`wheel()` というメソッドにマウスホイールを操作したときの処理を記述します。このメソッドもコールバック関数として使用しますので、`static` にします。しががって、このメソッドでも `resize()` と同様に `glfwGetWindowUserPointer()` でウィンドウの識別子 window からインスタンスのポインタを取り出します。

このメソッドでは、引数で与えられたマウスホイールの回転量を、メンバ変数の `scroll` に加算します。したがって、`scroll` の初期値は 0 である必要があります。

```cpp
  ///
  /// マウスホイールを操作した時の処理
  ///
  /// @param[in] window マウスホイールの操作を受け付けるウィンドウの識別子
  /// @param[in] x マウスホイールの x 方向の回転量
  /// @param[in] y マウスホイールの y 方向の回転量
  ///
  /// @note glfwSetScrollCallback() で登録するコールバック関数
  ///
  static void wheel(GLFWwindow* window, double x, double y)
  {
    // window が保持するインスタンスの this ポインタを得る
    const auto instance{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };

    // インスタンスからの呼び出しでなければ戻る
    if (instance == nullptr) return;

    // マウスホイールの回転量の保存
    instance->scroll += glm::dvec2{ x, y };
  }
```

### 10.3 コンストラクタの修正

`glfwSetScrollCallback()` でマウスホイールを操作したときに呼び出されるコールバック関数に `wheel()` を登録します。

```cpp
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

    // マウスホイールの操作時に呼び出す処理を登録する
    glfwSetScrollCallback(window, wheel);

    // マウスボタンの操作時に呼び出す処理を登録する
    glfwSetMouseButtonCallback(window, mouse);

    // 全てのボタンの回転を初期化する
    std::fill(rotation.begin(), rotation.end(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

    // 全てのボタンのモデル変換行列を初期化する
    std::fill(model.begin(), model.end(), glm::mat4(1.0f));

    // ウィンドウのサイズ変更時に呼び出す処理を登録する
    glfwSetWindowSizeCallback(window, resize);

    // 開いたウィンドウに初期設定を適用する
    resize(window, width, height);
  }    
```

### 10.4 モデル変換行列の取り出し

マウスボタンの `button` に割当てたモデル変換行列 `model[button]` を取り出す際に、その平行移動の x 成分 `model[button][3][0]` と z 成分 `model[button][3][2]` に、それぞれマウスの回転量の x 成分 `scroll.x` と y 成分 `scroll.y` を設定します。係数の 0.1 は自分で動作を試して、好みの値を設定してください。

なお、このようにすると、すべてのマウスボタンのモデル変換行列にマウスホイールによる同じ平行移動量が設定されてしまいます。したがって、マウスホイールによる平行移動量の設定は、特定のボタンのモデル変換行列に対してのみ行うようにした方が良いかもしれません。また、マウスホイールの回転は、多くのマウスでは y 方向しか取得できませんが、横スクロールの機能のあるマウスであれば、x 方向の回転量も得られます。

```cpp
  ///
  /// モデル変換行列を取り出す
  ///
  /// @param[in] button マウスボタンの識別子
  /// @return モデル変換行列
  ///
  const auto& getModel(int button)
  {
    // マウスホイールの回転量をモデル変換行列の平行移動量に設定する
    model[button][3][0] = static_cast<float>(scroll.x * 0.1);
    model[button][3][2] = static_cast<float>(scroll.y * 0.1);

    // 指定したボタンに割り当てたモデル変換行列を返す
    return model[button];
  }
};
```

### [ステップ 10](https://github.com/tokoik/particle/tree/step10)

## 11. コンピュートシェーダの読み取り処理

GPU に汎用の数値計算をさせる、いわゆる GPGPU (General Purpose GPU) を行うには、バーテックスシェーダで計算をして、ラスタライザを起動させずに Transform Feedback により out 変数から直接計算結果を取り出す方法や、フラグメントシェーダの出力をフレームバッファオブジェクトにするなど、いくつかの方法があります。コンピュートシェーダは、それらより汎用で、かつ GPU の並列性を活用できるプログラミング機能を提供します。

詳しくは [床井研究室 - 粒子のレンダリング (2) ポイントの移動](https://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20181018) や [床井研究室 - セパラブルフィルタ](https://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20230604) あたりも参考にしてください。ここでは以前に作成した shader.cpp と shader.h に、コンピュートシェーダを作成する関数を追加します。

## 11.1 shader.cpp の修正

shader.cpp を開き、以下の内容を追加します。

```cpp
///
/// コンピュートシェーダのソースプログラムの文字列を読み込んでプログラムオブジェクトを作成する
///
/// @param[in] csrc コンピュートシェーダのソースプログラムの文字列
/// @param[in] cmsg コンピュートシェーダのコンパイル時のメッセージに追加する文字列
/// @return プログラムオブジェクトのプログラム名、作成できなければ 0
///
auto createCompute(const char* csrc, const char* cmsg) -> GLuint
{
  // 空のプログラムオブジェクトを作成する
  const GLuint program{ glCreateProgram() };

  // プログラムオブジェクトが作成できなかったら
  if (program == 0)
  {
    // エラーメッセージを表示して 0 を返す
    std::cerr << "Error: Could not create program object." << std::endl;
    return 0;
  }

  // コンピュートシェーダのシェーダオブジェクトを作成する
  const auto compShader{ glCreateShader(GL_COMPUTE_SHADER) };

  // シェーダオブジェクトにソースプログラムの文字列を設定してコンパイルする
  glShaderSource(compShader, 1, &csrc, nullptr);
  glCompileShader(compShader);

  // エラーが無ければシェーダオブジェクトをプログラムオブジェクトに組み込む
  if (printShaderInfoLog(compShader, cmsg)) glAttachShader(program, compShader);

  // シェーダオブジェクトに削除マークをつけておく
  glDeleteShader(compShader);

  // プログラムオブジェクトをリンクしてエラーがなければプログラムオブジェクトを返す
  glLinkProgram(program);
  if (printProgramInfoLog(program)) return program;

  // エラーのときはプログラムオブジェクトを削除して 0 を返す
  glDeleteProgram(program);
  return 0;
}

///
/// コンピュートシェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
///
/// @param[in] comp コンピュートシェーダのソースファイル名
/// @return プログラムオブジェクトのプログラム名、作成できなければ 0
///
auto loadCompute(const char* comp) -> GLuint
{
  // コンピュートシェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
  std::vector<GLchar> csrc;
  if (readShaderSource(comp, csrc)) return createCompute(csrc.data(), comp);

  // ソースファイルが読めなかったので 0 を返す
  return 0;
}
```

### 11.2 shader.h の修正

shader.h を開き、以下の内容を追加します。

```cpp
///
/// コンピュートシェーダのソースプログラムの文字列を読み込んでプログラムオブジェクトを作成する
///
/// @param[in] csrc コンピュートシェーダのソースプログラムの文字列
/// @param[in] cmsg コンピュートシェーダのコンパイル時のメッセージに追加する文字列
/// @return プログラムオブジェクトのプログラム名、作成できなければ 0
///
extern auto createCompute(const char* csrc, const char* cmsg) -> GLuint;

///
/// コンピュートシェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
///
/// @param[in] comp コンピュートシェーダのソースファイル名
/// @return プログラムオブジェクトのプログラム名、作成できなければ 0
///
extern auto loadCompute(const char* comp) -> GLuint;
```

### [ステップ 11](https://github.com/tokoik/particle/tree/step11)

（まだまだ続く予定）
