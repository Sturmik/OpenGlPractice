#shader vertex
#version 330 core  
   
 layout(location = 0) in vec3 position;  // the position variable has attribute position 0

 void main()  
 {  
     gl_Position = vec4(position, 1.0); // directly give vec3 to vec4's constructor
 }  ;

 #shader fragment
     #version 330 core

     out vec4 FragColor;

    uniform vec4 ourColor; // the input variable from the vertex shader (same name and same type)
   
 void main()  
 {  
     FragColor = ourColor;
 }  ;