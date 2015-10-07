/*!
 * \file   mfront/src/CastemComputeStiffnessTensor.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<algorithm>

#include"TFEL/Math/st2tost2.hxx"
#include"MFront/Castem/CastemComputeStiffnessTensor.hxx"

namespace castem
{

  void
  CastemComputeIsotropicStiffnessTensor1D(tfel::config::Types<1u,CastemReal,false>::StiffnessTensor& C,
					const CastemReal* const props)
  {
    const CastemReal E = props[0];
    const CastemReal n = props[1];
    const CastemReal l = E*n/((1.-2.*n)*(1+n));
    const CastemReal G = E/(1+n);
    const CastemReal C11 = l+G;
    C(0,0)=C11;
    C(0,1)=l;
    C(0,2)=l;
    C(1,0)=l;
    C(1,1)=C11;
    C(1,2)=l;
    C(2,0)=l;
    C(2,1)=l;
    C(2,2)=C11;
  } // end of CastemComputeIsotropicStiffnessTensor1D
 
  void
  CastemComputeIsotropicStiffnessTensor2D(tfel::config::Types<2u,CastemReal,false>::StiffnessTensor& C,
					const CastemReal* const props)
  {
    const CastemReal E   = props[0];
    const CastemReal n   = props[1];
    const CastemReal l   = E*n/((1.-2.*n)*(1+n));
    const CastemReal G   = E/(1+n);
    const CastemReal C11 = l+G;
    C(0,0)=C11;
    C(0,1)=l;
    C(0,2)=l;
    C(0,3)=0.;
    C(1,0)=l;
    C(1,1)=C11;
    C(1,2)=l;
    C(1,3)=0.;
    C(2,0)=l;
    C(2,1)=l;
    C(2,2)=C11;
    C(2,3)=0.;
    C(3,0)=0.;
    C(3,1)=0.;
    C(3,2)=0.;
    C(3,3)=G;    
  }

  void
  CastemComputeIsotropicPlaneStressAlteredStiffnessTensor(tfel::config::Types<2u,CastemReal,false>::StiffnessTensor& C,
							const CastemReal* const props)
					       
  {
    const CastemReal E   = props[0];
    const CastemReal n   = props[1];
    const CastemReal C1  = E/(1-n*n);
    const CastemReal C2  = n*C1;
    const CastemReal C3  = (1-n)*C1;
    C(0,0)=C1;
    C(0,1)=C2;
    C(0,2)=0.;
    C(0,3)=0.;
    C(1,0)=C2;
    C(1,1)=C1;
    C(1,2)=0.;
    C(1,3)=0.;
    C(2,0)=0.;
    C(2,1)=0.;
    C(2,2)=0.;
    C(2,3)=0.;
    C(3,0)=0.;
    C(3,1)=0.;
    C(3,2)=0.;
    C(3,3)=C3;    
  } // end of struct CastemComputeIsotropicPlaneStressAlteredStiffnessTensor

  void
  CastemComputeIsotropicStiffnessTensor3D(tfel::config::Types<3u,CastemReal,false>::StiffnessTensor& C,
					const CastemReal* const props)
  {
    const CastemReal E = props[0];
    const CastemReal n = props[1];
    const CastemReal l = E*n/((1.-2.*n)*(1+n));
    const CastemReal G = E/(1+n);
    const CastemReal C11 = l+G;
    C(0,0)=C11;
    C(0,1)=l;
    C(0,2)=l;
    C(0,3)=0.;
    C(0,4)=0.;
    C(0,5)=0.;
    C(1,0)=l;
    C(1,1)=C11;
    C(1,2)=l;
    C(1,3)=0.;
    C(1,4)=0.;
    C(1,5)=0.;
    C(2,0)=l;
    C(2,1)=l;
    C(2,2)=C11;
    C(2,3)=0.;
    C(2,4)=0.;
    C(2,5)=0.;
    C(3,0)=0.;
    C(3,1)=0.;
    C(3,2)=0.;
    C(3,3)=G;
    C(3,4)=0.;
    C(3,5)=0.;
    C(4,0)=0.;
    C(4,1)=0.;
    C(4,2)=0.;
    C(4,3)=0.;
    C(4,4)=G;
    C(4,5)=0.;
    C(5,0)=0.;
    C(5,1)=0.;
    C(5,2)=0.;
    C(5,3)=0.;
    C(5,4)=0.;
    C(5,5)=G;
  } // end of struct CastemComputeStiffnessTensor

  void
  CastemComputeOrthotropicStiffnessTensor1D(tfel::config::Types<1u,CastemReal,false>::StiffnessTensor& C,
					  const CastemReal* const props)
			       
  {
    using namespace std;
    using namespace std;
    // Matrice de souplesse
    // props[0] :'YG1'
    // props[1] :'YG2'
    // props[2] :'YG3'
    // props[3] :'NU12'
    // props[4] :'NU23'
    // props[5] :'NU13'
    using namespace std;
    // S11 = 1/E1
    const CastemReal S11=1/props[0];
    // S22 = 1/E2
    const CastemReal S22=1/props[1];
    // S22 = 1/E3
    const CastemReal S33=1/props[2];
    // S12 = -n12/E1
    const CastemReal S12=-props[3]/props[0];
    // S13 = -n13/E1
    const CastemReal S13=-props[5]/props[0];
    // S23 = -n23/E2
    const CastemReal S23=-props[4]/props[1];
    const CastemReal inv_detS=1/(S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12);
    // Matrice de raideur
    C(0,0)=(S22*S33-S23*S23)*inv_detS;
    C(0,1)=(S13*S23-S12*S33)*inv_detS;
    C(0,2)=(S12*S23-S13*S22)*inv_detS;
    C(1,0)=(S13*S23-S12*S33)*inv_detS;
    C(1,1)=(S11*S33-S13*S13)*inv_detS;
    C(1,2)=(S12*S13-S11*S23)*inv_detS;
    C(2,0)=(S12*S23-S13*S22)*inv_detS;
    C(2,1)=(S12*S13-S11*S23)*inv_detS;
    C(2,2)=(S11*S22-S12*S12)*inv_detS;
  } // end of struct CastemComputeStiffnessTensor

  void
  CastemComputeOrthotropicStiffnessTensor2D(tfel::config::Types<2u,CastemReal,false>::StiffnessTensor& C,
					  const CastemReal* const props)
  {
    using namespace std;
    // props[0] :'YG1'
    // props[1] :'YG2'
    // props[2] :'YG3'
    // props[3] :'NU12'
    // props[4] :'NU23'
    // props[5] :'NU13'
    // props[6] :'G12'
    using namespace std;
    // S11 = 1/E1
    const CastemReal S11=1/props[0];
    // S22 = 1/E2
    const CastemReal S22=1/props[1];
    // S22 = 1/E3
    const CastemReal S33=1/props[2];
    // S12 = -n12/E1
    const CastemReal S12=-props[3]/props[0];
    // S13 = -n13/E1
    const CastemReal S13=-props[5]/props[0];
    // S23 = -n23/E2
    const CastemReal S23=-props[4]/props[1];
    const CastemReal inv_detS=1/(S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12);
    // Matrice de raideur
    fill(C.begin(),C.end(),CastemReal(0.));
    C(0,0)=(S22*S33-S23*S23)*inv_detS;
    C(0,1)=(S13*S23-S12*S33)*inv_detS;
    C(0,2)=(S12*S23-S13*S22)*inv_detS;
    C(1,0)=(S13*S23-S12*S33)*inv_detS;
    C(1,1)=(S11*S33-S13*S13)*inv_detS;
    C(1,2)=(S12*S13-S11*S23)*inv_detS;
    C(2,0)=(S12*S23-S13*S22)*inv_detS;
    C(2,1)=(S12*S13-S11*S23)*inv_detS;
    C(2,2)=(S11*S22-S12*S12)*inv_detS;
    C(3,3)=2.*props[6];
  } // end of struct CastemComputeStiffnessTensor

  void
  CastemComputeOrthotropicPlaneStressAlteredStiffnessTensor(tfel::config::Types<2u,CastemReal,false>::StiffnessTensor& C,
							  const CastemReal* const props)
  {
    // props[0] :'YG1'
    // props[1] :'YG2'
    // props[2] :'NU12'
    // props[3] :'G12'
    // props[6] :'YG3'
    // props[7] :'NU23'
    // props[8] :'NU13'
    using namespace std;
    // S11 = 1/E1
    const CastemReal S11=1/props[0];
    // S22 = 1/E2
    const CastemReal S22=1/props[1];
    // S22 = 1/E3
    const CastemReal S33=1/props[6];
    // S12 = -n12/E1
    const CastemReal S12=-props[2]/props[0];
    // S13 = -n13/E1
    const CastemReal S13=-props[8]/props[0];
    // S23 = -n23/E2
    const CastemReal S23=-props[7]/props[1];
    const CastemReal inv_detS=1/(S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12);
    // Matrice de raideur
    fill(C.begin(),C.end(),CastemReal(0.));
    const CastemReal C00=(S22*S33-S23*S23)*inv_detS;
    const CastemReal C01=(S13*S23-S12*S33)*inv_detS;
    const CastemReal C02=(S12*S23-S13*S22)*inv_detS;
    const CastemReal C10=(S13*S23-S12*S33)*inv_detS;
    const CastemReal C11=(S11*S33-S13*S13)*inv_detS;
    const CastemReal C12=(S12*S13-S11*S23)*inv_detS;
    const CastemReal C20=(S12*S23-S13*S22)*inv_detS;
    const CastemReal C21=(S12*S13-S11*S23)*inv_detS;
    const CastemReal C22=(S11*S22-S12*S12)*inv_detS;
    const CastemReal tmp20 = C20/C22;
    const CastemReal tmp21 = C21/C22;
    C(0,0)=C00-C02*tmp20;
    C(0,1)=C01-C02*tmp21;
    C(1,0)=C10-C12*tmp20;
    C(1,1)=C11-C12*tmp21;
    C(3,3)=2.*props[3];
  } // end of struct CastemComputeOrthotropicPlaneStressUnAlteredStiffnessTensor

  void
  CastemComputeOrthotropicPlaneStressUnAlteredStiffnessTensor(tfel::config::Types<2u,CastemReal,false>::StiffnessTensor& C,
							  const CastemReal* const props)
  {
    using namespace std;
    // props[0] :'YG1'
    // props[1] :'YG2'
    // props[2] :'NU12'
    // props[3] :'G12'
    // props[6] :'YG3'
    // props[7] :'NU23'
    // props[8] :'NU13'
    using namespace std;
    // S11 = 1/E1
    const CastemReal S11=1/props[0];
    // S22 = 1/E2
    const CastemReal S22=1/props[1];
    // S22 = 1/E3
    const CastemReal S33=1/props[6];
    // S12 = -n12/E1
    const CastemReal S12=-props[2]/props[0];
    // S13 = -n13/E1
    const CastemReal S13=-props[8]/props[0];
    // S23 = -n23/E2
    const CastemReal S23=-props[7]/props[1];
    const CastemReal inv_detS=1/(S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12);
    // Matrice de raideur
    fill(C.begin(),C.end(),CastemReal(0.));
    C(0,0)=(S22*S33-S23*S23)*inv_detS;
    C(0,1)=(S13*S23-S12*S33)*inv_detS;
    C(0,2)=(S12*S23-S13*S22)*inv_detS;
    C(1,0)=(S13*S23-S12*S33)*inv_detS;
    C(1,1)=(S11*S33-S13*S13)*inv_detS;
    C(1,2)=(S12*S13-S11*S23)*inv_detS;
    C(2,0)=(S12*S23-S13*S22)*inv_detS;
    C(2,1)=(S12*S13-S11*S23)*inv_detS;
    C(2,2)=(S11*S22-S12*S12)*inv_detS;
    C(3,3)=2.*props[3];
  } // end of struct CastemComputeOrthotropicPlaneStressUnAlteredStiffnessTensor

  void
  CastemComputeOrthotropicStiffnessTensor3D(tfel::config::Types<3u,CastemReal,false>::StiffnessTensor& C,
					  const CastemReal* const props)
  {
    // props[0] :'YG1'
    // props[1] :'YG2'
    // props[2] :'YG3'
    // props[3] :'NU12'
    // props[4] :'NU23'
    // props[5] :'NU13'
    // props[6] :'G12'
    // props[7] :'G23'
    // props[8] :'G13'
    using namespace std;
    // S11 = 1/E1
    const CastemReal S11=1/props[0];
    // S22 = 1/E2
    const CastemReal S22=1/props[1];
    // S22 = 1/E3
    const CastemReal S33=1/props[2];
    // S12 = -n12/E1
    const CastemReal S12=-props[3]/props[0];
    // S13 = -n13/E1
    const CastemReal S13=-props[5]/props[0];
    // S23 = -n23/E2
    const CastemReal S23=-props[4]/props[1];
    const CastemReal inv_detS=1/(S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12);
    // Matrice de raideur
    fill(C.begin(),C.end(),CastemReal(0.));
    C(0,0)=(S22*S33-S23*S23)*inv_detS;
    C(1,1)=(S11*S33-S13*S13)*inv_detS;
    C(2,2)=(S11*S22-S12*S12)*inv_detS;
    C(0,1)=C(1,0)=(S13*S23-S12*S33)*inv_detS;
    C(0,2)=C(2,0)=(S12*S23-S13*S22)*inv_detS;
    C(1,2)=C(2,1)=(S12*S13-S11*S23)*inv_detS;
    C(3,3)=2.*props[6];
    C(4,4)=2.*props[8];
    C(5,5)=2.*props[7];
  } // end of struct CastemComputeStiffnessTensor

} // end of namespace castem
