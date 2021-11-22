#pragma once

void ImDrawSetColor(vec3 color);
void ImDrawPushQuad(vec2 p1, vec2 p2, vec2 p3, vec2 p4);
void ImDrawPushTriangle(vec2 p1, vec2 p2, vec2 p3);
void ImDrawPoint(vec2 p, float size);
void ImDrawLine(vec2 a, vec2 b,float size);
void ImDrawRect(Rect rect);
void ImDrawText(vec2 pos, char *text);
