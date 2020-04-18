#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "PID.h"
#include <cstdlib>

// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main(int argc, char *argv[]) {
  uWS::Hub h;

  PID pid;
  /**
   * TODO: Initialize the pid variable.
   */
  for (int i(0); i<argc; ++i) {
    std::cout << "arg " << i << ": " << argv[i] << std::endl;
  }
  
  double scale = 1.0; 
  if (argc > 1) {
    scale = atof(argv[1]); 
  }
  std::cout << "scale: " << scale << std::endl;
  
  double Kp_ = 0.08; 
  if (argc > 2) {
    Kp_ = atof(argv[2]); 
  }
  std::cout << "Kp_: " << Kp_ << std::endl;
  Kp_ *= scale;
  
  double Ki_ = 0.001; 
  if (argc > 3) {
    Ki_ = atof(argv[3]); 
  }
  std::cout << "Ki_: " << Ki_ << std::endl;
  Ki_ *= scale;

  double Kd_ = 1.8; 
  if (argc > 4) {
    Kd_ = atof(argv[4]); 
  }
  std::cout << "Kd_: " << Kd_ << std::endl;
  Kd_ *= scale;
  
  double Vmax_ = 20.0; 
  if (argc > 5) {
    Vmax_ = atof(argv[5]); 
  }
  std::cout << "Vmax_: " << Vmax_ << std::endl;
  
  pid.Init(Kp_, Ki_, Kd_);

  h.onMessage([&pid, &Vmax_](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, 
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<string>());
          double speed = std::stod(j[1]["speed"].get<string>());
          double angle = std::stod(j[1]["steering_angle"].get<string>());
          double steer_value;
          /**
           * TODO: Calculate steering value here, remember the steering value is
           *   [-1, 1].
           * NOTE: Feel free to play around with the throttle and speed.
           *   Maybe use another PID controller to control the speed!
           */
          pid.UpdateError(cte);
          steer_value = pid.TotalError();
          if (steer_value > 1.0)
            steer_value = 1.0;
          if (steer_value < -1.0)
            steer_value = -1.0;
          
          double throttle = 0.3;
          if (speed > Vmax_) {
            throttle = 0.0;
          }
          
          // DEBUG
          // std::cout << "CTE: " << std::setw(5) << std::setprecision(2) << std::fixed << cte;
          // std::cout << "  SteeringValue: " << std::setw(5) << std::setprecision(2) << std::fixed << steer_value;
          // std::cout << "  angle: " << std::setw(5) << std::setprecision(2) << std::fixed << angle;
          // std::cout << "  speed: " << std::setw(6) << std::setprecision(2) << std::fixed << speed;
          // std::cout << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          //std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}