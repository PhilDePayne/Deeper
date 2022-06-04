//#include <ft2build.h>
//#include FT_FREETYPE_H
//
//#include <map>

#ifndef DEEPER_TEXT_H
#define DEEPER_TEXT_H

class TextRenderer {
public:
    TextRenderer(glm::mat4 proj) {
        points = 0;
        projection = proj;

//        if (FT_Init_FreeType(&ft))
//        {
//            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
//        }
//
//        if (FT_New_Face(ft, "./res/fonts/OCR-A.ttf", 0, &face))
//        {
//            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
//        }
//
//        FT_Set_Pixel_Sizes(face, 0, 48);
//
//        if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
//        {
//            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
//        }
//
//        FT_Set_Pixel_Sizes(face, 0, 48);
//
//        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
//
//        for (unsigned char c = 0; c < 128; c++)
//        {
//            // load character glyph
//            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
//            {
//                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
//                continue;
//            }
//            // generate texture
//            unsigned int texture;
//            glGenTextures(1, &texture);
//            glBindTexture(GL_TEXTURE_2D, texture);
//            glTexImage2D(
//                    GL_TEXTURE_2D,
//                    0,
//                    GL_RED,
//                    face->glyph->bitmap.width,
//                    face->glyph->bitmap.rows,
//                    0,
//                    GL_RED,
//                    GL_UNSIGNED_BYTE,
//                    face->glyph->bitmap.buffer
//            );
//            // set texture options
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//            // now store character for later use
//            Character character = {
//                    texture,
//                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//                    static_cast<unsigned int>(face->glyph->advance.x)
//            };
//            Characters.insert(std::pair<char, Character>(c, character));
//        }
//
//        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//        FT_Done_Face(face);
//        FT_Done_FreeType(ft);
//
//
//        glGenVertexArrays(1, &tVAO);
//        glGenBuffers(1, &tVBO);
//        glBindVertexArray(tVAO);
//        glBindBuffer(GL_ARRAY_BUFFER, tVBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindVertexArray(0);
//
//        glEnable(GL_DEPTH_TEST);
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

//    void renderPoints(glm::mat4 proj, Shader& shader, int display_w, int display_h) {
//        projection = proj;
//        int digits = NumDigits(points);
//        char array[20 + sizeof(char)];
//        sprintf(array, "%i", points);
//
//        float scale = display_w/1920.0f * 1.5f;
//
//        RenderText(shader, array, display_w/2.0f - 15.0f * digits * scale, display_h - 100.0f, scale, glm::vec3(0.831f, 0.796f, 0.768f));
//
//    }

    void addPoints(int p){
        points += p;
    }

    void removePoints(int p){
        points -= p;
        if(points < 0){
            points = 0;
        }
    }

    int getPoints() const {
        return points;
    }

    void setPoints(int points) {
        TextRenderer::points = points;
    }

private:
    int points;

//    unsigned int tVAO, tVBO;
//    FT_Library ft;
//    FT_Face face;
    glm::mat4 projection;

//    struct Character {
//        unsigned int TextureID;  // ID handle of the glyph texture
//        glm::ivec2   Size;       // Size of glyph
//        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
//        unsigned int Advance;    // Offset to advance to next glyph
//    };
//
//    std::map<char, Character> Characters;
//
//    void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color)
//    {
//        // activate corresponding render state
//        shader.use();
//        glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
//        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//        glActiveTexture(GL_TEXTURE0);
//        glBindVertexArray(tVAO);
//
//        // iterate through all characters
//        std::string::const_iterator c;
//        for (c = text.begin(); c != text.end(); c++)
//        {
//            Character ch = Characters[*c];
//
//            float xpos = x + ch.Bearing.x * scale;
//            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
//
//            float w = ch.Size.x * scale;
//            float h = ch.Size.y * scale;
//            // update VBO for each character
//            float vertices[6][4] = {
//                    { xpos,     ypos + h,   0.0f, 0.0f },
//                    { xpos,     ypos,       0.0f, 1.0f },
//                    { xpos + w, ypos,       1.0f, 1.0f },
//
//                    { xpos,     ypos + h,   0.0f, 0.0f },
//                    { xpos + w, ypos,       1.0f, 1.0f },
//                    { xpos + w, ypos + h,   1.0f, 0.0f }
//            };
//
//
//            // render glyph texture over quad
//            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
//            // update content of VBO memory
//            glBindBuffer(GL_ARRAY_BUFFER, tVBO);
//            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
//
//            glBindBuffer(GL_ARRAY_BUFFER, 0);
//            // render quad
//            glDrawArrays(GL_TRIANGLES, 0, 6);
//            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
//            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
//        }
//        glBindVertexArray(0);
//        glBindTexture(GL_TEXTURE_2D, 0);
//    }
//
//    int NumDigits(int x)
//    {
//        x = abs(x);
//        return (x < 10 ? 1 :
//                (x < 100 ? 2 :
//                 (x < 1000 ? 3 :
//                  (x < 10000 ? 4 :
//                   (x < 100000 ? 5 :
//                    (x < 1000000 ? 6 :
//                     (x < 10000000 ? 7 :
//                      (x < 100000000 ? 8 :
//                       (x < 1000000000 ? 9 :
//                        10)))))))));
//    }

};

#endif //DEEPER_TEXT_H
