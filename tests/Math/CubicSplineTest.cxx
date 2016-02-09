/*! 
 * \file   tests/Math/CubicSplineTest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  12 avr 2011
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<fstream>
#include<cstdlib>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/CubicSpline.hxx"

struct CubicSplineTest final
  : public tfel::tests::TestCase
{
  CubicSplineTest()
    : tfel::tests::TestCase("TFEL/Math",
			    "CubicSplineTest")
  {} // end of CubicSplineTest

  template<double (*F)(double),
	   double (*f)(double),
	   double (*df)(double)>
  static double computeIntegral(const double xa,
				const double xb)
  {
    using namespace std;
    if(xb<xa){
      return -computeIntegral<F,f,df>(xb,xa);
    }
    if(xa<-1){
      double d=df(-1);
      double y=f(-1);
      double s=y*(-1.-xa)-0.5*d*(xa+1)*(xa+1);
      return s+computeIntegral<F,f,df>(-1.,xb);
    }
    if(xb>1){
      double d=df(1);
      double y=f(1);
      double s=y*(xb-1.)+0.5*d*(xb-1.)*(xb-1.);
      return s+computeIntegral<F,f,df>(xa,1.);
    }
    return F(xb)-F(xa);
  }

  static double msin(double x){
    using namespace std;
    return -sin(x);
  }

  static double mcos(double x){
    using namespace std;
    return -cos(x);
  }

  virtual tfel::tests::TestResult
  execute() override
  {
    using namespace std;
    using namespace tfel::math;
    using std::vector;
    const vector<double>::size_type n = 10;
    vector<double> x(n);
    vector<tvector<2u,double> > y(n);
    vector<double>::size_type i;
    for(i=0;i!=x.size();++i){
      x[i] = -1.+(2./static_cast<double>(x.size()-1))*static_cast<double>(i);
      y[i][0] = cos(x[i]);
      y[i][1] = sin(x[i]);
    }
    CubicSpline<double,tvector<2u,double> > spline;
    spline.setCollocationPoints(x,y);
    ofstream out;
    out.open("CubicSplineTest.out");
    if(!out){
      string msg("CubicSplineTest::execute : ");
      msg += "can't open file 'CubicSplineTest.out'";
      throw(runtime_error(msg));
    }
    out << endl;
    for(i=0;i!=2*x.size();++i){
      double x1 = -1.+(2./static_cast<double>(2*x.size()-1))*static_cast<double>(i);
      tvector<2u> y1;
      y1 = spline.getValue(x1);
      out << x1 << " " << y1[0] << " " << y1[1] << endl;
    }
    const double eps = 1.e-2;
    tvector<2u,double> in1 = spline.computeIntegral(-1,1);
    TFEL_TESTS_ASSERT(abs(in1[0]-computeIntegral<sin,cos,msin>(-1,1))<eps);
    TFEL_TESTS_ASSERT(abs(in1[1]-computeIntegral<mcos,sin,cos>(-1,1))<eps);
    tvector<2u,double> in2 = spline.computeIntegral(-1.25,-1);
    TFEL_TESTS_ASSERT(abs(in2[0]-computeIntegral<sin,cos,msin>(-1.25,-1))<eps);
    TFEL_TESTS_ASSERT(abs(in2[1]-computeIntegral<mcos,sin,cos>(-1.25,-1))<eps);
    tvector<2u,double> in3 = spline.computeIntegral(1.,1.25);
    TFEL_TESTS_ASSERT(abs(in3[0]-computeIntegral<sin,cos,msin>(1.,1.25))<eps);
    TFEL_TESTS_ASSERT(abs(in3[1]-computeIntegral<mcos,sin,cos>(1.,1.25))<eps);
    tvector<2u,double> in4 = spline.computeIntegral(-1.25,1.25);
    TFEL_TESTS_ASSERT(abs(in4[0]-computeIntegral<sin,cos,msin>(-1.25,1.25))<eps);
    TFEL_TESTS_ASSERT(abs(in4[1]-computeIntegral<mcos,sin,cos>(-1.25,1.25))<eps);
    tvector<2u,double> in5 = spline.computeIntegral(-0.015,0.01);
    TFEL_TESTS_ASSERT(abs(in5[0]-computeIntegral<sin,cos,msin>(-0.015,0.01))<eps);
    TFEL_TESTS_ASSERT(abs(in5[1]-computeIntegral<mcos,sin,cos>(-0.015,0.01))<eps);
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(CubicSplineTest,
			  "CubicSplineTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main(void)
{
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("CubicSpline.xml");
  const auto r = manager.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
} // end of main
