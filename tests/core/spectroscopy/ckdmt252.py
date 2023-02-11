import pyarts
import numpy as np

f = np.linspace(1e9, 300e12, 101)

x = pyarts.arts.predef.get_co2_ckdmt252(f, 1e4, 250, 400e-6)

x_ref = np.array([4.17401029e-22, 1.15328319e-14, 2.00414322e-13, 1.82230910e-12,
                  1.57748149e-11, 2.41863048e-10, 1.92216391e-07, 1.78175584e-06,
                  3.16142672e-09, 3.19233617e-10, 1.62167254e-10, 2.13370407e-10,
                  2.32639859e-11, 1.68303638e-11, 1.51915115e-11, 1.72146410e-11,
                  2.41820579e-11, 4.08692996e-11, 8.35729570e-11, 7.40967482e-10,
                  6.88601237e-10, 3.41206787e-09, 2.15132409e-08, 1.06592604e-05,
                  9.66800853e-07, 1.81850218e-08, 2.26945153e-09, 4.88285604e-10,
                  1.52692106e-10, 6.32611905e-11, 3.08247184e-11, 1.79627959e-11,
                  2.28784044e-11, 4.64161682e-11, 7.56850632e-11, 6.58895109e-09,
                  1.23230632e-06, 1.79721850e-06, 2.04999832e-09, 1.03142780e-10,
                  2.56222707e-11, 6.59008248e-12, 2.53698344e-12, 1.04205581e-12,
                  4.02209110e-13, 1.52903674e-13, 2.70401005e-12, 1.52474472e-11,
                  1.46004398e-09, 7.87548651e-10, 1.99048512e-08, 1.49460836e-08,
                  1.09991142e-11, 7.15046928e-11, 1.10954815e-14, 2.70439181e-15,
                  2.28191906e-13, 3.52132069e-13, 4.85054263e-16, 1.40252190e-15,
                  7.26182973e-12, 3.05367652e-11, 3.94926704e-10, 8.97443357e-11,
                  4.99717369e-12, 6.79619307e-11, 4.88848840e-15, 1.87747148e-12,
                  8.65186544e-12, 1.31298278e-10, 2.42505166e-10, 1.31584712e-13,
                  1.99542285e-15, 1.81333127e-13, 5.80074279e-13, 8.11337001e-14,
                  1.16903096e-11, 1.55213697e-12, 5.03800152e-16, 5.81926805e-14,
                  3.38806692e-14, 3.16732340e-12, 4.42583078e-11, 6.65719569e-11,
                  1.66840390e-15, 1.35852010e-16, 2.56059389e-17, 7.05328242e-18,
                  2.42778689e-18, 9.99663375e-19, 5.49077249e-19, 6.67009569e-19,
                  2.69045083e-18, 5.80839704e-17, 2.75464790e-13, 3.03369383e-12,
                  5.98102978e-13, 1.07464961e-16, 4.87705451e-18, 8.13188442e-19,
                  0.00000000e+00])

assert np.allclose(x, x_ref), "CO2-CKDMT252"

f = np.linspace(400e12, 1000e12, 101)

x = pyarts.arts.predef.get_o2_vis_ckdmt252(f, 1e4, 250, 0.21)

