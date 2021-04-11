#include "graph.h"
#include <fstream>
#include <iomanip>
#include <strstream>
#include <cstdlib>
#include <cstring>
#include <iterator>

#include <iostream>

#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844

texture::texture( const char* path )
{
    log_file.open("log.txt", std::ios_base::app);
    log_file << "Creating a texture: " << path << std::endl;
    std::ifstream tex_file( path );
    if( !tex_file.is_open() )
    {
        log_file << "!!! ERROR! Can't open texture file: " << path << std::endl;
    }
    char* fileCd = new char[4];
    tex_file.read( fileCd, 4 );
    if( strncmp(fileCd, "DDS", 3) )
    {
        tex_file.close();
        log_file << "!!! EROOR! Not DDS texture format: " << path << std::endl;
        delete [] fileCd;
        return;
    }
    delete [] fileCd;

    char* header = new char [124];
    tex_file.read( header, 124);
    unsigned int height = *(unsigned int*)&(header[8]);
    unsigned int width = *(unsigned int*)&(header[12]);
    unsigned int lineSize = *(unsigned int*)&(header[16]);
    unsigned int mipmapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC = *(unsigned int*)&(header[80]);
    delete [] header;

    unsigned int dataSize = mipmapCount > 1 ? lineSize * 2 : lineSize;
    char* buffer = new char [dataSize];
    tex_file.read( buffer, dataSize );
    tex_file.close();


    unsigned short components = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch (fourCC)
    {
    case FOURCC_DXT1:
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
    case FOURCC_DXT3:
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;
    case FOURCC_DXT5:
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;
    default:
        log_file << "!!! ERROR! Unknown texture format: " << path << std::endl;
        delete [] buffer;
        return;
    }
    glGenTextures( 1, &textureID );
    glBindTexture( GL_TEXTURE_2D, textureID );

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;
    for(unsigned int level = 0; level < mipmapCount && (width || height); ++level)
    {
        unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
        glCompressedTexImage2D( GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset );
        offset += size;
        width /= 2;
        height /= 2;
    }

    delete [] buffer;
}


texture::texture( const texture& texCopy )
{
    textureID = texCopy.textureID;
}

