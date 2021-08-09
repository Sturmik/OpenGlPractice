#shader vertex
#version 330 core  
   
 layout(location = 0) in vec3 position;  // the position variable has attribute position 0

out vec4 vertexColor; // specify a color output to the fragment shader

 void main()  
 {  
     gl_Position = vec4(position, 1.0); // directly give vec3 to vec4's constructor
     vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
 }  ;

 #shader fragment
     #version 330 core

     out vec4 FragColor;

    in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)
   
 void main()  
 {  
     FragColor = vertexColor;
 }  ;