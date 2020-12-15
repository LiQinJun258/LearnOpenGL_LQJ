uniform sampler2D texture;
varying vec2 texcoord;
void main() {

        vec4 color = texture2D(texture, texcoord);
        gl_FragColor = vec4(color.xyz, 1.0);
    }
