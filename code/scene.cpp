#include "scene.h"


/*
  
this function should render a scene and somehow return a framebuffer texture


scene node should have:
- scene
- texture



we could have the texture as a function param





 */
global GLuint sceneFBO;
global GLuint scenePBO;
global GLuint sceneRBO;
global GLuint sceneTextureID;
global bool init = false;
// global GLuint testBox;
global Shader testShader;


// void RenderScene2(ObjectHandle sceneHandle, ObjectHandle textureHandle)
// {
// 	Scene *scene = GetScenes()->Get(sceneHandle);

// 	Texture *texture = GetTextures()->Get(textureHandle);
// 	// TODO (rhoe) this should not be hardcoded
// 	texture->Create(800, 600);

// 	glViewport(0, 0, texture->width, texture->height);


// 	// TODO (rhoe) perhaps we need fbo per scene
// 	if(!init) {

// 		// GLfloat vertices[] = {
// 		// 	-0.5f, -0.5f, 0.0f,  
// 		// 	0.5f, -0.5f, 0.0f,  
// 		// 	0.0f, 0.5f, 0.0f,  
// 		// };

// 		// GLuint indices[] = {
// 		// 	0, 1, 2,
// 		// 	2, 3, 0
// 		// };

// 		// glGenVertexArrays(1, &testBox);
// 		// glBindVertexArray(testBox);

// 		// GLuint VBO;
// 		// GLuint EBO;
// 		// glGenBuffers(1, &VBO);
// 		// glGenBuffers(1, &EBO);
// 		// glBindBuffer(GL_ARRAY_BUFFER, VBO);
// 		// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
// 		// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

// 		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
// 		// glEnableVertexAttribArray(0);
// 		// glBindVertexArray(0);
// 		testShader = Shader("assets/test.vert", "assets/test.frag");











		

// 		glGenFramebuffers(1, &sceneFBO);
// 		glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);

// 		glGenTextures(1, &sceneTextureID);
// 		glBindTexture(GL_TEXTURE_2D, sceneTextureID);
// 		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 		glBindTexture(GL_TEXTURE_2D, 0);

// 		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneTextureID, 0);

// 		glGenRenderbuffers(1, &sceneRBO);
// 		glBindRenderbuffer(GL_RENDERBUFFER, sceneRBO);
// 		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, texture->width, texture->height); // use a single renderbuffer object for both a depth AND stencil buffer.
// 		glBindRenderbuffer(GL_RENDERBUFFER, 0);


// 		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, sceneRBO); // now actually attach it

// 		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
// 			printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
// 		glBindFramebuffer(GL_FRAMEBUFFER, 0);  


// 		glGenBuffers(1, &scenePBO);
// 		glBindBuffer(GL_PIXEL_PACK_BUFFER, scenePBO);
// 		glBufferData(GL_PIXEL_PACK_BUFFER, texture->width * texture->height * sizeof(Pixel), NULL, GL_STREAM_COPY);
			
// 		init = true;
// 	}

// 	// render scene to framebuffer
// 	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
//     glDisable(GL_CULL_FACE);
//     glEnable(GL_DEPTH_TEST);
// 	glClearColor(scene->bgColor.r, scene->bgColor.g, scene->bgColor.b, 1.0f);
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

// 	// SHADER
// 	// testShader.Use();
// 	_viewerRenderState.defaultShader.Use();

// 	// float aspectRatio = texture->width / texture->height;
// 	float aspectRatio = (float)VIEWER_SIZE / (float)VIEWER_SIZE;
// 	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
// 											aspectRatio,
// 											0.1f, 1000.0f);

// 	mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
// 	mat4 view = _viewerRenderState.cam.GetViewMatrix();

// 	_viewerRenderState.defaultShader.SetUniform("projection", projection);
// 	_viewerRenderState.defaultShader.SetUniform("view", view);

// 	// glBindVertexArray(testBox);
// 	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// 	// // glDrawArrays(GL_TRIANGLES, 0, 3);
// 	// glBindVertexArray(0);

