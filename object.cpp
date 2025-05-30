///
/// 図形関連の処理の実装
/// 
/// @file object.cpp
/// @author Kohe Tokoi
/// @date 2025/5/29
/// 
#include "object.h"

///
/// 頂点配列オブジェクトの作成
///
/// @param[in] count 頂点数
/// @param[in] position 頂点位置
/// @return 作成された頂点配列オブジェクト名
///
auto createObject(GLuint count, const GLfloat(*position)[3]) -> GLuint
{
  // 頂点配列オブジェクトを作成して結合する
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // 頂点バッファオブジェクトを作成して結合する
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat[3]) * count, position, GL_DYNAMIC_DRAW);

  // 結合されている頂点バッファオブジェクトを in 変数から参照できるようにする
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // 頂点配列オブジェクトの結合と頂点バッファオブジェクトの結合を解除する
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // 頂点配列オブジェクトの名前を返す
  return vao;
}
