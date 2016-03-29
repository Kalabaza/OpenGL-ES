// Attribute to indicate the position of the vertex
attribute vec3 Position;

// Varing that will be passed to the fragment shader
varying vec3 TexCoords;

// Uniform that acts as a constant for all the vertices
uniform mat4 Projection;

// Modelview transform matrix (transform/scale/rotate)
uniform mat4 Modelview;

void main(void) {
    TexCoords = Position;
    vec4 pos = Projection * Modelview * vec4(Position, 1.0);
    gl_Position = pos.xyww;
}