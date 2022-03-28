import argparse
from pwm import generate_pwm


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-filling", type=float, default=1,
                        help="filling (from 0 to 1)")
    parser.add_argument("-seconds", type=int, default=5,
                        help="timeout (s)")
    parser.add_argument("--draw", action="store_true",
                        help="show the plot")
    args = parser.parse_args()

    t_pwm = generate_pwm([5, 2, 4, 1, 3, 1], [args.filling], args.seconds)
    if args.draw:
        import matplotlib.pyplot as plt
        t, pwm = t_pwm.keys(), t_pwm.values()
        plt.plot(t, pwm)
        plt.xticks([n for n in range(args.seconds+1)])
        plt.show()