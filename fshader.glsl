#version 150

in vec4 color;  // Color passed from vertex shader
out vec4 fragColor;  // Final color to be output

void main()
{
    fragColor = color;  // Set the fragment color to the input color
}
