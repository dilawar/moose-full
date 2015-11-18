#include "callbacks/MeshUpdateCallback.hpp"

MeshUpdateCallback::MeshUpdateCallback(Mesh * mesh): UpdateCallback()
                                                   , mesh(mesh)
{
}

void
MeshUpdateCallback::update( osg::NodeVisitor *nv
                          , osg::Drawable *drawable
                          )
{
    if(mesh -> allocation_required())
    {
        mesh -> allocate();
        mesh -> construct_indices();
        mesh -> construct_vertices();
        mesh -> color();
    }
    if(mesh -> construction_required())
    {
        mesh -> construct_vertices();
    }
    if(mesh -> coloring_required())
    {
        mesh -> color();
    }
}
