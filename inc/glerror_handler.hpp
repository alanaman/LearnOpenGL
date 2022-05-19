#include <glad/glad.h>
#include <iostream>

#define ASSERT(x) if (!(x)) assertCall(__FILE__, __LINE__);


#define GLCall(x) GLClearError();\
    x;\
    GLLogCall(#x, __FILE__, __LINE__);

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
void assertCall(const char* file, int line);
