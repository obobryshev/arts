/* Copyright (C) 2015 Richard Larsson <ric.larsson@gmail.com>
 * 
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2, or (at your option) any
 *  later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 *  USA. */

/*!
 * \file   test_propagationmatrix.cc
 * \author <ric.larsson@gmail.com>
 * \date   2015-10-20
 * 
 * \brief  Test Propagation Matrix Internal Partial Derivatives and PropagationMatrix
 */

#include "absorption.h"
#include "arts.h"
#include "global_data.h"
#include "hitran_species.h"
#include "linescaling.h"
#include "lineshapemodel.h"
#include "matpack_eigen.h"
#include "predefined_absorption_models.h"
#include "transmissionmatrix.h"
#include "wigner_functions.h"
#include "zeeman.h"
#include "zeemandata.h"
#include <Faddeeva/Faddeeva.hh>
#include <random>

#include "linemixing_hitran.h"
#include <auto_md.h>

void test_matrix_buildup() {
  const Numeric k11 = 1;
  const Numeric k12 = -0.51;
  const Numeric k13 = -0.21;
  const Numeric k14 = 0.31;
  const Numeric k23 = -0.1;
  const Numeric k24 = -0.99;
  const Numeric k34 = 2;

  const Numeric r = 0.5;

  const Numeric a = -k11 * r;
  const Numeric b = -k12 * r;
  const Numeric c = -k13 * r;
  const Numeric d = -k14 * r;
  const Numeric u = -k23 * r;
  const Numeric v = -k24 * r;
  const Numeric w = -k34 * r;

  const Numeric b2 = b * b, c2 = c * c, d2 = d * d, u2 = u * u, v2 = v * v,
                w2 = w * w;

  const Numeric Const2 = b2 + c2 + d2 - u2 - v2 - w2;

  Numeric Const1;
  Const1 = b2 * (b2 * 0.5 + c2 + d2 - u2 - v2 + w2);
  Const1 += c2 * (c2 * 0.5 + d2 - u2 + v2 - w2);
  Const1 += d2 * (d2 * 0.5 + u2 - v2 - w2);
  Const1 += u2 * (u2 * 0.5 + v2 + w2);
  Const1 += v2 * (v2 * 0.5 + w2);
  Const1 *= 2;
  Const1 += 8 * (b * d * u * w - b * c * v * w - c * d * u * v);
  Const1 += w2 * w2;

  if (Const1 > 0.0)
    Const1 = sqrt(Const1);
  else
    Const1 = 0.0;

  const Complex sqrt_BpA = sqrt(Complex(Const2 + Const1, 0.0));
  const Complex sqrt_BmA = sqrt(Complex(Const2 - Const1, 0.0));
  const Numeric x = sqrt_BpA.real() * sqrt(0.5);
  const Numeric y = sqrt_BmA.imag() * sqrt(0.5);
  const Numeric x2 = x * x;
  const Numeric y2 = y * y;
  const Numeric cos_y = cos(y);
  const Numeric sin_y = sin(y);
  const Numeric cosh_x = cosh(x);
  const Numeric sinh_x = sinh(x);
  const Numeric x2y2 = x2 + y2;
  const Numeric inv_x2y2 = 1.0 / x2y2;

  std::cout << x << " " << y << " " << Const1 << " " << Const2 << "\n";

  Numeric C0, C1, C2, C3;
  Numeric inv_y = 0.0, inv_x = 0.0;  // Init'd to remove warnings

  // X and Y cannot both be zero
  if (x == 0.0) {
    inv_y = 1.0 / y;
    C0 = 1.0;
    C1 = 1.0;
    C2 = (1.0 - cos_y) * inv_x2y2;
    C3 = (1.0 - sin_y * inv_y) * inv_x2y2;
  } else if (y == 0.0) {
    inv_x = 1.0 / x;
    C0 = 1.0;
    C1 = 1.0;
    C2 = (cosh_x - 1.0) * inv_x2y2;
    C3 = (sinh_x * inv_x - 1.0) * inv_x2y2;
  } else {
    inv_x = 1.0 / x;
    inv_y = 1.0 / y;

    C0 = (cos_y * x2 + cosh_x * y2) * inv_x2y2;
    C1 = (sin_y * x2 * inv_y + sinh_x * y2 * inv_x) * inv_x2y2;
    C2 = (cosh_x - cos_y) * inv_x2y2;
    C3 = (sinh_x * inv_x - sin_y * inv_y) * inv_x2y2;
  }

  std::cout << C0 << " " << C1 << " " << C2 << " " << C3 << "\n";

  Matrix F(4, 4, 0), A(4, 4, 0);

  auto eigF = matpack::eigen::mat(F);
  Eigen::Matrix4d eigA;
  eigA << 0, b, c, d, b, 0, u, v, c, -u, 0, w, d, -v, -w, 0;

  eigF = C1 * eigA + C2 * eigA * eigA + C3 * eigA * eigA * eigA;
  eigF(0, 0) += C0;
  eigF(1, 1) += C0;
  eigF(2, 2) += C0;
  eigF(3, 3) += C0;
  eigF *= exp(a);

  std::cout << F << "\n";
}

