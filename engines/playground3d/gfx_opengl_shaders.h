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

#ifndef PLAYGROUND3D_GFX_OPENGL_SHADERS_H
#define PLAYGROUND3D_GFX_OPENGL_SHADERS_H

#include "common/rect.h"

#include "math/rect2d.h"

#include "graphics/opengl/shader.h"
#include "graphics/opengl/system_headers.h"

#include "engines/playground3d/gfx.h"

namespace OpenGL {
class Texture;
}

namespace Playground3d {

class ShaderRenderer : public Renderer {
public:
	ShaderRenderer(OSystem *_system);
	virtual ~ShaderRenderer();

	void init() override;
	void deinit() override;

	void clear(const Math::Vector4d &clearColor) override;
	void loadTextureRGBA(Graphics::Surface *texture) override;
	void loadTextureRGB(Graphics::Surface *texture) override;
	void loadTextureRGB565(Graphics::Surface *texture) override;
	void loadTextureRGBA5551(Graphics::Surface *texture) override;
	void loadTextureRGBA4444(Graphics::Surface *texture) override;

	void setupViewport(int x, int y, int width, int height) override;
	void drawCube(const Math::Vector3d &pos, const Math::Vector3d &roll) override;
	void drawPolyOffsetTest(const Math::Vector3d &pos, const Math::Vector3d &roll) override;
	void dimRegionInOut(float fade) override;
	void drawInViewport() override;
	void drawRgbaTexture() override;

	void enableFog(const Math::Vector4d &fogColor) override;
	void disableFog() override;

	void enableScissor(int x, int y, int width, int height) override;
	void disableScissor() override;

private:
	OpenGL::Shader *_cubeShader;
	OpenGL::Shader *_offsetShader;
	OpenGL::Shader *_fadeShader;
	OpenGL::Shader *_viewportShader;
	OpenGL::Shader *_bitmapShader;

	GLuint _cubeVBO;
	GLuint _offsetVBO;
	GLuint _fadeVBO;
	GLuint _viewportVBO;
	GLuint _bitmapVBO;
	GLuint _bitmapTexVBO;

	Common::Rect _currentViewport;
	Math::Vector2d _pos;
	OpenGL::Texture *_textures[TextureType::MAX];
};

} // End of namespace Playground3d

#endif // PLAYGROUND3D_GFX_OPENGL_SHADERS_H
