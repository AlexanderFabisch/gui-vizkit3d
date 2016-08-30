#include <boost/test/unit_test.hpp>
#include <vizkit3d/Vizkit3DWidget.hpp>
#include <vizkit3d/QtThreadedWidget.hpp>
#include <vizkit3d/Vizkit3DPluginLoader.hpp>

BOOST_AUTO_TEST_CASE(load_vizkit_plugins_test)
{
    QtThreadedWidget<vizkit3d::Vizkit3DWidget> app;
    app.start();

    while(!app.isRunning()) {usleep(10000);}

    vizkit3d::Vizkit3DPluginLoader* loader = vizkit3d::Vizkit3DPluginLoader::getInstance();

    boost::shared_ptr<vizkit3d::VizPluginBase> grid_plugin;
    BOOST_CHECK(loader->createVizPlugin("GridVisualization", grid_plugin));
    app.getWidget()->addPlugin(grid_plugin.get());

    boost::shared_ptr<vizkit3d::VizPluginBase> model_plugin;
    BOOST_CHECK(loader->createVizPlugin("ModelVisualization", model_plugin));
    app.getWidget()->addPlugin(model_plugin.get());

    boost::shared_ptr<vizkit3d::VizPluginBase> textrure_box_plugin;
    BOOST_CHECK(loader->createVizPlugin("TextureBoxVisualization", textrure_box_plugin));
    app.getWidget()->addPlugin(textrure_box_plugin.get());

    unsigned count = 0;
    while(count < 1000) {usleep(10000); count++;}
    app.stop();
}