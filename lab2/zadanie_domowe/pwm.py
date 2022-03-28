from typing import Dict, List
import argparse
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


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--draw", action="store_true",
                        help="show the plot")
    args = parser.parse_args()

    t_pwm = generate_pwm([4, 2], [0.3], 4)
    t, pwm = t_pwm.keys(), t_pwm.values()

    if args.draw:
        import matplotlib.pyplot as plt
        plt.plot(t, pwm)
        plt.show()