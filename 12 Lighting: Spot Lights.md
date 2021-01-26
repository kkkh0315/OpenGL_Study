## Spot Lights
  * Spot lights are just like point lights, but added with a direction and a cut-off angle.
    * Direction: Where the spot light is facing.
    * Cut-off angle: The angle describing the 'edges'of the light from the direction vector.
      * Compare the cut-off angle(cutOffAngle) and the angle(angleToFragment) made between the vector from the light to the fragment, and the direction.
      * If the latter is smaller than the former, the fragment should be lit.
      * The comparison is done by comparing the dot products of the two. Larger the dot product is, smaller the angle.
    * Soft edges: Even though a fragment is within the cut-off angle, not every fragment is lit with the same amount. Spot light has soft edges that is the brightest at the center and gets dimmed as it gets further from the center.
      * How to implement: Using the dot product itself is not enough. We should scale dot product range to 0 to 1 for more dynamic and realistic effect.
      * spotLightFade = 1 - (1 - angleToFragment) / (1 - cutOffAngle)
      * color = spotLightColor * spotLightFade
      
## Implmenting spot lights with C++ and OpenGL
