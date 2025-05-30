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

///
/// 頂点配列オブジェクト
///
struct Object
{
  /// 頂点配列オブジェクト名
  const GLuint vao;

  /// 頂点バッファオブジェクト名
  const GLuint vbo;

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
