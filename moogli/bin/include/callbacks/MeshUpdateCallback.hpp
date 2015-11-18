#ifndef __MESH_UPDATE_CALLBACK_HPP__
#define __MESH_UPDATE_CALLBACK_HPP__

#include "utility/libraries.hpp"
#include "mesh/Mesh.hpp"

class MeshUpdateCallback : public osg::Drawable::UpdateCallback
{
public:
    MeshUpdateCallback(Mesh * mesh);

    virtual void
    update( osg::NodeVisitor * nv
          , osg::Drawable * drawable
          );
private:
    Mesh * mesh;
};

#endif /* __MESH_UPDATE_CALLBACK_HPP__ */
