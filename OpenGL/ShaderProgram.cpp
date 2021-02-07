#include"ShaderProgram.h"


bool ShaderProgram::LoadShaders(const char* vsFilename, const char* fsFilename) {

    using namespace std;

    // Load shaders as strings and as const char (for interoperability).
    string vsString = _fileToString(vsFilename);
    string fsString = _fileToString(fsFilename);
    const GLchar* vsPtr = vsString.c_str();
    const GLchar* fsPtr = fsString.c_str();



    // Shader creation for mesh...
    // Vertex shader...
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsPtr, NULL); // Assing source for shader.
    glCompileShader(vs); // Compile it...

    _compilingCheck(vs, ShaderType::SHADER_TYPE_VERTEXSHADER);




    // Fragment shader...
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsPtr, NULL);
    glCompileShader(fs);

    _compilingCheck(fs, ShaderType::SHADER_TYPE_FRAGMENTSHADER);




    // Make shader programm..
    m_ProgramHandle = glCreateProgram();
    glAttachShader(m_ProgramHandle, vs); // Attach shader to program...
    glAttachShader(m_ProgramHandle, fs); // Attach shader to program...


    // Link program...
    glLinkProgram(m_ProgramHandle);

    _compilingCheck(m_ProgramHandle, ShaderType::SHADER_TYPE_SHADERPROGRAM);


    // As shaders are now in program, we can delete them... to avoid memory leak etc.
    glDeleteShader(vs);
    glDeleteShader(fs);


	return true;
}




void ShaderProgram::Use() {

	if (m_ProgramHandle > 0) glUseProgram(m_ProgramHandle);
}



std::string ShaderProgram::_fileToString(const std::string& filename) {

	using namespace std;
	stringstream ss;
	ifstream file;

	try {

		file.open(filename, ios::in);

		if (!file.fail()) {


			ss << file.rdbuf(); // Load content to string stream.
		}

		file.close();
	}
	catch (exception e) {
		cout << "Error: " << e.what() << endl;
	}
	
	return ss.str();
}



void ShaderProgram::_compilingCheck(GLuint shaderObj, ShaderType sType) {

    using namespace std;

    int status = 0;

    if (sType == ShaderType::SHADER_TYPE_VERTEXSHADER || sType == ShaderType::SHADER_TYPE_FRAGMENTSHADER) {

        glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {

            GLint length = 0;
            glGetShaderiv(shaderObj, GL_INFO_LOG_LENGTH, &length);
            string errorLog(length, ' ');
            glGetShaderInfoLog(shaderObj, length, &length, &errorLog[0]);

            cout << color(colors::RED);
            cout << "Shader Compile Error! " << errorLog << white << endl;
        }
    }
    else {

        glGetProgramiv(m_ProgramHandle, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {

            GLint length = 0;
            glGetProgramiv(m_ProgramHandle, GL_INFO_LOG_LENGTH, &length);
            string errorLog(length, ' ');
            glGetProgramInfoLog(m_ProgramHandle, length, &length, &errorLog[0]);

            cout << color(colors::RED);
            cout << "Link Error! " << errorLog << white << endl;
        }
    }
}



ShaderProgram::~ShaderProgram() {

    glDeleteProgram(m_ProgramHandle);
    m_UniformLocationsMap.clear();
}



GLint ShaderProgram::_getUniformLocation(const GLchar* name){

    std::map<std::string, GLint>::iterator it = m_UniformLocationsMap.find(name);

    if (it == m_UniformLocationsMap.end()) { // Nothing found.

        m_UniformLocationsMap[name] = glGetUniformLocation(m_ProgramHandle, name); // Set a location index.

    }


    return m_UniformLocationsMap[name]; // Return the location index.
}


void ShaderProgram::SetUniform(const GLchar* name, const glm::vec2& v) {

    GLint location = _getUniformLocation(name); // Get location index

    glUniform2f(location, v.x, v.y); // Set values for the uniform.
}



void ShaderProgram::SetUniform(const GLchar* name, const glm::vec3& v) {

    GLint location = _getUniformLocation(name); // Get location index

    glUniform3f(location, v.x, v.y, v.z); // Set values for the uniform.
}


void ShaderProgram::SetUniform(const GLchar* name, const glm::vec4& v){


    GLint location = _getUniformLocation(name); // Get location index

    glUniform4f(location, v.x, v.y, v.z, v.w); // Set values for the uniform.
}


void ShaderProgram::SetUniform(const GLchar* name, const glm::mat4& m) {

    GLint location = _getUniformLocation(name); // Get location index

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m));
}


void ShaderProgram::SetUniform(const GLchar* name, const GLfloat& f) {

    GLint location = _getUniformLocation(name); // Get location index

    glUniform1f(location, f); // Set values for the uniform.
}

void ShaderProgram::SetUniformSampler(const GLchar* name, const GLint& textureSlot){

    glActiveTexture(GL_TEXTURE + textureSlot); // Activate texture.

    GLint location = _getUniformLocation(name); // Get location index

    glUniform1i(location, textureSlot); // Set values for the uniform.
}



void ShaderProgram::SetUniform(const GLchar* name, const GLint& i) {

    GLint location = _getUniformLocation(name); // Get location index

    glUniform1i(location, i); // Set values for the uniform.
}
