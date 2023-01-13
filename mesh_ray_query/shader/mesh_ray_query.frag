#version 460
#extension GL_EXT_ray_query : enable

layout (location = 0) in VertexInput {
    vec4 color;
    vec4 normal;
    vec4 position;
} vertexInput;

layout(location = 0) out vec4 outFragColor;

layout(binding = 2) uniform accelerationStructureEXT topLevelAS;

void main() {
    vec3 lightDir = normalize(vec3(1, -1, 2));
    float lighting = dot(vertexInput.normal.xyz, lightDir);
    //float lighting = 1.0;
    outFragColor = vertexInput.color * lighting;
    //outFragColor = vertexInput.position;

    rayQueryEXT rayQuery;
    rayQueryInitializeEXT(
        rayQuery,
        topLevelAS,
        gl_RayFlagsOpaqueEXT, // rayFlags
        0xFF, // cullMask
        vertexInput.position.xyz, // origin
        0.01, // tMin
        lightDir, // direction
        100.0 // tMax
    );
    while (rayQueryProceedEXT(rayQuery)) { }
    if (rayQueryGetIntersectionTypeEXT(rayQuery, true) != gl_RayQueryCommittedIntersectionNoneEXT ) {
        outFragColor *= 0.1;
    }
}
