#include <osg/Group>
#include <typeinfo>
#include <cxxabi.h>
#include "Vizkit3DPlugin.hpp"
#include "Vizkit3DHelper.hpp"

using namespace vizkit;

/** this adapter is used to forward the update call to the plugin
 */
struct VizPluginBase::CallbackAdapter : public osg::NodeCallback 
{
    VizPluginBase* plugin;
    CallbackAdapter( VizPluginBase* plugin ) : plugin( plugin ) {}
    void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
	plugin->updateCallback( node );
	osg::NodeCallback::operator()(node, nv);
    }
};

VizPluginBase::VizPluginBase(QObject *parent)
    : QObject(parent),dirty( false ), plugin_enabled(true)
{
    position.setZero();
    orientation = Eigen::Quaterniond::Identity();
    vizNode = new osg::PositionAttitudeTransform();
    nodeCallback = new CallbackAdapter( this );
    vizNode->setUpdateCallback( nodeCallback );
}

osg::ref_ptr<osg::Group> VizPluginBase::getVizNode() const 
{
    return vizNode;
}

void VizPluginBase::setPose(const base::Vector3d& position, const base::Quaterniond& orientation)
{
    boost::mutex::scoped_lock lock(updateMutex);
    
    this->position = position;
    this->orientation = orientation;    
}

const QString VizPluginBase::getPluginName() const 
{
    return abi::__cxa_demangle(typeid(*this).name(), 0, 0, 0);
}

osg::ref_ptr<osg::Node> VizPluginBase::createMainNode()
{
    return new osg::Group();
}

std::vector< QDockWidget* > VizPluginBase::getDockWidgets()
{
    if (dockWidgets.empty()) createDockWidgets();
    return dockWidgets;
}

void VizPluginBase::createDockWidgets()
{

}

void VizPluginBase::updateCallback(osg::Node* node)
{
    boost::mutex::scoped_lock lockit(updateMutex);

    vizNode->setPosition(eigenVectorToOsgVec3(position));
    vizNode->setAttitude(eigenQuatToOsgQuat(orientation));
    
    if (!mainNode)
    {
        mainNode = createMainNode();
        vizNode->addChild(mainNode);
    }

    if( isDirty() )
    {
	updateMainNode(mainNode);
	dirty = false;
    }
}

bool VizPluginBase::isDirty() const
{
    return dirty;
}

void VizPluginBase::setDirty() 
{
    dirty = true;
}

QObject* vizkit::VizPluginBase::getRubyAdapterCollection()
{
    return &adapterCollection;
}

bool VizPluginBase::isPluginEnabled()
{
    return plugin_enabled;
}

void VizPluginBase::setPluginEnabled(bool enabled)
{
    plugin_enabled = enabled;
    emit pluginActivityChanged(enabled);
    emit propertyChanged("enabled");
}
