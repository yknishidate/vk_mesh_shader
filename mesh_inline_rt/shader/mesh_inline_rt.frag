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
    //float lighting = dot(vertexInput.normal.xyz, lightDir);
    //float lighting = 1.0;
    outFragColor = vertexInput.color;

    rayQueryEXT rayQuery;
    rayQueryInitializeEXT(
        rayQuery,
        topLevelAS,
        gl_RayFlagsTerminateOnFirstHitEXT, // rayFlags
        0xFF,                              // cullMask
        vertexInput.position.xyz,                          // origin
        0.001,                              // tMin
        lightDir,                    // direction
        1000.0                              // tMax
    );
    while (rayQueryProceedEXT(rayQuery)) { }
    if (rayQueryGetIntersectionTypeEXT(rayQuery, true) == gl_RayQueryCommittedIntersectionTriangleEXT ) {
        outFragColor *= 0.2;
    }
    
    //outFragColor = vertexInput.color * lighting;
}
