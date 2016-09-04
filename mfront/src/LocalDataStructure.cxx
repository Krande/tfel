/*!
 * \file   LocalDataStructure.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   02 mai 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>
#include<algorithm>
#include"MFront/BehaviourDescription.hxx"
#include"MFront/LocalDataStructure.hxx"

namespace mfront{

  LocalDataStructure&
  LocalDataStructure::addVariable(const Hypothesis h,
				  const Variable& v)
  {
    auto add = [&v](std::vector<Variable>& c){
      auto comp = [&v](const Variable& x){
	return x.name==v.name;
      };
      if(std::find_if(c.begin(),c.end(),comp)!=c.end()){
	throw(std::runtime_error("LocalDataStructure::addVariable: "
				 "member '"+v.name+"' multiply defined"));
      }
      c.push_back(v);
    };
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      add(this->uv);
      for(auto& c: this->sv){
	add(c.second);
      }
    } else {
      if(this->sv.find(h)==std::end(this->sv)){
	auto& lv = this->sv[h];
	lv.insert(lv.begin(),this->uv.begin(),this->uv.end());
      }
      add(this->sv[h]);
    }
    return *this;
  } // end of LocalDataStructure::addVariable

  std::vector<LocalDataStructure::Hypothesis>
  LocalDataStructure::getSpecialisedHypotheses() const{
    auto mh = std::vector<Hypothesis>{};
    for(const auto& v:this->sv){
      mh.push_back(v.first);
    }
    return mh;
  }

  const std::vector<LocalDataStructure::Variable>&
  LocalDataStructure::get(const Hypothesis h) const{
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      return this->uv;
    }
    const auto p = this->sv.find(h);
    if(p==std::end(this->sv)){
      throw(std::runtime_error("LocalDataStructure::get: "
			       "no variable defined for hypothesis '"+
			       ModellingHypothesis::toString(h)+"'"));
    }
    return p->second;
  } // end of LocalDataStructure::get

  bool LocalDataStructure::contains(const Hypothesis h,
				    const std::string& n) const{
    const auto& d = this->get(h);
    const auto p = std::find_if(d.begin(),d.end(),[&n](const Variable& v){
	return v.name==n;
      });
    return p!=d.end();
  }
  
  
} // end of mfront
