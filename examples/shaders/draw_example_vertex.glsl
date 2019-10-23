layout(location = 0) 
in vec3 pos_ws;

void main() {
	gl_Position = vec4(pos_ws, 1.0);
}