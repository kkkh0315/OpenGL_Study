## The Camera
  * The Camera processes the scene as seen in 'View Space'.
      * View Space is the co-ordinate system with each vertex as seen from the camera.
      * Use a View Matrix to convert from World Space to View Space.
      * **View Matrix requires 4 values: Camera Position, Direction, Right, Up.**
          1. Camera Position: The position of the camera.
          2. Direction: The direction the camera is looking in.
              * Direction vector actually points in opposite direction of the intuitive 'direction'. **This is because when we want to move the camera to shoot motion, in fact, we move the world in the opposite direction and let the camera stay at its position.**
          3. Right: Vector facing right of the camera.
              * Can calculate by doing cross product of Direction and 'up' vector, which is [0, 1, 0]. This is because Right always makes 90 degrees with both Direction and 'up'.
          4. Up: Upwards relative to where camera is facing.
              5. Can calcuate by doing cross product of Direction and Right vectors.
      * View Matrix  
          ![view matrix](https://user-images.githubusercontent.com/60923302/102607479-b772eb00-416b-11eb-9f13-50d86ca02094.png)
      * How to create the View Matrix with GLM
          * **glm::lookAt(position, target, up)**
              * position: Camera position
              * target: Point for camera to look at. Usually the camera's position with a direction added on to it.
              * up: Usually (0, 1, 0).
              ~~~~
              // vertex shader
              gl_Position = projection matrix * view matrix * model matrix * vec4(pos, 1.0); //ORDER MATTERS!!
              ~~~~
## Input: Moving the Camera
  * Just need to change the camera position. 
  * How to get input with GLFW
      * glfwGetKey(window, GLFW_KEY_W)
      * Then, add the value to the camera position while key held.
      * (Use deltaTime to keep consistent speed!)
          * [deltaTime reference](https://gafferongames.com/post/fix_your_timestep/)
  * Turning
      * Three types of angle exist.
          1. **Pitch**: Looking up and down. x-axis turn.
          2. **Yaw**: Looking left and right. y-axis turn.
          3. **Roll**: z-axis turn.
          * **IMPORTANT! Pitching needs to rotate the view up and down using an axis relative to the yaw.** 
          * Math of pitch and yaw
              ![pitch](https://user-images.githubusercontent.com/60923302/102611972-49322680-4173-11eb-8064-6511a021fe60.png)  
              ![yaw](https://user-images.githubusercontent.com/60923302/102611975-4afbea00-4173-11eb-84a7-a641df95e032.png)  
              ![pitchandyaw](https://user-images.githubusercontent.com/60923302/102611976-4afbea00-4173-11eb-8d7f-c610136e1396.png)
      * How to do turning with GLFW
          * **glfwSetCursorPosCallback(window, callback)**
          * Store old mouse position, compare to new position. Use difference to decide ptich and yaw change.
  * Callback Function
      * A callback function is a function passed into another function as an argument, which is then invoked inside the outer function to complete some kind of routine or action.
      * Why necessary?
          * If there is a X function which should be called under a specific condition, for instance, when the key is pressed, we pass that X function to the function that checks if the key is pressed as an argument.
