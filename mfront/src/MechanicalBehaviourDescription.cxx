/*!
 * \file   MechanicalBehaviourDescription.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   18 Jan 2007
 */

#include<sstream>
#include<stdexcept>

#include"MFront/MechanicalBehaviourDescription.hxx"

namespace mfront{

  MechanicalBehaviourDescription::MechanicalBehaviourDescription()
    : usableInPurelyImplicitResolution(false),
      sOperator(false),
      aTensor(false),
      use_qt(false),
      type(MechanicalBehaviourDescription::GENERALBEHAVIOUR),
      // By default, a behaviour is isotropic 
      stype(mfront::ISOTROPIC),
      // By default, a behaviour is isotropic 
      estype(mfront::ISOTROPIC)
  {} // end of MechanicalBehaviourDescription::MechanicalBehaviourDescription()

  void
  MechanicalBehaviourDescription::setMaterialName(const std::string& m)
  {
    using namespace std;
    if(!this->material.empty()){
      string msg("MechanicalBehaviourDescription::setMaterialName : ");
      msg += "material name alreay defined";
      throw(runtime_error(msg));
    }
    this->material = m;
  } // end of MechanicalBehaviourDescription::setMaterialName

  const std::string&
  MechanicalBehaviourDescription::getMaterialName(void) const
  {
    return this->material;
  } // end of MechanicalBehaviourDescription::getMaterialName

  void
  MechanicalBehaviourDescription::addStaticVariable(const StaticVariableDescription& v)
  {
    this->staticVars.push_back(v);
  } // end of MechanicalBehaviourDescription::addStaticVariable

  const StaticVariableDescriptionContainer&
  MechanicalBehaviourDescription::getStaticVariables(void) const
  {
    return this->staticVars;
  } // end of MechanicalBehaviourDescription::getStaticVariables


  std::vector<BoundsDescription>&
  MechanicalBehaviourDescription::getBounds(void)
  {
    return this->bounds;
  } // end of MechanicalBehaviourDescription::getBoundsDescriptions

  const std::vector<BoundsDescription>&
  MechanicalBehaviourDescription::getBounds(void) const
  {
    return this->bounds;
  } // end of MechanicalBehaviourDescription::getBoundsDescriptions

  const VariableDescription&
  MechanicalBehaviourDescription::getStateVariableHandler(const std::string& v) const
  {
    return this->getVariableHandler(this->getStateVariables(),v);
  } // end of MechanicalBehaviourDescription::getStateVariableHandler

