#version 150

in vec3 vertexPosition;       // Vertex position input
uniform mat4 trans;           // Transformation matrix (model space)
uniform mat4 view;            // View matrix (camera)
uniform mat4 projection;      // Projection matrix (perspective)
uniform vec4 vColor;          // Color to be passed to the fragment shader
out vec4 color;               

void main()
{
  
    gl_Position = projection * view * trans * vec4(vertexPosition, 1.0);
   
    color = vColor;
}
