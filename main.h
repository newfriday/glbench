//    This file is part of glBench.

//    glBench is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    glBench is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with glBench.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <sys/time.h>

#include <deque>
#include <bitset>

////////////////////////////////////////////////////////////////////////
// Vector structure
////////////////////////////////////////////////////////////////////////
struct Vector3d
{
    // methods
    Vector3d() {}
    Vector3d(double in_x, double in_y, double in_z) : x(in_x), y(in_y), z(in_z) {}
    inline Vector3d operator+(const Vector3d& in_v) const { return Vector3d(x + in_v.x, y + in_v.y, z + in_v.z); }
    inline Vector3d operator-(const Vector3d& in_v) const { return Vector3d(x - in_v.x, y - in_v.y, z - in_v.z); }
    inline Vector3d operator*(double in_a) const { return Vector3d(x * in_a, y * in_a, z * in_a); }
    inline Vector3d operator/(double in_a) const { return Vector3d(x / in_a, y / in_a, z / in_a); }

    // members
    double x, y, z;
};

////////////////////////////////////////////////////////////////////////
// Model structure
////////////////////////////////////////////////////////////////////////
struct Vertex
{
    Vector3d coord;
    Vector3d normal;
    Vector3d color;
    Vector3d texture_coordinate;
};

struct TriangleStrip
{
    std::vector<unsigned int> vertex_ids;
};

struct Geometry
{
    std::vector<Vertex> vertices;
    std::list<TriangleStrip> triangles_strip;
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
enum EventType
{
    NO_EVENT,
    QUIT_REQUESTED,
    BENCH_REQUESTED,
    RENDERING_CONFIG_CHANGED
};


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
enum RenderingMethod
{
    INVALID = 0,
    IMMEDIATE,
    CALL_LIST,
    STATIC_VBO,
    DYNAMIC_VBO,

    NB_RENDERING_METHOD
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
enum RenderingOption
{
    TRIANGLE_STRIP = 0,
    COLOR,
    TEXTURE,
    SMOOTH_SHADING,
    BACK_FACE_PAINTING,

    NB_BENCH_RENDERING_OPTION,

    WIREFRAME,

    NB_RENDERING_OPTION
};

////////////////////////////////////////////////////////////////////////
// Config and Data structure
////////////////////////////////////////////////////////////////////////
struct DisplayConfig
{
    unsigned int windows_width;
    unsigned int windows_height;
    double rotation_angle_y;
    double rotation_angle_x;
    double move_forward;
    bool rotation;
};

struct RenderingConfig
{
    RenderingMethod rendering_method;
    unsigned int nb_triangles;

    std::bitset<NB_RENDERING_OPTION> rendering_options;
};

struct RenderingData
{
    Geometry geometry;
    GLuint texture_id;
    GLuint call_list_id;
    GLuint index_buffer_id;
    GLuint vertex_buffer_id;
};

////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////
int main(int, char**);

long elapsed_time(const struct timeval& in_start, const struct timeval& in_end);

void init_sdl(const DisplayConfig& in_display_config);
void init_gl_extensions();
void init_gl(RenderingData& in_rendering_data, const DisplayConfig& in_display_config, const RenderingConfig& in_rendering_config);

EventType event_sdl(DisplayConfig& io_display_config, RenderingConfig& io_rendering_config);

void print_config(const RenderingConfig& in_rendering_config, std::ostream& out_stream);
void print_rendering_time (std::ostream& out_stream, unsigned int in_nb_triangles, const std::deque<long>& in_rendering_times);

Vector3d compute_normal(const Vector3d& in_v1, const Vector3d& in_v2, const Vector3d& in_v3);
void fill_normal(std::vector<Vertex>& out_vertices, unsigned int in_id1, unsigned int in_id2, unsigned int in_id3);
void generate_model(const RenderingConfig& in_rendering_config, RenderingData& out_rendering_data);

void process_texturing(RenderingData& io_rendering_data, const RenderingConfig& in_rendering_config);
void delete_texturing(RenderingData& io_rendering_data);

void process_call_list(RenderingData& io_rendering_data, const RenderingConfig& in_rendering_config);
void delete_call_list(RenderingData& io_rendering_data);

void process_vbo(RenderingData& io_rendering_data, const RenderingConfig& in_rendering_config);
void delete_vbo(RenderingData& io_rendering_data);

void paint_gl(const Geometry& in_geometry, const RenderingConfig& in_rendering_config);
void paint_gl(const RenderingConfig& in_rendering_config, const Vertex& in_vertex);

void render(const RenderingData& in_rendering_data, const RenderingConfig& in_rendering_config, const DisplayConfig& in_display_config);

void generate_bench_rendering_config_list(std::deque<RenderingConfig>& in_rendering_config_list, unsigned int in_nb_triangles);
