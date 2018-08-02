#include <Arduino.h>
#include "SHAB.h"

double PascalToMeter(double pressurePa) {
      const double R = 8.3144598;
      const double g = 9.80665;
      const double M = 0.0289644;
      const int32_t N = 5;
      const double h_b[N] = {0.0, 11000.0, 20000.0, 32000.0, 47000.0};
      const double P_b[N] = {101325.0, 22632.1, 5474.89, 868.02, 110.91};
      const double T_b[N] = {288.15, 216.65, 216.65, 228.65, 270.65};
      const double L_b[N] = {-0.0065, 0, 0.001, 0.0028, 0};
      if (pressurePa > P_b[0]) {
        return 0;
      }
  
      int i;
      for(i = 0; i < N; i++) {
        if(pressurePa > P_b[i]) break;
      }
      i = i - 1;
      double height;
      if (L_b[i] == (double)(0)) {
        height = (double)(R*T_b[i]*log((double)(pressurePa)/(double)(P_b[i]))/((double)(-1) * g * M)) + (h_b[i]);
      } else {
        height = ((T_b[i]/pow((double)(pressurePa)/(double)(P_b[i]),  (R*L_b[i])/(g*M))) - T_b[i])/L_b[i] + (double)(h_b[i]);
      }
      
      return height;
}