/**
 * @file    TextGlyph.h
 * @brief   Definition of TextGlyph for SBML Layout.
 * @author  Ralph Gauges
 * 
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2009-2013 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EBML-EBI), Hinxton, UK
 *  
 * Copyright (C) 2004-2008 by European Media Laboratories Research gGmbH,
 *     Heidelberg, Germany
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 *
 * @class TextGlyph
 * @sbmlbrief{layout} The %TextGlyph class describes the position and
 * dimension of text labels in the &ldquo;layout&rdquo; package.
 *
 * It inherits from GraphicalObject and adds the attributes graphicalObject,
 * text and originOfText.
 */

#ifndef TextGlyph_H__
#define TextGlyph_H__


#include <sbml/common/extern.h>
#include <sbml/common/sbmlfwd.h>
#include <sbml/packages/layout/common/layoutfwd.h>


#ifdef __cplusplus


#include <string>
#include <sbml/packages/layout/sbml/GraphicalObject.h>
#include <sbml/packages/layout/extension/LayoutExtension.h>

LIBSBML_CPP_NAMESPACE_BEGIN

class LIBSBML_EXTERN TextGlyph : public GraphicalObject
{
protected:
  /** @cond doxygenLibsbmlInternal */
  std::string mText;
  std::string mGraphicalObject;
  std::string mOriginOfText;
  /** @endcond */


public:

  /**
   * Creates a new TextGlyph with the given SBML level, versin and package
   * version. The ids of the associated GraphicalObject and
   * the originOfText are set to the empty string. The actual text is set
   * to the empty string as well.
   */  
  
  TextGlyph (unsigned int level      = LayoutExtension::getDefaultLevel(),
             unsigned int version    = LayoutExtension::getDefaultVersion(),
             unsigned int pkgVersion = LayoutExtension::getDefaultPackageVersion());


  /**
   * Ctor.
   */
  TextGlyph(LayoutPkgNamespaces* layoutns);

        
  /**
   * Creates a new TextGlpyh. The id is given as the first argument.
   *
   * (FOR BACKWARD COMPATIBILITY)
   *
   */ 
  
  TextGlyph (LayoutPkgNamespaces* layoutns, const std::string& id);

  /**
   * Creates a new TextGlpyh. The id is given as the first argument, the
   * text to be displayed as the second.  All other attirbutes are set to
   * the empty string.
   *
   * (FOR BACKWARD COMPATIBILITY)
   *
   */ 
  
  TextGlyph (LayoutPkgNamespaces* layoutns, const std::string& id, const std::string& text);
        

  /**
   * Creates a new TextGlyph from the given XMLNode
   *
   * (FOR BACKWARD COMPATIBILITY)
   *
   */
   TextGlyph(const XMLNode& node, unsigned int l2version=4);

  /**
   * Copy constructor.
   */
   TextGlyph(const TextGlyph& source);

  /**
   * Assignment operator.
   */
  virtual TextGlyph& operator=(const TextGlyph& source);

  /**
   * Destructor.
   */ 
  
  virtual ~TextGlyph ();
        

  /**
   * Renames all the @c SIdRef attributes on this element, including any
   * found in MathML content (if such exists).
   *
   * This method works by looking at all attributes and (if appropriate)
   * mathematical formulas, comparing the identifiers to the value of @p
   * oldid.  If any matches are found, the matching identifiers are replaced
   * with @p newid.  The method does @em not descend into child elements.
   *
   * @param oldid the old identifier
   * @param newid the new identifier
   */
  virtual void renameSIdRefs(const std::string& oldid, const std::string& newid);


  /**
   * Returns the text to be displayed by the text glyph.
   */ 
  
  const std::string& getText () const;
        
  /**
   * Sets the text to be displayed by the text glyph.
   */ 
  
  void setText (const std::string& text); 
        
  /**
   * Returns the id of the associated graphical object.
   */ 
  
  const std::string& getGraphicalObjectId () const;
        
  /**
   * Sets the id of the associated graphical object.
   */ 
  
  int setGraphicalObjectId (const std::string& id);
        
  /**
   * Returns the id of the origin of text.
   */ 
  
  const std::string& getOriginOfTextId () const;
        
  /**
   * Sets the id of the origin of text.
   */ 
  
  int setOriginOfTextId (const std::string& orig); 
        
  /**
   * Returns true if the text is not the empty string.
   */ 
  
  bool isSetText () const;
        
  /**
   * Returns true if the id of the origin of text is not the empty string.
   */ 
  
  bool isSetOriginOfTextId () const;
        
  /**
   * Returns true if the id of the associated graphical object is not the
   * empty string.
   */ 
  
  bool isSetGraphicalObjectId () const;
        
  /**
   * Calls initDefaults from GraphicalObject.
   */ 
  
  void initDefaults ();

  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to write out their contained
   * SBML objects as XML elements.  Be sure to call your parents
   * implementation of this method as well.  For example:
   *
   *   SBase::writeElements(stream);
   *   mReactants.write(stream);
   *   mProducts.write(stream);
   *   ...
   */
  virtual void writeElements (XMLOutputStream& stream) const;
  /** @endcond */

  /**
   * Returns the XML element name of
   * this SBML object.
   */
  virtual const std::string& getElementName () const ;

