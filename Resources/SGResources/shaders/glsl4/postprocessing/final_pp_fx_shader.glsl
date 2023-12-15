#define SG_NOT_INCLUDE_LIGHTS

// #include "../uniform_bufs_decl.glsl"
#include "../color_correction/aces.glsl"
#include "../defines.glsl"
#include "../primitives.glsl"

#ifdef VERTEX_SHADER
    out vec2 vs_UVAttribute;

    void main()
    {
        vec2 pos = quad2DVerticesPositions[gl_VertexID].xy;

        vs_UVAttribute = quad2DUVs[gl_VertexID];

        gl_Position = vec4(pos, 0.0, 1.0);
    }
#endif

#ifdef FRAGMENT_SHADER
    uniform int combinedBufferAttachmentsCount;
    uniform sampler2D combinedBuffer[32];

    in vec2 vs_UVAttribute;

    void main()
    {
        vec2 finalUV = vs_UVAttribute.xy;

        #ifdef FLIP_TEXTURES_Y
            finalUV.y = 1.0 - vs_UVAttribute.y;
        #endif

        vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);

        finalColor.rgb = texture(combinedBuffer[0], finalUV).rgb;

        gl_FragColor = finalColor;

        // -----------------------------------------------------
    }
#endif