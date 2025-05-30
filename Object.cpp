///
/// 図形関連の処理の実装
///
/// @file object.cpp
/// @author Kohe Tokoi
/// @date 2025/5/29
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
