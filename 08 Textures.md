## Textures
  * Textures are images used to add extra detail to an object, but can be used to hold generic data too.
  * Points on textures are 'texels'.
  * Texels are defined between 0 and 1. (So, it's more like a percentage concept.)
      * So to sample a point at the top-middle, you reference texel (0.5, 1.0).
  * Map texels to vertices.
  * Interpolation over each fragment will calculate appropriate texels in between the assigned texels.

## Texture Objects
  * Createing textures works much like creating VBOs/VAOs.
      * glGenTextures(1, &textureID);
      * glBindTexture(GL_TEXTURE_2D, textureID);
          * There are different types of textures, such as GL_TEXTURE_1D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP.
      * glTexImage2D(*Texture Target*, *Mipmap Level*, *Format of the stored data*, *texture width*, *texture height*, 0, *Format of the data being loaded*, *The data type of the values(int, float, byte, etc)*, *The data itself*)
  * **Mipmaps**
      * Resolution limitations for textures.
      * The closer we get to an object, the more pixelated the texture becomes. Further away. it attemps to render multiple texels on one pixel.
          * Solution: Create multiple versions of image at different resolutions and switch between them based on distance.
      * If mipmap level is 0, it means that we are using one texture for the whole time regardless of the distance from it.
  * **Texture Parameters**
      1. **Filters**
          * Off center render
          * Two possibilities:
              1. Nearest: Use the texel with most overlap (creates pixelated effect)
              2. Linear: Use a weighted average of surrounding texels (blends pixel boundaries)
          * How?
              * glTexParameteri(*Texture Target*, GL_TEXTURE_MIN_FILTER, GL_LINEAR(or GL_NEAREST))
              * glTexParameteri(*Texture Target*, GL_TEXTURE_MAG_FILTER, GL_LINEAR(or GL_NEAREST))
              * GL_TEXTURE_MIN_FILTER: Filter to apply when texture is made smaller (is further away).
              * GL_TEXTURE_MAG_FILTER: Filter to apply when texture is made bigger (is very close).
      2. **Wrap**
          * Sample a point outside the 0, 1 range.
          * Multiple ways to handle it:
              1. Repeat the texture.
              2. Repeat a mirrored form of the texture.
              3. Extend pixels at the edge.
              4. Apply a coloured border.
          * How?
              * glTexParameteri(*Texture Target*, GL_TEXTURE_WRAP_S, GL_REPEAT (or GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER)) 
              * glTexParameteri(*Texture Target*, GL_TEXTURE_WRAP_T, GL_REPEAT (or GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER))
  * **Loading Images for Textures**
      * Libraries: SOIL(Simple OpenGL Image Library), stb_image
          * stb_image
              * the project should start with *#define STB_IMAGE_IMPLEMENTATION*
                  ~~~~
                  unsigned char *data = stbi_load("image.jpg", &width, &height, &bitDepth, 0);
                  ~~~~
  * **Texture Samplers**
      * Textures in shaders are accessed via 'Sapmlers'.
      * Textures are attached to a 'Texture Unit'.
      * Samplers access textures attached to their Texture Units.
      * In shader, use 'sampler2D' type.
      * To get the value of a texel, use GLSL 'texture' function.
          * texture(textureSampler, TexCoord)
              * textureSampler: the sampler2D object
              * TexCoord: the interpolated texel co-ordinate in the Fragment Shader  
