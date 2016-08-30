#ifndef __VIZKIT3D_PLUGIN_LOADER_HPP__
#define __VIZKIT3D_PLUGIN_LOADER_HPP__

#include "Vizkit3DPlugin.hpp"
#include "Vizkit3DPluginMetaInfo.hpp"

#include <boost/shared_ptr.hpp>
#include <base-logging/Singleton.hpp>
#include <plugin_manager/PluginLoader.hpp>

namespace vizkit3d
{

/**
 * @class Vizkit3DPluginLoader
 * @brief A singleton class used to load class loader based vizkit3d plugins
 * This class inherits from the PluginLoader
 */
class Vizkit3DPluginLoader : public plugin_manager::PluginLoader
{
    friend class base::Singleton<Vizkit3DPluginLoader>;
    static const std::string viz_plugin_base_name;

public:
    /**
     * @brief Returns the singleton instance of this class
     */
    static Vizkit3DPluginLoader* getInstance();

    /**
     * @brief Returns true if the class is registerd and inherits from vizkit3d::VizkitPluginBase.
     * @param plugin_name the name of the plugin class
     * @returns True if class is available
     */
    bool hasVizPlugin(const std::string& plugin_name) const;

    /**
     * @brief Creates an instance of the given class, which has to be inherited from vizkit3d::VizkitPluginBase
     * @param plugin_name the name of the vizkit3d plugin class
     * @param viz_plugin_base pointer to the vizkit plugin base class
     * @return True if an instance of the class could be created
     */
    bool createVizPlugin(const std::string& plugin_name, boost::shared_ptr<VizPluginBase>& viz_plugin_base);

    /**
     * @brief Creates an vizkit plugin for the type that shall be visualized
     * @param visualized_type The class name of the type that shall be visualized
     * @param viz_plugin_base pointer to the vizkit plugin base class
     * @return True if an instance of the class could be created
     */
    bool createVizPluginFor(const std::string& visualized_type, boost::shared_ptr<VizPluginBase>& viz_plugin_base);

    /**
     * @brief Provides the name of the update method.
     * @param plugin_name the name of the vizkit3d plugin class
     * @param visualized_type type name of the visualized type
     * @param method_name update method name that can be called on the given plugin with the given type
     * @return True if a method name could be found
     */
    bool getUpdateMethodName(const std::string& plugin_name, const std::string& visualized_type, std::string& method_name) const;

    /**
     * @brief Provides all type names that can be visualized by the given plugin
     * @param plugin_name the name of the vizkit3d plugin class
     * @param visualized_types All types that can be visualized by the given plugin
     * @return True if plugin information is available
     */
    bool getVisualizedTypes(const std::string& plugin_name, std::vector<std::string>& visualized_types) const;

    /**
     * @brief Provides all available vizkit3d plugins that can visualize the given type.
     * @param visualized_type type name of the visualized type
     * @param plugin_names names of available vizkit3d plugins that can visualize the given type
     * @return True if plugins for the given type are available
     */
    bool getVizPluginsFor(const std::string& visualized_type, std::vector<std::string>& plugin_names);

    /**
     * @brief Clears all plugin informations.
     */
    virtual void clear();

protected:

    /**
     * @overload
     * @brief Parses plugin specific meta information defined inside of the <meta>..</meta> block.
     */
    virtual void parsePluginMetaInformation(const PluginInfoPtr& plugin_info, TiXmlElement* meta_element);

private:
    /**
     * @brief Constructor for Vizkit3DPluginLoader
     * It is protected because this class is a singleton class.
     */
    Vizkit3DPluginLoader();

    /**
     * @brief Destructor for Vizkit3DPluginLoader
     */
    virtual ~Vizkit3DPluginLoader();

private:
    /** Mapping between full class name and vizkit plugin meta information */
    std::map<std::string, Vizkit3DPluginMetaInfo> vizkit_plugin_meta_info;

    /** Mapping between the visualized type name and the available vizkit plugins */
    std::multimap<std::string, PluginInfoPtr> visualized_types_map;
};

}

#endif