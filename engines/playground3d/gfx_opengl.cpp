/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "common/rect.h"
#include "common/textconsole.h"

#if defined(USE_OPENGL_GAME)

#include "graphics/opengl/context.h"
#include "graphics/opengl/texture.h"
#include "graphics/surface.h"

#include "engines/playground3d/gfx.h"
#include "engines/playground3d/gfx_opengl.h"

namespace Playground3d {

static const GLfloat dimRegionVertices[] = {
	//  X      Y
	-0.5f,  0.5f,
	 0.5f,  0.5f,
	-0.5f, -0.5f,
	 0.5f, -0.5f,
};

static const GLfloat boxVertices[] = {
	//  X      Y
	-1.0f,  1.0f,
	 1.0f,  1.0f,
	-1.0f, -1.0f,
	 1.0f, -1.0f,
};

static const GLfloat bitmapVertices[] = {
	//  X      Y
	-0.2f,  0.2f,
	 0.2f,  0.2f,
	-0.2f, -0.2f,
	 0.2f, -0.2f,
};

Renderer *CreateGfxOpenGL(OSystem *system) {
	return new OpenGLRenderer(system);
}

OpenGLRenderer::OpenGLRenderer(OSystem *system) :
		Renderer(system), _textures{} {
}

OpenGLRenderer::~OpenGLRenderer() {
}

void OpenGLRenderer::init() {
	debug("Initializing OpenGL Renderer");

	computeScreenViewport();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void OpenGLRenderer::deinit() {
	for(int i = 0; i < ARRAYSIZE(_textures); i++) {
		delete _textures[i];
		_textures[i] = nullptr;
	}
}

void OpenGLRenderer::clear(const Math::Vector4d &clearColor) {
	glClearColor(clearColor.x(), clearColor.y(), clearColor.z(), clearColor.w());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::loadTextureRGBA(Graphics::Surface *texture) {
	if (!_textures[TextureType::RGBA8888]) {
		_textures[TextureType::RGBA8888] = new OpenGL::Texture(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	}
	_textures[TextureType::RGBA8888]->setSize(texture->w, texture->h);
	_textures[TextureType::RGBA8888]->updateArea(Common::Rect(texture->w, texture->h), *texture);
}

void OpenGLRenderer::loadTextureRGB(Graphics::Surface *texture) {
	if (!_textures[TextureType::RGB888]) {
		_textures[TextureType::RGB888] = new OpenGL::Texture(GL_RGBA, GL_RGB, GL_UNSIGNED_BYTE);
	}
	_textures[TextureType::RGB888]->setSize(texture->w, texture->h);
	_textures[TextureType::RGB888]->updateArea(Common::Rect(texture->w, texture->h), *texture);
}

void OpenGLRenderer::loadTextureRGB565(Graphics::Surface *texture) {
	if (!_textures[TextureType::RGB565]) {
		_textures[TextureType::RGB565] = new OpenGL::Texture(GL_RGBA, GL_RGB, GL_UNSIGNED_SHORT_5_6_5);
	}
	_textures[TextureType::RGB565]->setSize(texture->w, texture->h);
	_textures[TextureType::RGB565]->updateArea(Common::Rect(texture->w, texture->h), *texture);
}

void OpenGLRenderer::loadTextureRGBA5551(Graphics::Surface *texture) {
	if (!_textures[TextureType::RGBA5551]) {
		_textures[TextureType::RGBA5551] = new OpenGL::Texture(GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1);
	}
	_textures[TextureType::RGBA5551]->setSize(texture->w, texture->h);
	_textures[TextureType::RGBA5551]->updateArea(Common::Rect(texture->w, texture->h), *texture);
}

void OpenGLRenderer::loadTextureRGBA4444(Graphics::Surface *texture) {
	if (!_textures[TextureType::RGBA4444]) {
		_textures[TextureType::RGBA4444] = new OpenGL::Texture(GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4);
	}
	_textures[TextureType::RGBA4444]->setSize(texture->w, texture->h);
	_textures[TextureType::RGBA4444]->updateArea(Common::Rect(texture->w, texture->h), *texture);
}

void OpenGLRenderer::setupViewport(int x, int y, int width, int height) {
	glViewport(x, y, width, height);
}

void OpenGLRenderer::enableFog(const Math::Vector4d &fogColor) {
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_START, 1.0f);
	glFogf(GL_FOG_END, 1.0f);
	glFogf(GL_FOG_DENSITY, 0.1f);
	GLfloat color[4] = { fogColor.x(), fogColor.y(), fogColor.z(), fogColor.w() };
	glFogfv(GL_FOG_COLOR, color);
	glEnable(GL_FOG);
}

void OpenGLRenderer::disableFog() {
	glDisable(GL_FOG);
}

void OpenGLRenderer::enableScissor(int x, int y, int width, int height) {
	glScissor(x, y, width, height);
	glEnable(GL_SCISSOR_TEST);
}

void OpenGLRenderer::disableScissor() {
	glDisable(GL_SCISSOR_TEST);
}

void OpenGLRenderer::drawFace(uint face) {
	glBegin(GL_TRIANGLE_STRIP);
	for (uint i = 0; i < 4; i++) {
		glColor4f(cubeVertices[11 * (4 * face + i) + 8], cubeVertices[11 * (4 * face + i) + 9], cubeVertices[11 * (4 * face + i) + 10], 1.0f);
		glVertex3f(cubeVertices[11 * (4 * face + i) + 2], cubeVertices[11 * (4 * face + i) + 3], cubeVertices[11 * (4 * face + i) + 4]);
		glNormal3f(cubeVertices[11 * (4 * face + i) + 5], cubeVertices[11 * (4 * face + i) + 6], cubeVertices[11 * (4 * face + i) + 7]);
	}
	glEnd();
}

void OpenGLRenderer::drawCube(const Math::Vector3d &pos, const Math::Vector3d &roll) {
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(_projectionMatrix.getData());

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(_modelViewMatrix.getData());

	glDisable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ZERO);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_TEXTURE_2D);

	glTranslatef(pos.x(), pos.y(), pos.z());
	glRotatef(roll.x(), 1.0f, 0.0f, 0.0f);
	glRotatef(roll.y(), 0.0f, 1.0f, 0.0f);
	glRotatef(roll.z(), 0.0f, 0.0f, 1.0f);

	for (uint i = 0; i < 6; i++) {
		drawFace(i);
	}
}

void OpenGLRenderer::drawPolyOffsetTest(const Math::Vector3d &pos, const Math::Vector3d &roll) {
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(_projectionMatrix.getData());

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(_modelViewMatrix.getData());

	glDisable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ZERO);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_TEXTURE_2D);

	glTranslatef(pos.x(), pos.y(), pos.z());
	glRotatef(roll.y(), 0.0f, 1.0f, 0.0f);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(-1.0f,  1.0, 0.0f);
	glVertex3f( 1.0f,  1.0, 0.0f);
	glVertex3f( 0.0f, -1.0, 0.0f);
	glEnd();

	glPolygonOffset(-1.0f, 0.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5f,  0.5, 0.0f);
	glVertex3f( 0.5f,  0.5, 0.0f);
	glVertex3f( 0.0f, -0.5, 0.0f);
	glEnd();
	glDisable(GL_POLYGON_OFFSET_FILL);
}

