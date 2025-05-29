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