void test_transmissionmatrix() {
  // Initializes as unity matrices
  TransmissionMatrix a(2, 4);
  std::cout << "Initialized TransmissionMatrix(2, 4):\n" << a << "\n";

  // Set a single input
  Eigen::Matrix4d A;
  A << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16;
  std::cout << "New Matrix:\n" << A << "\n\n";
  a.Mat4(0) = A;
  std::cout << "Updated TransmissionMatrix Position 1 wit New Matrix:\n"
            << a << "\n";

  // The stream can also set the values
  String S =
      "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 125 26 27 28 29 30 31 32";
  std::cout << "Stream:\n" << S << "\n\n";
  std::istringstream astream(S);
  astream >> a;
  std::cout << "Streamed into TransmissionMatrix:\n" << a << "\n";

  // Initialize empty
  RadiationVector b(3, 3);
  std::cout << "Initialized RadiationVector(3, 3)\n" << b << "\n";

  // Set is not defined but add is
  Eigen::Vector3d B;
  B << 1, 2, 3;
  std::cout << "New Vector:\n" << B << "\n\n";  // nb. not transposed
  b.Vec3(1).noalias() += B;
  std::cout << "Updated RadiationVector Position 1 with New Vector:\n"
            << b << "\n";

  // The stream can also set the values
  String T = "1 2 3 4 5 6 7 8 90";
  std::cout << "Stream:\n" << T << "\n\n";
  std::istringstream bstream(T);
  bstream >> b;
  std::cout << "Streamed into RadiationVector:\n" << b << "\n";
}

void test_r_deriv_propagationmatrix() {
  auto f = [](Numeric x) { return 0.1 * x; };
  auto df = [](Numeric x) { return 0.1 + 0 * x; };

  Index nstokes = 4;
  const Numeric x1 = 30;
  const Numeric x2 = -0.1;
  const Numeric r_normal = 1000;
  const Numeric r_extra1 = r_normal + f(x1);
  const Numeric r_extra2 = r_normal + f(x2);

  PropagationMatrix a(1, nstokes);
  a.Kjj() = 10 + Numeric(rand() % 1000) / 100;
  if (nstokes > 1) {
    a.K12() = 2 + Numeric(rand() % 1000) / 100;
    if (nstokes > 2) {
      a.K13() = 3 + Numeric(rand() % 1000) / 100;
      a.K23() = -1 + Numeric(rand() % 1000) / 100;
      if (nstokes > 3) {
        a.K14() = 5 + Numeric(rand() % 1000) / 100;
        a.K24() = -3 + Numeric(rand() % 1000) / 100;
        a.K34() = -2 + Numeric(rand() % 1000) / 100;
      }
    }
  }
  a.Data() *= 1e-5;

  PropagationMatrix b(1, nstokes);
  b.Kjj() = 5 + Numeric(rand() % 1000) / 100;
  if (nstokes > 1) {
    b.K12() = -1 + Numeric(rand() % 1000) / 100;
    if (nstokes > 2) {
      b.K13() = -3 + Numeric(rand() % 1000) / 100;
      b.K23() = 4 + Numeric(rand() % 1000) / 100;
      if (nstokes > 3) {
        b.K14() = 2 + Numeric(rand() % 1000) / 100;
        b.K24() = -1 + Numeric(rand() % 1000) / 100;
        b.K34() = 3 + Numeric(rand() % 1000) / 100;
      }
    }
  }
  b.Data() *= 5e-6;

  ArrayOfPropagationMatrix da(1, PropagationMatrix(1, nstokes));
  da[0].Data() = 0;
  Tensor3 T_normal(1, nstokes, nstokes), T_extra(1, nstokes, nstokes);
  Tensor4 dT1(1, 1, nstokes, nstokes), dT2(1, 1, nstokes, nstokes);

  compute_transmission_matrix_and_derivative(
      T_normal, dT1, dT2, r_normal, a, b, da, da, df(x1), df(x2), 0);

  std::cout << "Transmission at r=" << r_normal << ":\n"
            << matpack::eigen::mat(T_normal(0, joker, joker)) << "\n"
            << "\n";
  std::cout << "First derivative:\n"
            << matpack::eigen::mat(dT1(0, 0, joker, joker)) << "\n"
            << "\n";
  std::cout << "Second derivative:\n"
            << matpack::eigen::mat(dT2(0, 0, joker, joker)) << "\n"
            << "\n";

  compute_transmission_matrix(T_extra, r_extra1, a, b);

  std::cout << "Transmission at perturbed r1=" << r_extra1 << ":\n"
            << matpack::eigen::mat(T_extra(0, joker, joker)) << "\n"
            << "\n";
  T_extra -= T_normal;
  T_extra /= x1;
  std::cout << "First derivative perturbed:\n"
            << matpack::eigen::mat(T_extra(0, joker, joker)) << "\n"
            << "\n";
  T_extra /= dT1(0, joker, joker, joker);
  std::cout << "First derivative perturbed relative:\n"
            << matpack::eigen::mat(T_extra(0, joker, joker)) << "\n"
            << "\n";

  compute_transmission_matrix(T_extra, r_extra2, a, b);

  std::cout << "Transmission at perturbed r2=" << r_extra2 << ":\n"
            << matpack::eigen::mat(T_extra(0, joker, joker)) << "\n"
            << "\n";
  T_extra -= T_normal;
  T_extra /= x2;
  std::cout << "Second derivative perturbed:\n"
            << matpack::eigen::mat(T_extra(0, joker, joker)) << "\n"
            << "\n";
  T_extra /= dT2(0, joker, joker, joker);
  std::cout << "Second derivative perturbed relative:\n"
            << matpack::eigen::mat(T_extra(0, joker, joker)) << "\n"
            << "\n";
}