  const VariableDescription&
  MechanicalBehaviourDescription::getVariableHandler(const VariableDescriptionContainer& cont,
						     const std::string& v) const
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    for(p=cont.begin();p!=cont.end();++p){
      if(p->name==v){
	return *p;
      }
    }
    string msg("MechanicalBehaviourDescription::getVariableHandler : ");
    msg += "no state variable '"+v+"'";
    throw(runtime_error(msg));
    return *(static_cast<VariableDescription*>(0));
  } // end of MechanicalBehaviourDescription::getVariableHandler

  bool
  MechanicalBehaviourDescription::isDrivingVariableName(const std::string& n) const
  {
    using namespace std;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    for(pm=this->getMainVariables().begin();pm!=this->getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(dv.name==n){
	return true;
      }
    }
    return false;
  } // end of MechanicalBehaviourDescription::isDrivingVariableName

  bool
  MechanicalBehaviourDescription::isDrivingVariableIncrementName(const std::string& n) const
  {
    using namespace std;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    for(pm=this->getMainVariables().begin();pm!=this->getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(dv.increment_known){
	if("d"+dv.name==n){
	  return true;
	}
      }
    }
    return false;
  } // end of MechanicalBehaviourDescription::isDrivingVariableIncrementName

  std::pair<SupportedTypes::TypeSize,
	    SupportedTypes::TypeSize>
  MechanicalBehaviourDescription::getMainVariablesSize(void) const
  {
    using namespace std;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    SupportedTypes::TypeSize ov;
    SupportedTypes::TypeSize of;
    for(pm=this->getMainVariables().begin();pm!=this->getMainVariables().end();++pm){
      ov += this->getTypeSize(pm->first.type,1u);
      of += this->getTypeSize(pm->second.type,1u);
    }
    return make_pair(ov,of);
  } // end of MechanicalBehaviourDescription::getMainVariablesSize

  bool
  MechanicalBehaviourDescription::isMaterialPropertyName(const std::string& n) const
  {
    return this->getMaterialProperties().contains(n);
  } // end of MechanicalBehaviourDescription::isMaterialPropertyName

  bool
  MechanicalBehaviourDescription::isLocalVariableName(const std::string& n) const
  {
    return this->getLocalVariables().contains(n);
  } // end of MechanicalBehaviourDescription::isLocalVariableName

  bool
  MechanicalBehaviourDescription::isParameterName(const std::string& n) const
  {
    return this->getParameters().contains(n);
  } // end of MechanicalBehaviourDescription::isParameterName
  
  bool
  MechanicalBehaviourDescription::isInternalStateVariableName(const std::string& n) const
  {
    return this->getStateVariables().contains(n);
  } // end of MechanicalBehaviourDescription::isInternalStateVariableName

  bool
  MechanicalBehaviourDescription::isInternalStateVariableIncrementName(const std::string& n) const
  {
    if(n.size()<2){
      return false;
    }
    if(n[0]!='d'){
      return false;
    }
    return this->getStateVariables().contains(n.substr(1));
  } // end of MechanicalBehaviourDescription::isInternalStateVariableName

  bool
  MechanicalBehaviourDescription::isAuxiliaryInternalStateVariableName(const std::string& n) const
  {
    return this->getAuxiliaryStateVariables().contains(n);
  } // end of MechanicalBehaviourDescription::isInternalStateVariableName
  
  bool
  MechanicalBehaviourDescription::isExternalStateVariableName(const std::string& n) const
  {
    return this->getExternalStateVariables().contains(n);
  } // end of MechanicalBehaviourDescription::isExternalStateVariableName

  bool
  MechanicalBehaviourDescription::isExternalStateVariableIncrementName(const std::string& n) const
  {
    if(n.size()<2){
      return false;
    }
    if(n[0]!='d'){
      return false;
    }
    return this->getExternalStateVariables().contains(n.substr(1));
  } // end of MechanicalBehaviourDescription::isExternalStateVariableName

  VariableDescriptionContainer&
  MechanicalBehaviourDescription::getMaterialProperties(void)
  {
    return this->materialProperties;
  } // end of MechanicalBehaviourDescription::getMaterialProperties

  const VariableDescriptionContainer&
  MechanicalBehaviourDescription::getMaterialProperties(void) const
  {
    return this->materialProperties;
  } // end of MechanicalBehaviourDescription::getMaterialProperties

  VariableDescriptionContainer&
  MechanicalBehaviourDescription::getStateVariables(void)
  {
    return this->stateVariables;
  } // end of MechanicalBehaviourDescription::getStateVariables

  const VariableDescriptionContainer&
  MechanicalBehaviourDescription::getStateVariables(void) const
  {
    return this->stateVariables;
  } // end of MechanicalBehaviourDescription::getStateVariables

  VariableDescriptionContainer&
  MechanicalBehaviourDescription::getAuxiliaryStateVariables(void)
  {
    return this->auxiliaryStateVariables;
  } // end of MechanicalBehaviourDescription::getAuxiliaryStateVariables

  const VariableDescriptionContainer&
  MechanicalBehaviourDescription::getAuxiliaryStateVariables(void) const
  {
    return this->auxiliaryStateVariables;
  } // end of MechanicalBehaviourDescription::getAuxiliaryStateVariables

  VariableDescriptionContainer&
  MechanicalBehaviourDescription::getExternalStateVariables(void)
  {
    return this->externalStateVariables;
  } // end of MechanicalBehaviourDescription::getExternalStateVariables

  const VariableDescriptionContainer&
  MechanicalBehaviourDescription::getExternalStateVariables(void) const
  {
    return this->externalStateVariables;
  } // end of MechanicalBehaviourDescription::getExternalStateVariables

  VariableDescriptionContainer&
  MechanicalBehaviourDescription::getLocalVariables(void)
  {
    return this->localVariables;
  } // end of MechanicalBehaviourDescription::getLocalVariables

  const VariableDescriptionContainer&
  MechanicalBehaviourDescription::getLocalVariables(void) const
  {
    return this->localVariables;
  } // end of MechanicalBehaviourDescription::getLocalVariables

  VariableDescriptionContainer&
  MechanicalBehaviourDescription::getParameters(void)
  {
    return this->parameters;
  } // end of MechanicalBehaviourDescription::getParameters
  
  const VariableDescriptionContainer&
  MechanicalBehaviourDescription::getParameters(void) const
  {
    return this->parameters;
  } // end of MechanicalBehaviourDescription::getParameters

  std::map<std::string,double>&
  MechanicalBehaviourDescription::getParametersDefaultValues()
  {
    return this->parametersDefaultValues;
  } // end of MechanicalBehaviourDescription::getParametersDefaultValues

  const std::map<std::string,double>&
  MechanicalBehaviourDescription::getParametersDefaultValues() const
  {
    return this->parametersDefaultValues;
  } // end of MechanicalBehaviourDescription::getParametersDefaultValues

  std::map<std::string,int>&
  MechanicalBehaviourDescription::getIntegerParametersDefaultValues()
  {
    return this->iParametersDefaultValues;
  } // end of MechanicalBehaviourDescription::getIntegerParametersDefaultValues

  const std::map<std::string,int>&
  MechanicalBehaviourDescription::getIntegerParametersDefaultValues() const
  {
    return this->iParametersDefaultValues;
  } // end of MechanicalBehaviourDescription::getIntegerParametersDefaultValues

  std::map<std::string,unsigned short>&
  MechanicalBehaviourDescription::getUnsignedShortParametersDefaultValues()
  {
    return this->uParametersDefaultValues;
  } // end of MechanicalBehaviourDescription::getUnsignedShortParametersDefaultValues

  const std::map<std::string,unsigned short>&
  MechanicalBehaviourDescription::getUnsignedShortParametersDefaultValues() const
  {
    return this->uParametersDefaultValues;
  } // end of MechanicalBehaviourDescription::getUnsignedShortParametersDefaultValues

  void
  MechanicalBehaviourDescription::declareAsASmallStrainStandardBehaviour(void)
  {
    using namespace std;
    typedef map<DrivingVariable,ThermodynamicForce>::value_type MVType;
    if(!this->mvariables.empty()){
      string msg("MechanicalBehaviourDescription::declareAsASmallStrainStandardBehaviour : ");
      msg += "some driving variables are already declared";
      throw(runtime_error(msg));
    }
    DrivingVariable eto;
    eto.name = "eto";
    eto.type = "StrainStensor";
    eto.increment_known = true;
    ThermodynamicForce sig;
    sig.name = "sig";
    sig.type = "StressStensor";
    this->mvariables.insert(MVType(eto,sig));
    this->type = MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR;
  }

  void
  MechanicalBehaviourDescription::declareAsAFiniteStrainStandardBehaviour(void)
  {
    using namespace std;
    typedef map<DrivingVariable,ThermodynamicForce>::value_type MVType;
    if(!this->mvariables.empty()){
      string msg("MechanicalBehaviourDescription::declareAsAFiniteStrainStandardBehaviour : ");
      msg += "some driving variables are already declared";
      throw(runtime_error(msg));
    }
    DrivingVariable F;
    F.name = "F";
    F.type = "DeformationGradientTensor";
    F.increment_known = false;
    ThermodynamicForce sig;
    sig.name = "sig";
    sig.type = "StressStensor";
    this->mvariables.insert(MVType(F,sig));
    this->type = MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR;
  }
    
  void
  MechanicalBehaviourDescription::declareAsACohesiveZoneModel(void)
  {
    using namespace std;
    typedef map<DrivingVariable,ThermodynamicForce>::value_type MVType;
    if(!this->mvariables.empty()){
      string msg("MechanicalBehaviourDescription::declareAsACohesiveZoneModel : ");
      msg += "some driving variables are already declared";
      throw(runtime_error(msg));
    }
    DrivingVariable u;
    u.name = "u";
    u.type = "DisplacementTVector";
    u.increment_known = true;
    ThermodynamicForce t;
    t.name = "t";
    t.type = "ForceTVector";
    this->mvariables.insert(MVType(u,t));
    this->type = MechanicalBehaviourDescription::COHESIVEZONEMODEL;
  }

  void
  MechanicalBehaviourDescription::addMainVariable(const DrivingVariable&    v,
						  const ThermodynamicForce& f)
  {
    using namespace std;
    typedef map<DrivingVariable,ThermodynamicForce>::value_type MVType;
    if(this->type!=MechanicalBehaviourDescription::GENERALBEHAVIOUR){
      string msg("MechanicalBehaviourDescription::addMainVariables : "
		 "one can not add a main variable if the behaviour "
		 "don't have a general behaviour type");
      throw(runtime_error(msg));
    }
    if(!this->mvariables.insert(MVType(v,f)).second){
      string msg("MechanicalBehaviourDescription::addMainVariables : "
		 "a driving variable '"+v.name+"' has already been declared");
      throw(runtime_error(msg));
    }
  } // end of MechanicalBehaviourDescription::addMainVariables

  const std::map<DrivingVariable,
		 ThermodynamicForce>&
  MechanicalBehaviourDescription::getMainVariables(void) const
  {
    return this->mvariables;
  } // end of MechanicalBehaviourDescription::getMainVariables
  
  void
  MechanicalBehaviourDescription::setUseQt(const bool b)
  {
    this->use_qt = b;
  } // end of MechanicalBehaviourDescription::setUseQt

  bool
  MechanicalBehaviourDescription::useQt(void) const
  {
    return this->use_qt;
  } // end of MechanicalBehaviourDescription::useQt

  MechanicalBehaviourDescription::BehaviourType
  MechanicalBehaviourDescription::getBehaviourType() const
  {
    return this->type;
  } // end of MechanicalBehaviourDescription::getBehaviourType

  SymmetryType
  MechanicalBehaviourDescription::getElasticSymmetryType() const
  {
    return this->estype;
  } // end of MechanicalBehaviourDescription::getElasticSymmetryType

  void
  MechanicalBehaviourDescription::setElasticSymmetryType(const SymmetryType t)
  {
    this->estype = t;
  } // end of MechanicalBehaviourDescription::setElasticSymmetryType

  SymmetryType
  MechanicalBehaviourDescription::getSymmetryType() const
  {
    return this->stype;
  } // end of MechanicalBehaviourDescription::getSymmetryType

  void
  MechanicalBehaviourDescription::setSymmetryType(const SymmetryType t)
  {
    this->stype = t;
  } // end of MechanicalBehaviourDescription::setSymmetryType

  std::string
  MechanicalBehaviourDescription::getStiffnessOperatorType(void) const
  {
    using namespace std;
    if(this->type==GENERALBEHAVIOUR){
      pair<SupportedTypes::TypeSize,
	   SupportedTypes::TypeSize> msizes = this->getMainVariablesSize();
      ostringstream t;
      t << "tfel::math::tmatrix<"
	<< msizes.first  << "," 
	<< msizes.second << ",real>";
      return t.str();
    } else if(this->type==SMALLSTRAINSTANDARDBEHAVIOUR){
      return "StiffnessTensor";
    } else if(this->type==FINITESTRAINSTANDARDBEHAVIOUR){
      return "FiniteStrainStiffnessTensor";
    } else if(this->type==COHESIVEZONEMODEL){
      return "tfel::math::tmatrix<N,N,stress>";
    }
    string msg("MechanicalBehaviourDescription::getStiffnessOperatorType : "
	       "internal error (unsupported behaviour type)");
    throw(runtime_error(msg));
    return "";
  } // end of MechanicalBehaviourDescription::getStiffnessOperatorType


  bool
  MechanicalBehaviourDescription::requiresStiffnessOperator(void) const
  {
    return this->sOperator; 
  } // end of MechanicalBehaviourDescription::requiresStiffnessOperator
  
  void
  MechanicalBehaviourDescription::setRequireStiffnessOperator(const bool b)
  {
    this->sOperator = b;
  } // end of MechanicalBehaviourDescription::setRequireStiffnessOperator

  bool
  MechanicalBehaviourDescription::requiresThermalExpansionTensor(void) const
  {
    return this->aTensor; 
  } // end of MechanicalBehaviourDescription::requiresThermalExpansionTensor
  
  void
  MechanicalBehaviourDescription::setRequireThermalExpansionTensor(const bool b)
  {
    this->aTensor = b;
  } // end of MechanicalBehaviourDescription::setRequireThermalExpansionTensor
  
  bool
  MechanicalBehaviourDescription::isUsableInPurelyImplicitResolution(void) const
  {
    return this->usableInPurelyImplicitResolution;
  } // end of MechanicalBehaviourDescription::isUsableInPurelyImplicitResolution

  void
  MechanicalBehaviourDescription::setUsableInPurelyImplicitResolution(const bool b)
  {
    this->usableInPurelyImplicitResolution = b;
  } // end of MechanicalBehaviourDescription::setUsableInPurelyImplicitResolution

  void
  MechanicalBehaviourDescription::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const std::string& n)
  {
    this->pupirv.insert(n);
  } // end of MechanicalBehaviourDescription::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  const std::set<std::string>&
  MechanicalBehaviourDescription::getExternalStateVariablesDeclaredProbablyUnusableInPurelyImplicitResolution(void) const
  {
    return this->pupirv;
  } // end of MechanicalBehaviourDescription::getExternalStateVariablesDeclaredProbablyUnusableInPurelyImplicitResolution


  const std::set<MechanicalBehaviourDescription::Hypothesis>&
  MechanicalBehaviourDescription::getHypotheses(void) const
  {
    return this->hypotheses;
  } // end of MechanicalBehaviourDescription::getHypotheses

  void
  MechanicalBehaviourDescription::addHypothesis(const MechanicalBehaviourDescription::Hypothesis h)
  {
    this->hypotheses.insert(h);
  }

  void
  MechanicalBehaviourDescription::setDefaultHypotheses(void)
  {
    using namespace tfel::material;
    typedef ModellingHypothesis MH;
    static const Hypothesis h[6u] = {MH::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				     MH::AXISYMMETRICAL,
				     MH::PLANESTRAIN,
				     MH::GENERALISEDPLANESTRAIN,
				     MH::TRIDIMENSIONAL};
    this->hypotheses.clear();
    this->hypotheses.insert(h,h+6u);
  } // end of MechanicalBehaviourDescription::setDefaultHypotheses  

} // end of namespace mfront
