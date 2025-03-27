#include "AssetDescriptor.hpp"

Json AssetDescriptor::ToJson() const
{
    Json json= {
        {imported_path__identifier, imported_path},
        {type__identifier, AssetTypeString.at(type)},
        {last_write_time__identifier, formatTime(last_write_time)}
        
    };

    switch (type)
    {
    case AssetType::eTEXTURE:
         json[texture_data__identifier] = std::get<TextureAssetDescriptor>(descriptor_data).ToJson();
        break;
    
    default:
        break;
    }

    //std::cout << json.dump(4) << std::endl;
    return json;
}

void AssetDescriptor::FromJson(const Json &json)
{
   // std::cerr << json.dump(4) << std::endl;
   imported_path = json[imported_path__identifier].get<std::string>();
   last_write_time = unformatTime(json[last_write_time__identifier]);
   type = AssetTypeStringReverse.at( json[type__identifier]);
   switch (type)
   {
   case AssetType::eTEXTURE:
   descriptor_data = TextureAssetDescriptor::FromJson(json[texture_data__identifier]);
    /* code */
    break;
   
   default:
    break;
   }
}

std::filesystem::file_time_type AssetDescriptor::GetLastWriteTimeFromJson(const Json &json){
    return unformatTime( json[last_write_time__identifier]);

}

std::filesystem::file_time_type AssetDescriptor::unformatTime(const std::string & str)
{
    std::tm tm = {};
    std::istringstream ss(str);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S"); // Parse time string

    if (ss.fail()) {
        throw std::runtime_error("Failed to parse time string.");
    }

    std::time_t time_t_val = std::mktime(&tm); // Convert to time_t (system clock)
    if (time_t_val == -1) {
        throw std::runtime_error("Failed to convert to time_t.");
    }

    // Convert to file_time_type using system clock
    auto sys_time = std::chrono::system_clock::from_time_t(time_t_val);

    // Convert system_clock time to filesystem_clock time
    return std::chrono::time_point_cast<std::filesystem::file_time_type::clock::duration>(
        sys_time - std::chrono::system_clock::now() + std::filesystem::file_time_type::clock::now()
    );
}

std::string AssetDescriptor::formatTime(std::filesystem::file_time_type tm)
{
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        tm - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now()
    );
    std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
    std::ostringstream oss;
    oss << std::put_time(std::localtime( &cftime), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
