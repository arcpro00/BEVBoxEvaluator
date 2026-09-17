import random
import math

def save_boxes(filename, data_list):
    """Writes our 3d box pair-data to a given filepath"""
    with open(filename, "w") as fil:
        fil.write("pair_id x y z length width height yaw\n")
    with open(filename, "a") as fil:
        for row in data_list:
            fil.write(f"{" ".join([str(row[i]) for i in range(len(row))])}\n")

def create_random_box_row(pair_id):
    row = [pair_id]\
        +[random.uniform(0.0,25.0) for i in range(3)]\
        +[random.uniform(0.0,25.0) for i in range(3)]\
        +[random.uniform(0.000001-math.pi,math.pi-0.000001)]
    return row
def get_noisy_pair(row):
    return [row[0]]+[value*random.uniform(0.75,1.25) for value in row[1:]]

def main():
    data_list = []
    for i in range(100):
        new_row = create_random_box_row(i)
        data_list.append(new_row)
        data_list.append(get_noisy_pair(new_row))
    save_boxes("noisy_test_data.txt", data_list)

if __name__ == "__main__":
    main()
