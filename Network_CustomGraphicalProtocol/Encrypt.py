from PIL import Image
import numpy as np
import random
import string

print("Entrez votre message : ")

msg = list(bytearray(input().encode()))

print("Charger un logo (vide si pas d'image) : ")

# Handle logo values
try:
	logo = Image.open(input())
	thresh = 200
	fn = lambda x : 255 if x > thresh else 0
	r = logo.convert('L').point(fn, mode='1')
	pixels = list(r.getdata())
	logo_width, logo_height = r.size
	logo_size = len(pixels)
	pixels = [pixels[i * logo_width:(i + 1) * logo_width] for i in range(logo_height)]
	print("Position x du logo : ")
	logo_x_pos = int(input())
	print("Position y du logo : ")
	logo_y_pos = int(input())
except:
	print("No logo loaded.")
	logo_size, logo_width, logo_height, logo_x_pos, logo_y_pos = 0, 0, 0, 0, 0	
	pixels = []

# Protocole version
ver = 1
# Message size
msg_size = len(msg) * 16
# Bytes for message size
msize_byt = 15

for i in range(16):
	if pow(2, 8 * i) > msg_size:
		msize_byt = i
		break

# Process hamming 8,4
def hamming_protect(n, factor = 1):
	b = [n >> i & 1 for i in range(7,-1,-1)]
	
	p1a = b[0] ^ b[2] ^ b[3] 
	p2a = b[0] ^ b[1] ^ b[3] 
	p4a = b[0] ^ b[1] ^ b[2]
	p8a = p1a ^ p2a ^ p4a ^ b[0] ^ b[1] ^ b[2] ^ b[3]
	
	p1b = b[4] ^ b[6] ^ b[7] 
	p2b = b[4] ^ b[5] ^ b[7] 
	p4b = b[4] ^ b[5] ^ b[6]
	p8b = p1b ^ p2b ^ p4b ^ b[4] ^ b[5] ^ b[6] ^ b[7]
	
	ret = [p8b, b[4], b[5], b[6], p4b, b[7], p2b, p1b, p8a, b[0], b[1], b[2], p4a, b[3], p2a, p1a]
	return [x * factor for x in ret]

# Matrix for code rendering

# Blue : 1, Red : 2, Blank = 0  

# Process header
code_mat = hamming_protect(ver)
code_mat.extend(hamming_protect(msize_byt))
while msg_size > 0:
	tmp_sz = msg_size % 256
	msg_size = msg_size // 256
	code_mat.extend(hamming_protect(tmp_sz))

# Process data
for byte in msg:
	code_mat.extend(hamming_protect(byte, 2))
	
# Process code image size
def code_size(dims, logo_size = 0):
	size = dims * dims
	size -= 3 * (8 * 8)
	size -= logo_size
	return (size)

# Define code dimentions
base_size = 20
for i in range(20, 4000, 4):
	if code_size(i, logo_size) > len(code_mat):
		base_size = i
		break

# Fill complete matrix with anchors
tmp_index = 0
code = np.empty(shape=(base_size, base_size))
for x in range(base_size):
	for y in range(base_size):
		if ((x == 0 or x == 7) and (y < 8 or y >= (base_size - 8))):
			code[x][y] = 4
		elif ((x == base_size - 8 or x == base_size - 1) and y < 8):
			code[x][y] = 4
		elif ((y == 0 or y == 7) and (x < 8 or x >= (base_size - 8))):
			code[x][y] = 4
		elif ((y == base_size - 8 or y == base_size - 1) and x < 8):
			code[x][y] = 4
		elif (x == y and x >= 2 and x <= 5):
			code[x][y] = 4
		elif (logo_size > 0 and x >= logo_x_pos and y >= logo_y_pos and x < (logo_x_pos + logo_width) and y < (logo_y_pos + logo_height)):
			if (pixels[x - logo_x_pos][y - logo_y_pos] == 255):
				code[x][y] = 0
			else:
				code[x][y] = 3
		elif (((x > 7 and x < base_size - 8) or (y > 7 and y < base_size - 8) or (x > 7 and y > 7)) and tmp_index < len(code_mat)):
			code[x][y] = code_mat[tmp_index]
			tmp_index += 1
		else:
			code[x][y] = 0

# Generate and display image 
pixel_size = 5
w, h = base_size * pixel_size, base_size * pixel_size
img = np.zeros((h, w, 3), dtype=np.uint8)
for x in range(h):
	for y in range(w):
		case = code[x // pixel_size][y // pixel_size]
		if (case == 0):
			img[x][y] = [255, 255, 255]
		elif (case == 1):
			img[x][y] = [0, 0, 255]
		elif (case == 2):
			img[x][y] = [255, 0, 0]
		elif (case == 3):
			img[x][y] = [0, 255, 0]
		else:
			img[x][y] = [0, 0, 0]
img_ret = Image.fromarray(img, 'RGB')
fn = 'code_' + ''.join(random.choice(string.ascii_letters) for i in range(16)) + '.png'
img_ret.save(fn)
img_ret.show()

print("Image saved");
