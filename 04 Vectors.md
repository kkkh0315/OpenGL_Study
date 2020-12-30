## Vector
  * **Vector**
    * Definition: A quantity with magnitude and direction.
    * Vector magnitude
        * Pythogorean Theorem used
        * |v| = sqrt(pow(v(x)) + pow(v(y)) + pow(v(z)))
    * **(VERY IMPORTANT!!) Dot product(or Scalar Product) / 점곱(혹은 스칼라곱)**
        * Used for lots of things: reflection, collision detection, project one vector to another, checking where objects are relation to each other, calculating how far objects are from each other
        * Methodology
            1. [a,b,c]·[d,e,f] = a × d + b × e + c × f
            2. v1·v2 = |v1| × |v2| × cos(θ)
                * |v| is the magnitude of the vector v
                * θ is the angle between v1 and v2
        * If we know two vectors, we can calculate the angle between them by using methodology 1 and 2.
            * θ = arccos((v1·v2) / (|v1| × |v2|))
        * Used to calculate the angle of the light instance and the normal of the surface.  
            <img src="https://user-images.githubusercontent.com/60923302/102349049-11e23f00-3fe6-11eb-95ae-ebad7f0f4531.png" width="380" height="280">
    * **Unit Vector**
        * Definition: Vector with the magnitude of 1.
        * Use 'u' as a symbol.
        * u = v/|v|  
            ex) v = [1,2,2], |v| = 3, u = v/|v| = [1,2,2] / 3 = [1/3, 2/3, 2/3]
        * Unit vector(u) has the same direction as v but is only one unit in magnitude.
        * How to initialize a unit vector using GLM
            ~~~~
            glm::mat4 model(1.0f);
            ~~~~
    * **(IMPORTANT) Cross product**
        * Creates a vector at right angles to two other vectors.
        * 두 벡터에 모두 수직인 벡터를 구합니다. 예컨대, 두 벡터가 한 평면 위에 있다면 그 평면에 수직인 벡터를 구합니다.
        * Methodology
            ![cross_product](https://user-images.githubusercontent.com/60923302/102348689-97192400-3fe5-11eb-9ee1-3911feeaf7d9.png)  
    * 'Matrix × Vector' is used to modify the vector.  
        <img src="https://user-images.githubusercontent.com/60923302/102350526-64245f80-3fe8-11eb-8dc7-21116e4e45c1.png" width="600" height="200">
        * This allows us to do transforms(translation, rotation, scaling).
            * Translating  
                <img src="https://user-images.githubusercontent.com/60923302/102351055-2bd15100-3fe9-11eb-9eb9-e64ce7fef623.png" width="600" height="200">
            * Scaling  
                <img src="https://user-images.githubusercontent.com/60923302/102351360-a8fcc600-3fe9-11eb-9588-c67013e252d9.png" width="600" height="200">
            * Rotating  
                <img src="https://user-images.githubusercontent.com/60923302/102351923-8cad5900-3fea-11eb-9af0-8c83c42e2b6a.png" width="600" height="340">
        * Multiplying these matrices, and multiplying it with the vector allows us to do translating, scaling, and rotating at once.
            * BUT!! Transform happens in reverse order of the multiplication done.  
                ex) translate matrix × scale matrix × vector -> scale done first, translate done next  
                ex) scale matrix × translate matrix × vector -> translate done first, scale done next
        * However, we don't need to memorize all this because there are math libraries (like GLM) that do the math.
