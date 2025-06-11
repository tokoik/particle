#version 430 core

// フラグメントの色
layout (location = 0) out vec4 color;

void main()
{
  // フラグメントの色として白色を出力する
  color = vec4(0.32f, 0.18f, 0.1f, 1.0f);
}
