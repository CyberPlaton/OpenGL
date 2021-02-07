#include "Mesh.h"

#include<fstream>
#include<sstream>

std::vector<std::string> split(std::string str, std::string delimiter) {

    using namespace std;

    vector<string> returnVec;

    while (true) {

        int pos = str.find(delimiter);
        if (pos == -1) { // Cant find position of delimiter.

            returnVec.push_back(str);
            break;
        }


        returnVec.push_back(str.substr(0, pos)); // Else save a string from 0 to the found position.


        str = str.substr(pos + 1, str.size() - pos - 1);
    }

    return returnVec;
}


Mesh::Mesh() : m_IsLoaded(false), m_VAO(0), m_VBO(0) {
}


Mesh::~Mesh() {

    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

bool Mesh::LoadOBJ(const std::string& fileName) {

    using namespace std;

    vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    vector<glm::vec3> tempVertices;
    vector<glm::vec2> tempUVs;
    vector<glm::vec3> tempNormals;



    // Open file.
    if (fileName.find(".obj") != string::npos) { // It has an obj in it...

        ifstream fin(fileName, ios::in); // Try open file..
        if (!fin) {
            cout << color(colors::RED);
            cout << "Error Opening " << fileName << white << endl;
            return false;
        }


        cout << color(colors::GREEN);
        cout << "Loading \"" << fileName << "\" ..." << white << endl;


        string lineBuffer;
        while (getline(fin, lineBuffer)) { // While file has a next line..


            stringstream ss(lineBuffer); // SS represents first token of line...
            string cmd;
            ss >> cmd;


            // Switch based on specified type of vertex...
            if (cmd == "v") { // vertex

                cout << color(colors::YELLOW);
                cout << "Reading vertex..." << white << endl;

                /*
                E.g.
                v 1.000000 -1.000000 -1.000000
                v 1.000000 -1.000000 1.000000
                ...
                */

                int dim = 0;
                glm::vec3 vertex;

                while (dim < 3 && ss >> vertex[dim]) { // Read all vertices in vertex[dim].
                    dim++;
                }

                tempVertices.push_back(vertex);

            }
            else if (cmd == "vt") { // vertextexture

                cout << color(colors::YELLOW);
                cout << "Reading vertex texture..." << white << endl;

                /*
                E.g.
                vt 0.500 1
                vt 1 1
                ...
                */

                int dim = 0;
                glm::vec2 uv;
               
                while (dim < 2 && ss >> uv[dim]) { // Read all uv coords. in uv[dim].
                    dim++;
                }

                tempUVs.push_back(uv);

            }
            else if (cmd == "vn") { // vertexnormal

                cout << color(colors::YELLOW);
                cout << "Reading texture normals..." << white << endl;

                /*
                E.g.
                vn 0.000000 1.000000 0.000000
                vn 1.000000 -0.000000 0.000000
                ...
                */

                int dim = 0;
                glm::vec3 normal;

                while (dim < 3 && ss >> normal[dim]) { // Read all normals in normal[dim].
                    dim++;
                }

                      
                tempNormals.push_back(glm::normalize(normal)); // Normalize the normals if they arent already
            }
            else if (cmd == "f") { // face

                cout << color(colors::YELLOW);
                cout << "Reading face..." << white << endl;

                string faceData;
                int vertexIndex, uvIndex, normalIndex; // v/vt/vn

                while (ss >> faceData) { // Rest of line, so numbers..

                    
                    vector<string> data = split(faceData, "/");

                    // Base vertex...
                    if (data[0].size() > 0) {
                        
                        sscanf_s(data[0].c_str(), "%d", &vertexIndex); // Read the vertex from face.
                        vertexIndices.push_back(vertexIndex);
                    }

                    // Texture uvs...
                    if (data.size() >= 1) { // If face has at least a texture coord.


                        if (data[1].size() > 0) {

                            sscanf_s(data[1].c_str(), "%d", &uvIndex); // Read the uv coord. from face.
                            uvIndices.push_back(uvIndex);
                        }
                    }

                    // Normal...
                    if (data.size() >= 2) { // If face has a normal def. too...

                        if (data[2].size() > 0) {

                            sscanf_s(data[2].c_str(), "%d", &normalIndex); // Read the normal from face.
                            normalIndices.push_back(normalIndex);
                        }
                    }
                }

                /*
                int p1, p2, p3; //Store mesh index
                int t1, t2, t3; //Store tex index
                int n1, n2, n3;

                const char* face = lineBuffer.c_str();

                int match = sscanf_s(face, "f %i/%i/%i %i/%i/%i %i/%i/%i", // Old c-way. Read string and place data into given vars based on format.
                                            &p1, &t1, &n1,
                                            &p2, &t2, &n2,
                                            &p3, &t3, &n3);

                if (match != 9) {
                    cout << color(colors::RED);
                    cout << "Failed to match OBJ format at scanning face \"" << face << "\"." << white << endl;
                
                    return false;
                }

                // Else get the indices for vertices and textures and save them.
                // Normals are ignored for now.
                vertexIndices.push_back(p1);
                vertexIndices.push_back(p2);
                vertexIndices.push_back(p3);

                uvIndices.push_back(t1);
                uvIndices.push_back(t2);
                uvIndices.push_back(t3);
                */
            }
            else {

                cout << color(colors::YELLOW);
                cout << "Reading unknown..." << white << endl;
                cout << color(colors::BLUE);
                cout << "\"" << lineBuffer << "\"" << white << endl;
            }
        }



        // After reading is done, we close file.
        fin.close();

        for (unsigned int i = 0; i < vertexIndices.size(); i++) {

            Vertex meshVertex;

            // Vertices....
            if (tempVertices.size() > 0) {

                glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];

                meshVertex.position = vertex; // Assign poition in OBJ to our poition for Vertex.
            }


            // Normals....
            if (tempNormals.size() > 0) {

                glm::vec3 normal = tempNormals[normalIndices[i] - 1];

                meshVertex.normal = normal; // Assign normal in OBJ to our normal for Vertex.
            }


            // Texture Coordinates...
            if (tempUVs.size() > 0) {

                glm::vec2 uv = tempUVs[uvIndices[i] - 1];

                meshVertex.texCoords = uv; // Assign texture coords. in OBJ to our texture coords. for Vertex.
            }




            m_Vertices.push_back(meshVertex);
        }



        // Critically to initialize and create buffers from data we loaded.
        _initBuffers();

        return(m_IsLoaded = true);
    }
    else {


    return false;

    }
}


