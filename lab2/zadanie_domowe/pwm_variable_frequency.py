from pwm import generate_pwm
import matplotlib.pyplot as plt


time = 6
t_pwm = generate_pwm([5, 2, 4, 1, 3, 1], [.3], time)
t, pwm = t_pwm.keys(), t_pwm.values()
plt.plot(t, pwm)
plt.xticks([n for n in range(time+1)])
plt.savefig("img/variable_frequency")