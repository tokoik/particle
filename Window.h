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
#include <GLM/gtc/quaternion.hpp>

// 標準ライブラリ
#include <array>

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

  /// ボタンごとのマウスボタンを押した位置
  std::array<glm::dvec2, GLFW_MOUSE_BUTTON_LAST + 1> start{};

  /// ボタンごとの回転
  std::array<glm::dquat, GLFW_MOUSE_BUTTON_LAST + 1> rotation{};

  /// ボタンごとのモデル変換行列
  std::array<glm::mat4, GLFW_MOUSE_BUTTON_LAST + 1> model{};

  /// トラックボール処理の途中経過
  glm::dquat trackball{};

  /// マウスホイールの回転量
  glm::dvec2 scroll{ 0.0, 0.0 };

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

    // マウスボタンを押していたら
    if (action != GLFW_RELEASE)
    {
      // 押したマウスボタンを記録する
      instance->button = button;

      // ドラッグ開始時のカーソル位置を保存する
      auto& cursor{ instance->start[button] };
      glfwGetCursorPos(window, &cursor.x, &cursor.y);
    }
    else
    {
      // マウスボタンを離したことを記録する
      instance->button = -1;

      // ドラッグ終了時の回転を保存する
      instance->rotation[button] = instance->trackball;
    }
  }

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

    // マウスホイールの操作時に呼び出す処理を登録する
    glfwSetScrollCallback(window, wheel);

    // マウスボタンの操作時に呼び出す処理を登録する
    glfwSetMouseButtonCallback(window, mouse);

    // 各種の状態の復帰処理を行う
    reset();

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
  /// 更新処理
  ///
  auto update() -> void
  {
    // マウスのいずれのボタンも押されていなければ何もしない
    if (button < GLFW_MOUSE_BUTTON_LEFT) return;

    // マウスの現在位置を取り出す
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    // マウスの相対変位
    const auto dx{ (x - start[button].x) / size.x };
    const auto dy{ (start[button].y - y) / size.y };

    // マウスポインタの位置のドラッグ開始位置からの距離
    const auto length{ hypot(dx, dy) };

    // マウスポインタの位置がドラッグ開始位置から移動していれば何もしない
    if (length == 0.0) return;

    // マウスの移動方向と直交するベクトルを回転軸にする
    const auto axis{ glm::normalize(glm::dvec3(-dy, dx, 0.0)) };

    // マウスの移動量を回転角とした回転を現在の回転と合成する
    trackball = glm::angleAxis(length * M_PI, axis) * rotation[button];

    // 合成した回転の四元数から回転の変換行列を求める
    model[button] = glm::mat4_cast(static_cast<glm::quat>(trackball));
  }

  ///
  /// 復帰処理
  ///
  auto reset() -> void
  {
    // 全てのボタンの回転を初期化する
    std::fill(rotation.begin(), rotation.end(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

    // 全てのボタンのモデル変換行列を初期化する
    std::fill(model.begin(), model.end(), glm::mat4(1.0f));
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
