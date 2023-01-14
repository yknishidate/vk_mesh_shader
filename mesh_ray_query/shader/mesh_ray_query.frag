#version 460

layout (location = 0) in VertexInput {
    vec4 color;
} vertexInput;

layout(location = 0) out vec4 outFragColor;

void main() {
    outFragColor = vertexInput.color;
}
