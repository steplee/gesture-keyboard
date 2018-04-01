rows = [
        list('qwertyuiop'),
        list('asdfghjkl'),
        list('zxcvbnm')
        ]
max_row_len = 10

row_offsets = [o/max_row_len for o in [0,.2,.65]]

num_keys = 26
W = .8
H = .9

print("{")
print('"width": {},'.format(W))
print('"height": {},'.format(H))
#print("[")
print('"keys": {')
cnt = 0
for j,row in enumerate(rows):
    for i,ch in enumerate(row):
        cnt += 1
        x = W * (i/max_row_len) + (1-W)/2 + row_offsets[j]
        y = H * (j/3) + (1-H)/2
        comma = ',' if cnt < num_keys else ''
        print('"{}":[{},{}]'.format(ch,y,x) + comma)
#print("]")
print("}")
print("}")



