#version 460 core

out vec4 FragColor;

in vec3 worldPos;


void main()
{
    float cellSize = 1.0;
    vec4 lineColor = vec4(0.5, 0.5, 0.5, 1.0);

    vec2 coord = worldPos.xz / cellSize;
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);

    float line = min(grid.x, grid.y);
    float alpha = 1.0 - min(line, 1.0);

    float lineWidth = 1.0;

    if (line > lineWidth) discard;

    FragColor = vec4(lineColor.rgb, alpha);

}