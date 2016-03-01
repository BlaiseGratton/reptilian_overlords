/*#include "speedometer.h"

float get_distance(sensor) {
  return analogRead(sensor);
}

bool verify_distance(float old, float knew) {
  if (knew > MAX_THRESHOLD)
    return false;
  if ((knew-old)/old > VARIANT_THRESHOLD)
    return false;
  return true;
}

void determine_velocity() {
  int invalid = 0;
  float new_distance;
  float old_distance;

  while (true) {
    new_distance = get_distance(sensor);
    if (!verify_distance(new_distance, old_distance)) {
      invalid++;
      if (invalid==FLUSH_VALUE) {
        flush_data();
        show_messages;
      }
      delay(TIMER_WAIT);
      continue;
    }
    invalid = 0;
    speed_list.push(ABS(old_distance - new_distance)/TIMER_WAIT);
    old_distance = new_distance;
    display_speed(ave_speed(speed_list));
    delay(TIMER_WAIT);
  }
}*/
