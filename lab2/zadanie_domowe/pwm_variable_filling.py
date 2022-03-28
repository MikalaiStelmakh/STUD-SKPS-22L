from pwm import generate_pwm
import matplotlib.pyplot as plt


time = 5
t_pwm = generate_pwm([3], [.3, .5, .7, .1, .9], time)
t, pwm = t_pwm.keys(), t_pwm.values()
plt.plot(t, pwm)
plt.xticks([n for n in range(time+1)])
plt.savefig("img/variable_filling")