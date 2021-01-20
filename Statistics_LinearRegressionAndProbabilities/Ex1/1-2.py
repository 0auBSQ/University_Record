import numpy
import sys
import matplotlib.pyplot as plt

input_file = "data.csv"
output_file = "output.csv"

def estimate(t, mileage):
	return (t[0] + (t[1] * mileage))

def normalize(d):
	e = (d - min(d)) / (max(d) - min(d))
	return (e)
	
def unnormalize(d, o):
	e = d * (max(o) - min(o)) + min(o)
	return (e)
	
def	load_file(n):
	try:
		d = numpy.loadtxt(n, delimiter = ',', skiprows = 1)
	except:
		print ("data.csv file missing")
		sys.exit()
	return (d)	

def save_file(n, t):
	try:
		numpy.savetxt(n, t, delimiter = ',')
	except:
		print ("file save failed")
		sys.exit()

# Learning ratio, an ideal value here is 1, a too big value can causes the result to diverge
def enter_learning_rate():
	try:
		learning_rate = float(input("Learning rate (default 1) : "))
	except:
		print ("type error")
		sys.exit()
	return (learning_rate)

# Precision is the "maximum OK value for cost function", useful to end the program a bit faster, but reduce results accuracy, 0.01035 is fast
def enter_precision():
	try:
		precision = float(input("Desired precision (default 0) : "))
	except:
		print ("type error")
		sys.exit()
	return (precision)

def process_gradiant_algorithm(d, n, learning_rate, precision):
    # Mat plotlib init with dots from the CSV file
	plt.ion()
	fig = plt.figure()
	ax = fig.add_subplot(111)
	line1, = ax.plot(d[:, 0], d[:, 1], 'x')
    # Normalise the datasets first
	mileage = normalize(d[:, 0])
	price = normalize(d[:, 1])
    # Init theta (and derivate) array, variables and calculating line w/ current estimation
	t = [0, 0]
	dt = [0, 0]
	line2, = ax.plot(d[:, 0], unnormalize(estimate(t, mileage), d[:, 1]))
	cost = 1
	prev = 0
	current_prediction = 0
	while (abs(cost) > precision):
		current_prediction = estimate(t, mileage);
        # Process derivates and cost
		dt[1] = learning_rate * (1 / n) * sum(mileage * (current_prediction - price))
		dt[0] = learning_rate * (1 / n) * sum(current_prediction - price)
		cost = (1 / (2 * n)) * sum(pow(current_prediction - price, 2))
        # Process gradiant descent
		t[1] = t[1] - dt[1]
		t[0] = t[0] - dt[0]
		print("Current cost :", abs(cost))
		if (abs(prev - cost) < 0.00000000000001):
			print("Notice : The result seems to converge, the learning process is stopped")
			break
		prev = cost
		if (abs(cost) > 1000000):
			print("Error : The result seems to diverge, please decrease the learning rate")
			sys.exit()
        # Refresh matplotlib visual
		line2.set_ydata(unnormalize(current_prediction, d[:, 1]))
		fig.canvas.draw()
		plt.pause(0.01)
	input("Press Enter to continue")
	plt.close()
    # Reverse the thetas (using unnormalization and delta average calculation) and return file
	un_data = unnormalize(current_prediction, d[:, 1])
	tmp_delta = [0, 0]
	for i in range(len(un_data) - 1):
		tmp_delta[0] = tmp_delta[0] + un_data[i] - un_data[i + 1]
		tmp_delta[1] = tmp_delta[1] + d[i, 0] - d[(i + 1), 0]
	t[1] = tmp_delta[0] / tmp_delta[1]
	t[0] = -t[1] * d[0, 0] + un_data[0]
	save_file(output_file, t)
	print("file saved :", output_file)

def main():
	d = load_file(input_file)
	n = len(d)
	if (n < 2):
		print ("not enough data")
		sys.exit()
	precision = enter_precision()
	learning_rate = enter_learning_rate()
	t = process_gradiant_algorithm(d, n, learning_rate, precision)	

if (__name__ == "__main__"):
	main()