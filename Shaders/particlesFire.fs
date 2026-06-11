#version 400

uniform sampler2D ParticleTex;

in float Transp;

layout (location = 0) out vec4 FragColor;

void main()
{
    vec4 texColor = texture(ParticleTex, gl_PointCoord);

    float alpha = texColor.a;

    // Si la textura no tiene canal alpha útil, usamos intensidad del color
    if (alpha >= 0.99) {
        alpha = max(texColor.r, max(texColor.g, texColor.b));
    }

    if (alpha < 0.05 || Transp <= 0.0) {
        discard;
    }

    vec3 finalColor = mix(vec3(0.0), texColor.rgb, Transp);

    FragColor = vec4(finalColor, alpha * Transp);
}