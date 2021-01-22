## Phong Lighting
  * 3 parts
    * **Ambient Light**: Light that is always present, even if a light source's direct path is blocked.
      1. Simplest lighting concept.
      2. Simulates light bouncing off other objects.
      3. How to calculate ambient
          * ambient lighting factor
          * ambient = lightColor * ambientStrength  
          * **ambient is ususally between 0(no power) to 1(full power)**
    * **Diffuse Light**: Light determined by direction of light source. Creates a faded effect further from the  light.
      1. Simulates the drop-off of lighting as angle of lighting becomes more shallow.
      2. Side facing directly at a light is brightly lit, whereas that facing at an angle is more dim.
      3. Can use the angle between the vector connecting light source to fragment and the vector perpendicular to the surface (which is "normal").
      4. How to calculate diffuse
          * Use dot product
            * v1 * v2 = |v1| * |v2| * cos(θ)
            * If both vectors are normalized, |v1| = |v2| = 1
            * So: **v1 * v2 = cos(θ)**
            * cf) cos(0°) = 1, cos(90°) = 0. Therefore, diffuse is between 0 and 1.
            * If diffuse is negative, the light is behind the object, so default to 0.
            ![diffuse](https://user-images.githubusercontent.com/60923302/105457691-96516d00-5cca-11eb-8eaa-a2342eab5ce1.png)
      5. How to calculate normals
          * **Phong Shading**
            * Each vertex has an average of the normals of all the surfaces it is part of.
            * Interpolate between these averages in shader to create smooth effect.
            * Problem: non-uniform scales.
              * When scaling is done non-uniformly, normals are wrongly skewed.
              * Solution: create a **normal matrix** from model matrix.
                * Transform normals with: mat3(transpose(inverse(model)))
                * Full explanation: [normal matrix](http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/)  
    * **Specular Light**: Light reflected perfectly from the source to the viewer's eye. Efectively a reflection of the light source. More prominent on shiny objects.
      1. Specular relies on the position of the viewer.
      2. It is the direct reflection of the light source hitting the viewer's eye.
      3. Therefore, we need: light vector, normal vector, reflection vector, view vector
         ![specular](https://user-images.githubusercontent.com/60923302/105457543-55f1ef00-5cca-11eb-86e1-f8427400e2c4.png)
      4. How to calculate specular
          * Dot product of normalized forms of viewer and reflection vector, and put it to the power of shininess value  
            **specular = (view * reflection)^shininess
      5. How to calculate reflection vector
          * built-in GLSL function: reflect(light_source, normal)
