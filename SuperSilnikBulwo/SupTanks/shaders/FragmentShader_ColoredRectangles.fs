
uniform vec4 fragColor;

void main() {
    gl_FragColor = vec4(fragColor[0], fragColor[1], fragColor[2], fragColor[3]);
}
