#pragma once

///
/// ウィンドウ関連の処理
///
/// @file Window.h
/// @author Kohe Tokoi
/// @date 2025/5/24
///

// OpenGL 関連
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM 関連
#if !defined(_USE_MATH_DEFINES)
#  define _USE_MATH_DEFINES
#endif
#if !defined(GLM_FORCE_RADIANS)
#  define GLM_FORCE_RADIANS
#endif
#include <GLM/glm.hpp>

///
/// ウィンドウ関連の処理クラス
///
class Window
{
  /// ウィンドウの識別子
  GLFWwindow* const window;

  /// ウィンドウのサイズ
  glm::dvec2 size;

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
  Window& operator=(const Window& draw) = delete;

  // ムーブ代入演算子はデフォルトのものを使用する
  Window& operator=(Window&& window) = default;

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
  explicit operator bool() const
  {
    // イベントを取り出す
    glfwPollEvents();

    // ウィンドウを閉じないなら true を返す
    return glfwWindowShouldClose(window) == GLFW_FALSE;
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
