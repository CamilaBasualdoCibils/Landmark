#include "MeshParser.hpp"
static constexpr uint64_t CharMask(std::vector<char> chars)
{
    uint64_t mask = 0;
    if (chars.size() > 8)
        throw "Invalid size";
    for (int i = 0; i < chars.size(); i++)
    {
        uint64_t b = chars[i];
        mask |= (b << i * 8);
    }
    return mask;
}
bool MeshParser::ParseOBJ(std::string filename, MeshParser::OBJ_File_content &output)
{
    output = MeshParser::OBJ_File_content{};
    // const std::string file_string = file.ReadText();
    // std::optional<size_t> pos_start,pos_end,element_start,element_end,normal_start,normal_end,texcoord_start,texcoord_end;
    std::vector<std::string> v_lines, f_lines, vn_lines, vt_lines;

    // std::vector<std::string> file_lines;
    {
        std::ifstream input_file(filename);
        if (!input_file.is_open())
        {
            LASSERT(false, "Invalid File");
            return false;
        }
        int line_num = 0;
        for (std::string line; std::getline(input_file, line);)
        {

            const uint64_t bitmask = CharMask({(char)std::tolower(line[0]), (char)std::tolower(line[1])});

            line = line.substr(1);
            switch (bitmask)
            {
            case CharMask({'v', ' '}):
                /* code */
                {
                    v_lines.push_back(line);
                }

                break;
            case CharMask({'f', ' '}):
            {
                f_lines.push_back(line);
            }
            break;
            default:
                break;
            }
            line_num++;
        }
    }

    // parse positions

    std::vector<vec3> positions;
    {
        positions.resize(v_lines.size());
        std::cerr << "Parsing positions\n";
        const auto indicies = std::views::iota((size_t)0,v_lines.size());
        std::for_each(std::execution::par_unseq, indicies.begin(), indicies.end(), [&](const size_t index)
                      {
                
                    const std::string& line = v_lines[index];
                std::stringstream ss(line);
        
                vec3& pos = positions[index];
                ss >> pos.x >> pos.y >> pos.z;
                pos;
            
            });
    }

    // check if normals and texcoords exist
    bool skip_steps[3] = {false, vn_lines.empty(), vt_lines.empty()};

    using VertexBlueprint = std::array<uint64_t, 3>; // pos_normal_texcoord

    // a face of 3 verticies. each vertex with a Vertex Blueprint
    std::vector<std::array<VertexBlueprint, 3>> vertex_blueprints;
    {
        vertex_blueprints.resize(f_lines.size());
        const auto indicies = std::views::iota((size_t)0,f_lines.size());
    
        std::for_each(std::execution::par_unseq, indicies.begin(), indicies.end(), [&](const size_t index)
                      {
     
            std::stringstream ss(f_lines[index].substr(1));
            std::string vertex_blueprint;
            
            int f = 0;
            while (ss >> vertex_blueprint) {
                LASSERT(f <3, "FUCK");
                std::stringstream ss(vertex_blueprint);
                std::string vert_blue_str;
                for (int i = 0; i < 3;i++) {
                    if (skip_steps[i]) continue;
                    std::getline(ss, vert_blue_str, '/');
                    vertex_blueprints[index][f][i] = std::stoi(vert_blue_str);
                }
                f++;
            } });
    }
   

    struct ArrayHash
    {
        std::size_t operator()(const std::array<uint64_t, 3> &arr) const
        {
            std::size_t h1 = std::hash<uint64_t>{}(arr[0]);
            std::size_t h2 = std::hash<uint64_t>{}(arr[1]);
            std::size_t h3 = std::hash<uint64_t>{}(arr[2]);
            return h1 ^ (h2 << 1) ^ (h3 << 2); // Combine hashes
        }
    };

    std::unordered_map<std::array<uint64_t, 3>, uint64_t, ArrayHash> assembled_verticies_index;
    auto &assembled_verticies = output.verticies;
    auto &parsed_indicies = output.indicies;
    assembled_verticies.clear();
    parsed_indicies.clear();
    // for every face
    for (uint64_t f = 0; f < vertex_blueprints.size(); f++)
    { // each triangle
        auto &face_elements = vertex_blueprints[f];
        for (int v = 0; v < 3; v++)
        { // each vertex in triangle
            auto &vertex_elements = face_elements[v];

            uint64_t vert_index;
            if (!assembled_verticies_index.contains(vertex_elements))
            { // not registered
                vert_index = assembled_verticies.size();
                assembled_verticies_index.insert(std::make_pair(vertex_elements, vert_index));
                OBJ_File_content::Vertex vertex;
                vertex.position = positions[vertex_elements[0] - 1]; // objs are 1 indexed
                assembled_verticies.push_back(vertex);
            }
            else
            {
                vert_index = assembled_verticies_index.at(vertex_elements);
            }
            parsed_indicies.push_back(vert_index);
        }
    }

    return true;
}