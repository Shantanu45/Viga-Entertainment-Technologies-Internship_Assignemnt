### Directory Structure

  - **Shader/** 
  
      Contains `shader.vert` (Vertex Shader) and `shader.frag` (Fragment Shader).
  
  - **Texture/**
  
      Contains all required textures.
      
  - **Camera.cpp and Camera.h**
      
      Controls camera movement and contains  information for View Matrix.
      
  - **imports.h**
  
      Contains includes for Main.cpp. Created to make Main.cpp cleaner.
      
  - **Light.cpp and Light.h**
  
      Setup for Diffuse and Ambient Lighting.
      
  - **Main.cpp**
      
      main file that combines everything.
      
  - **Matetial.cpp and Material.h**
  
      Setup for specular highlights for the materials
      
  - **Mesh.cpp and Mesh.h**
  
      Contians VAO, VBO and IBO configurations
      
 - **Shader.cpp and Shader.h**
 
      For Reading `shader.vert` and `shader.frag` shaders form `Shader/` and Compiling them
      
 - **stb_image.h**
    
      Simple library for reading image and manipulating data
      
 - **Texture.cpp and Texture.h**
 
      Setup for projecting Textures onto the models
      
 - **Window.cpp and Window.h**
 
      For creating simple GLFW window and providing Key and Mouse Controls
