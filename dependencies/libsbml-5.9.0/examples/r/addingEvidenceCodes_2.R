# 
# \file    addingEvidenceCodes_2.R
# \brief   adds evidence codes to a species in a model
# \author  Frank Bergmann
# 
# This file is part of libSBML.  Please visit http://sbml.org for more
# information about SBML, and the latest version of libSBML.
# 
#
# Usage: R --slave -f addingEvidenceCodes_2.R --args <input-filename> <output-filename>
#
#
library(libSBML)

args <- commandArgs(trailingOnly = TRUE)

if (length(args) != 2) {
  stop(
         "  usage: addingEvidenceCodes_2 <input-filename> <output-filename>\n  Adds controlled vocabulary term to a species\n"
      );
}

d      = readSBML(args[1]);
errors = SBMLDocument_getNumErrors(d);

if (errors > 0) {
  printf("Read Error(s):\n");
  SBMLDocument_printErrors(d);	 
  printf("Correct the above and re-run.\n");
} else {

  m = SBMLDocument_getModel(d);
  n =  Model_getNumSpecies(m);
  
  if (n <= 0) {
   cat( "Model has no reactions.\n Cannot add CV terms\n");
  } else {
    s = Model_getSpecies(m ,0);

    # check that the species has a metaid
    # no CVTerms will be added if there is no metaid to reference
    # 
    if (!SBase_isSetMetaId(s))
      SBase_setMetaId(s, "metaid_0000052");

    cv1 = CVTerm("BIOLOGICAL_QUALIFIER");
    CVTerm_setBiologicalQualifierType(cv1, "BQB_OCCURS_IN");
    CVTerm_addResource(cv1, "urn:miriam:obo.go:GO%3A0005764");

    SBase_addCVTerm(s, cv1);

    #  now create the additional annotation
    # 
    # <rdf:Statement> 
    #   <rdf:subject rdf:resource="#metaid_0000052"/> 
    #   <rdf:predicate rdf:resource="http://biomodels.net/biology-qualifiers/occursIn"/> 
    #   <rdf:object rdf:resource="urn:miriam:obo.go:GO%3A0005764"/> 
    #   <bqbiol:isDescribedBy> 
    #     <rdf:Bag> 
    #       <rdf:li rdf:resource="urn:miriam:obo.eco:ECO%3A0000004"/> 
    #       <rdf:li rdf:resource="urn:miriam:pubmed:7017716"/> 
    #     </rdf:Bag> 
    #   </bqbiol:isDescribedBy> 
    # </rdf:Statement> 

    # attributes
    blank_att = XMLAttributes()
    
    resource_att = XMLAttributes()

    # create the outer statement node 
    statement_triple = XMLTriple("Statement", "http://www.w3.org/1999/02/22-rdf-syntax-ns#","rdf");

    statement_token = XMLToken(statement_triple, blank_att);
    statement = XMLNode (statement_token);

    # create the subject node
    subject_triple = XMLTriple("subject", 
      "http://www.w3.org/1999/02/22-rdf-syntax-ns#",
      "rdf");
    
    XMLAttributes_clear(resource_att);      
    XMLAttributes_add(resource_att, "rdf:resource", SBase_getMetaId(s));
    
    
    subject_token = XMLToken (subject_triple, resource_att);
    
    subject = XMLNode (subject_token);


    # create the predicate node
    predicate_triple = XMLTriple("predicate", 
      "http://www.w3.org/1999/02/22-rdf-syntax-ns#",
      "rdf");
    
    XMLAttributes_clear(resource_att);      
    XMLAttributes_add(resource_att, "rdf:resource", 
      "http://biomodels.net/biology-qualifiers/occursIn");
    

    predicate_token = XMLToken (predicate_triple, resource_att);

    
    predicate = XMLNode (predicate_token);

    # create the object node
    object_triple = XMLTriple("object", 
      "http://www.w3.org/1999/02/22-rdf-syntax-ns#",
      "rdf");
    
    XMLAttributes_clear(resource_att);      
    XMLAttributes_add(resource_att, "rdf:resource", 
      "urn:miriam:obo.go:GO%3A0005764");
    
    object_token = XMLToken(object_triple, resource_att);

    object = XMLNode(object_token);

    # create the bqbiol node
    bqbiol_triple = XMLTriple("isDescribedBy", 
      "http://biomodels.net/biology-qualifiers/",
      "bqbiol");

    bqbiol_token = XMLToken(bqbiol_triple, blank_att);

    bqbiol = XMLNode (bqbiol_token);

    # create the bag node 
    bag_triple = XMLTriple("Bag", 
      "http://www.w3.org/1999/02/22-rdf-syntax-ns#",
      "rdf");
    bag_token = XMLToken(bag_triple, blank_att);
    
    bag = XMLNode(bag_token);

    # create each li node and add to the bag 
    li_triple = XMLTriple("li", 
      "http://www.w3.org/1999/02/22-rdf-syntax-ns#",
      "rdf");

    XMLAttributes_clear(resource_att);      
    XMLAttributes_add(resource_att, "rdf:resource", 
      "urn:miriam:obo.eco:ECO%3A0000004");

    li_token = XMLToken(li_triple, resource_att);
    XMLToken_setEnd(li_token);
    
    li = XMLNode(li_token);

    XMLNode_addChild(bag, li);
    
    XMLAttributes_clear(resource_att);      
    XMLAttributes_add(resource_att, "rdf:resource", 
     "urn:miriam:pubmed:7017716");
    li_token = XMLToken(li_triple, resource_att);
    XMLToken_setEnd(li_token);

    li = XMLNode(li_token);

    XMLNode_addChild(bag, li);

    # add the bag to bqbiol
    XMLNode_addChild(bqbiol, bag);
    

    # add subject, predicate, object and bqbiol to statement
    XMLNode_addChild(statement, subject);
    XMLNode_addChild(statement, predicate);
    XMLNode_addChild(statement, object);
    XMLNode_addChild(statement, bqbiol);
    

    # create a top-level RDF element 
    # this will ensure correct merging
    # 

    xmlns = XMLNamespaces()
    XMLNamespaces_add(xmlns, "http://www.w3.org/1999/02/22-rdf-syntax-ns#", "rdf");
    XMLNamespaces_add(xmlns, "http://purl.org/dc/elements/1.1/", "dc");
    XMLNamespaces_add(xmlns, "http://purl.org/dc/terms/", "dcterms");
    XMLNamespaces_add(xmlns, "http://www.w3.org/2001/vcard-rdf/3.0#", "vCard");
    XMLNamespaces_add(xmlns, "http://biomodels.net/biology-qualifiers/", "bqbiol");
    XMLNamespaces_add(xmlns, "http://biomodels.net/model-qualifiers/", "bqmodel");

    RDF_triple = XMLTriple("RDF", 
      "http://www.w3.org/1999/02/22-rdf-syntax-ns#",
      "rdf");

    RDF_token = XMLToken(RDF_triple, blank_att, xmlns);

    annotation = XMLNode(RDF_token);

    # add the staement node to the RDF node
    XMLNode_addChild(annotation, statement);      

    SBase_appendAnnotation(s, annotation);

    writeSBML(d, args[2]);
  }
}

q(status=errors);
