/*! 
 * \file  Expr.cxx
 * \brief
 * \author Helfer Thomas
 * \date   19 févr. 2015
 */

#include<cstdlib>
#include<iostream>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<type_traits>
#include"TFEL/Math/tensor.hxx"

struct Expr9Test final
  : public tfel::tests::TestCase
{
  Expr9Test()
    : tfel::tests::TestCase("TFEL/Math","Expr9Test")
  {} // end of Expr9Test()
  virtual tfel::tests::TestResult
  execute() override
  {
    using namespace std;
    using namespace tfel::meta;
    using namespace tfel::math;
    using expr = Expr<t2tot2<2u,double>,TensorProductLeftDerivativeExpr<2u>>;
    TFEL_TESTS_STATIC_ASSERT((Implements<expr,T2toT2Concept>::cond));
    TFEL_TESTS_STATIC_ASSERT((std::is_same<T2toT2Type<expr>::type,
					   t2tot2<2u,double>>::value));
    TFEL_TESTS_STATIC_ASSERT((T2toT2Traits<T2toT2Type<expr>::type>::dime==2u));
    TFEL_TESTS_STATIC_ASSERT((T2toT2Traits<expr>::dime==2u));
    using expr2 = BinaryOperationHandler<tensor<1u, double>&,
					  tensor<1u, double>,OpMult>;
    tensor<1u,double> t1;
    tensor<1u,double> t2;
    expr2 e(t1,t2);
    return this->result;
  }
  virtual ~Expr9Test() = default;
};

TFEL_TESTS_GENERATE_PROXY(Expr9Test,"Expr9Test");

/* coverity [UNCAUGHT_EXCEPT]*/
int main()
{
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("ResultOf.xml");
  return manager.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
} // end of main