// 	// ADD SCENE POINTLIGHTS
// 	i32 lightCount = 0;
// 	char buffer[128];
// 	for(i32 i = 0; i < scene->pointLights.Count(); i++) {
// 		Node *node = GetNode(*scene->pointLights.GetAt(i));
// 		PointLight *light = GetPointLights()->Get(node->GetData());
// 		if(light) {
// 			// get pointlight pos uniform
// 			sprintf(buffer, "pointLights[%d].pos", lightCount);
// 			_viewerRenderState.defaultShader.SetUniform(buffer, light->pos);

// 			// get pointlight color uniform
// 			sprintf(buffer, "pointLights[%d].color", lightCount);
// 			_viewerRenderState.defaultShader.SetUniform(buffer, light->color);

// 			lightCount++;
// 		}
// 	}
// 	_viewerRenderState.defaultShader.SetUniform("pointLightAmount", lightCount);


// 	// ////////////////
// 	// // RENDER ACTUAL 3D SCENE HERE
// 	for(i32 i = 0; i < scene->sceneObjects.Count(); i++) {
// 		SceneObject *sceneObject = scene->sceneObjects.GetAt(i);
// 		if(sceneObject) {
// 			Node *node = GetNode(sceneObject->handle);
// 			if(node) {
// 				switch(node->type) {
// 					case DATA_RENDEROBJECT: {
// 						RenderObject *renderObject = GetRenderObjects()->Get(node->GetData());

// 						// TODO (rhoe) we shouldnt have to do so much context switching
// 						SetContextMain();

// 						glActiveTexture(GL_TEXTURE0);
// 						if(renderObject->hasTexture) {
// 							glBindTexture(GL_TEXTURE_2D, renderObject->textureID);
// 						}
// 						else {
// 							glBindTexture(GL_TEXTURE_2D, _viewerRenderState.defaultTexture);
// 						}

// 						// MODEL
// 						model = glm::translate(model, renderObject->pos);
// 						quat q = quat(renderObject->rot);
// 						mat4 rotationMatrix = glm::mat4_cast(q);
// 						model = model * rotationMatrix;
// 						model = glm::scale(model, renderObject->scale);
// 						// _viewerRenderState.defaultShader.SetUniform("model", model);
// 						_viewerRenderState.defaultShader.SetUniform("model", model);

// 						mat3 model3x3 = mat3(model);
// 						mat3 normalMatrix = glm::inverseTranspose(model3x3);
// 						_viewerRenderState.defaultShader.SetUniform("normalMatrix", normalMatrix);

// 						// set color
// 						_viewerRenderState.defaultShader.SetUniform("objectColor", vec4(1, 1, 1, 1));
	
// 						// DRAW
// 						// TODO (rhoe) super duper hack, we need to be more clear about what context we should render to here, and get the correct VAO

// 						// SetContextMain();
// 						GLuint VAO = GetMainContextVAO(renderObject->VAOHandle);
// 						// SetContextViewer();
// 						// GLuint VAO = GetCurrentContextVAO(renderObject->VAOHandle);
// 						glBindVertexArray(VAO);
// 						glDrawElements(GL_TRIANGLES, renderObject->indicesCount, GL_UNSIGNED_INT, 0);
// 						glBindVertexArray(0);

// 						break;
// 					}
// 					case DATA_POINTLIGHT: {
// 						break;
// 					}
// 				}

// 			}
// 		}
// 	}
	
// 	///////////////


// 	glBindFramebuffer(GL_FRAMEBUFFER, 0);

// 	// get texture from GPU
// 	glBindBuffer(GL_PIXEL_PACK_BUFFER, scenePBO);
// 	glBindTexture(GL_TEXTURE_2D, sceneTextureID);
// 	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
// 	void *mappedBuffer = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
// 	if(mappedBuffer) {
// 		memcpy(texture->pixels, mappedBuffer, sizeof(Pixel) * texture->width * texture->height);
// 		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
// 	}
// }
