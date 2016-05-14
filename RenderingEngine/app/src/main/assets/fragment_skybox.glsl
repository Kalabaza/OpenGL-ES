// Input variable from the vertex shader, in the fragment shader the precision is needed
// and always start from the lowest to the highest
varying lowp vec3 TexCoords;

// Texture object
uniform samplerCube Texture;

void main(void) {
    // Set the fragment color to the texture by using the interpolated values
    gl_FragColor = textureCube(Texture, TexCoords);
}