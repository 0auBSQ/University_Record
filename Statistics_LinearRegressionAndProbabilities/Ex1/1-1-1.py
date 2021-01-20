import numpy
import statistics
import sys
import matplotlib.pyplot as plt

input_file = "data.csv"
theta = [0, 0]

def	load_file(n):
	try:
		d = numpy.loadtxt(n, delimiter = ',', skiprows = 1)
	except:
		print ("data.csv file missing")
		sys.exit()
	return (d)	

def main():
	d = load_file(input_file)
	n = len(d)
	if (n < 2):
		print ("not enough data")
		sys.exit()
	x = d[:, 0]
	y = d[:, 1]

	# Compute (a)x + b
	theta[0] = numpy.cov(x, y, ddof = 0)[0][1] / numpy.var(x)
	# Compute ax + (b)
	theta[1] = statistics.mean(y) - theta[0] * statistics.mean(x)
	print(theta)

	# Mat plotlib init with dots from the CSV file
	plt.ion()
	fig = plt.figure()
	ax = fig.add_subplot(111)
	line1, = ax.plot(x, y, 'x')
	xa = numpy.linspace(0, 100, 100)
	ya = theta[0] * xa + theta[1]
	line2, = ax.plot(xa, ya)
	fig.canvas.draw()
	input("Press Enter to continue")
	plt.close()
	
	
if (__name__ == "__main__"):
	main()