  /**
   * Creates and returns a deep copy of this TextGlyph.
   * 
   * @return a (deep) copy of this TextGlyph.
   */
  virtual TextGlyph* clone () const;


  /**
   * Returns the libSBML type code of this object instance.
   *
   * @copydetails doc_what_are_typecodes
   *
   * @return the SBML type code for this object:
   * @link SBMLLayoutTypeCode_t#SBML_LAYOUT_TEXTGLYPH SBML_LAYOUT_TEXTGLYPH@endlink
   *
   * @copydetails doc_warning_typecodes_not_unique
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode () const;


   /**
    * Creates an XMLNode object from this.
    */
    virtual XMLNode toXML() const;
    
protected:
  /** @cond doxygenLibsbmlInternal */
  /**
   * Create and return an SBML object of this class, if present.
   *
   * @return the SBML object corresponding to next XMLToken in the
   * XMLInputStream or NULL if the token was not recognized.
   */
  virtual SBase*
  createObject (XMLInputStream& stream);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to get the list of
   * expected attributes.
   * This function is invoked from corresponding readAttributes()
   * function.
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to read values from the given
   * XMLAttributes set into their specific fields.  Be sure to call your
   * parents implementation of this method as well.
   */
  virtual void readAttributes (const XMLAttributes& attributes, 
                               const ExpectedAttributes& expectedAttributes);
  /** @endcond */


  /** @cond doxygenLibsbmlInternal */
  /**
   * Subclasses should override this method to write their XML attributes
   * to the XMLOutputStream.  Be sure to call your parents implementation
   * of this method as well.  For example:
   *
   *   SBase::writeAttributes(stream);
   *   stream.writeAttribute( "id"  , mId   );
   *   stream.writeAttribute( "name", mName );
   *   ...
   */
  virtual void writeAttributes (XMLOutputStream& stream) const;
  /** @endcond */
};

LIBSBML_CPP_NAMESPACE_END

#endif /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


/*
 * Creates a new TextGlyph and returns the pointer to it.
 */
LIBSBML_EXTERN
TextGlyph_t *
TextGlyph_create (void);

/*
 * Creates a new TextGlyph from a template.
 */
LIBSBML_EXTERN
TextGlyph_t *
TextGlyph_createFrom (const TextGlyph_t *temp);

/*
 * Creates a new TextGlyph with the given @p id
 */
LIBSBML_EXTERN
TextGlyph_t *
TextGlyph_createWith (const char *sid);

/*
 * Creates a new TextGlyph referencing the give text.
 */
LIBSBML_EXTERN
TextGlyph_t *
TextGlyph_createWithText (const char *id, const char *text);

/*
 * Frees the memory taken by the given text glyph.
 */
LIBSBML_EXTERN
void
TextGlyph_free (TextGlyph_t *cg);

/*
 * Sets the text for the text glyph.
 */
LIBSBML_EXTERN
void
TextGlyph_setText (TextGlyph_t *cg, const char *text);

/*
 * Sets the id of the origin of the text for the text glyph.  This can be
 * the id of any valid sbml model object. The name of the object is then
 * taken as the text for the TextGlyph.
 */
LIBSBML_EXTERN
void
TextGlyph_setOriginOfTextId (TextGlyph_t *cg, const char *sid);

/*
 * Sets the assoziated GraphicalObject id for the text glyph.  A TextGlyph
 * which is assoziated with a GraphicalObject can be considered as a label
 * to that object and they might for example be moved together in an
 * editor.
 */
LIBSBML_EXTERN
void
TextGlyph_setGraphicalObjectId (TextGlyph_t *cg, const char *sid);


/*
 * Returns the text associated with this text glyph.
 */
LIBSBML_EXTERN
const char *
TextGlyph_getText (const TextGlyph_t *cg);

/*
 * Returns the id of the origin of the text associated with this text
 * glyph.
 */
LIBSBML_EXTERN
const char *
TextGlyph_getOriginOfTextId (const TextGlyph_t *cg);

/*
 * Returns the id of the graphical object associated with this text glyph.
 */
LIBSBML_EXTERN
const char *
TextGlyph_getGraphicalObjectId (const TextGlyph_t *cg);


/*
 * Returns true is the text attribute is not the empty string.
 */
LIBSBML_EXTERN
int
TextGlyph_isSetText (const TextGlyph_t *tg);


/*
 * Returns true is the originOfText attribute is not the empty string.
 */
LIBSBML_EXTERN
int
TextGlyph_isSetOriginOfTextId (const TextGlyph_t *tg);


/*
 * Returns true is the id of the associated graphical object is not the
 * empty string.
 */
LIBSBML_EXTERN
int
TextGlyph_isSetGraphicalObjectId (const TextGlyph_t *tg);

/*
 * Calls initDefaults from GraphicalObject.
 */ 
LIBSBML_EXTERN
void
TextGlyph_initDefaults (TextGlyph_t *tg);

/*
 * @return a (deep) copy of this TextGlyph.
 */
LIBSBML_EXTERN
TextGlyph_t *
TextGlyph_clone (const TextGlyph_t *m);


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif /* !SWIG */
#endif /* TextGlyph_H__ */
