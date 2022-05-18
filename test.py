def claus():
    rows = 2
    cols = 3
    init = 2
    print("claus kode:\n")

    for col in range(init, init+(cols-1)): 
        for row_i in range(init+(col*rows),init+rows+(col*rows)):
            for row_i1 in range(init+rows+(col*rows),init+(rows*2)+(col*rows)):
                print(row_i, row_i1)

def henrik():
    start = 1
    rows = 3
    cols = 3

    for i in range(1,4):
        for j in range(1,4):
            for k in range(1,4):
                print(k+((i-1)%3)*3, j+i*3)

if __name__ == "__main__":
    claus()