objModel::objModel(const char* path, bool dump_obj_data)
{
    log_file.open("log.txt", std::ios_base::app);
    log_file << "Creating an object: " << path << std::endl;
    std::vector<float> vertices;
    std::vector<float> uvCords;
    std::vector<float> normals;
    std::vector<int> indeces;
    std::ifstream objFile( path );
    if(!objFile.is_open())
    {
        log_file << "!!! ERROR! Unable to open file: " << path << std::endl;
    }
    float val[3];
    while(!objFile.eof())
    {
        char line[128];
        objFile.getline(line, 128);
        std::strstream lnStr;
        lnStr << line;
        std::string sIgnore;
        std::string dev = "/";
        if(line[0] == 'v' && line[1] == 't')
        {
            lnStr >> sIgnore >> val[0] >> val[1];
            uvCords.push_back(val[0]);
            uvCords.push_back(val[1]);
        }
        else if(line[0] == 'v' && line[1] == 'n')
        {
            lnStr >> sIgnore >> val[0] >> val[1] >> val[2];
            normals.push_back(val[0]);
            normals.push_back(val[1]);
            normals.push_back(val[2]);
        }
        else if(line[0] == 'v')
        {
            lnStr >> sIgnore >> val[0] >> val[1] >> val[2];
            vertices.push_back(val[0]);
            vertices.push_back(val[1]);
            vertices.push_back(val[2]);
        }
        else if(line[0] == 'f')
        {
            lnStr >> sIgnore;
            for(int i = 0; i < 3; ++ i)
            {
                lnStr >> sIgnore;
                size_t prev = 0, next = sIgnore.find("/", prev);
                while(next != std::string::npos )
                {
                    //std::cout << sIgnore.substr(prev, next-prev) << " " << prev << " " << next << std::endl;
                    indeces.push_back( std::stoi( sIgnore.substr(prev, next - prev) ));
                    prev = next+1;
                    next = sIgnore.find("/", prev);
                }
                indeces.push_back( std::stoi( sIgnore.substr(prev, next - prev) ));
            }
        }
    }

    vaSize = indeces.size();
    naSize = indeces.size();
    uvcaSize = indeces.size()/3 *2;
    verticies_array = new float [vaSize];
    normals_array = new float [naSize];
    UV_cords_array = new float [uvcaSize];
    float *vaIt = verticies_array, *naIt = normals_array, *uvcIt = UV_cords_array;

    //log_file << std::endl << "Indx: " << std::endl;
    size_t i = 0;
    while(i < indeces.size())
    {
        int n = ( indeces[i]-1 )*3;
        *vaIt = vertices[n];
        if(dump_obj_data)
            log_file << *vaIt << " ";
        ++vaIt;
        *vaIt = vertices[n + 1];
        if(dump_obj_data)
            log_file << *vaIt << " ";
        ++vaIt;
        *vaIt = vertices[n + 2];
        if(dump_obj_data)
            log_file << *vaIt << " | ";
        ++vaIt;

        ++i;
        n = ( indeces[i]-1 )*2;
        *uvcIt = uvCords[n];
        if(dump_obj_data)
            log_file << *uvcIt << " ";
        ++uvcIt;
        *uvcIt = uvCords[n+1];
        if(dump_obj_data)
            log_file << *uvcIt << " || ";
        ++uvcIt;

        ++i;
        //if(int((i+1)%9) == 0)
        //{
            if(dump_obj_data)
                log_file << "n: ";
            n = ( indeces[i]-1 )*3;
            *naIt = normals[n];
            if(dump_obj_data)
                log_file << *naIt << " ";
            ++naIt;
            *naIt = normals[n+1];
            if(dump_obj_data)
                log_file << *naIt << " ";
            ++naIt;
            *naIt = normals[n+2];
            if(dump_obj_data)
                log_file << *naIt << " ";
            ++naIt;
            if(dump_obj_data)
                log_file << std::endl;
        //}
        ++i;
        //log_file << it << std::endl;
    }

}

text::text(shader_prog *font_shader, texture* font_tex, std::string text, int max_str_size, float offset_x, float offset_y, float text_size)
{
    shader = font_shader;
    shader->use_program();
    uv_offset = glGetUniformLocation( font_shader->shader_program, "tex_cord_offset" );
    pos_offset = glGetUniformLocation( font_shader->shader_program, "position_offset" );
    scale = glGetUniformLocation( font_shader->shader_program, "scale" );
    textColor = glGetUniformLocation( font_shader->shader_program, "textColor" );
    tex = font_tex;
    x = offset_x;
    y = offset_y;
    fSize = text_size;
    str = text;
    strSize = max_str_size;
    letter = new myGeometry(cr, sizeof(cr), uv, sizeof(uv));
    myGeometry tmp(cr, sizeof(cr), uv, sizeof(uv));
    *letter = tmp;
    //glUniform2f(uv_offset, 0.0, 0.0);
    glUniform3f(pos_offset, x, y, 0.0);
    glUniform4f(textColor, rc, bc, gc, ac);
    glUniform1f(scale, fSize);
}