void test_transmat_from_propmat() {
  const Numeric a = 2;
  const Numeric b = 3;
  const Numeric c = 4;
  const Numeric d = 1;
  const Numeric u = 5;
  const Numeric v = 1;
  const Numeric w = 5;

  PropagationMatrix test1(1, 1);
  PropagationMatrix test2(1, 2);
  PropagationMatrix test3(1, 3);
  PropagationMatrix test4(1, 4);
  test1.Kjj() = a;
  test2.Kjj() = a;
  test3.Kjj() = a;
  test4.Kjj() = a;
  test2.K12() = b;
  test3.K12() = b;
  test4.K12() = b;
  test3.K13() = c;
  test4.K13() = c;
  test3.K23() = u;
  test4.K23() = u;
  test4.K14() = d;
  test4.K24() = v;
  test4.K34() = w;

  std::cout << test1 << "\n\n"
            << test2 << "\n\n"
            << test3 << "\n\n"
            << test4 << "\n\n";

  TransmissionMatrix ans1(1, 1);
  TransmissionMatrix ans2(1, 2);
  TransmissionMatrix ans3(1, 3);
  TransmissionMatrix ans4(1, 4);

  ArrayOfTransmissionMatrix empty(0);

  stepwise_transmission(ans1,
                        empty,
                        empty,
                        test1,
                        test1,
                        ArrayOfPropagationMatrix(0),
                        ArrayOfPropagationMatrix(0),
                        1,
                        0,
                        0,
                        -1);

  stepwise_transmission(ans2,
                        empty,
                        empty,
                        test2,
                        test2,
                        ArrayOfPropagationMatrix(0),
                        ArrayOfPropagationMatrix(0),
                        1,
                        0,
                        0,
                        -1);

  stepwise_transmission(ans3,
                        empty,
                        empty,
                        test3,
                        test3,
                        ArrayOfPropagationMatrix(0),
                        ArrayOfPropagationMatrix(0),
                        1,
                        0,
                        0,
                        -1);

  stepwise_transmission(ans4,
                        empty,
                        empty,
                        test4,
                        test4,
                        ArrayOfPropagationMatrix(0),
                        ArrayOfPropagationMatrix(0),
                        1,
                        0,
                        0,
                        -1);

  std::cout << ans1 << "\n\n"
            << ans2 << "\n\n"
            << ans3 << "\n\n"
            << ans4 << "\n\n";
}

