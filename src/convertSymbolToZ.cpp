#include "functions.h"

int convertSymbolToZ(const std::string &el)
{
       if (el == "n" ) return   0;
  else if (el == "H" ) return   1;
  else if (el == "He") return   2;
  else if (el == "Li") return   3;
  else if (el == "Be") return   4;
  else if (el == "B" ) return   5;
  else if (el == "C" ) return   6;
  else if (el == "N" ) return   7;
  else if (el == "O" ) return   8;
  else if (el == "F" ) return   9;
  else if (el == "Ne") return  10;
  else if (el == "Na") return  11;
  else if (el == "Mg") return  12;
  else if (el == "Al") return  13;
  else if (el == "Si") return  14;
  else if (el == "P" ) return  15;
  else if (el == "S" ) return  16;
  else if (el == "Cl") return  17;
  else if (el == "Ar") return  18;
  else if (el == "K" ) return  19;
  else if (el == "Ca") return  20;
  else if (el == "Sc") return  21;
  else if (el == "Ti") return  22;
  else if (el == "V" ) return  23;
  else if (el == "Cr") return  24;
  else if (el == "Mn") return  25;
  else if (el == "Fe") return  26;
  else if (el == "Co") return  27;
  else if (el == "Ni") return  28;
  else if (el == "Cu") return  29;
  else if (el == "Zn") return  30;
  else if (el == "Ga") return  31;
  else if (el == "Ge") return  32;
  else if (el == "As") return  33;
  else if (el == "Se") return  34;
  else if (el == "Br") return  35;
  else if (el == "Kr") return  36;
  else if (el == "Rb") return  37;
  else if (el == "Sr") return  38;
  else if (el == "Y" ) return  39;
  else if (el == "Zr") return  40;
  else if (el == "Nb") return  41;
  else if (el == "Mo") return  42;
  else if (el == "Tc") return  43;
  else if (el == "Ru") return  44;
  else if (el == "Rh") return  45;
  else if (el == "Pd") return  46;
  else if (el == "Ag") return  47;
  else if (el == "Cd") return  48;
  else if (el == "In") return  49;
  else if (el == "Sn") return  50;
  else if (el == "Sb") return  51;
  else if (el == "Te") return  52;
  else if (el == "I" ) return  53;
  else if (el == "Xe") return  54;
  else if (el == "Cs") return  55;
  else if (el == "Ba") return  56;
  else if (el == "La") return  57;
  else if (el == "Ce") return  58;
  else if (el == "Pr") return  59;
  else if (el == "Nd") return  60;
  else if (el == "Pm") return  61;
  else if (el == "Sm") return  62;
  else if (el == "Eu") return  63;
  else if (el == "Gd") return  64;
  else if (el == "Tb") return  65;
  else if (el == "Dy") return  66;
  else if (el == "Ho") return  67;
  else if (el == "Er") return  68;
  else if (el == "Tm") return  69;
  else if (el == "Yb") return  70;
  else if (el == "Lu") return  71;
  else if (el == "Hf") return  72;
  else if (el == "Ta") return  73;
  else if (el == "W" ) return  74;
  else if (el == "Re") return  75;
  else if (el == "Os") return  76;
  else if (el == "Ir") return  77;
  else if (el == "Pt") return  78;
  else if (el == "Au") return  79;
  else if (el == "Hg") return  80;
  else if (el == "Tl") return  81;
  else if (el == "Pb") return  82;
  else if (el == "Bi") return  83;
  else if (el == "Po") return  84;
  else if (el == "At") return  85;
  else if (el == "Rn") return  86;
  else if (el == "Fr") return  87;
  else if (el == "Ra") return  88;
  else if (el == "Ac") return  89;
  else if (el == "Th") return  90;
  else if (el == "Pa") return  91;
  else if (el == "U" ) return  92;
  else if (el == "Np") return  93;
  else if (el == "Pu") return  94;
  else if (el == "Am") return  95;
  else if (el == "Cm") return  96;
  else if (el == "Bk") return  97;
  else if (el == "Cf") return  98;
  else if (el == "Es") return  99;
  else if (el == "Fm") return 100;
  else if (el == "Md") return 101;
  else if (el == "No") return 102;
  else if (el == "Lr") return 103;
  else if (el == "Rf") return 104;
  else if (el == "Db") return 105;
  else if (el == "Sg") return 106;
  else if (el == "Bh") return 107;
  else if (el == "Hs") return 108;
  else if (el == "Mt") return 109;
  else if (el == "Ea") return 110;
  else if (el == "Eb") return 111;
  else if (el == "Ec") return 112;
  else if (el == "Ed") return 113;
  else if (el == "Ee") return 114;
  else if (el == "Ef") return 115;
  else if (el == "Eg") return 116;
  else if (el == "Eh") return 117;
  else if (el == "Ei") return 118;
  else
    {
      std::cout << "\n" << el << " isn't an element" << std::endl;
      return 500;
    }
}
