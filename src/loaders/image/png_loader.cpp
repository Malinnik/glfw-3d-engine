#include <glad/glad.h>
#include <loguru.hpp>
#include <fmt/format.h>
#include "png.h"

#include "graphics/texture.h"
#include "png_loader.h"

int _png_load(const char* file, int* width, int* height)
{
    FILE *f;
    int is_png, bit_depth, color_type, row_bytes;
    png_infop info_ptr, end_info;
    png_uint_32 t_width, t_height;
    png_byte header[8], *image_data;
    png_bytepp row_pointers;
    png_structp png_ptr;
    GLuint texture;
    int alpha;
 
    if ( !( f = fopen(file, "rb" ) ) ) {
        return 0;
    }
    fread( header, 1, 8, f );
    is_png = !png_sig_cmp( header, 0, 8 );
    if ( !is_png ) {
        fclose( f );
        return 0;
    }
    png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL,
        NULL, NULL );
    if ( !png_ptr ) {
        fclose( f );
        return 0;
    }
    info_ptr = png_create_info_struct( png_ptr );
    if ( !info_ptr ) {
        png_destroy_read_struct( &png_ptr, (png_infopp) NULL,
            (png_infopp) NULL );
        fclose( f );
        return 0;
    }
    end_info = png_create_info_struct( png_ptr );
    if ( !end_info ) {
        png_destroy_read_struct( &png_ptr, (png_infopp) NULL,
            (png_infopp) NULL );
        fclose( f );
        return 0;
    }
    if ( setjmp( png_jmpbuf( png_ptr ) ) ) {
        png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );
        fclose( f );
        return 0;
    }
    png_init_io( png_ptr, f );
    png_set_sig_bytes( png_ptr, 8 );
    png_read_info( png_ptr, info_ptr );
    png_get_IHDR( png_ptr, info_ptr, &t_width, &t_height, &bit_depth,
        &color_type, NULL, NULL, NULL );
    *width = t_width;
    *height = t_height;
    png_read_update_info( png_ptr, info_ptr );
    row_bytes = png_get_rowbytes( png_ptr, info_ptr );
    image_data = (png_bytep) malloc( row_bytes * t_height * sizeof(png_byte) );
    if ( !image_data ) {
        png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );
        fclose( f );
        return 0;
    }
    row_pointers = (png_bytepp) malloc( t_height * sizeof(png_bytep) );
    if ( !row_pointers ) {
        png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );
        free( image_data );
        fclose( f );
        return 0;
    }
    for (unsigned int i = 0; i < t_height; ++i ) {
        row_pointers[t_height - 1 - i] = image_data + i * row_bytes;
    }
    png_read_image( png_ptr, row_pointers );
    switch ( png_get_color_type( png_ptr, info_ptr ) ) {
        case PNG_COLOR_TYPE_RGBA:
            alpha = GL_RGBA;
            break;
        case PNG_COLOR_TYPE_RGB:
            alpha = GL_RGB;
            break;
        default:
            printf( "Color type %d not supported!\n",
                png_get_color_type( png_ptr, info_ptr ) );
            png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );
            return 0;
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0,
        alpha, GL_UNSIGNED_BYTE, (GLvoid *) image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
 
    png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );
    free( image_data );
    free( row_pointers );
    fclose( f );
 
    return texture;
}


Texture* load_texture(std::string filepath){
    int width, height;
    GLuint texture = _png_load(filepath.c_str(), &width, &height);
    if (texture == 0) {
        LOG_F(ERROR, fmt::format("Could not load texture {}", filepath).c_str());
        return nullptr;
    }
    return new Texture(texture, width, height);
}



unsigned char* load_png_data(const char* file, int* width, int* height)
{
    FILE *f;
    int is_png, bit_depth, color_type, row_bytes;
    png_infop info_ptr, end_info;
    png_uint_32 t_width, t_height;
    png_byte header[8], *image_data;
    png_bytepp row_pointers;
    png_structp png_ptr;
    unsigned char* result = nullptr;
 
    if (!(f = fopen(file, "rb"))) {
        return nullptr;
    }
    
    fread(header, 1, 8, f);
    is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png) {
        fclose(f);
        return nullptr;
    }
    
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(f);
        return nullptr;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(f);
        return nullptr;
    }
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(f);
        return nullptr;
    }
    
    png_init_io(png_ptr, f);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    
    png_get_IHDR(png_ptr, info_ptr, &t_width, &t_height, &bit_depth,
                 &color_type, NULL, NULL, NULL);
    
    *width = t_width;
    *height = t_height;
    
    // Преобразуем в 8-битный RGBA формат
    if (bit_depth == 16) png_set_strip_16(png_ptr);
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) 
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) 
        png_set_tRNS_to_alpha(png_ptr);
    
    // Конвертируем в RGBA
    if (color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);
    }
    
    png_read_update_info(png_ptr, info_ptr);
    
    row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    image_data = (png_bytep)malloc(row_bytes * t_height);
    
    row_pointers = (png_bytepp)malloc(t_height * sizeof(png_bytep));
    for (unsigned int i = 0; i < t_height; ++i) {
        row_pointers[t_height - 1 - i] = image_data + i * row_bytes;
    }
    
    png_read_image(png_ptr, row_pointers);
    
    // Копируем данные в результат
    result = new unsigned char[t_width * t_height * 4];
    memcpy(result, image_data, t_width * t_height * 4);
    
    // Очистка
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    free(image_data);
    free(row_pointers);
    fclose(f);
    
    return result;
}