void test_transmat_to_cumulativetransmat() {
  int i = 1;
  ArrayOfPropagationMatrix propmats(5, PropagationMatrix(1, 4));
  for (auto& pm : propmats) {
    pm.K12() = i;
    i++;
    pm.K13() = i;
    i++;
    pm.K23() = i;
    i++;
    pm.K14() = i;
    i++;
    pm.K24() = i;
    i++;
    pm.K34() = i;
    i++;
    pm.Kjj() = 2 * i;
    i++;
    pm.Data() *= 1e-2;
  }

  std::cout << "Propmats:\n" << propmats << "\n\n";

  ArrayOfTransmissionMatrix layers(5, TransmissionMatrix(1, 4));
  ArrayOfTransmissionMatrix empty(0);
  for (i = 0; i < 4; i++) {
    stepwise_transmission(layers[i + 1],
                          empty,
                          empty,
                          propmats[i],
                          propmats[i + 1],
                          ArrayOfPropagationMatrix(0),
                          ArrayOfPropagationMatrix(0),
                          1,
                          0,
                          0,
                          -1);
  }

  std::cout << "Layers:\n" << layers << "\n\n";

  ArrayOfTransmissionMatrix cumulative_forward =
      cumulative_transmission(layers, CumulativeTransmission::Forward);
  ArrayOfTransmissionMatrix cumulative_reflect =
      cumulative_transmission(layers, CumulativeTransmission::Reverse);

  std::cout << "Forward accumulation:\n" << cumulative_forward << "\n\n";
  std::cout << "Reflect accumulation:\n" << cumulative_reflect << "\n\n";
}

void test_sinc_likes_0limit() {
  Numeric start = 1.0;
  Numeric end = 1e-7;
  int n = 10000;

  Vector x(n), sx(n), shx(n), cx(n), chx(n);
  nlogspace(x, start, end, n);

  for (int i = 0; i < n; i++) {
    sx[i] = std::sin(x[i]);
    cx[i] = std::cos(x[i]);
    shx[i] = std::sinh(x[i]);
    chx[i] = std::cosh(x[i]);
  }

  std::cout
      << std::scientific << std::setprecision(15)
      << "x\tabs(sx/x-1)\tabs(shx/x-1)\tabs((chx-cx)/2x^2-1/2)\tabs((shx/x-sx/x)/2x^2-1/6)\n";

  for (int i = 0; i < n; i++)
    std::cout << x[i] << '\t' << std::abs(sx[i] / x[i] - 1.0) << '\t'
              << std::abs(shx[i] / x[i] - 1.0) << '\t'
              << std::abs((chx[i] - cx[i]) / (2 * x[i] * x[i]) - 0.5) << '\t'
              << std::abs((shx[i] / x[i] - sx[i] / x[i]) / (2 * x[i] * x[i]) -
                          1.0 / 6.0)
              << '\n';
}


void test_mpm20()
{
  // Test constants
  constexpr Index nf = 501;
  constexpr Numeric fstart = 25e9;
  constexpr Numeric fend = 165e9;
  constexpr Numeric t = 200;
  constexpr Numeric p = 1e4;
  Vector f(nf);
  nlinspace(f, fstart, fend, nf);
  PropagationMatrix xsec(nf, 1);
  ArrayOfPropagationMatrix dxsec(2, PropagationMatrix(nf, 1));

  ArrayOfRetrievalQuantity jacs(2);
  jacs[0].Target(Jacobian::Target(Jacobian::Atm::Temperature));
  jacs[1].Target(Jacobian::Target(Jacobian::Atm::WindU));
  Absorption::PredefinedModel::VMRS vmrs_predef;
  vmrs_predef.O2 = 1;
  Absorption::PredefinedModel::compute(
      xsec,
      dxsec,
      Species::Isotopologues[Species::find_species_index("O2", "MPM2020")],
      f,
      p,
      t,
      vmrs_predef,
      jacs,
      {});

  constexpr auto df = 1000;
  constexpr auto dt = 0.1;
  Matrix pxsec(nf, 1, 0);
  Matrix pxsec_dt(nf, 1, 0);
  Matrix pxsec_df(nf, 1, 0);
  Vector f_pert = f;
  f_pert += df;
  
  std::cout<< "xr = np.array([";
  for (Index i=0; i<nf; i++)
    std::cout<<'['<<xsec(i, 0)<<','<<' '<<pxsec(i, 0) << ']' << ',' << ' ';
  std::cout << ']' << ')' << '\n';
  
  std::cout<< "dxr_dt = np.array([";
  for (Index i=0; i<nf; i++)
    std::cout<<'['<<dxsec[0](i, 0)<<','<<' '<<(pxsec_dt(i, 0)-pxsec(i, 0))/dt << ']' << ',' << ' ';
  std::cout << ']' << ')' << '\n';
  
  std::cout<< "dxr_df = np.array([";
  for (Index i=0; i<nf; i++)
    std::cout<<'['<<dxsec[1](i, 0)<<','<<' '<<(pxsec_df(i, 0)-pxsec(i, 0))/df << ']' << ',' << ' ';
  std::cout << ']' << ')' << '\n';
}

