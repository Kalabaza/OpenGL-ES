// Input variable from the vertex shader, in the fragment shader the precision is needed
// and always start from the lowest to the highest
varying lowp vec2 TexCoordOut;

// Texture object
uniform sampler2D Texture;

void main(void) {
    // Set the fragment color to the texture by using the interpolated values
    gl_FragColor = texture2D(Texture, TexCoordOut);
}