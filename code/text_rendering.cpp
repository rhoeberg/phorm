// #define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
// #include "stb_truetype.h"

unsigned char ttf_buffer[1<<20];
unsigned char temp_bitmap[512*512];

stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
GLuint ftex;
GLuint textShader;

void my_stbtt_initfont(void)
{
	FILE *file = fopen("c:/windows/fonts/times.ttf", "rb");
	if(!file) {
		printf("font file not found\n");
	}
	fread(ttf_buffer, 1, 1<<20, file);
	stbtt_BakeFontBitmap(ttf_buffer,0, 32.0, temp_bitmap,512,512, 32,96, cdata); // no guarantee this fits!
	// can free ttf_buffer at this point

	glGenTextures(1, &ftex);
	glBindTexture(GL_TEXTURE_2D, ftex);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512,512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512,512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
	// can free temp_bitmap at this point
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	textShader = createShaderProgram("assets/font_shader.vert", "assets/font_shader.frag");
}

void my_stbtt_print(float x, float y, char *text)
{
	
	// assume orthographic projection with units = screen pixels, origin at top left
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ftex);
	glUseProgram(textShader);

	while (*text) {
		if (*text >= 32 && *text < 128) {
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(cdata, 512,512, *text-32, &x,&y,&q,1);//1=opengl & d3d10+,0=d3d9

			GLfloat vertices[] = {
				q.x0, q.y0, q.s0, q.t0,
				q.x1, q.y0, q.s1, q.t0,
				q.x1, q.y1, q.s1, q.t1,
				q.x0, q.y1, q.s0, q.t1
			};
			// GLfloat vertices[] = {
			// 	q.x0, q.y0, q.s0, q.t0,
			// 	q.x1, q.y0, q.s1, q.t0,
			// 	q.x1, q.y1, q.s1, q.t1,
			// 	q.x0, q.y1, q.s0, q.t1
			// };

			GLuint indices[] = {
				0, 1, 2,
				2, 3, 0
			};

			GLuint VAO, VBO, EBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glBindVertexArray(0);

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		++text;
	}

	glUseProgram(0);
}
