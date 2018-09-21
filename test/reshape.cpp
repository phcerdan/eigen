// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2017 Gael Guennebaud <gael.guennebaud@inria.fr>
// Copyright (C) 2014 yoco <peter.xiau@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "main.h"

template<typename T1,typename T2>
typename internal::enable_if<internal::is_same<T1,T2>::value,bool>::type
is_same_eq(const T1& a, const T2& b)
{
  return (a.array() == b.array()).all();
}

template <int Order,typename MatType>
void check_auto_reshape4x4(MatType m)
{
  internal::VariableAndFixedInt<MatType::SizeAtCompileTime==Dynamic?-1: 1>  v1( 1);
  internal::VariableAndFixedInt<MatType::SizeAtCompileTime==Dynamic?-1: 2>  v2( 2);
  internal::VariableAndFixedInt<MatType::SizeAtCompileTime==Dynamic?-1: 4>  v4( 4);
  internal::VariableAndFixedInt<MatType::SizeAtCompileTime==Dynamic?-1: 8>  v8( 8);
  internal::VariableAndFixedInt<MatType::SizeAtCompileTime==Dynamic?-1:16> v16(16);

  VERIFY(is_same_eq(m.template reshaped<Order>( 1,       AutoSize), m.template reshaped<Order>( 1, 16)));
  VERIFY(is_same_eq(m.template reshaped<Order>(AutoSize, 16      ), m.template reshaped<Order>( 1, 16)));
  VERIFY(is_same_eq(m.template reshaped<Order>( 2,       AutoSize), m.template reshaped<Order>( 2,  8)));
  VERIFY(is_same_eq(m.template reshaped<Order>(AutoSize, 8       ), m.template reshaped<Order>( 2,  8)));
  VERIFY(is_same_eq(m.template reshaped<Order>( 4,       AutoSize), m.template reshaped<Order>( 4,  4)));
  VERIFY(is_same_eq(m.template reshaped<Order>(AutoSize, 4       ), m.template reshaped<Order>( 4,  4)));
  VERIFY(is_same_eq(m.template reshaped<Order>( 8,       AutoSize), m.template reshaped<Order>( 8,  2)));
  VERIFY(is_same_eq(m.template reshaped<Order>(AutoSize, 2       ), m.template reshaped<Order>( 8,  2)));
  VERIFY(is_same_eq(m.template reshaped<Order>(16,       AutoSize), m.template reshaped<Order>(16,  1)));
  VERIFY(is_same_eq(m.template reshaped<Order>(AutoSize, 1       ), m.template reshaped<Order>(16,  1)));

  VERIFY(is_same_eq(m.template reshaped<Order>(fix< 1>,   AutoSize),  m.template reshaped<Order>(fix< 1>, v16    )));
  VERIFY(is_same_eq(m.template reshaped<Order>(AutoSize,  fix<16> ),  m.template reshaped<Order>( v1,     fix<16>)));
  VERIFY(is_same_eq(m.template reshaped<Order>(fix< 2>,   AutoSize),  m.template reshaped<Order>(fix< 2>, v8     )));
  VERIFY(is_same_eq(m.template reshaped<Order>(AutoSize,  fix< 8> ),  m.template reshaped<Order>( v2,     fix< 8>)));
  VERIFY(is_same_eq(m.template reshaped<Order>(fix< 4>,   AutoSize),  m.template reshaped<Order>(fix< 4>, v4     )));
  VERIFY(is_same_eq(m.template reshaped<Order>(AutoSize,  fix< 4> ),  m.template reshaped<Order>( v4,     fix< 4>)));
  VERIFY(is_same_eq(m.template reshaped<Order>(fix< 8>,   AutoSize),  m.template reshaped<Order>(fix< 8>, v2     )));
  VERIFY(is_same_eq(m.template reshaped<Order>(AutoSize,  fix< 2> ),  m.template reshaped<Order>( v8,     fix< 2>)));
  VERIFY(is_same_eq(m.template reshaped<Order>(fix<16>,   AutoSize),  m.template reshaped<Order>(fix<16>, v1     )));
  VERIFY(is_same_eq(m.template reshaped<Order>(AutoSize,  fix< 1> ),  m.template reshaped<Order>(v16,     fix< 1>)));
}

