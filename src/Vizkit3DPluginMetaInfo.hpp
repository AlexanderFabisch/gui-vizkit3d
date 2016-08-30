#define __VIZKIT3D_PLUGIN_METAINFO_HPP__

#include <string>
#include <vector>

namespace vizkit3d
{

/**
 * Stores meta information for a vizkit plugin
 */
struct Vizkit3DPluginMetaInfo
{
    struct VisualizedType
    {
        std::string type_name;
        std::string method_name;
    };

    std::vector<VisualizedType> visualized_types;
};

}