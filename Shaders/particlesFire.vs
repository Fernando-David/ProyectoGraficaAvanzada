#version 400

layout (location = 0) in vec3 VertexInitVel;
layout (location = 1) in float VertexStartTime;

out float Transp;

uniform float Time;
uniform float ParticleLifetime;
uniform float ParticleSize;

uniform vec3 Accel;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    float t = Time - VertexStartTime;

    if (t < 0.0 || t > ParticleLifetime) {
        Transp = 0.0;
        gl_PointSize = 0.0;
        gl_Position = vec4(2.0, 2.0, 2.0, 1.0);
        return;
    }

    vec3 pos = VertexInitVel * t + 0.5 * Accel * t * t;

    gl_Position = projection * view * model * vec4(pos, 1.0);

    gl_PointSize = ParticleSize;

    Transp = clamp(1.0 - t / ParticleLifetime, 0.0, 1.0);
}