void OpenGLRenderer::dimRegionInOut(float fade) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f - fade);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), dimRegionVertices);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void OpenGLRenderer::drawInViewport() {
	static GLfloat box2Vertices[] = {
		//  X      Y
		-0.1f,  0.1f,
		 0.1f,  0.1f,
		-0.1f, -0.1f,
		 0.1f, -0.1f,
	};
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), &boxVertices[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPushMatrix();
	_pos.x() += 0.01f;
	_pos.y() += 0.01f;
	if (_pos.x() >= 1.1f) {
		_pos.x() = -1.1f;
		_pos.y() = -1.1f;
	}
	glTranslatef(_pos.x(), _pos.y(), 0);

	glPolygonOffset(-1.0f, 0.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), &box2Vertices[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_POLYGON_OFFSET_FILL);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void OpenGLRenderer::drawRgbaTexture() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_TEXTURE_2D);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glTranslatef(-0.8f, 0.8f, 0);

	glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), bitmapVertices);
	glTexCoordPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), _textures[TextureType::RGBA8888]->getTexCoords());
	if (_textures[TextureType::RGBA8888]->bind()) {
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glTranslatef(0.5, 0, 0);

	glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), bitmapVertices);
	glTexCoordPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), _textures[TextureType::RGB888]->getTexCoords());
	if (_textures[TextureType::RGB888]->bind()) {
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glTranslatef(0.5, 0, 0);

	glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), bitmapVertices);
	glTexCoordPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), _textures[TextureType::RGB565]->getTexCoords());
	if (_textures[TextureType::RGB565]->bind()) {
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glTranslatef(0.5, 0, 0);

	glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), bitmapVertices);
	glTexCoordPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), _textures[TextureType::RGBA5551]->getTexCoords());
	if (_textures[TextureType::RGBA5551]->bind()) {
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glTranslatef(-1.5, -0.5, 0);

	glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), bitmapVertices);
	glTexCoordPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), _textures[TextureType::RGBA4444]->getTexCoords());
	if (_textures[TextureType::RGBA4444]->bind()) {
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

} // End of namespace Playground3d

#endif
