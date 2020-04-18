#include "PID.h"
#include <iostream>
#include <iomanip>

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;
  p_error = 0.0;
  i_error = 0.0;
  d_error = 0.0;
  last_diff_n = 3;
  last_diff_idx = 0;
  last_diff.resize(last_diff_n, 0.0);
  last_diff_sum = 0.0;
  total_error = 0.0;
  previous_cte = 0.0;
}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */
  p_error  = -cte * Kp;
  i_error += -cte * Ki;
  // calculate difference of current cte versus previous:
  double diff = (cte-previous_cte);
  // calculate mean over last last_diff_n differences:
  last_diff_sum -= last_diff.at(last_diff_idx);
  last_diff_sum += diff;
  last_diff.at(last_diff_idx) = diff;
  
  last_diff_idx = (last_diff_idx+1);
  if (last_diff_idx >= last_diff_n)
    last_diff_idx = 0;
  
  d_error  = -(diff/static_cast<double>(last_diff_n)) * Kd;
  previous_cte = cte;  
  total_error = p_error + i_error + d_error;  // TODO: Add your total error calc here!
  
  std::cout << "PID: error: " << std::setw(5) << std::setprecision(2) << std::fixed << total_error;
  std::cout << "  P: " << std::setw(5) << std::setprecision(2) << std::fixed << p_error;
  std::cout << "  I: " << std::setw(5) << std::setprecision(2) << std::fixed << i_error;
  std::cout << "  D: " << std::setw(5) << std::setprecision(2) << std::fixed << d_error;
  std::cout << std::endl;
  
}

double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */
  return total_error;
}