x_ref = np.array([0.00000000e+00,  0.00000000e+00,  0.00000000e+00,  0.00000000e+00,
                  0.00000000e+00,  0.00000000e+00,  0.00000000e+00,  0.00000000e+00,
                  0.00000000e+00,  1.13697807e-09,  2.19587600e-09,  1.60985165e-08,
                  9.09448572e-08,  1.11564762e-07,  3.82757424e-08,  1.21627036e-08,
                  4.23853121e-09,  3.17889841e-09,  9.06745575e-09,  6.63727699e-08,
                  1.91307545e-07,  9.60434407e-08,  2.98889657e-08,  1.20871030e-08,
                  7.75500731e-09,  7.41742962e-09,  9.51264190e-09,  2.43361444e-08,
                  1.75177090e-08,  6.33740168e-09,  3.17889841e-09,  2.11926561e-09,
                  1.05963280e-09,  1.05963280e-09,  2.11926561e-09,  1.05963280e-09,
                  2.11926561e-09,  2.78380929e-08,  1.13510095e-07,  3.94397845e-08,
                  1.06468735e-08,  3.31554952e-09,  2.11926561e-09,  2.11926561e-09,
                  3.18718333e-09,  1.06031006e-08,  5.29816402e-09,  2.11702716e-09,
                  2.11926561e-09,  2.00308665e-09,  1.05963280e-09,  1.05963280e-09,
                  1.05963280e-09,  1.05963280e-09,  0.00000000e+00,  0.00000000e+00,
                  -6.92554180e-14,  1.05963280e-09,  1.05963280e-09,  0.00000000e+00,
                  0.00000000e+00,  6.51973170e-10,  1.05963280e-09,  2.98605273e-09,
                  2.10948489e-08,  3.93769482e-08,  8.13274819e-09,  1.66009493e-09,
                  1.05963280e-09,  1.05963280e-09,  3.65005545e-09,  2.79263167e-08,
                  7.36367616e-08,  3.58593948e-08,  1.22337380e-08,  6.35779682e-09,
                  4.26644743e-09,  4.10760361e-09,  1.31384493e-08,  2.11537656e-08,
                  1.00476240e-08,  5.29816402e-09,  4.23853121e-09,  0.00000000e+00,
                  0.00000000e+00,  0.00000000e+00,  0.00000000e+00,  0.00000000e+00,
                  0.00000000e+00,  0.00000000e+00,  0.00000000e+00,  0.00000000e+00,
                  0.00000000e+00,  0.00000000e+00,  0.00000000e+00,  0.00000000e+00,
                  0.00000000e+00,  0.00000000e+00,  0.00000000e+00,  0.00000000e+00,
                  0.00000000e+00])

assert np.allclose(x, x_ref), "O2-visCKDMT252"

f = pyarts.arts.convert.kaycm2freq(np.linspace(2000, 2750, 101))

x = pyarts.arts.predef.get_n2_fun_ckdmt252(f, 1e4, 250, 0.79, 5e-3, 0.21)

x_ref = np.array([0.00000000e+00, 0.00000000e+00, 0.00000000e+00, 0.00000000e+00,
                  0.00000000e+00, 0.00000000e+00, 0.00000000e+00, 6.77285356e-11,
                  7.61217516e-10, 2.12778818e-09, 4.10134702e-09, 6.65125012e-09,
                  9.79856776e-09, 1.35996680e-08, 1.82365910e-08, 2.38286740e-08,
                  3.06370026e-08, 3.89786937e-08, 4.92267359e-08, 6.18633211e-08,
                  7.73214188e-08, 9.63928222e-08, 1.19401659e-07, 1.47065395e-07,
                  1.80228074e-07, 2.20020012e-07, 2.66981009e-07, 3.19879248e-07,
                  3.78211205e-07, 4.42321651e-07, 5.08997264e-07, 5.78191739e-07,
                  6.45536463e-07, 7.08966534e-07, 7.66279230e-07, 8.15732780e-07,
                  8.60568063e-07, 9.00506085e-07, 9.52319412e-07, 1.03047250e-06,
                  1.17371078e-06, 1.42204486e-06, 1.72365733e-06, 1.95341089e-06,
                  1.91767336e-06, 1.71578136e-06, 1.45745734e-06, 1.35687291e-06,
                  1.31878841e-06, 1.31636826e-06, 1.32832009e-06, 1.35187434e-06,
                  1.35642943e-06, 1.34364426e-06, 1.30411428e-06, 1.24853065e-06,
                  1.16396370e-06, 1.06711263e-06, 9.61926280e-07, 8.56807536e-07,
                  7.48377829e-07, 6.46208403e-07, 5.49744317e-07, 4.63518868e-07,
                  3.87907392e-07, 3.23562153e-07, 2.69743367e-07, 2.24984502e-07,
                  1.88174670e-07, 1.57917140e-07, 1.32004283e-07, 1.11851975e-07,
                  9.41122982e-08, 7.96477184e-08, 6.73124219e-08, 5.74621471e-08,
                  4.82572002e-08, 4.12649231e-08, 3.48285949e-08, 2.93078574e-08,
                  2.48682316e-08, 2.06522398e-08, 1.74595735e-08, 1.39749511e-08,
                  1.16428928e-08, 9.59510993e-09, 7.86969520e-09, 6.45950546e-09,
                  4.84234626e-09, 3.49855385e-09, 2.14257070e-09, 8.68904340e-10,
                  2.06216426e-10, 6.19307537e-12, 0.00000000e+00, 0.00000000e+00,
                  0.00000000e+00, 0.00000000e+00, 0.00000000e+00, 0.00000000e+00,
                  0.00000000e+00])

