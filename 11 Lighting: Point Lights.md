## Point Lights
  * Lights with a position that emit light in all directions.
  * Need to determine direction vector manually.
  * Get difference between light position and fragment position.
  * Apply directional lighting maths to the calculated direction vector.
  * Attenuation
    * Distance from point being lit changes power of lighting, because unlike directional light, point lights have positions.
    * In reality, light intensity intially drops quickly with distance, but the further you are, the slower it decreases.
    * For positive values, the reciprocal of a quadratic function can create this effect.
      * 1/(ax² + bx + c), where x is the distance between the light source and the fragment.
      * c is usually 1.0 to ensure denominator is always greater than 1
      * [Wiki for Point Light Attenuation](http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation)
    * Apply attenuation to ambient, diffuse, and specular.
    
## Implementing Point Lights with C++ and OpenGL
  
      
