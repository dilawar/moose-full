#include "core/CoordinateSystem.hpp"

// http://www.bricoworks.com/articles/stategraph/stategraph.html
// http://opensg.org/wiki/Tutorial/OpenSG1/Traversal
// http://stackoverflow.xluat.com/questions/28104362/how-to-rotate-camera-in-openscenegraphwalk-through-in-model

CoordinateSystem::CoordinateSystem(): PositionAttitudeTransform()
                                    , plane(new osg::Geometry())
{
  plane -> setVertexArray(new Vec3Array(44));
  //  plane -> setNormalArray(new Vec3Array(2), osg::Array::BIND_PER_VERTEX);
  osg::Vec3Array * vertices = static_cast<osg::Vec3Array *>(plane -> getVertexArray());
  //osg::Vec3Array * normals  = static_cast<osg::Vec3Array *>(plane -> getNormalArray());
  // (*vertices)[0] = (*vertices)[4] = osg::Vec3f(0.0f, 0.0f, 0.0f);
  // (*vertices)[1] = (*vertices)[5] = osg::Vec3f(1000.0f, 0.0f, 0.0f);
  // (*vertices)[2] = (*vertices)[6] = osg::Vec3f(1000.0f, 1000.0f, 0.0f);
  // (*vertices)[3] = (*vertices)[7] = osg::Vec3f(0.0f, 1000.0f, 0.0f);

  // (*normals)[0] = (*normals)[1] = (*normals)[2] = (*normals)[3] = osg::Z_AXIS;
  // (*normals)[4] = (*normals)[5] = (*normals)[6] = (*normals)[7] = osg::Z_AXIS * -1.0f;

  osg::Vec4Array * colors = new Vec4Array(1);
  //(*colors)[0] = osg::Vec4f(173 / 255.0f, 216 / 255.0f, 230.0f / 255, 0.1f);
  (*colors)[0] = osg::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
  plane -> setColorArray(colors, osg::Array::BIND_OVERALL);
  plane -> getOrCreateStateSet() -> setAttribute(new osg::LineWidth(1.0f));

  osg::DrawArrays * primitive_set = new osg::DrawArrays(GL_LINES, 0, 44);
  // osg::DrawElementsUShort * primitive_set = new osg::DrawElementsUShort(GL_TRIANGLES, 6);
  // (*primitive_set)[0] = 0;
  // (*primitive_set)[1] = 1;
  // (*primitive_set)[2] = 2;
  // (*primitive_set)[3] = 2;
  // (*primitive_set)[4] = 3;
  // (*primitive_set)[5] = 0;
  plane -> addPrimitiveSet(primitive_set);
  plane -> setUseDisplayList(false);
  plane -> setUseVertexBufferObjects(true);
  osg::Geode * geode = new osg::Geode();
  geode -> addDrawable(plane.get());
  addChild(geode);
  //setReferenceFrame(osg::Transform::ABSOLUTE_RF);
}

CoordinateSystem::CoordinateSystem( const CoordinateSystem & pat
                                  , const CopyOp & copyop
                                  ): PositionAttitudeTransform(pat, copyop)
{ }

