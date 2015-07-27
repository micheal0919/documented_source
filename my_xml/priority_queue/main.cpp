#include <iostream>
#include "Grader03.h"

int main(int argc, char * argv[]){
  Grader03 * grader = new Grader03();
  bool all_passed = true;
  long total_time = 0;
  int total_points = 0;
  for(int i = 0; i < grader->size(); ++i)  
  {
    TestCase * curr = grader->grade(i);
    std::cout << "Test Case: " << i << std::endl;
    std::string passed = "false";
    if(curr->passed){
      passed = "TRUE";
    }
    int points = grader->getPoints(i);
    if(curr->passed == false){
      points = 0;
    }
    std::cout << "Passed: " << passed << std::endl;
    std::cout << "Points: " << points << std::endl;
    std::cout << "Description: " << grader->getDescription(i) << std::endl;
    std::cout << "Error Message: " << curr->error_message << std::endl;
    std::cout << "Elapsed Time (ms): " << curr->time << std::endl;
    std::cout << std::endl;

    if(!curr->passed){
      all_passed = false;
    }
    total_time += curr->time;
    total_points += points;
  }

  if(all_passed){
    std::cout << "ALL TESTS PASS!!" << std::endl;
    std::cout << "total time: " << total_time << " (ms)" << std::endl;
  } else {
    std::cout << "SOME TESTS FAIL." << std::endl;
  }
  std::cout << "raw score: " << total_points << std::endl;
  return 0;
}

