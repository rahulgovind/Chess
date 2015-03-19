const char* vertex_shader_source =  "#version 150 core\n"

                                    "in vec3 color; \n"
                                    "in vec2 position;\n"
                                    "in vec2 texcoord;\n"
                                    "out vec2 Texcoord;\n"
                                    "out vec3 Color;\n"
                                    "void main()\n"
                                    "{\n"
                                    "Color = color;\n"
                                    "Texcoord = texcoord;\n"
                                    "gl_Position = vec4(position, 0.0, 1.0);\n"
                                    "}";

const char* fragment_shader_source =    "#version 150 core\n"
                                        "in vec3 Color;\n"
                                        "in vec2 Texcoord;\n"
                                        "uniform sampler2D tex;\n"
                                        "out vec4 outColor;\n"
                                        "void main()\n"
                                        "{\n"
                                        "vec4 temp = texture(tex, Texcoord);\n"
                                        "if(temp[3]==0)\n"
                                        "outColor = vec4(Color, 1.0);\n"
                                        "else\n"
                                        "outColor = temp;\n"
                                        "}";