void
CoordinateSystem::accept(NodeVisitor& nv)
{
    std::cerr << "Accept" << std::endl;
    //index++;

    if (nv.validNodeMask(*this))
    {
        // if app traversal update the frame count.
        if (nv.getVisitorType()==NodeVisitor::UPDATE_VISITOR)
        {
                std::cerr << "Update Visitor" << std::endl;
                double x_percentage = 0.1;
                double y_percentage = 0.1;
                double z_percentage = 0.1;
                double fovy;
                double aspect_ratio;
                double z_near;
                double z_far;
                // osg::Camera * camera = cs -> getCurrentCamera();
                camera -> getProjectionMatrixAsPerspective( fovy
                                                          , aspect_ratio
                                                          , z_near
                                                          , z_far
                                                          );
                z_near = 1.0f;
                z_far = 10000.0f;
                float z = z_near + (z_far - z_near) * z_percentage;
                float height = 2 * z * tan(osg::PI * fovy / 360.0f);
                float width  = height * aspect_ratio;
                // std::cerr << "Height " << height << std::endl;
                // std::cerr << "Width  " << width  << std::endl;
                // std::cerr << "fovy " << fovy << std::endl;
                // std::cerr << "z_near " << z_near << std::endl;
                // std::cerr << "z_far" << z_far << std::endl;
                // std::cerr << "aspect_ratio " << aspect_ratio << std::endl;
                // float z = (z_near + z_far) / 2.0f;
                //osg::Vec3f position(0.0f, z, 0.0f);
                osg::Vec3d eye, center, up;
                camera -> getViewMatrixAsLookAt(eye, center, up);
                osg::Vec3f direction = center - eye;
                direction.normalize();
                osg::Vec3f side = direction ^ up;
                side.normalize();

        }
        else if (nv.getVisitorType()==NodeVisitor::CULL_VISITOR)
        {
            std::cerr << "Cull Visitor" << std::endl;
            osgUtil::CullVisitor* cs = dynamic_cast<osgUtil::CullVisitor*>(&nv);
            cs -> setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
            if(cs)
            {

                // osg::Viewport* viewport = cs->getViewport();
                // float width = viewport->width();
                // float height = viewport->height();
                // std::cerr << "Viewport width => " << width << std::endl;
                // std::cerr << "Viewport height => " << height << std::endl;
                // std::cerr << "Near Plane => " << cs -> getCalculatedNearPlane() << std::endl;
                // std::cerr << "Far  Plane => " << cs -> getCalculatedFarPlane() << std::endl;
                // osg::Vec3d eyePoint = cs -> getEyeLocal();
                // osg::Vec3f viewPoint = cs -> getViewPoint();
                // osg::Vec3d localUp = cs  -> getUpLocal();
                // //osg::Vec3d position = getPosition();
                // std::cerr << std::endl;
                // std::cerr << eyePoint[0] << ", " << eyePoint[1] << ", " << eyePoint[2] << std::endl;
                // std::cerr << viewPoint[0] << ", " << viewPoint[1] << ", " << viewPoint[2] << std::endl;
                double x_percentage = 0.05;
                double y_percentage = 0.05;
                double z_percentage = 0.15;
                double fovy;
                double aspect_ratio;
                double z_near;
                double z_far;
                // osg::Camera * camera = cs -> getCurrentCamera();
                camera -> getProjectionMatrixAsPerspective( fovy
                                                          , aspect_ratio
                                                          , z_near
                                                          , z_far
                                                          );
                z_near = 1.0f;
                z_far = 10000.0f;
                float z = z_near + (z_far - z_near) * z_percentage;
                float height = 2 * z * tan(osg::PI * fovy / 360.0f);
                float width  = height * aspect_ratio;
                // std::cerr << "Height " << height << std::endl;
                // std::cerr << "Width  " << width  << std::endl;
                // std::cerr << "fovy " << fovy << std::endl;
                // std::cerr << "z_near " << z_near << std::endl;
                // std::cerr << "z_far" << z_far << std::endl;
                // std::cerr << "aspect_ratio " << aspect_ratio << std::endl;
                // float z = (z_near + z_far) / 2.0f;
                //osg::Vec3f position(0.0f, z, 0.0f);
                osg::Vec3d eye, center, up;
                camera -> getViewMatrixAsLookAt(eye, center, up);
                std::cerr << eye[0] << ", " << eye[1] << ", " << eye[2] << std::endl;
                std::cerr << center[0] << ", " << center[1] << ", " << center[2] << std::endl;
                osg::Vec3f direction = center - eye;
                direction.normalize();
                osg::Vec3f side = direction ^ up;
                side.normalize();
                osg::Quat up_rotation;
                up_rotation.makeRotate(osg::PI * 30 / 180.0f, up);
                osg::Quat side_rotation;
                side_rotation.makeRotate(osg::PI * 30 / 180.0f, side);
                //setAttitude(up_rotation * side_rotation);

                osg::Vec3f position = eye
                                    + side * width * (x_percentage - 0.5)
                                    + up * height * (y_percentage - 0.5)
                                    + direction * z;

                setPosition(position);
                //                               setPosition(osg::Vec3f(0.0f, 0.0f, 0.0f));
                // + camera -> getInverseViewMatrix() * position);
                //osg::Matrixd::inverse(camera -> getProjectionMatrix()) * position);
                //cs -> getViewPoint() + osg::Vec3f(30.0f, 1000.0f, 30.0f));
                //eyePoint + osg::Vec3f(10.0f, 10.0f, 100.0f));
                  osg::Vec3Array * vertices = static_cast<osg::Vec3Array *>(plane -> getVertexArray());
                  //osg::Vec3Array * normals  = static_cast<osg::Vec3Array *>(plane -> getNormalArray());
                  float z_delta = 200.0f;
                  z = z_near;
                  height = 2 * z * tan(osg::PI * fovy / 360.0f);
                  width  = height * aspect_ratio;

                  for(int i = 0; i < 11; ++i)
                  {
                    (*vertices)[(i) * 2] = osg::Vec3f( eye
                                                            + up * height * -0.2f
                                                            + side * width * (0.5f -i/10.0f)
                                                            + direction * z_near// + i * z_delta)
                                                            )
                                                            - position;
                    (*vertices)[(i) * 2 + 1] = osg::Vec3f( eye
                                                                + up * height * -0.2f
                                                                + side * width * (0.5f - i/10.0f)
                                                                + direction * z_far //  + i * z_delta)
                                                                ) - position;
                    // z = z_near + (z_far - z_near) * i / 10.0f;
                    // (*vertices)[i * 2] = osg::Vec3f( eye
                    //                                + up   * height * 0.1f
                    //                                + side * width  * -0.5f
                    //                                + direction * z// + i * z_delta)
                    //                                ) - position;
                    // (*vertices)[i * 2 + 1] = osg::Vec3f( eye
                    //                                    + up   * height * 0.1f
                    //                                    + side * width  * 0.5f
                    //                                    + direction * z//  + i * z_delta)
                    //                                    ) - position;
                  }
                  for(int i = 0; i < 11; ++i)
                  {
                    (*vertices)[(11 + i) * 2] = osg::Vec3f( eye
                                                          + up * height * 0.2f
                                                          + side * width * (0.5f -i/10.0f)
                                                          + direction * z_near// + i * z_delta)
                                                          )
                                                          - position;
                    (*vertices)[(11 + i) * 2 + 1] = osg::Vec3f( eye
                                                              + up * height * 0.2f
                                                              + side * width * (0.5f - i/10.0f)
                                                              + direction * z_far //  + i * z_delta)
                                                              )
                                                              - position;

                  }
                      // (*vertices)[0] = (*vertices)[4] = osg::Vec3f( eye
                  //                                             + up * height * 0.1
                  //                                             + side * width * -0.2f
                  //                                             + direction * z_near
                  //                                             )
                  //                                 - position;
                  // (*vertices)[1] = (*vertices)[5] = osg::Vec3f( eye
                  //                                             + up   * height * 0.1
                  //                                             + side * width  * 0.2f
                  //                                             + direction * z_near
                  //                                             )
                  //                                 - position;
                  // (*vertices)[2] = (*vertices)[6] = osg::Vec3f( eye
                  //                                             + up * height * 0.1
                  //                                             + side * width * 0.2f
                  //                                             + direction * z_far
                  //                                             )
                  //                                 - position;
                  // (*vertices)[3] = (*vertices)[7] = osg::Vec3f( eye
                  //                                             + up * height * 0.1
                  //                                             + side * width * -0.2f
                  //                                             + direction * z_far
                  //                                             )
                  //                                   - position;
                  // (*vertices)[0] = (*vertices)[4] = osg::Vec3f( eye
                  //                                             + up * height * 0.5
                  //                                             + side * width * 0.1f
                  //                                             + direction * 0.0f
                  //                                             )
                  //                                 - position;
                  // (*vertices)[1] = (*vertices)[5] = osg::Vec3f( eye
                  //                                             + up * height * -0.5f
                  //                                             + side * width * 0.1f
                  //                                             + direction * 0.0f
                  //                                             )
                  //                                 - position;
                  // (*vertices)[2] = (*vertices)[6] = osg::Vec3f( eye
                  //                                             + up * height * -0.5f
                  //                                             + side * width * 0.1f
                  //                                             + direction * z_far * 10.0f
                  //                                             )
                  //                                 - position;
                  // (*vertices)[3] = (*vertices)[7] = osg::Vec3f( eye
                  //                                             + up * height * 0.5f
                  //                                             + side * width * 0.1f
                  //                                             + direction * z_far * 10.0f
                  //                                             )
                  //                                 - position;
                  // (*vertices)[0] = (*vertices)[4] = osg::Vec3f(0.0f, 0.0f, 0.0f);
                  // (*vertices)[1] = (*vertices)[5] = osg::Vec3f(10.0f, 0.0f, 0.0f);
                  // (*vertices)[2] = (*vertices)[6] = osg::Vec3f(10.0f, 10.0f, 0.0f);
                  // (*vertices)[3] = (*vertices)[7] = osg::Vec3f(0.0f, 10.0f, 0.0f);

                  // (*normals)[0] = (*normals)[1] = (*normals)[2] = (*normals)[3] = direction * -1.0f;
                  // (*normals)[4] = (*normals)[5] = (*normals)[6] = (*normals)[7] = direction * -1.0f;
                  vertices -> dirty();
                  //                  normals  -> dirty();
                  plane -> dirtyBound();
            }
        }

        // now do the proper accept
        PositionAttitudeTransform::accept(nv);
    }
}