void test_hitran2017(bool newtest = true)
{
  const Numeric p = 1.0;
  const Numeric t = 296;
  const Numeric xco2 = 1.5e-2;
  const Numeric xh2o = 0;
  const Numeric sigmin = 600;
  const Numeric sigmax = 900;
  const Numeric dsig = 0.005;
  const Numeric stotmax = 0.1e-21;
  
  const Index nsig = Index(((sigmax - sigmin) / dsig) + 0.5) + 1;
  Vector invcm_grid(nsig);
  Vector f_grid(nsig);
  Numeric sigc = sigmin-dsig;
  for (Index isig=0; isig<nsig; isig++) {
    sigc += dsig;
    invcm_grid[isig] = sigc;
    f_grid[isig] = Conversion::kaycm2freq(sigc);
  }
  
  constexpr Index n=6;
  auto types = 
  std::array<std::pair<lm_hitran_2017::ModeOfLineMixing, lm_hitran_2017::calctype>, 6>{
    std::pair<lm_hitran_2017::ModeOfLineMixing, lm_hitran_2017::calctype>{lm_hitran_2017::ModeOfLineMixing::FullW, lm_hitran_2017::calctype::FullW},
    {lm_hitran_2017::ModeOfLineMixing::VP_W, lm_hitran_2017::calctype::FullW},
    {lm_hitran_2017::ModeOfLineMixing::VP, lm_hitran_2017::calctype::NoneVP},
    {lm_hitran_2017::ModeOfLineMixing::VP_Y, lm_hitran_2017::calctype::NoneRosenkranz},
    {lm_hitran_2017::ModeOfLineMixing::SDVP, lm_hitran_2017::calctype::SDVP},
    {lm_hitran_2017::ModeOfLineMixing::SDVP_Y, lm_hitran_2017::calctype::SDRosenkranz}};
  
  ArrayOfVector absorption(n);
  make_wigner_ready(int(250), int(20000000), 6);
  
  ArrayOfAbsorptionLines bands;
  HitranRelaxationMatrixData hitran;
  for (Index i=0;i<n; i++) {
    auto type=types[i];
    
    lm_hitran_2017::read(hitran, bands, Hitran::isotopologue_ratios(), "data_new", -1, Conversion::kaycm2freq(sigmin), Conversion::kaycm2freq(sigmax), Conversion::kaycm_per_cmsquared2hz_per_msquared(stotmax), type.first);
    Vector vmrs = {1-xco2/100-xh2o/100, xh2o/100, xco2/100};
    
    if (not newtest)
      absorption[i] = lm_hitran_2017::compute(p, t, xco2, xh2o, invcm_grid, stotmax, type.second);
    else
      absorption[i] = lm_hitran_2017::compute(hitran, bands, Hitran::isotopologue_ratios(), Conversion::atm2pa(p), t, vmrs, f_grid);
  }
  
  for (Index i=0; i<nsig; i++) {
    for (Index j=0; j<n; j++) {
      std::cout<<absorption[j][i]<<' ';
    }
    std::cout<<'\n';
  }
}
    

int main(int n, char **argc) {
  /*test_speed_of_pressurebroadening();
    test_transmissionmatrix();
    test_r_deriv_propagationmatrix();
    test_transmat_from_propmat();
    test_transmat_to_cumulativetransmat();
    test_sinc_likes_0limit();*/
  
  if (n == 2 and String(argc[1]) == "new") {
    std::cout<<"new test\n";
    test_hitran2017(true);
  }
  else {
    std::cout<<"old test\n";
    test_hitran2017(false);
  }
  return 0;
}
