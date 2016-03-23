// Attribute to indicate the position of the vertex
attribute vec3 Position;

// Attribute to indeicate the texture coordinate
attribute vec2 TexCoordIn;

// Uniform that acts as a constant for all the vertices
uniform mat4 Projection;

// Model-View transform matrix (transform/scale/rotate)
uniform mat4 Modelview;

// Varing that will be passed to the fragment shader
varying vec2 TexCoordOut;

void main(void) {
    // The destination texture is equal the source texrture coordinate, let opengl interpolate the values
    TexCoordOut = TexCoordIn;
    // Set the original positin without change
    gl_Position = Projection * Modelview * vec4(Position, 1.0);
}