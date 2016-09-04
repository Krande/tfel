/*! 
 * \file  bindings/python/tfel/utilities.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 ao� 2010
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<boost/python.hpp>

void declareTextData();

BOOST_PYTHON_MODULE(utilities)
{
  declareTextData();
}
