#include "Vizkit3DPluginLoader.hpp"
#include <tinyxml.h>

namespace vizkit3d
{

const std::string Vizkit3DPluginLoader::viz_plugin_base_name = "vizkit3d::VizkitPluginBase";

Vizkit3DPluginLoader::Vizkit3DPluginLoader() : PluginLoader(false)
{
    // this is called here in order to trigger the correct parsePluginMetaInformation callback
    reloadXMLPluginFiles();
}

Vizkit3DPluginLoader::~Vizkit3DPluginLoader()
{

}

Vizkit3DPluginLoader* Vizkit3DPluginLoader::getInstance()
{
    return base::Singleton<Vizkit3DPluginLoader>::getInstance();
}

bool Vizkit3DPluginLoader::hasVizPlugin(const std::string& plugin_name) const
{
    return hasClassOfType(plugin_name, viz_plugin_base_name);
}

bool Vizkit3DPluginLoader::createVizPlugin(const std::string& plugin_name, boost::shared_ptr< VizPluginBase >& viz_plugin_base)
{
    return createInstance<VizPluginBase>(plugin_name, viz_plugin_base);
}

bool Vizkit3DPluginLoader::createVizPluginFor(const std::string& visualized_type, boost::shared_ptr< VizPluginBase >& viz_plugin_base)
{
    std::vector<std::string> available_plugins;
    if(getVizPluginsFor(visualized_type, available_plugins) && !available_plugins.empty())
    {
        return createVizPlugin(available_plugins.front(), viz_plugin_base);
    }
    return false;
}

bool Vizkit3DPluginLoader::getUpdateMethodName(const std::string& plugin_name, const std::string& visualized_type, std::string& method_name) const
{
    std::string full_class_name;
    if(!getFullClassName(plugin_name, full_class_name))
        return false;

    std::map<std::string, Vizkit3DPluginMetaInfo>::const_iterator it = vizkit_plugin_meta_info.find(full_class_name);
    if(it != vizkit_plugin_meta_info.end())
    {
        for(std::vector<Vizkit3DPluginMetaInfo::VisualizedType>::const_iterator viz_type_it = it->second.visualized_types.begin(); viz_type_it != it->second.visualized_types.end(); viz_type_it++)
        {
            if(viz_type_it->type_name == visualized_type)
            {
                method_name = viz_type_it->method_name;
                return true;
            }
        }
    }
    return false;
}

bool Vizkit3DPluginLoader::getVisualizedTypes(const std::string& plugin_name, std::vector< std::string >& visualized_types) const
{
    std::string full_class_name;
    if(!getFullClassName(plugin_name, full_class_name))
        return false;

    std::map<std::string, Vizkit3DPluginMetaInfo>::const_iterator it = vizkit_plugin_meta_info.find(full_class_name);
    if(it != vizkit_plugin_meta_info.end())
    {
        visualized_types.reserve(it->second.visualized_types.size());
        for(std::vector<Vizkit3DPluginMetaInfo::VisualizedType>::const_iterator viz_type_it = it->second.visualized_types.begin(); viz_type_it != it->second.visualized_types.end(); viz_type_it++)
            visualized_types.push_back(viz_type_it->type_name);
        return true;
    }
    return false;
}

bool Vizkit3DPluginLoader::getVizPluginsFor(const std::string& visualization_type, std::vector< std::string >& plugin_names)
{
    size_t elements = visualized_types_map.count(visualization_type);
    if(elements > 0)
    {
        plugin_names.reserve(elements);
        std::pair<std::multimap<std::string, PluginInfoPtr>::const_iterator, std::multimap<std::string, PluginInfoPtr>::const_iterator> range;
        range = visualized_types_map.equal_range(visualization_type);
        for(std::multimap<std::string, PluginInfoPtr>::const_iterator it = range.first; it != range.second; it++)
        {
            plugin_names.push_back(it->second->full_class_name);
        }
        return true;
    }
    return false;
}

void Vizkit3DPluginLoader::clear()
{
    vizkit_plugin_meta_info.clear();
    visualized_types_map.clear();
    PluginLoader::clear();
}

void Vizkit3DPluginLoader::parsePluginMetaInformation(const PluginInfoPtr& plugin_info, TiXmlElement* meta_element)
{
    if(plugin_info->base_class_name == viz_plugin_base_name)
    {
        // parse meta information
        Vizkit3DPluginMetaInfo& vizkit_plugin_info = vizkit_plugin_meta_info[plugin_info->class_name];
        TiXmlElement* argument_element = meta_element->FirstChildElement("viz_type");
        while (argument_element != NULL)
        {
            Vizkit3DPluginMetaInfo::VisualizedType viz_type;
            viz_type.type_name = argument_element->Attribute("data_type");
            if(viz_type.type_name.empty())
            {
                std::cerr << "viz_type in meta information of class " << plugin_info->class_name << " has no data_type!" << std::endl;
                continue;
            }
            viz_type.method_name = argument_element->Attribute("method_name");
            if(viz_type.method_name.empty())
                viz_type.method_name = "updateData";

            vizkit_plugin_info.visualized_types.push_back(viz_type);
            visualized_types_map.insert(std::make_pair(viz_type.type_name, plugin_info));
            argument_element = argument_element->NextSiblingElement("viz_type");
        }
    }
}

}