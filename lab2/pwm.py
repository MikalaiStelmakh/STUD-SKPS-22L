from typing import Dict, List
import time
import math


TIME_INTERVAL = 1_000


def generate_pwm(frequencies: List[int], fillings: List[int], seconds: int = None) -> Dict[float, bool]:
    if len(frequencies) == 1 and len(fillings) != 1:
        frequencies = [frequencies[0] for _ in fillings]
    if len(frequencies) != 1 and len(fillings) == 1:
        fillings = [fillings[0] for _ in frequencies]

    t = [n/TIME_INTERVAL for n in range(seconds * TIME_INTERVAL)]
    t_each_frequency = seconds/len(frequencies)
    current_index = 0
    pwm = []

    for frequency, filling in zip(frequencies, fillings):
        period = 1 / frequency
        next_index = current_index + math.ceil(t_each_frequency*TIME_INTERVAL)
        pwm.extend([time % period < period * filling for time in t[current_index:next_index]])
        current_index = next_index

    result = {time_mark: state for time_mark, state in zip(t, pwm)}
    return result

def control_state(t_pwm, controller):
    prev_state = True
    prev_t = 0
    for t, state in zip(t_pwm.keys(), t_pwm.values()):
        time_sleep = t - prev_t
        if state != prev_state:
            controller.value = 1 if state else 0
            prev_state = state 
        time.sleep(time_sleep)
        prev_t = t
    