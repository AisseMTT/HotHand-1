import numpy as np
import dnest4.classic as dn4
import matplotlib.pyplot as plt

data = np.loadtxt("data.txt")
posterior_sample = dn4.my_loadtxt("posterior_sample.txt")

plt.hold(False)
plt.plot(data, "ko", label="Data")
plt.xlim([-0.5, len(data)-0.5])
plt.ylim([-0.02, 1.02])
plt.xlabel("Attempt")
plt.ylabel("Prob(success | parameters)")
plt.hold(True)

# Plot up to 100 posterior samples
for i in range(0, min([posterior_sample.shape[0], 100])):
    prob = posterior_sample[i, 3:]
    label = None
    if i == 0:
        label = "Model"
    plt.plot(prob, "g", alpha=0.1, label=label)

plt.legend()
plt.show()

