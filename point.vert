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
