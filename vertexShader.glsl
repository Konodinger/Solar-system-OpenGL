#version 330 core            // Minimal GL version support expected from the GPU

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoord;
uniform mat4 viewMat, projMat, transMat;
out vec3 fNormal, fPosition;
out vec2 fTexCoord;

void main() {
        gl_Position = projMat * viewMat * transMat * vec4(vPosition, 1.0); // mandatory to rasterize properly
        // ...

        fNormal = mat3(transMat) * vNormal;
        fPosition = vec3((transMat * vec4(vPosition, 1.0))); //will be passed to the next stage
        fTexCoord = vTexCoord;
}