void Mesh::DrawMesh() {

    if (!m_IsLoaded) return;


    glBindVertexArray(m_VAO); // Bind what to be drawn...

    // New:
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // Worked without too...
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer); m_IndexBuffer = Gluint.

    // Bind texture.
    //if(m_MeshTexture) m_MeshTexture->Bind();

    // New. Allow some options.
    glEnable(GL_CULL_FACE);
    glEnable(GL_CCW);

    // glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_BIT, 0);

    glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size()); // Draw triangles of mesh...

    glBindVertexArray(0); // Unbind after drawing...
    //if (m_MeshTexture) m_MeshTexture->Unbind();
    glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size()); // Draw triangles of mesh...

    glBindVertexArray(0); // Unbind after drawing...
}



void Mesh::_initBuffers() {


    // Send vetrices to GPU for storing.
    glGenBuffers(1, &m_VBO); // Create chunk of memory in graphics card.
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // Make this buffer the active one.
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size()*sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW); // Pass mesh data to active buffer.



    // Create vertex array object, which holds information of vertex buffer object.
    glGenVertexArrays(1, &m_VAO); // Create..
    glBindVertexArray(m_VAO);     // Make it the active...


    // Describe usage of above vertex object for GPU.
    // Position attributes.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL); // For texture
    glEnableVertexAttribArray(0); // Enable vertex array object.


    // Normal attributes.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(GL_FLOAT) * 3)); // Offset is 12 Bytes = 3 Float numbers.
    glEnableVertexAttribArray(1); // Enable the set attribute.



    // Texture Coords.
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(GL_FLOAT) * 6)); // Offset is 24 Bytes = 6 Float numbers.
    glEnableVertexAttribArray(2); // Enable the set attribute.


    /*
    TODO: create index buffer.

    glGenBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_IndicesVector.size(), &m_IndicesVector[0], GL_STATIC_DRAW);
    */
   

    glBindVertexArray(0); // Unbind vertex array after done work.
}

/*
void Mesh::SetTexture(const std::string& fileName) {

    using namespace std;

    m_MeshTexture = new Texture2D();
    bool r = m_MeshTexture->LoadTexture(fileName);

    if (!r) {
        cout << color(colors::RED);
        cout << "Could not load \""<< fileName <<"\"." << white << endl;
    }
}


Texture2D* Mesh::GetTexture() {
    return m_MeshTexture;
}
*/