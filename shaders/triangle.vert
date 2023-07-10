#version 450

layout(location = 0) in vec2 in_positions;

void main() {
    gl_Position = vec4(in_positions, 0.0f, 1.0f);
}