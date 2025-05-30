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
/// 頂点配列オブジェクトの作成
///
/// @param[in] count 頂点数
/// @param[in] position 頂点位置
/// @return 作成された頂点配列オブジェクト名
///
extern auto createObject(GLuint count, const GLfloat(*position)[3]) -> GLuint;
