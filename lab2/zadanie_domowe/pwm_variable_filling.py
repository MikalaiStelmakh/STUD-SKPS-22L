import argparse
from pwm import generate_pwm


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-frequency", type=int, default=1,
                        help="frequency (Hz)")
    parser.add_argument("-seconds", type=int, default=5,
                        help="timeout (s)")
    parser.add_argument("--draw", action="store_true",
                        help="show the plot")
    args = parser.parse_args()

    t_pwm = generate_pwm([args.frequency], [.3, .5, .7, .1, .9], args.seconds)
    if args.draw:
        import matplotlib.pyplot as plt
        t, pwm = t_pwm.keys(), t_pwm.values()
        plt.plot(t, pwm)
        plt.xticks([n for n in range(args.seconds+1)])
        plt.show()