void text::draw()
{
    tex->bind_texture();
    shader->use_program();
    float tempX = x, tempY = y;
    int lCordX, lCordY;
    glUniform3f(pos_offset, tempX, tempY, 0.0);

    glBindVertexArray( letter->vaoHandle );
    int line_break_counter = 0;
    for(size_t i = 0; i < str.size(); ++i)
    {
        if(str[i] == '\n')
        {
            tempX = x;
            tempY -= fSize*2;
            line_break_counter = i;
            glUniform3f(pos_offset, tempX, tempY, 0.0);
            continue;
        }
        int c = str[i] - 32;
        lCordX = c % 10;
        lCordY = c / 10;
        glUniform2f(uv_offset, lCordX/10.0, lCordY/10.0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        tempX += fSize;
        if( i - line_break_counter >= strSize )
        {
            tempX = x;
            tempY -= fSize*2;
            line_break_counter = i;
        }
        glUniform3f(pos_offset, tempX, tempY, 0.0);
    }
}

hitbox::hitbox( const char* path, bool dump_obj_data )
{
    log_file.open("log.txt", std::ios_base::app);
    log_file << "Creating a hitbox: " << path << std::endl;
    struct box_vals
    {
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<int> indeces;
    };
    std::vector<box_vals> boxes;
    std::vector<box_vals>::iterator boxIt;
    std::ifstream objFile( path );
    if(!objFile.is_open())
    {
        log_file << "!!! ERROR! Unable to open file: " << path << std::endl;
    }
    float val[3];
    while(!objFile.eof())
    {
        char line[128];
        objFile.getline(line, 128);
        std::strstream lnStr;
        lnStr << line;
        std::string sIgnore;
        std::string dev = "/";
        if(line[0] == 'o')
        {
            box_vals tempt;
            boxes.push_back(tempt);
            boxIt = boxes.end()-1;
        }
        else if(line[0] == 'v' && line[1] == 'n')
        {
            lnStr >> sIgnore >> val[0] >> val[1] >> val[2];
            boxIt->normals.push_back(val[0]);
            boxIt->normals.push_back(val[1]);
            boxIt->normals.push_back(val[2]);
        }
        else if(line[0] == 'v')
        {
            lnStr >> sIgnore >> val[0] >> val[1] >> val[2];
            boxIt->vertices.push_back(val[0]);
            boxIt->vertices.push_back(val[1]);
            boxIt->vertices.push_back(val[2]);
        }
        else if(line[0] == 'f')
        {
            lnStr >> sIgnore;
            for(int i = 0; i < 3; ++ i)
            {
                lnStr >> sIgnore;
                size_t prev = 0, next = sIgnore.find("//", prev);
                while(next != std::string::npos )
                {
                    boxIt->indeces.push_back( std::stoi( sIgnore.substr(prev, next - prev) ));
                    prev = next+2;
                    next = sIgnore.find("//", prev);
                }
                boxIt->indeces.push_back( std::stoi( sIgnore.substr(prev, next - prev) ));
            }
        }
    }

    for(auto& nbox : boxes)
    {
        vertRV_N tempt;
        std::vector<int>::iterator index_it;
        index_it = nbox.indeces.begin();
        while( index_it < nbox.indeces.end() )
        {
            tempt.vCords.push_back(nbox.vertices[*index_it-1]);
            index_it++;
            tempt.nCords.push_back(nbox.normals[*index_it-1]);
            index_it += 4;
        }
        hbs.push_back( tempt );
    }

    if( dump_obj_data == true )
    {
        log_file << boxes.size() << std::endl;
        for(auto& nbox : boxes)
        {
            for(auto& aa : nbox.indeces)
                log_file << aa << " ";
            log_file << std::endl;
            for(auto& aa : nbox.normals)
                log_file << aa << " ";
            log_file << std::endl;
            for(auto& aa : nbox.vertices)
                log_file << aa << " ";
            log_file << std::endl << std::endl;
        }
        for(auto& nbox : hbs)
        {
            for(auto i : nbox.vCords)
                log_file << i << " ";
            log_file << std::endl;
            for(auto i : nbox.nCords)
                log_file << i << " ";
            log_file << std::endl;log_file << std::endl;
        }
    }
}



/*
objModel::objModel( const char* path, bool t, bool dump_obj_data )
{
    log_file.open("log.txt", std::ios_base::app);
    std::vector<float> vertices;
    std::vector<float> uvCords;
    std::vector<float> normals;
    std::vector<int> indeces;
    std::ifstream objFile( path );
    if(!objFile.is_open())
    {
        log_file << "!!! ERROR! Unable to open file: " << path << std::endl;
    }
    float val[3];
    int count = 0;
    while(!objFile.eof())
    {
        char line[128];
        objFile.getline(line, 128);
        std::strstream lnStr;
        lnStr << line;
        std::string sIgnore;
        if(line[0] == '#')
        {
            lnStr >> sIgnore >> sIgnore >> sIgnore;
            log_file << sIgnore << std::endl;
            if(sIgnore == "Count:")
            {
                objFile.getline(line, 128);
                count = atoi(line);
                continue;
            }
            if(count != 0 && sIgnore == "Buffer:")
            {
                for(int i = 0; i < count; ++i)
                {
                    lnStr.clear();
                    char ign;
                    objFile.getline(line, 128);
                    lnStr << line;
                    lnStr >> val[0] >> ign >> val[1] >> ign >> val[2];
                    vertices.push_back(val[0]); vertices.push_back(val[1]); vertices.push_back(val[2]);

                    lnStr.clear();
                    objFile.getline(line, 128);
                    lnStr << line;
                    lnStr >> val[0] >> ign >> val[1] >> ign >> val[2];
                    normals.push_back(val[0]); normals.push_back(val[1]); normals.push_back(val[2]);

                    lnStr.clear();
                    objFile.getline(line, 128);
                    lnStr << line;
                    lnStr >> val[0] >> ign >> val[1];
                    uvCords.push_back(val[0]); uvCords.push_back(val[1]); uvCords.push_back(val[2]);
                }
                continue;
            }
            if(sIgnore == "of")
            {
                objFile.getline(line, 128);
                count = atoi(line);
                continue;
            }
            if(sIgnore == "Data:")
            {
                for(int i = 0; i < count; ++i)
                {
                    lnStr.clear();
                    char ign;
                    objFile.getline(line, 128);
                    lnStr << line;
                    lnStr >> val[0] >> ign >> val[1] >> ign >> val[2];
                    indeces.push_back(val[0]); indeces.push_back(val[1]); indeces.push_back(val[2]);
                }
            }
        }
    }
    vaSize = indeces.size();
    naSize = indeces.size();
    uvcaSize = indeces.size()/3 *2;
    verticies_array = new float [vaSize];
    normals_array = new float [naSize];
    UV_cords_array = new float [uvcaSize];
    float *vaIt = verticies_array, *naIt = normals_array, *uvcIt = UV_cords_array;

    //log_file << std::endl << "Indx: " << std::endl;
    size_t i = 0;
    while(i < indeces.size())
    {
        int n = ( indeces[i]-1 )*3;
        *vaIt = vertices[n];
        if(dump_obj_data)
            log_file << *vaIt << " ";
        ++vaIt;
        *vaIt = vertices[n + 1];
        if(dump_obj_data)
            log_file << *vaIt << " ";
        ++vaIt;
        *vaIt = vertices[n + 2];
        if(dump_obj_data)
            log_file << *vaIt << " | ";
        ++vaIt;

        ++i;
        n = ( indeces[i]-1 )*2;
        *uvcIt = uvCords[n];
        if(dump_obj_data)
            log_file << *uvcIt << " ";
        ++uvcIt;
        *uvcIt = uvCords[n+1];
        if(dump_obj_data)
            log_file << *uvcIt << " || ";
        ++uvcIt;

        ++i;
        //if(int((i+1)%9) == 0)
        //{
            if(dump_obj_data)
                log_file << "n: ";
            n = ( indeces[i]-1 )*3;
            *naIt = normals[n];
            if(dump_obj_data)
                log_file << *naIt << " ";
            ++naIt;
            *naIt = normals[n+1];
            if(dump_obj_data)
                log_file << *naIt << " ";
            ++naIt;
            *naIt = normals[n+2];
            if(dump_obj_data)
                log_file << *naIt << " ";
            ++naIt;
            if(dump_obj_data)
                log_file << std::endl;
        //}
        ++i;
        //log_file << it << std::endl;
    }
}*/