// just test a 4x4 matrix, enumerate all combination manually
template <typename MatType>
void reshape4x4(MatType m)
{
  internal::VariableAndFixedInt<MatType::SizeAtCompileTime==Dynamic?-1: 1>  v1( 1);
  internal::VariableAndFixedInt<MatType::SizeAtCompileTime==Dynamic?-1: 2>  v2( 2);
  internal::VariableAndFixedInt<MatType::SizeAtCompileTime==Dynamic?-1: 4>  v4( 4);
  internal::VariableAndFixedInt<MatType::SizeAtCompileTime==Dynamic?-1: 8>  v8( 8);
  internal::VariableAndFixedInt<MatType::SizeAtCompileTime==Dynamic?-1:16> v16(16);

  if((MatType::Flags&RowMajorBit)==0)
  {
    typedef Map<MatrixXi> MapMat;
    // dynamic
    VERIFY_IS_EQUAL((m.reshaped( 1, 16)), MapMat(m.data(),  1, 16));
    VERIFY_IS_EQUAL((m.reshaped( 2,  8)), MapMat(m.data(),  2,  8));
    VERIFY_IS_EQUAL((m.reshaped( 4,  4)), MapMat(m.data(),  4,  4));
    VERIFY_IS_EQUAL((m.reshaped( 8,  2)), MapMat(m.data(),  8,  2));
    VERIFY_IS_EQUAL((m.reshaped(16,  1)), MapMat(m.data(), 16,  1));

    // static
    VERIFY_IS_EQUAL(m.reshaped(fix< 1>, fix<16>), MapMat(m.data(),  1, 16));
    VERIFY_IS_EQUAL(m.reshaped(fix< 2>, fix< 8>), MapMat(m.data(),  2,  8));
    VERIFY_IS_EQUAL(m.reshaped(fix< 4>, fix< 4>), MapMat(m.data(),  4,  4));
    VERIFY_IS_EQUAL(m.reshaped(fix< 8>, fix< 2>), MapMat(m.data(),  8,  2));
    VERIFY_IS_EQUAL(m.reshaped(fix<16>, fix< 1>), MapMat(m.data(), 16,  1));


    // reshape chain
    VERIFY_IS_EQUAL(
      (m
      .reshaped( 1, 16)
      .reshaped(fix< 2>,fix< 8>)
      .reshaped(16,  1)
      .reshaped(fix< 8>,fix< 2>)
      .reshaped( 2,  8)
      .reshaped(fix< 1>,fix<16>)
      .reshaped( 4,  4)
      .reshaped(fix<16>,fix< 1>)
      .reshaped( 8,  2)
      .reshaped(fix< 4>,fix< 4>)
      ),
      MapMat(m.data(), 4,  4)
    );
  }

  VERIFY(is_same_eq(m.reshaped( 1,       AutoSize), m.reshaped( 1, 16)));
  VERIFY(is_same_eq(m.reshaped(AutoSize, 16),       m.reshaped( 1, 16)));
  VERIFY(is_same_eq(m.reshaped( 2,       AutoSize), m.reshaped( 2,  8)));
  VERIFY(is_same_eq(m.reshaped(AutoSize, 8),        m.reshaped( 2,  8)));
  VERIFY(is_same_eq(m.reshaped( 4,       AutoSize), m.reshaped( 4,  4)));
  VERIFY(is_same_eq(m.reshaped(AutoSize, 4),        m.reshaped( 4,  4)));
  VERIFY(is_same_eq(m.reshaped( 8,       AutoSize), m.reshaped( 8,  2)));
  VERIFY(is_same_eq(m.reshaped(AutoSize, 2),        m.reshaped( 8,  2)));
  VERIFY(is_same_eq(m.reshaped(16,       AutoSize), m.reshaped(16,  1)));
  VERIFY(is_same_eq(m.reshaped(AutoSize,  1),       m.reshaped(16,  1)));

  VERIFY(is_same_eq(m.reshaped(fix< 1>,   AutoSize),  m.reshaped(fix< 1>, v16)));
  VERIFY(is_same_eq(m.reshaped(AutoSize,  fix<16>),   m.reshaped( v1,     fix<16>)));
  VERIFY(is_same_eq(m.reshaped(fix< 2>,   AutoSize),  m.reshaped(fix< 2>, v8)));
  VERIFY(is_same_eq(m.reshaped(AutoSize,  fix< 8>),   m.reshaped( v2,     fix< 8>)));
  VERIFY(is_same_eq(m.reshaped(fix< 4>,   AutoSize),  m.reshaped(fix< 4>, v4)));
  VERIFY(is_same_eq(m.reshaped(AutoSize,  fix< 4>),   m.reshaped( v4,     fix< 4>)));
  VERIFY(is_same_eq(m.reshaped(fix< 8>,   AutoSize),  m.reshaped(fix< 8>, v2)));
  VERIFY(is_same_eq(m.reshaped(AutoSize,  fix< 2>),   m.reshaped( v8,     fix< 2>)));
  VERIFY(is_same_eq(m.reshaped(fix<16>,   AutoSize),  m.reshaped(fix<16>, v1)));
  VERIFY(is_same_eq(m.reshaped(AutoSize,  fix< 1>),   m.reshaped(v16,     fix< 1>)));

  check_auto_reshape4x4<ColMajor> (m);
  check_auto_reshape4x4<RowMajor> (m);
  check_auto_reshape4x4<AutoOrder>(m);
  check_auto_reshape4x4<ColMajor> (m.transpose());
  check_auto_reshape4x4<ColMajor> (m.transpose());
  check_auto_reshape4x4<AutoOrder>(m.transpose());

  VERIFY_IS_EQUAL(m.reshaped( 1, 16).data(), m.data());
  VERIFY_IS_EQUAL(m.reshaped( 1, 16).innerStride(), 1);

  VERIFY_IS_EQUAL(m.reshaped( 2, 8).data(), m.data());
  VERIFY_IS_EQUAL(m.reshaped( 2, 8).innerStride(), 1);
  VERIFY_IS_EQUAL(m.reshaped( 2, 8).outerStride(), 2);

  if((MatType::Flags&RowMajorBit)==0)
  {
    VERIFY_IS_EQUAL(m.template reshaped<ColMajor>(2,8),m.reshaped(2,8));
    VERIFY_IS_EQUAL(m.template reshaped<ColMajor>(2,8),m.template reshaped<AutoOrder>(2,8));
    VERIFY_IS_EQUAL(m.transpose().template reshaped<RowMajor>(2,8),m.transpose().template reshaped<AutoOrder>(2,8));
  }
  else
  {
    VERIFY_IS_EQUAL(m.template reshaped<ColMajor>(2,8),m.reshaped(2,8));
    VERIFY_IS_EQUAL(m.template reshaped<RowMajor>(2,8),m.template reshaped<AutoOrder>(2,8));
    VERIFY_IS_EQUAL(m.transpose().template reshaped<ColMajor>(2,8),m.transpose().template reshaped<AutoOrder>(2,8));
    VERIFY_IS_EQUAL(m.transpose().reshaped(2,8),m.transpose().template reshaped<AutoOrder>(2,8));
  }

  MatrixXi m28r1 = m.template reshaped<RowMajor>(2,8);
  MatrixXi m28r2 = m.transpose().template reshaped<ColMajor>(8,2).transpose();
  VERIFY_IS_EQUAL( m28r1, m28r2);

  using placeholders::all;
  VERIFY(is_same_eq(m.reshaped(v16,fix<1>), m(all)));
  VERIFY_IS_EQUAL(m.reshaped(16,1), m(all));
  VERIFY_IS_EQUAL(m.reshaped(1,16), m(all).transpose());
  VERIFY_IS_EQUAL(m(all).reshaped(2,8), m.reshaped(2,8));
  VERIFY_IS_EQUAL(m(all).reshaped(4,4), m.reshaped(4,4));
  VERIFY_IS_EQUAL(m(all).reshaped(8,2), m.reshaped(8,2));

  VERIFY(is_same_eq(m.reshaped(AutoSize,fix<1>), m(all)));
  VERIFY_IS_EQUAL(m.template reshaped<RowMajor>(fix<1>,AutoSize), m.transpose()(all).transpose());
}

void test_reshape()
{
  typedef Matrix<int,Dynamic,Dynamic> RowMatrixXi;
  typedef Matrix<int,4,4> RowMatrix4i;
  MatrixXi mx = MatrixXi::Random(4, 4);
  Matrix4i m4 = Matrix4i::Random(4, 4);
  RowMatrixXi rmx = RowMatrixXi::Random(4, 4);
  RowMatrix4i rm4 = RowMatrix4i::Random(4, 4);

  // test dynamic-size matrix
  CALL_SUBTEST(reshape4x4(mx));
  // test static-size matrix
  CALL_SUBTEST(reshape4x4(m4));
  // test dynamic-size const matrix
  CALL_SUBTEST(reshape4x4(static_cast<const MatrixXi>(mx)));
  // test static-size const matrix
  CALL_SUBTEST(reshape4x4(static_cast<const Matrix4i>(m4)));

  CALL_SUBTEST(reshape4x4(rmx));
  CALL_SUBTEST(reshape4x4(rm4));
}