assert np.allclose(x, x_ref), "N2-CIAfunCKDMT252"

f = pyarts.arts.convert.kaycm2freq(np.linspace(1, 360, 101))

x = pyarts.arts.predef.get_n2_rot_ckdmt252(f, 1e4, 250, 0.79, 5e-3, 0.21)

x_ref = np.array([1.45221665e-09, 2.59078046e-08, 7.42787526e-08, 1.36478121e-07,
                  2.08066344e-07, 2.87877922e-07, 3.88261383e-07, 5.04316594e-07,
                  6.31073246e-07, 7.71472989e-07, 9.43414793e-07, 1.13771600e-06,
                  1.31851839e-06, 1.52809884e-06, 1.76221707e-06, 2.00856458e-06,
                  2.19799618e-06, 2.42798436e-06, 2.68279585e-06, 2.89011928e-06,
                  3.04573572e-06, 3.22042819e-06, 3.43483634e-06, 3.52842488e-06,
                  3.60813307e-06, 3.69450750e-06, 3.79248658e-06, 3.77202385e-06,
                  3.74529745e-06, 3.75297185e-06, 3.70545970e-06, 3.60179582e-06,
                  3.48893934e-06, 3.42755368e-06, 3.28157440e-06, 3.11938966e-06,
                  2.97242739e-06, 2.85415187e-06, 2.68071368e-06, 2.49569076e-06,
                  2.35418229e-06, 2.21120361e-06, 2.05031628e-06, 1.88126040e-06,
                  1.75356658e-06, 1.61984638e-06, 1.48328781e-06, 1.35084144e-06,
                  1.23989477e-06, 1.13101068e-06, 1.02270745e-06, 9.24184999e-07,
                  8.36018548e-07, 7.56891488e-07, 6.85829055e-07, 6.24033812e-07,
                  5.69193604e-07, 5.20372419e-07, 4.77011539e-07, 4.36944963e-07,
                  4.01045688e-07, 3.69454501e-07, 3.39736536e-07, 3.13076217e-07,
                  2.89798731e-07, 2.70148969e-07, 2.51389858e-07, 2.34445394e-07,
                  2.19584779e-07, 2.06197188e-07, 1.92394005e-07, 1.79238743e-07,
                  1.68435794e-07, 1.58313026e-07, 1.48576427e-07, 1.39401144e-07,
                  1.32131214e-07, 1.24573584e-07, 1.16945249e-07, 1.09728240e-07,
                  1.03262047e-07, 9.69848773e-08, 9.09073602e-08, 8.56423435e-08,
                  8.06318627e-08, 7.56539868e-08, 7.06160940e-08, 6.64878310e-08,
                  6.23476757e-08, 5.81782006e-08, 5.45177557e-08, 5.12432741e-08,
                  4.80415642e-08, 4.47311937e-08, 4.17649628e-08, 3.90743829e-08,
                  3.67216493e-08, 3.54411569e-08, 0.00000000e+00, 0.00000000e+00,
                  0.00000000e+00])

assert np.allclose(x, x_ref), "N2-CIArotCKDMT252"
