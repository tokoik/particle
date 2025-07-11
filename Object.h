#pragma once

///
/// 図形関連の処理
///
/// @file object.h
/// @author Kohe Tokoi
/// @date 2025/5/29
///

// バッファオブジェクト関連の宣言は gl.h に含まれていないので glew.h を使う
#include <GL/glew.h>

// GLM 関連
#if !defined(_USE_MATH_DEFINES)
#  define _USE_MATH_DEFINES
#endif
#if !defined(GLM_FORCE_RADIANS)
#  define GLM_FORCE_RADIANS
#endif
#include <GLM/glm.hpp>

///
/// 粒子の物理量
///
struct Particle
{
  /// 位置
  alignas(16) glm::vec4 position;

  /// 速度
  alignas(16) glm::vec3 velocity;

  /// 力
  alignas(16) glm::vec3 force;
};

///
/// 頂点配列オブジェクト
///
struct Object
{
  /// 頂点配列オブジェクト名
  const GLuint vao;

  /// 頂点バッファオブジェクト名
  const GLuint vbo;

  /// 頂点の数
  const GLsizei count;

  ///
  /// コンストラクタ
  ///
  /// @param[in] count 頂点の数
  /// @param[in] data データ
  ///
  Object(GLsizei count, const void* data = nullptr);

  // コピーコンストラクタは使用しない
  Object(const Object& object) = delete;

  // ムーブコンストラクタはデフォルトのものを使用する
  Object(Object&& object) = default;

  ///
  /// デストラクタ
  ///
  virtual ~Object();

  // 代入演算子は使用しない
  Object& operator=(const Object& object) = delete;

  // ムーブ代入演算子はデフォルトのものを使用する
  Object& operator=(Object&& object) = default;
};
