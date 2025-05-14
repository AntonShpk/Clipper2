#include <gtest/gtest.h>
#include "clipper2/clipper.h"

TEST(Clipper2Tests, TestCarryCalculation) {
  EXPECT_EQ(Clipper2Lib::Multiply(0x51eaed81157de061, 0x3a271fb2745b6fe9).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0x3a271fb2745b6fe9, 0x51eaed81157de061).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0xc2055706a62883fa, 0x26c78bc79c2322cc).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0x26c78bc79c2322cc, 0xc2055706a62883fa).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0x874ddae32094b0de, 0x9b1559a06fdf83e0).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0x9b1559a06fdf83e0, 0x874ddae32094b0de).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0x81fb3ad3636ca900, 0x239c000a982a8da4).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0x239c000a982a8da4, 0x81fb3ad3636ca900).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0x4be0b4c5d2725c44, 0x990cd6db34a04c30).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0x990cd6db34a04c30, 0x4be0b4c5d2725c44).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0x978ec0c0433c01f6, 0x2df03d097966b536).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0x2df03d097966b536, 0x978ec0c0433c01f6).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0x49c5cbbcfd716344, 0xc489e3b34b007ad3).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0xc489e3b34b007ad3, 0x49c5cbbcfd716344).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0xd3361cdbeed655d5, 0x1240da41e324953a).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0x1240da41e324953a, 0xd3361cdbeed655d5).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0x51b854f8e71b0ae0, 0x6f8d438aae530af5).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0x6f8d438aae530af5, 0x51b854f8e71b0ae0).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0xbbecf7dbc6147480, 0xbb0f73d0f82e2236).hi, 0);
  EXPECT_EQ(Clipper2Lib::Multiply(0xbb0f73d0f82e2236, 0xbbecf7dbc6147480).hi, 0);
}

TEST(Clipper2Tests, TestIsCollinear) {
  // a large integer not representable by double
  const Int128 i = 1329227995784915872903807060280344576_int128_t; //2^120

  const Clipper2Lib::Point64 pt1(0, 0);
  const Clipper2Lib::Point64 sharedPt(i, i * 10);
  const Clipper2Lib::Point64 pt2(i * 10, i * 100);
  const bool isCollinear = IsCollinear(pt1, sharedPt, pt2);
  EXPECT_TRUE(isCollinear);
}

TEST(Clipper2Tests, TestIsCollinear2) {
  // see https://github.com/AngusJohnson/Clipper2/issues/831
  const Int128 i = 42535295865117307932921825928971026432_int128_t; //2^125

  const Clipper2Lib::Path64 subject = {
    Clipper2Lib::Point64(-i, -i),
    Clipper2Lib::Point64( i, -i),
    Clipper2Lib::Point64(-i,  i),
    Clipper2Lib::Point64( i,  i)
  };
  Clipper2Lib::Clipper64 clipper;
  clipper.AddSubject({ subject });
  Clipper2Lib::Paths64 solution;
  clipper.Execute(Clipper2Lib::ClipType::Union, Clipper2Lib::FillRule::EvenOdd, solution);

    const Clipper2Lib::Paths64 expected = {
        { Clipper2Lib::Point64(i, i), Clipper2Lib::Point64(-i, i), Clipper2Lib::Point64(0,  0) },
        { Clipper2Lib::Point64(0, 0), Clipper2Lib::Point64(-i,-i), Clipper2Lib::Point64(i, -i) }
    };
    EXPECT_EQ(expected, solution);
}

TEST(Clipper2Tests, Test2Triangles) {
  const Int128 i = 42535295865117307932921825928971026432_int128_t; //2^125

  const Clipper2Lib::Paths64 subject = {
      {
          Clipper2Lib::Point64( i, 2_int128_t),
          Clipper2Lib::Point64(-i, 2_int128_t),
          Clipper2Lib::Point64( i, 0_int128_t),
      },
      {
          Clipper2Lib::Point64( i, 0_int128_t),
          Clipper2Lib::Point64(-i,-2_int128_t),
          Clipper2Lib::Point64( i,-2_int128_t),
      }
  };
    
  Clipper2Lib::Clipper64 clipper;
  clipper.AddSubject(subject);
  Clipper2Lib::Paths64 solution;
  clipper.Execute(Clipper2Lib::ClipType::Union, Clipper2Lib::FillRule::EvenOdd, solution);
  EXPECT_EQ(solution.size(), 2);
    EXPECT_EQ(subject, solution);
}

TEST(Clipper2Tests, TestTriangleAndRect) {
    const Int128 halfWide = 42535295865117307932921825928971026432_int128_t; //2^125


    const Clipper2Lib::Paths64 subjects = {
    {
        Clipper2Lib::Point64( halfWide, 1_int128_t),
        Clipper2Lib::Point64( halfWide, 5_int128_t),
        Clipper2Lib::Point64( -halfWide, 5_int128_t),
        Clipper2Lib::Point64( -halfWide, 1_int128_t)
    },
    {
        Clipper2Lib::Point64( halfWide - 2, 2_int128_t),
        Clipper2Lib::Point64(-halfWide, -1_int128_t),
        Clipper2Lib::Point64( halfWide - 2, -1_int128_t),
    }};

    Clipper2Lib::Clipper64 clipper;
    clipper.AddSubject(subjects);
    Clipper2Lib::Paths64 solution;
    clipper.Execute(Clipper2Lib::ClipType::Union, Clipper2Lib::FillRule::Positive, solution);


    const Clipper2Lib::Paths64 expected = {
    {
//        Clipper2Lib::Point64( halfWide, height),
//        Clipper2Lib::Point64(-halfWide, height),
        Clipper2Lib::Point64( halfWide, 0_int128_t),
    },
    {
        Clipper2Lib::Point64( halfWide, 0_int128_t),
//        Clipper2Lib::Point64(-halfWide,-height),
//        Clipper2Lib::Point64( halfWide,-height),
    }};
    EXPECT_EQ(solution, expected);
}
