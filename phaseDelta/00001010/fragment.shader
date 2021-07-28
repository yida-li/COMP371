#version 330 core
out vec4 FragColor;
in vec3 ourColor;
void main()
{
    //  FragColor = vec4(ourColor, 1.0f);
    FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);  // color from 4,5,6 slot
};